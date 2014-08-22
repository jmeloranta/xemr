include ../make.conf

OBJS = file.o misc.o calc.o database.o fft.o peak.o conv.o base.o smooth.o line.o optim.o fit.o simul.o tensor.o hamilton.o powder.o imag.o data.o minpack/dpmpar.o minpack/enorm.o minpack/fdjac2.o minpack/lmdif.o minpack/lmpar.o minpack/qrfac.o minpack/qrsolv.o

all: libepr.a

libepr.a: $(OBJS)
	ar cr libepr.a $(OBJS)
	ranlib libepr.a

install: libepr.a
	cp libepr.a $(ROOT)/lib
	cp epr.h $(ROOT)/include
	ranlib $(ROOT)/lib/libepr.a

clean:
	-rm *.o *.a minpack/*.o
