#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "utilities.h"

#define S_ROWS 16
#define S_COLS 16

extern int Nb;
extern int Nr;
extern unsigned char S[S_ROWS][S_COLS];

void initialize(char *);
void set2DArray(unsigned char [][16], int , int , char *);
void SubWord(unsigned char [4], unsigned char[S_ROWS][S_COLS]);
void RotWord(unsigned char [4]);
void getStringFromHex(unsigned char [4], char [8]);
