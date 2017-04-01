hw5: hw5.o tablecheck.o modprod.o keyexpand.o aes.o inverse.o utilities.o aesutil.o
	gcc -o hw5 -g hw5.o tablecheck.o modprod.o aesutil.o keyexpand.o utilities.o aes.o inverse.o -L/home/scf-22/csci551b/openssl/lib -lcrypto -lm

hw5.o: hw5.c 
	gcc -g -c -Wall hw5.c

aes.o: aes.c aes.h
	gcc -g -c -Wall aes.c

keyexpand.o: keyexpand.c keyexpand.h
	gcc -g -c -Wall keyexpand.c

modprod.o: modprod.c modprod.h
	gcc -g -c -Wall modprod.c

tablecheck.o: tablecheck.c tablecheck.h 
	gcc -g -c -Wall tablecheck.c 

inverse.o: inverse.c inverse.h
	gcc -g -c -Wall inverse.c

aesutil.o: aesutil.c aesutil.h
	gcc -g -c -Wall aesutil.c

utilities.o: utilities.c utilities.h
	gcc -g -c -Wall utilities.c

clean:
	rm -f *.o hw5
