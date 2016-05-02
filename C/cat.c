#include <stdio.h>
#include <fcntl.h>
int main(int argc, char **argv)
{
	int i=1, fd, numbytes;
	char buf[5*sizeof(char)];
	if(argc==1)
		while((numbytes=read(0, buf, 5*sizeof(char)))>0)
			write(1, buf, numbytes);
	else
		while(i<argc)
		{
			fd=open(argv[i], O_RDONLY);
			while((numbytes=read(fd, buf, 5*sizeof(char)))>0)
				write(1, buf, numbytes);
			++i;
		}
	return 0;
}
