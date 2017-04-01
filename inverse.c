#include "aesutil.h"
#include "modprod.h"
#include "inverse.h"

int mulInv[256];

void initializeInv() {
	char str_inv[]  = "00018df6cb527bd1e84f29c0b0e1e5c7"
	    		  "74b4aa4b992b605f583ffdccff40eeb2"
	        	  "3a6e5af1554da8c9c10a98153044a2c2"
		    	  "2c45926cf3396642f235206f77bb5919"
		          "1dfe37672d31f569a764ab135425e909"
			  "ed5c05ca4c2487bf183e22f051ec6117"
			  "165eafd349a63643f44791df3393213b"
		          "79b7978510b5ba3cb670d006a1fa8182"
	 	          "837e7f809673be569b9e95d9f702b9a4"
			  "de6a326dd88a84722a149f88f9dc899a"
	  	          "fb7c2ec38fb8654826c8124acee7d262"
			  "0ce01fef11757871a58e763dbdbc8657"
			  "0b282fa3dad4e40fa92753041bfcace6"
			  "7a07ae63c5dbe2ea948bc4d59df8906b"
			  "b10dd6ebc60ecfad084ed7e35d501eb3"
			  "5b2338346846038cdd9c7da0cd1a411c";
	int i = 0;
	for(i = 0x00; i <= 0xff; i++) {
		mulInv[i] = (getDecFromHex(str_inv[2 * i]) << 4) | getDecFromHex(str_inv[(2 * i) + 1]);
	}
	return;
}

unsigned char getXORone(unsigned char ch) {
	int i = 0;
	for(i = 0x00; i < 0xff; i++) {
		if((ch ^ i) == 0x01)
			return i;
	}
	return -1;
}

void getQuoAndRem(unsigned char dividend[5], unsigned char divisor[4], unsigned char quo[4]) {
	int dd_size = 5;
	int dd_i = 0;
	for(dd_i = 0; dd_i < 5; dd_i++) {
		if(dividend[dd_i] != 0) 
			break;
		dd_size--;
	}
	int ds_size = 4;
	int ds_i = 0;
	for(ds_i = 0; ds_i < 4; ds_i++) {
		if(divisor[ds_i] != 0)
			break;
		ds_size--;
	}
	int q_i = 0;
	ds_i = 0;
	unsigned char temp_quo[4];
	memset(temp_quo, 0, 4);
	for(dd_i = (5 - dd_size); dd_i < (5 - ds_size + 1); dd_i++) {
		while(divisor[ds_i] == 0)
			ds_i++;
		unsigned char inv = mulInv[divisor[ds_i]];
		unsigned char q[4];
		memset(q, 0, 4);
		if(dividend[dd_i] == 0x01) {
			if((ds_size == 1) && (dd_i == (5 - ds_size))) {
				inv = 0x00;
			}
			q[3] = inv;
		} else {
			int val = dividend[dd_i];
			if((ds_size == 1) && (dd_i == (5 - ds_size))) {
				val = getXORone(val);
			}
			q[3] = dotprod(inv, val);
		}
		temp_quo[q_i++] = q[3]; 
		char sdivis[8];
		memset(sdivis, 0, 8);
		getStringFromHex(divisor, sdivis);
		char sq[8];
		memset(sq, 0, 8);
		getStringFromHex(q, sq);
		unsigned char mulval[ds_size]; // TODO change this declaration to malloc
		unsigned char out[4];
		modprod(sdivis, sq, out, 4);
		int j = 0;
		for(j = 0; j < 4; j++) {
			mulval[j] = out[3 - j];
		}
		j = dd_i;
		for(j = dd_i; j < (dd_i + ds_size); j++) {
			dividend[j] = (dividend[j] ^ mulval[j + (4 - ds_size) - dd_i]);
		}
	}
	int i = 0;
	memset(quo, 0, 4);
	int j = (4 - q_i);
	for(i = 0; i < 4; i++) {
		if(temp_quo[i] > 0) {
			quo[j++] = temp_quo[i];
		} else {
			break;
		}
	}
	return;
}

