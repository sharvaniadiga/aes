#include "tablecheck.h"
#include "modprod.h"
#include "keyexpand.h"
#include "aes.h"
#include "inverse.h"

#define TABLECHECK "tablecheck"
#define MODPROD    "modprod"
#define KEYEXPAND  "keyexpand"
#define ENCRYPT    "encrypt"
#define DECRYPT    "decrypt"
#define INVERSE    "inverse"

int Nb = 4;
int Nr = 10;
unsigned char S[S_ROWS][S_COLS];
unsigned char invS[S_ROWS][S_COLS];
char Ax[8];
char invAx[8];

int main(int argc, char** argv)
{
	if((argc < 3) || (argc > 5)){
		fprintf(stderr, "%s\n", "Malformed command");
		exit(1);
	}
	if(!strcmp(argv[1], TABLECHECK)) {
		if(argc != 3) {
			fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 tablecheck -t=tablefile");
			exit(1);
		}
		char *tablefile = NULL;
		int index = 2;
		while(index < argc) {
			char *token = strtok(argv[index], "=");
			if(!strcmp(token, "-t")) {
				tablefile = strtok(NULL, "=");
			} else {
				fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 tablecheck -t=tablefile");
				exit(1);
			}
			index++;
		}
		tablecheck(tablefile);
	} else if(!strcmp(argv[1], MODPROD)){
		if(argc != 4) {
			fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 modprod -p1=poly1 -p2=poly2");
			exit(1);
		}
		char *poly1 = NULL;
		char *poly2 = NULL;
		int index = 2;
		while(index < argc) {
			char* token = strtok(argv[index], "=");
			if(!strcmp(token, "-p1")) {
				poly1 = strtok(NULL, "=");
			} else if(!strcmp(token, "-p2")) {
				poly2 = strtok(NULL, "=");
			} else {
				fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 modprod -p1=poly1 -p2=poly2");
				exit(1);
			}
			index++;
		}
		unsigned char out[4];
		memset(out, 0, 4);
		modprod(poly1, poly2, out, 4);
		int i = 0;
		for(i = 0; i < 4; i++) {
			printf("{%c%c}", poly1[2*i], poly1[(2*i) + 1]);
		}
		printf(" CIRCLEX ");
		for(i = 0; i < 4; i++) {
			printf("{%c%c}", poly2[2*i], poly2[(2*i) + 1]);
		}
		printf(" = ");
		for(i = 3; i >= 0; i--) {
			printf("{%02x}", out[i]);
		}
		printf("\n");
	} else if(!strcmp(argv[1], KEYEXPAND)) {
		if(argc != 4) {
			fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 keyexpand -k=key -t=tablefile");
			exit(1);
		}
		char *key = NULL;
		char *tablefile = NULL;
		int index = 2;
		while(index < argc) {
			char *token = strtok(argv[index], "=");
			if(!strcmp(token, "-k")) {
				key = strtok(NULL, "=");	
			} else if(!strcmp(argv[index], "-t")) {
				tablefile = strtok(NULL, "=");
			}else {
				fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 keyexpand -k=key -t=tablefile");
				exit(1);
			} 
			index++;
		}
		keyexpand(key, tablefile);
	}else if(!strcmp(argv[1], ENCRYPT)) {
		if((argc != 4) && (argc != 5)) {
			fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 encrypt -k=key -t=tablefile [file]");
			exit(1);
		}
		char *tablefile = NULL;
		char *key = NULL;
		char *filename = NULL;
		int index = 2;
		while(index < argc) {
			char *token = strtok(argv[index], "=");
			if(!strcmp(token, "-t")) {
				tablefile = strtok(NULL, "=");
			} else if(!strcmp(token, "-k")) {
				key = strtok(NULL, "=");
				/*if(key && (checkKey(key, strlen(key), 0) == -1)) {
					exit(1);
				}*/
			} else if(!filename) {
				filename = token;	
			} else {
				fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 encrypt -k=key -t=tablefile [file]");
				exit(1);
			}
			index++;
		}
		encrypt(key, tablefile, filename);
	} else if(!strcmp(argv[1], DECRYPT)) {
		if((argc != 4) && (argc != 5)) {
			fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 decrypt -k=key -t=tablefile [file]");
			exit(1);
		}
		char *tablefile = NULL;
		char *key = NULL;
		char *filename = NULL;
		int index = 2;
		while(index < argc) {
			char *token = strtok(argv[index], "=");
			if(!strcmp(token, "-t")) {
				tablefile = strtok(NULL, "=");
			} else if(!strcmp(token, "-k")) {
				key = strtok(NULL, "=");
				/*if(key && (checkKey(key, strlen(key), 0) == -1)) {
					exit(1);
				}*/
			} else if(!filename) {
				filename = token;	
			} else {
				fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 decrypt -k=key -t=tablefile [file]");
				exit(1);
			}
			index++;
		}
		decrypt(key, tablefile, filename);
	} else if(!strcmp(argv[1], INVERSE)) {
		if(argc != 3) {
			fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 inverse -p=poly");
			exit(1);
		}
		char *poly = NULL;
		int index = 2;
		while(index < argc) {
			char *token = strtok(argv[index], "=");
			if(!strcmp(token, "-p")) {
				poly = strtok(NULL, "=");	
			} else{
				fprintf(stderr, "%s\n", "Malformed command. Usage: hw5 inverse -p=poly");
				exit(1);
			}
			index++;
		}
		inverse(poly);
	} else {
		fprintf(stderr, "%s\n", "Malformed command: Please specify appropriate operation");
		exit(1);
	}
	return 0;
}
