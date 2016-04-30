#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
int cmp(const void *a, const void *b)
{
	char *s1=*(char* const*)a, *s2=*(char* const*)b;
	if(strlen(s1)<strlen(s2))
		return -1;
	if(strlen(s1)>strlen(s2))
		return 1;
	int i=0;
	while(s1[i]!=0)
		if(s1[i]==s2[i]||abs(s1[i]-s2[i])==abs('A'-'a'))
			++i;
		else
		{
			char a=s1[i], b=s2[i];
			if(a>='A'&&a<='Z')
				a-='A'-'a';
			if(b>='A'&&b<='Z')
				b-='A'-'a';
			if(a<b)
				return -1;
			else
				return 1;
		}
	return 0;
}
int CMP(const char *s1, const char *s2)
{
	if(strlen(s1)<strlen(s2))
		return -1;
	if(strlen(s1)>strlen(s2))
		return 1;
	int i=0;
	while(s1[i]!=0)
		if(s1[i]==s2[i]||abs(s1[i]-s2[i])==abs('A'-'a'))
			++i;
		else
		{
			char a=s1[i], b=s2[i];
			if(a>='A'&&a<='Z')
				a-='A'-'a';
			if(b>='A'&&b<='Z')
				b-='A'-'a';
			if(a<b)
				return -1;
			else
				return 1;
		}
	return 0;
}
void diff(const char *path1, const char *path2)
{
	char *full_path1=malloc((strlen(path1)+3)*sizeof(char)), *full_path2=malloc((strlen(path2)+3)*sizeof(char));
	strcpy(full_path1, "./");
	strcat(full_path1, path1);
	strcpy(full_path2, "./");
	strcat(full_path2, path2);
	DIR *dir1=opendir(full_path1), *dir2=opendir(full_path2);
	if(dir1==NULL)
	{
		printf("Can't be opened ");
		puts(path1);
		if(dir2==NULL)
		{
			printf("Can't be opened ");
			puts(path2);
		}
		return;
	}
	if(dir2==NULL)
	{
		printf("Can't be opened ");
		puts(path2);
		if(dir1==NULL)
		{
			printf("Can't be opened ");
			puts(path1);
		}
		return;
	}
	char **files1=malloc(sizeof(char*)), **files2=malloc(sizeof(char*));
	int size_files1=0, size_files2=0;
	struct dirent *file;
	while((file=readdir(dir1))!=NULL)
		if(file->d_name[0]!='.')
		{
			++size_files1;
			files1=realloc(files1, size_files1*sizeof(char*));
			files1[size_files1-1]=malloc((strlen(file->d_name)+1)*sizeof(char));
			strcpy(files1[size_files1-1], file->d_name);
		}
	while((file=readdir(dir2))!=NULL)
		if(file->d_name[0]!='.')
		{
			++size_files2;
			files2=realloc(files2, size_files2*sizeof(char*));
			files2[size_files2-1]=malloc((strlen(file->d_name)+1)*sizeof(char));
			strcpy(files2[size_files2-1], file->d_name);
		}
	qsort(files1, size_files1, sizeof(char*), cmp);
	qsort(files2, size_files2, sizeof(char*), cmp);
	int i=0, j=0;
	while(i<size_files1&&j<size_files2)
	{
		char *path_file1=malloc((strlen(path1)+strlen(files1[i])+2)*sizeof(char));
		strcpy(path_file1, path1);
		strcat(path_file1, "/");
		strcat(path_file1, files1[i]);
		char *full_path_file1=malloc((strlen(path_file1)+3)*sizeof(char));
		strcpy(full_path_file1, "./");
		strcat(full_path_file1, path_file1);
		struct stat info1;
		lstat(full_path_file1, &info1);
		char *path_file2=malloc((strlen(path2)+strlen(files2[j])+2)*sizeof(char));
		strcpy(path_file2, path2);
		strcat(path_file2, "/");
		strcat(path_file2, files2[j]);
		char *full_path_file2=malloc((strlen(path_file2)+3)*sizeof(char));
		strcpy(full_path_file2, "./");
		strcat(full_path_file2, path_file2);
		struct stat info2;
		lstat(full_path_file2, &info2);
//		puts(path_file1);
//		puts(full_path_file1);
//		puts(path_file2);
//		puts(full_path_file2);
		if(CMP(files1[i], files2[j])==0)
		{
			if(S_ISREG(info1.st_mode)&&S_ISREG(info2.st_mode))
			{
				if(info1.st_size!=info2.st_size)
					printf("Different sizes %s and %s\n", path_file1, path_file2);
				++i, ++j;
				continue;
			}
			if(S_ISLNK(info1.st_mode)&&S_ISLNK(info2.st_mode))
			{
				char *link1=malloc(1000*sizeof(char)), *link2=malloc(1000*sizeof(char));
				int k1=0, k2=0;
				readlink(full_path_file1, link1, 1000);
				link1=realloc(link1, 1000*sizeof(char));
				while(readlink(link1, link1, 1000)!=-1)
				{
					link1=realloc(link1, 1000*sizeof(char));
					++k1;
					if(k1>1000)
					{
						printf("Incorrect link %s\n", path_file1);
						break;
					}
				}
				readlink(full_path_file2, link2, 1000);
				link2=realloc(link1, 1000*sizeof(char));
				while(readlink(link2, link2, 1000)!=-1)
				{
					link2=realloc(link1, 1000*sizeof(char));
					++k2;
					if(k2>1000)
					{
						printf("Incorrect link %s\n", path_file2);
						break;
					}
				}
//				puts(link1);
//				puts(link2);
				if(k1<=1000&&k2<=1000)
					if(strcmp(link1, link2)!=0)
						printf("Different links %s and %s\n", path_file1, path_file2);
				++i, ++j;
				continue;
			}
			if(S_ISREG(info1.st_mode)&&!S_ISREG(info2.st_mode)||!S_ISREG(info1.st_mode)&&S_ISREG(info2.st_mode))
			{
				printf("Different %s and %s\n", path_file1, path_file2);
				++i, ++j;
				continue;
			}
			if(S_ISLNK(info1.st_mode)&&!S_ISLNK(info2.st_mode)||!S_ISLNK(info1.st_mode)&&S_ISLNK(info2.st_mode))
			{
				printf("Different %s and %s\n", path_file1, path_file2);
				++i, ++j;
				continue;
			}
			if(S_ISDIR(info1.st_mode)&&!S_ISDIR(info2.st_mode)||!S_ISDIR(info1.st_mode)&&S_ISDIR(info2.st_mode))
			{
				printf("Different %s and %s\n", path_file1, path_file2);
				++i, ++j;
				continue;
			}
			++i, ++j;
			continue;
		}
		if(CMP(files1[i], files2[j])==-1)
		{
			printf("Exist only %s\n", path_file1);
			++i;
		}
		else
		{
			printf("Exist only %s\n", path_file2);
			++j;
		}
	}
	while(i<size_files1)
	{
		char *path_file1=malloc((strlen(path1)+strlen(files1[i])+2)*sizeof(char));
		strcpy(path_file1, path1);
		strcat(path_file1, "/");
		strcat(path_file1, files1[i]);
		printf("Exist only %s\n", path_file1);
		++i;
	}
	while(j<size_files2)
	{
		char *path_file2=malloc((strlen(path2)+strlen(files2[i])+2)*sizeof(char));
		strcpy(path_file2, path2);
		strcat(path_file2, "/");
		strcat(path_file2, files2[j]);
		printf("Exist only %s\n", path_file2);
		++j;
	}
	i=0, j=0;
	while(i<size_files1&&j<size_files2)
	{
		if(CMP(files1[i], files2[j])==-1)
		{
			++i;
			continue;
		}
		if(CMP(files1[i], files2[j])==1)
		{
			++j;
			continue;
		}
		char *path_file1=malloc((strlen(path1)+strlen(files1[i])+2)*sizeof(char));
		strcpy(path_file1, path1);
		strcat(path_file1, "/");
		strcat(path_file1, files1[i]);
		char *full_path_file1=malloc((strlen(path_file1)+3)*sizeof(char));
		strcpy(full_path_file1, "./");
		strcat(full_path_file1, path_file1);
		struct stat info1;
		lstat(full_path_file1, &info1);
		char *path_file2=malloc((strlen(path2)+strlen(files2[j])+2)*sizeof(char));
		strcpy(path_file2, path2);
		strcat(path_file2, "/");
		strcat(path_file2, files2[j]);
		char *full_path_file2=malloc((strlen(path_file2)+3)*sizeof(char));
		strcpy(full_path_file2, "./");
		strcat(full_path_file2, path_file2);
		struct stat info2;
		lstat(full_path_file2, &info2);
		if(S_ISDIR(info1.st_mode)&&S_ISDIR(info2.st_mode))
			diff(path_file1, path_file2);
		++i, ++j;
	}
}
int main(int argc, char **argv)
{
	if(argc!=3)
	{
		puts("Incorrect input");
		return 0;
	}
	diff(argv[1], argv[2]);
	return 0;
}
