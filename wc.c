#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

/* print msg, followed by the usage message, and exit the program */
void print_usage(char *msg);

/* return an array that has the number of lines, words, and characters in filename */
/* should pass in a filename of "" to indicate to read from stdin */
int *get_counts(char *filename);

/* print the indicated counts for file name */
/* show should be an array of three ints that indicates if the number of lines,
 * words, and characters should be printed */
/* count is an array of the three counts */
/* name is the name to print after the counts */
void print_counts(int *show, int *count, char *name);

void print_usage(char *msg){
	printf("%s\n\n", msg);
	printf("Usage: wc [-1] [-w] [-c] [FILES. . .]\nwhere:\n\t-l\t prints the number of lines\n\t-w\t prints the number of words\n\t -c prints the number of characters\n\tFILES if no files are given, then read from standard input\n");
	exit(1);

}

void print_counts(int *show, int *count, char *name){
	int allthree = 0;
	if (show[0] == 0 && show[1] == 0 && show[2] == 0){
		allthree = 1;
	}

	for (int i =0; i < 3; ++i){
		if (show[i] != 0 || allthree == 1)
		{
			printf("%8d ", count[i]);
		}
	}
	printf("%s\n", name);
	
}


int *get_counts(char *filename){	
	int threecount[3];
	int fd; 
	fd = open(filename, O_RDONLY);
	if (fd == -1){
		print_usage("open failed");
	} 
	if (filename[0] =='\0'){
		fd = 0;
	}
	char *c = (char*) calloc(10000000, sizeof(char));
	int fileread = read(fd,c, 10000000);
	int whitespace = 1;
	for (int i =0; i < fileread; ++i){
		if (fileread == 0){
			printf("END OF FILE");
			break;
		}
		else{
			threecount[0] += 1;
			
			if (c[i]== '\n'){
				threecount[1] +=1;
			}
			if (isspace(c[i])>0 && whitespace == 0){
				threecount[2] +=1;
			whitespace = 1; 
			}
			if (isgraph(c[i])>0 && whitespace == 1)	{
				whitespace = 0;
		}
		}	
		}
        
	close(fd);

	int *threepoint = threecount;
	return threepoint;

	
}


int main(int argc, char **argv)
{
	int showcount[3];
	int totalcount[3];
	for (int i = 1; i < argc; ++i){
		int counts[3];
		if (argv[i][0] == '-'){
			if (strcmp(argv[i], "-l")== 0){
				showcount[0] += 1;
				continue;
			}

			if (strcmp(argv[i], "-w")==0){
				showcount[1] += 1;
				continue;
			}
			if (strcmp(argv[i], "-c")==0){
				showcount[2] +=1;
				continue;
			}
			else{
			print_usage("inavlid argument");
			}
		}
		else{
			int *cp = get_counts(argv[i]);
			counts[0] += *cp;
			counts[1] += *(cp+1);
			counts[2] += *(cp+2);
			totalcount[0] += *cp; 
			totalcount[1] += *(cp+1);
			totalcount[2] += *(cp+2);
			int *showpointer = showcount; 
			int *countpointer = counts;
		        char *namepointer = argv[i];	
			print_counts(showpointer, countpointer, namepointer);
			if (i == argc -1){
				namepointer = "Total";
				print_counts(showpointer, totalcount, namepointer);
			}
			counts[0] = 0;
			counts[1] = 0;
			counts[2] = 0; 
			cp[0] = 0;
			cp[1] = 0; 
			cp[2] = 0;
		}
	}

}


