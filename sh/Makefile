CC=gcc -I../libepr
CFLAGS=-O
APPLIBS=../libepr/libepr.a -lblas -llapack -lrfftw -lfftw -lm

main: main.o
	$(CC) $(CFLAGS) -o main main.o $(APPLIBS)

clean:
	-rm *.o main *~

