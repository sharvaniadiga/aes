#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

#define MAX_CHAR 1024

int readLine(FILE *, char *, size_t);
void initializeArray(int* , int, int);
FILE* openFileOrStdin(char*);
void copyCharArray(char *, char*, int);
void printCharArrayInHex(unsigned char *, int, FILE*);
int getNthBit(unsigned char , int);
int getDecFromHex(unsigned char);
