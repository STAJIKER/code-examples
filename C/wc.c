#include <stdio.h>
int main(int argc, char **argv)
{
	unsigned int i=1, length_word=0, lines=0, words=0, bytes=0, total_lines=0, total_words=0, total_bytes=0;
	char c;
	if(argc==1)
	{
		while((c=getchar())!=EOF)
		{
			if(!isspace(c))
			{
				++length_word;
				if(length_word==1)
					++words;
			}
			else
			{
				length_word=0;
				if(c=='\n')
					++lines;
			}
			++bytes;
		}
		printf("%d %d %d\n", lines, words, bytes);
	}
	else
	{
		while(i<argc)
		{
			FILE *current_file=fopen(argv[i], "r");
			lines=0, words=0, bytes=0;
			while((c=getc(current_file))!=EOF)
				if(!isspace(c))
				{
					++length_word;
					if(length_word==1)
						++words;
				}
				else
				{
					length_word=0;
					if(c=='\n')
						++lines;
				}
			fseek(current_file, 0, SEEK_END);
			bytes=ftell(current_file);
			total_lines+=lines;
			total_words+=words;
			total_bytes+=bytes;
			printf("%d %d %d ", lines, words, bytes);
			puts(argv[i]);
			++i;
		}
		printf("%d %d %d total\n", total_lines, total_words, total_bytes);
	}
	return 0;
}
