#include "aesutil.h"
#include "tablecheck.h"

extern char Ax[8];
extern char invAx[8];

void initialize(char *tablefile) {
	tablecheck(tablefile);
	FILE *fp = fopen(tablefile, "r");
	if(!fp) {
		fprintf(stderr, "%s\n", "Unable to open tablefile");
		exit(1);
	}
	char line[MAX_CHAR];
	while(!feof(fp)){
		readLine(fp, line, MAX_CHAR);
		if(line[0] == '\0') break;
		if(!strncmp(line, "S", 1)) {
			char values[MAX_CHAR];
			strncpy(values, (line + 1 + 1), (sizeof(line) - 2));
			set2DArray(S, S_ROWS, S_COLS, values);
		} else if(!strncmp(line, "P", 1)) {
			char values[MAX_CHAR];
			strncpy(values, (line + 1 + 1), (sizeof(line) - 2));
			copyCharArray(values, Ax, 8);
		} else if(!strncmp(line, "INVP", 4)) {
			char values[MAX_CHAR];
			strncpy(values, (line + 4 + 1), (sizeof(line) - 5));
			copyCharArray(values, invAx, 8);
		}
	}
	fclose(fp);
	return;
}

void set2DArray(unsigned char array[][16], int rows, int cols, char *values)
{
	int i = 0;
	int j = 0;
	int k = 0;
	for(k = 0; k < (2 * rows * cols); k = (k + 2)) {
		array[i][j] = (getDecFromHex(values[k]) << 4) | (getDecFromHex(values[k + 1]));
		j++;
		if((j % cols) == 0) {
			i++;
			j = 0;
		}
	}
	return;
}

void SubWord(unsigned char a[4], unsigned char arr[S_ROWS][S_COLS]) {
	unsigned char temp[Nb];
	copyCharArray((char *)a, (char *)temp, Nb);
	memset(a, 0, Nb);
	int i = 0;
	for(; i < Nb; i++) {
		a[i] = arr[(temp[i] >> 4)][(temp[i] & 0x0f)];
	}
	return;
}

void RotWord(unsigned char a[4]) {
	unsigned char temp = a[0];
	int i = 0;
	for( ; i < (Nb - 1); i++) {
		a[i] = a[i + 1];
	}
	a[i] = temp;
	return;
}

void getStringFromHex(unsigned char in[4], char out[8]) {
	int i = 0;
	int k = 0;
	memset(out, 0, 8);
	for(i = 0; i < 4; i++) {
		int tens = (in[i] / 16);
		int units = (in[i] % 16);
		if(tens <= 0x09) {
			out[k++] = tens + '0';
		} else {
			out[k++] = (tens - 10) + 'a';
		}
		if(units <= 0x09) {
			out[k++] = units + '0';
		} else {
			out[k++] = (units - 10) + 'a';
		}
	}
	return;
}
