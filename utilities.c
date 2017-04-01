#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>


/*
 * Begin code I did not write
 * The following code was obtained from http://stackoverflow.com/questions/2372813/reading-one-line-at-a-time-in-c
 */
int readLine(FILE *fp, char *buffer, size_t len)
{
	int i = 0;
	memset(buffer, 0, len);
	for(i = 0; i < len; i++) {
		int ch = fgetc(fp);
		if(!feof(fp)) {
			if(ch == '\n') {
				buffer[i] = '\0';
				return i + 1;
			}
			else
				buffer[i] = ch;
		} else {
			return -1;
		}
	}
	return -1;
}
/*
 * End code I did not write
 */

void initializeArray(int* array, int len, int value) {
	int i = 0;
	for(i = 0; i < len; i++) {
		array[i] = 0;
	}
	return;
}

/*
 * Function to open file - filename
 */
FILE* openFileOrStdin(char* filename)
{
	FILE* fp = NULL;
	if(filename == NULL)
	{
		fp = stdin;
	} else {
		fp = fopen(filename, "r");
	}
	if(fp == NULL)
	{
		perror("ERROR");
	}
	return fp;
}

void copyCharArray(char *old, char* new, int len)
{
	int i = 0;
	memset(new, 0, len);
	for(i = 0; i < len; i++) {
		new[i] = old[i];
	}
	return;
}

/*
 * Function to print char in hex
 */
void printCharArrayInHex(unsigned char *array, int len, FILE *fp)
{
	int i = 0;
	for(i = 0; i < len; i++) {
		fprintf(fp, "%02x", array[i]);
	}
	return;
}

/*
 * Function to get Nth bit in byte b7b6b5..b0
 * n should be between 0 to 7
 */
int getNthBit(unsigned char byte, int n)
{
	int nthbit;
	nthbit = (byte & (1 << n))? 1 : 0;
	return nthbit;
}

int getDecFromHex(unsigned char ch)
{
	int dec = (isalpha(ch)?(10 + (ch - 'a')) : (ch - '0'));
	return dec;
}
