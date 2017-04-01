#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "keyexpand.h"
#include "aes.h"

extern char Ax[8];
extern char invAx[8];
extern unsigned char invS[S_ROWS][S_COLS];

void AddRoundKey(unsigned char state[4][4], unsigned char w[Nb * (Nr + 1)][4], int round) {
	int c = 0;
	int r = 0;
	for(c = 0; c < 4; c++) {
		for(r = 0; r < 4; r++) {
			state[r][c] = (state[r][c] ^ w[4 * round + c][r] );
		}
	}
	return;
}

void ShiftRows(unsigned char state[4][4]) {
	int i = 1; //to ignore the first row
	int j = 1; 
	for(i = 1; i < 4; i++) {
		for(j = 1; j <= i; j++) {
			RotWord(state[i]);
		}
	}
	return;
}

void MixColumns(unsigned char state[4][4], char A[8]) {
	int i = 0;
	int j;
	for(i = 0; i < 4; i++) {
		int k = 0;	
		char poly[8];
		memset(poly, 0, 8);
		for(j = 3; j >= 0; j--) {
			if(state[j][i] <= 0xf) {
				poly[2*k] = '0';
				sprintf((poly + (2*k) + 1), "%x", state[j][i]);
			} else {
				sprintf((poly + 2*k), "%x", state[j][i]);
			}
			k++;
		}
		unsigned char d[4];
		modprod(A, poly, d, 4);
		for(k = 3; k >= 0; k--) {
			state[k][i] = d[k];
		}
	}
	return;
}

void DisplayColMajor(unsigned char a[4][4], int row, int col) {
	int r = 0;
	int c = 0;
	for(c = 0; c < col; c++) {
		for(r = 0; r < row; r++) {
			printf("%02x", a[r][c]);
		}
	}
	printf("\n");
	return;
}

void AesEncrypt(unsigned char in[4 * Nb], unsigned char out[4 * Nb], unsigned char w[Nb * (Nr + 1)][4]) {
	int i = 0;
	int j = 0;
	int k = 0;
	unsigned char state[4][4];
	for(i = 0; i < (4 * Nb); i++) {
		state[j++][k] = in[i];
		if((j % 4) == 0) { // column major order
			k++;
			j = 0;
		}
	}
	int round = 0;
	printf("round[%2d].input    ", round);
	DisplayColMajor(state, 4, 4);
	printf("round[%2d].k_sch    ", round);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			printf("%02x", w[i][j]);
		}
	}
	printf("\n");
	AddRoundKey(state, w, 0);
	for(round = 1; round < Nr; round++) {
		printf("round[%2d].start    ", round);
		DisplayColMajor(state, 4, 4);
		for(i = 0; i < 4; i++) {
			SubWord(state[i], S);
		}
		printf("round[%2d].s_box    ", round);
		DisplayColMajor(state, 4, 4);
		ShiftRows(state);
		printf("round[%2d].s_row    ", round);
		DisplayColMajor(state, 4, 4);
		MixColumns(state, Ax);
		printf("round[%2d].m_col    ", round);
		DisplayColMajor(state, 4, 4);
		AddRoundKey(state, w, round);
		printf("round[%2d].k_sch    ", round);
		for(i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++) {
				printf("%02x", w[(4 * round) + i][j]);
			}
		}
		printf("\n");
	}
	printf("round[%2d].start    ", round);
	DisplayColMajor(state, 4, 4);
	for(i = 0; i < 4; i++) {
		SubWord(state[i], S);
	}
	printf("round[%2d].s_box    ", round);
	DisplayColMajor(state, 4, 4);
	ShiftRows(state);
	printf("round[%2d].s_row    ", round);
	DisplayColMajor(state, 4, 4);
	AddRoundKey(state, w, round);
	printf("round[%2d].k_sch    ", round);
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			printf("%02x", w[(4 * round) + i][j]);
		}
	}
	printf("\n");
	printf("round[%2d].output   ", round);
	DisplayColMajor(state, 4, 4);
	return;
}

void encrypt(char *key, char *tablefile, char *filename)
{
	initialize(tablefile);
	FILE *fp = openFileOrStdin(filename);
	if(!fp) {
		exit(1);
	}
	unsigned char w[Nb * (Nr + 1)][4];
	KeyExpansion(key, w, 4);
	unsigned char in[16];
	while(!feof(fp)) {
		memset(in, 0, 16);
		int nbytes = 0;
		if((nbytes = fread(in, 16, 1, fp)) == 0) break;
		unsigned char out[16];
		AesEncrypt(in, out, w);
	}
	fclose(fp);
	return;
}

