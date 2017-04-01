#include "aesutil.h"
#include "keyexpand.h"

extern int Nb;
extern int Nr;
extern unsigned char S[S_ROWS][S_COLS];
unsigned char Rcon[11][4];

void Rcon_initialize() {
	unsigned char coef = 0x02;
	memset(Rcon[1], 0, 4);
	Rcon[1][0] = 1; // 0x02 pow 0
	Rcon[2][0] = coef;
	int i = 3;
	for(; i < (Nr + 1); i++) {
		memset(Rcon[i], 0, 4);
		Rcon[i][0] = xtimes(Rcon[i - 1][0]);
	}
	return;
}

void KeyExpansion(char *key, unsigned char w[Nb *(Nr + 1)][4], int Nk) {
	int i = 0;
	int k = 0;
	Rcon_initialize();
	for(i = 0; i < Nk; i++) {
		int j = 0;
		for(j = 0; j < 4; j++) {
			w[i][j] = (getDecFromHex(key[k]) << 4) | (getDecFromHex(key[k + 1]));
			k = k + 2;
		}
	}
	for(i = Nk; i < (Nb * (Nr + 1)); i++) {
		unsigned char temp[4];
		copyCharArray((char*)w[i-1], (char*)temp, 4);
		if((i % Nk) == 0) {
			RotWord(temp);
			SubWord(temp, S);
			int j = 0;
			for(j = 0; j < 4; j++) {
				temp[j] = (temp[j] ^ Rcon[(i / Nk)][j]);
			}
		}
		int j = 0;
		for(j = 0; j < 4; j++) {
			w[i][j] = (w[(i - Nk)][j] ^ temp[j]);
		}
	}
	return;
}

int isValidKey(char *key) {
	int i = 0;
	for(i = 0; i < 32; i++) {
		if(!((key[i] >= '0') && (key[i] <= '9')) && !((key[i] >= 'a') && (key[i] <= 'f'))) {
			return 0;	
		}
	}
	if(key[i] != '\0') {
		return 0;
	}
	return 1;
}

void keyexpand(char *key, char *tablefile) {
	if(!isValidKey(key)) {
		fprintf(stderr, "Bad key, invalid/not enough character(s)\n");
		exit(1);
	}
	initialize(tablefile);
	unsigned char w[Nb * (Nr + 1)][4];
	KeyExpansion(key, w, 4);
	int i = 0;
	for(i = 0; i < (Nb *(Nr + 1)); i++) {
		printf("w[%2d]: %02x%02x%02x%02x\n", i, w[i][0], w[i][1], w[i][2], w[i][3]);
	}
	return;
}
