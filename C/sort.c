#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int comparator(const void *a, const void *b)
{
	char *s1=*(char* const*)a, *s2=*(char* const*)b;
	unsigned int i1=0, i2=0;
	do
	{
		while(!isalnum(s1[i1])&&s1[i1]!=0)
			++i1;
		while(!isalnum(s2[i2])&&s2[i2]!=0)
			++i2;
		if(s1[i1]<s2[i2])
			return -1;
		if(s1[i1]>s2[i2])
			return 1;
		++i1, ++i2;
	}
	while(s1[i1]!=0&&s2[i2]!=0);
	if(s1[i1]==0&&s2[i2]==0)
		return strcmp(s1, s2);
	if(s1[i1]==0)
		return -1;
	if(s2[i2]==0)
		return 1;
}
int main(int argc, char **argv)
{
	unsigned int i=1, size_s=0, length_string=0;
	char c, **s;
	s=malloc(sizeof(char*));
	if(argc==1)
		while((c=getchar())!=EOF)
			if(c=='\n')
			{
				if(length_string==0)
				{
					++size_s;
					s=realloc(s, size_s*sizeof(char*));
					s[size_s-1]=malloc(sizeof(char));
				}
				s[size_s-1][length_string]=0;
				length_string=0;
			}
			else
			{
				++length_string;
				if(length_string==1)
				{
					++size_s;
					s=realloc(s, size_s*sizeof(char*));
					s[size_s-1]=malloc(2*sizeof(char));
				}
				else
					s[size_s-1]=realloc(s[size_s-1], (length_string+1)*sizeof(char));
				s[size_s-1][length_string-1]=c;
			}
	else
		while(i<argc)
		{
			FILE *current_file=fopen(argv[i], "r");
			while((c=getc(current_file))!=EOF)
				if(c=='\n')
				{
					if(length_string==0)
					{
						++size_s;
						s=realloc(s, size_s*sizeof(char*));
						s[size_s-1]=malloc(sizeof(char));
					}
					s[size_s-1][length_string]=0;
					length_string=0;
				}
				else
				{
					++length_string;
					if(length_string==1)
					{
						++size_s;
						s=realloc(s, size_s*sizeof(char*));
						s[size_s-1]=malloc(2*sizeof(char));
					}
					else
						s[size_s-1]=realloc(s[size_s-1], (length_string+1)*sizeof(char));
					s[size_s-1][length_string-1]=c;
				}
			++i;
		}
	qsort(&s[0], size_s, sizeof(char*), comparator);
	i=0;
	while(i!=size_s)
	{
		puts(s[i]);
		++i;
	}
	return 0;
}
