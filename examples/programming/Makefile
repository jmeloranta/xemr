all: tn tn2

tn:	tn.o ../../lib/libepr.a
	gcc -o tn tn.o ../../lib/libepr.a -lm -llapack -lblas -lrfftw -lfftw

tn.o:	tn.c
	gcc -I../../include -O3 -c tn.c

tn2:	tn2.o ../../lib/libepr.a
	gcc -o tn2 tn2.o ../../lib/libepr.a -lm -llapack -lblas -lrfftw -lfftw

tn2.o:	tn2.c
	gcc -I../../include -O3 -c tn2.c
