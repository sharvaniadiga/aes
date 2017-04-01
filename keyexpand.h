#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#include "utilities.h"
#include "aesutil.h"
#include "modprod.h"

void KeyExpansion(char *, unsigned char [][4], int );
void keyexpand(char*, char*);
