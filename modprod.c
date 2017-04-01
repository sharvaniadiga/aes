#include "modprod.h"
#include "utilities.h"

unsigned char xtimes(unsigned char a) {
	unsigned char out = a;
	out = (out << 1);
	if(getNthBit(a, 7) == 1) {
		out = (out ^ 0x1B);
	}
	return out;
}

unsigned char dotprod(unsigned char a, unsigned char b) {
	unsigned char p = 0x00;
	unsigned char A = a;
	unsigned char V[8];
	memset(V, 0, sizeof(V));
	V[0] = A;
	unsigned char B = b;
	if(getNthBit(B, 0) == 1) {
		p = A;
	}
	int j = 0;
	for(j = 1; j <= 7; j++) {
		V[j] = xtimes(A);
		A    = V[j];
		if(getNthBit(B, j) == 1)
			p = (A ^ p);
	}
	return p;
}

void modprod(char *poly1, char *poly2, unsigned char d[], int outlen)
{
	unsigned char p1[4]; // TODO verify this to check if it is always 4
	unsigned char p2[4];
	int i = 0;
	int index = 0;
	memset(p1, 0, sizeof(p1));
	memset(p2, 0, sizeof(p2));
	for(i = 3; i >= 0; i--) {
		p1[index] = (((getDecFromHex(poly1[2 * i])) << 4) | getDecFromHex(poly1[(2 * i) + 1]));
		p2[index] = (((getDecFromHex(poly2[2 * i])) << 4) | getDecFromHex(poly2[(2 * i) + 1]));
		index++;
	}
	d[0] = (dotprod(p1[0], p2[0]) ^ dotprod(p1[3], p2[1]) ^ dotprod(p1[2], p2[2]) ^ dotprod(p1[1], p2[3]));
	d[1] = (dotprod(p1[1], p2[0]) ^ dotprod(p1[0], p2[1]) ^ dotprod(p1[3], p2[2]) ^ dotprod(p1[2], p2[3]));
	d[2] = (dotprod(p1[2], p2[0]) ^ dotprod(p1[1], p2[1]) ^ dotprod(p1[0], p2[2]) ^ dotprod(p1[3], p2[3]));
	d[3] = (dotprod(p1[3], p2[0]) ^ dotprod(p1[2], p2[1]) ^ dotprod(p1[1], p2[2]) ^ dotprod(p1[0], p2[3]));
	return;
}
