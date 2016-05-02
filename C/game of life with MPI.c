#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <mpi.h>

#define ALIVE 'X'
#define DEAD '.'

int toindex(int row, int col, int N) {
	if (row < 0) {
		row = row + N;
	}
	else if (row >= N) {
		row = row - N;
	}
	if (col < 0) {
		col = col + N;
	}
	else if (col >= N) {
		col = col - N;
	}
	return row * N + col;
}

void printgrid(char* grid, char* buf, FILE* f, int N) {
	for (int i = 0; i < N; ++i) {
		strncpy(buf, grid + i * N, N);
		buf[N] = 0;
		fprintf(f, "%s\n", buf);
	}
}

int main(int argc, char* argv[]) {
	if (argc != 5) {
		fprintf(stderr, "Usage: %s N input_file iterations output_file\n", argv[0]);
		return 1;
	}

	int N = atoi(argv[1]); // grid size
	int iterations = atoi(argv[3]);

	int rank, size;
	MPI_Request reqs[4];
	MPI_Status stats[4];

	MPI_Init(&argc, &argv);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	int count = N * N / size;

	char* all_grid;
	if (rank == 0) {
		FILE* input = fopen(argv[2], "r");
		all_grid = (char*)malloc(N * N * sizeof(char));
		for (int i = 0; i < N; ++i) {
			fscanf(input, "%s", all_grid + i * N);
		}
		fclose(input);
	}

	char* grid = (char*)malloc((count + 2 * N) * sizeof(char));
	char* buf = (char*)malloc((count + 2 * N) * sizeof(char));
	char* front = (char*)malloc((count + 2 * N) * sizeof(char));
	memset(front, 1, (count + 2 * N));

	MPI_Scatter(all_grid, count, MPI_CHAR, grid + N, count, MPI_CHAR, 0, MPI_COMM_WORLD);

	for (int iter = 0; iter < iterations; ++iter) {
		int current_front = iter % 2 + 1;
		int next_front = (iter + 1) % 2 + 1;

		if (rank > 0) {
			MPI_Irecv(grid, N, MPI_CHAR, rank - 1, 0, MPI_COMM_WORLD, &reqs[0]);
			MPI_Isend(grid + N, N, MPI_CHAR, rank - 1, 1, MPI_COMM_WORLD, &reqs[1]);
		}
		else {
			MPI_Irecv(grid, N, MPI_CHAR, size - 1, 0, MPI_COMM_WORLD, &reqs[0]);
			MPI_Isend(grid + N, N, MPI_CHAR, size - 1, 1, MPI_COMM_WORLD, &reqs[1]);
		}
		if (rank < size - 1) {
			MPI_Irecv(grid + count + N, N, MPI_CHAR, rank + 1, 1, MPI_COMM_WORLD, &reqs[2]);
			MPI_Isend(grid + count, N, MPI_CHAR, rank + 1, 0, MPI_COMM_WORLD, &reqs[3]);
		}
		else {
			MPI_Irecv(grid + count + N, N, MPI_CHAR, 0, 1, MPI_COMM_WORLD, &reqs[2]);
			MPI_Isend(grid + count, N, MPI_CHAR, 0, 0, MPI_COMM_WORLD, &reqs[3]);
		}

		for (int i = 2; i < N / size; ++i) {
			for (int j = 0; j < N; ++j) {
				int current = i * N + j;
				if (front[current] != 0) {
					int alive_count = 0;
					for (int di = -1; di <= 1; ++di) {
						for (int dj = -1; dj <= 1; ++dj) {
							if ((di != 0 || dj != 0) && grid[toindex(i + di, j + dj, N)] == ALIVE) {
								++alive_count;
							}
						}
					}
					if (alive_count == 3 || (alive_count == 2 && grid[current] == ALIVE)) {
						buf[current] = ALIVE;
					}
					else {
						buf[current] = DEAD;
					}
					if (buf[current] != grid[current]) {
						for (int di = -1; di <= 1; ++di) {
							for (int dj = -1; dj <= 1; ++dj) {
								front[toindex(i + di, j + dj, N)] = next_front;
							}
						}
					}
					else {
						if (front[current] != next_front) {
							front[current] = 0;
						}
					}
				}
			}
		}

		MPI_Waitall(4, reqs, stats);

		int i = 1;
		do {
			for (int j = 0; j < N; ++j) {
				int current = i * N + j;
				if (front[current] == 0) {
					front[current] = current_front;
				}
				int alive_count = 0;
				for (int di = -1; di <= 1; ++di) {
					for (int dj = -1; dj <= 1; ++dj) {
						if ((di != 0 || dj != 0) && grid[toindex(i + di, j + dj, N)] == ALIVE) {
							++alive_count;
						}
					}
				}
				if (alive_count == 3 || (alive_count == 2 && grid[current] == ALIVE)) {
					buf[current] = ALIVE;
				}
				else {
					buf[current] = DEAD;
				}
				if (buf[current] != grid[current]) {
					for (int di = -1; di <= 1; ++di) {
						for (int dj = -1; dj <= 1; ++dj) {
							front[toindex(i + di, j + dj, N)] = next_front;
						}
					}
				}
				else {
					if (front[current] != next_front) {
						front[current] = 0;
					}
				}
			}

			i += N / size - 1;
		} while (i <= N / size);

		char* tmp = grid; grid = buf; buf = tmp;
	}

	MPI_Gather(grid + N, count, MPI_CHAR, all_grid, count, MPI_CHAR, 0, MPI_COMM_WORLD);

	if (rank == 0) {
		FILE* output = fopen(argv[4], "w");
		printgrid(all_grid, buf, output, N);
		fclose(output);
	}

	free(all_grid);
	free(grid);
	free(buf);
	free(front);

	MPI_Finalize();

	return 0;
}