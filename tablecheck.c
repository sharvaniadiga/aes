#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#include "utilities.h"
#include "modprod.h"

int isCharsNotHexValues(char *line, int sindex, int len) {
	int i = 0;
	for(i = sindex; i < len; i++) {
		char ch = line[i];
		if(!((ch >= '0') && (ch <= '9')) && !((ch >= 'a') && (ch <= 'f'))) {
			return i;
		}
	}
	return 0;
}

int hasAllValuesInSBox(char *line, int sindex, int len) {
	int val[256];
	int i = 0;
	for(i = 0; i < 256; i++) {
		val[i] = 0;
	}
	for(i = 0; i < (256); i++) {
		int tabval = (getDecFromHex(line[sindex + (2 * i)]) << 4) | getDecFromHex(line[sindex + (2 * i) + 1]);
		if(val[tabval] != 0) 
			return 0;
		val[tabval] = 1;
	}
	return 1;
}

void tablecheck(char *tablefile)
{
	FILE *fp = fopen(tablefile, "r");
	if(!fp) {
		fprintf(stderr, "%s\n", "Error in opening tablefile");
		exit(1);
	}
	char line[1024];
	int tablesread = 0;
	int lcount = 0;
	char p[8];
	char invp[8];
	while(!feof(fp)) {
		if(((lcount = readLine(fp, line, 1024)) == -1) && !feof(fp)) {
			fprintf(stderr, "%s\n", "Error in reading tablefile");
			fclose(fp);
			exit(1);
		}
		if(feof(fp))    break;
		lcount--; //excluding the string terminator
		if(!strncmp(line, "S", 1)) {
			int sindex = 2;
			int valcount = lcount - sindex;
			if((valcount % 2) != 0) {
				fprintf(stderr, "%s\n", "Error: S table doesn't have full byte at the end");
				fclose(fp);
				exit(1);
			}
			if(valcount != 512) {
				fprintf(stderr, "%s\n", "Error: S table doesn't have 256 entries");
				fclose(fp);
				exit(1);
			}
			int out = 0;
			if((out = isCharsNotHexValues(line, sindex, lcount)) > 0) {
				fprintf(stderr, "%s%d\n", "Error: S table has invalid character at index ", out+1);
				fclose(fp);
				exit(1);
			}
			if(!hasAllValuesInSBox(line, sindex, lcount)) {
				fprintf(stderr, "%s\n", "Error: S table has repeated entries");
				fclose(fp);
				exit(1);
			}
			tablesread++;
		} else if(!strncmp(line, "P", 1)) {
			int sindex = 2; //removing key P and = from line
			int valcount = lcount - sindex;
			if(((valcount % 2) != 0) || ((valcount / 2) != 4)) {
				fprintf(stderr, "%s\n", "Error: P table should consists of 4 bytes");
				fclose(fp);
				exit(1);
			}
			int out = 0;
			if((out = isCharsNotHexValues(line, sindex, lcount)) > 0) {
				fprintf(stderr, "%s%d\n", "Error: P table has invalid character at index  ", out+1);
				fclose(fp);
				exit(0);
			}
			copyCharArray((line + sindex), p, 8);
			tablesread++;
		} else if(!strncmp(line, "INVP", 4)) {
			int sindex = 5; //removing key INVP and = from line
			int valcount = lcount - sindex;
			if(((valcount % 2) != 0) || ((valcount / 2) != 4)) {
				fprintf(stderr, "%s\n", "Error: INVP table should consists of 4 bytes");
				fclose(fp);
				exit(1);
			}
			int out = 0;
			if((out = isCharsNotHexValues(line, sindex, lcount)) > 0) {
				fprintf(stderr, "%s%d\n", "Error: INVP table has invalid character at index  ", out+1);
				fclose(fp);
				exit(0);
			}
			copyCharArray((line + sindex), invp, 8);
			tablesread++;
		} else {
			fprintf(stderr,"%s\n", "Error: extra entry other than tables S, P, INVP, is present");
			fclose(fp);
			exit(1);
		}
	}
	if(tablesread != 3) {
		fprintf(stderr, "%s\n", "One or more tables missing");
		fclose(fp);
		exit(1);
	}
	unsigned char res[4];
	modprod(p, invp, res, 4);
	if((res[0] != 0x01) || (res[2] != 0x00) || (res[1] != 0x00) || (res[3] != 0x00)) {
		fprintf(stderr, "%s\n", "Product of the polynomials represented by P and INVP is not {00}{00}{00}{01}");
		fclose(fp);
		exit(1);
	}
	fclose(fp);
	return;
}