void InvShiftRows(unsigned char state[4][4]) {
	int i = 1; //to ignore the first row
	int j = 1; 
	for(i = 1; i < 4; i++) {
		for(j = 1; j <= i; j++) {
			int k = Nb - 1;
			unsigned char temp = state[i][k];
			for( ; k > 0; k--) {
				state[i][k] = state[i][k - 1];
			}
			state[i][k] = temp;
		}
	}
	return;
}

void getInvSBox() {
	int i = 0;
	int j = 0;
	for(i = 0; i < 16; i++) {
		for(j = 0; j < 16; j++) {
			int r = (S[i][j] / 16);
			int c = (S[i][j] % 16);
			invS[r][c] = (i << 4) | j;
		}
	}
	return;
}

void AesDecrypt(unsigned char in[4 * Nb], unsigned char out[4 * Nb], unsigned char w[Nb * (Nr + 1)][4]) {
	int i = 0;
	int j = 0;
	int k = 0;
	unsigned char state[4][4];
	getInvSBox();
	for(i = 0; i < (4 * Nb); i++) {
		state[j++][k] = in[i];
		if((j % 4) == 0) { // column major order
			k++;
			j = 0;
		}
	}
	int round = Nr;
	printf("round[%2d].iinput   ", (Nr - round));
	DisplayColMajor(state, 4, 4);
	printf("round[%2d].ik_sch   ", (Nr - round));
	for(i = (Nr * Nb); i <= (((Nr + 1)*Nb) - 1); i++) {
		for(j = 0; j < 4; j++) {
			printf("%02x", w[i][j]);
		}
	}
	printf("\n");
	AddRoundKey(state, w, round);
	for(round = (Nr - 1); round > 0; round--) {
		printf("round[%2d].istart   ", (Nr - round));
		DisplayColMajor(state, 4, 4);
		InvShiftRows(state);
		printf("round[%2d].is_row   ", (Nr - round));
		DisplayColMajor(state, 4, 4);
		for(i = 0; i < 4; i++) {
			SubWord(state[i], invS);
		}
		printf("round[%2d].is_box   ", (Nr - round));
		DisplayColMajor(state, 4, 4);
		printf("round[%2d].ik_sch   ", (Nr - round));
		for(i = 0; i < 4; i++) {
			for(j = 0; j < 4; j++) {
				printf("%02x", w[(4 * round) + i][j]);
			}
		}
		printf("\n");
		AddRoundKey(state, w, round);
		printf("round[%2d].ik_add   ", (Nr - round));
		DisplayColMajor(state, 4, 4);
		MixColumns(state, invAx);
	}
	printf("round[%2d].istart   ", (Nr - round));
	DisplayColMajor(state, 4, 4);
	InvShiftRows(state);
	printf("round[%2d].is_row   ", (Nr - round));
	DisplayColMajor(state, 4, 4);
	for(i = 0; i < 4; i++) {
		SubWord(state[i], invS);
	}
	printf("round[%2d].is_box   ", (Nr - round));
	DisplayColMajor(state, 4, 4);
	AddRoundKey(state, w, round);
	printf("round[%2d].ik_sch   ", (Nr - round));
	for(i = 0; i < 4; i++) {
		for(j = 0; j < 4; j++) {
			printf("%02x", w[(4 * round) + i][j]);
		}
	}
	printf("\n");
	printf("round[%2d].ioutput  ", (Nr - round));
	DisplayColMajor(state, 4, 4);
	return;
}

void decrypt(char *key, char *tablefile, char *filename)
{
	initialize(tablefile);
	FILE *fp = openFileOrStdin(filename);
	if(!fp) {
		exit(1);
	}
	unsigned char w[Nb * (Nr + 1)][4];
	KeyExpansion(key, w, 4);
	unsigned char in[16];
	while(!feof(fp)) {
		memset(in, 0, 16);
		int nbytes = 0;
		if((nbytes = fread(in, 16, 1, fp)) == 0) break;
		unsigned char out[16];
		AesDecrypt(in, out, w);
	}
	fclose(fp);
	return;
}