void displayPoly(unsigned char *poly, int len) {
	int i = 0;
	for(i = 0; i < len; i++) {
		printf("{%02x}", *(poly + i));
	}
	return;
}

int isValid(char *poly) {
	int i = 0;
	for(i = 0; i < 8; i++) {
		if(!((poly[i] >= '0') && (poly[i] <= '9')) && !((poly[i] >= 'a') && (poly[i] <= 'f'))) {
			return 0;
		}
	}
	if(poly[i] != '\0') {
		return 0;
	}
	return 1;
}

void inverse(char *poly) {
	if(!isValid(poly)) {
		fprintf(stderr, "Invalid polynomial, invalid/not enough characters\n");
		exit(1);
	}
	initializeInv();
	unsigned char dividend[5];
	memset(dividend, 0, 5);
	dividend[0] = 0x01;
	dividend[4] = 0x01;
	unsigned char quo[4];
	memset(quo, 0, 4);
	/*
	 * in comparision with algorithm, here divisor is rem[i]
	 * and rem is rem[i-1]
	 */
	unsigned char rem[5];
	memset(rem, 0, 5);
	unsigned char divisor[4];
	int i = 0;
	for(i = 0; i < 4; i++) {
		divisor[i] = (((getDecFromHex(poly[2 * i])) << 4) | getDecFromHex(poly[(2 * i) + 1]));
	}
	int inv_found = 1;
	unsigned char aux[6][4];
	for(i = 0; i < 6; i++) {
		memset(aux[i], 0, 4);
	}
	aux[1][3] = 0x01;
	printf("i=1, rem[i]=");
	displayPoly((dividend + 1), 4);
	printf(", quo[i]=");
	displayPoly(quo, 4);
	printf(", aux[i]=");
	displayPoly(aux[0], 4);
	printf("\n");
	for(i = 0; i < 5; i++) {
		printf("i=%d, rem[i]=", (i + 2));
		displayPoly(divisor, 4);
		printf(", quo[i]=");
		displayPoly(quo, 4);
		printf(", aux[i]=");
		displayPoly(aux[i+1], 4);
		printf("\n");
		int j = 0;
		for(j = 0; j < 4; j++) {
			if(divisor[j] != 0)
				break;
		}
		if((j == 3) && divisor[j] == 0x01) {
			break;
		}
		if(j == 4) {
			inv_found = 0;
			break;
		}
		memset(rem, 0, 5);
		copyCharArray((char *)dividend, (char*)rem, 5);
		getQuoAndRem(rem, divisor, quo);
		memset(dividend, 0, 5);
		copyCharArray((char *)divisor, (char *)(dividend + 1), 4);
		memset(divisor, 0, 4);
		copyCharArray((char *)(rem + 1), (char *)divisor, 4);
		char saux[8];
		char squo[8];
		getStringFromHex(aux[i+1], saux);
		getStringFromHex(quo, squo);
		unsigned char out[4];
		modprod(saux, squo, out, 4);
		for(j = 0; j < 4; j++) {
			aux[i+2][j] = out[3 - j];
		}
		for(j = 0; j < 4; j++) {
			aux[i+2][j] = (aux[i+2][j] ^ aux[i][j]);
		}
	}
	int j = 0;
	for(j = 0; j < 4; j++) {
		divisor[j] = (((getDecFromHex(poly[2 * j])) << 4) | getDecFromHex(poly[(2 * j) + 1]));
	}
	if(inv_found) {
		printf("Multiplicative inverse of ");
		displayPoly(divisor, 4);
		printf(" is ");
		displayPoly(aux[i+1], 4);
	} else {
		displayPoly(divisor, 4);
		printf(" does not have a multiplicative inverse.");
	}
	printf("\n");
	return;
}
