#include <stdlib.h>
#include <stdio.h>
int find_pattern_in_s(char *pattern, unsigned int i, char *s, unsigned int j, int **dp)
{
	if(pattern[i]==0)
		return dp[i][j]=1;
	if(s[j]==0)
		return dp[i][j]=-1;
	if(pattern[i]=='?')
		return dp[i][j]=find_pattern_in_s(pattern, i+1, s, j+1, dp);
	if(pattern[i]=='*')
	{
		unsigned int k=j;
		while(s[k]!=0)
		{
			if(find_pattern_in_s(pattern, i+1, s, k, dp)==1)
				return dp[i][j]=1;
			++k;
		}
		return dp[i][j]=-1;
	}
	if(pattern[i]=='[')
	{
		unsigned int k=i+1, l;
		while(pattern[k]!=']')
			++k;
		l=i+1;
		while(l<k)
		{
			if(pattern[l]==s[j])
				return dp[i][j]=find_pattern_in_s(pattern, k+1, s, j+1, dp);
			++l;
		}
		return dp[i][j]=-1;
	}
	if(pattern[i]==s[j])
		return dp[i][j]=find_pattern_in_s(pattern, i+1, s, j+1, dp);
	return dp[i][j]=-1;
}
int main(int argc, char **argv)
{
	if(argc==1)
	{
		puts("No pattern");
		return 0;
	}
	unsigned int i=2, j=0, k, length_s=0, length_pattern=0;
	int **dp;
	char *pattern=argv[1], c, *s;
	s=malloc(sizeof(char));
	while(pattern[length_pattern]!=0)
		++length_pattern;
	dp=malloc((length_pattern+1)*sizeof(int*));
	while(j<=length_pattern)
	{
		dp[j]=malloc(sizeof(int));
		++j;
	}
	if(argc==2)
		while((c=getchar())!=EOF)
			if(c=='\n')
			{
				s[length_s]=0;
				j=0;
				while(j<=length_pattern)
				{
					dp[j]=realloc(dp[j], (length_s+1)*sizeof(int));
					k=0;
					while(k<=length_s)
					{
						dp[j][k]=0;
						++k;
					}
					++j;
				}
				j=0;
				while(s[j]!=0)
				{
					if(find_pattern_in_s(pattern, 0, s, j, dp)==1)
					{
						puts(s);
						break;
					}
					++j;
				}
				length_s=0;
				s=realloc(s, sizeof(char));
			}
			else
			{
				++length_s;
				s=realloc(s, (length_s+1)*sizeof(char));
				s[length_s-1]=c;
			}
	else
		while(i<argc)
		{
			FILE *current_file=fopen(argv[i], "r");
			while((c=getc(current_file))!=EOF)
				if(c=='\n')
				{
					s[length_s]=0;
					j=0;
					while(j<=length_pattern)
					{
						dp[j]=realloc(dp[j], (length_s+1)*sizeof(int));
						k=0;
						while(k<=length_s)
						{
							dp[j][k]=0;
							++k;
						}
						++j;
					}
					j=0;
					while(s[j]!=0)
					{
						if(find_pattern_in_s(pattern, 0, s, j, dp)==1)
						{
							if(argc>3)
								printf("%s:", argv[i]);
							puts(s);
							break;
						}
						++j;
					}
					length_s=0;
					s=realloc(s, sizeof(char));
				}
				else
				{
					++length_s;
					s=realloc(s, (length_s+1)*sizeof(char));
					s[length_s-1]=c;
				}
			++i;
		}
	return 0;
}
