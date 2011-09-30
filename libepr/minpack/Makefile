#
# NOTE: This is not the full minpack library!
#
include ../../make.conf

OBJ=dpmpar.o enorm.o fdjac2.o lmdif.o lmpar.o qrfac.o qrsolv.o

minpack: $(OBJ)
	ar cr ../libepr.a $(OBJ)
	-ranlib ../libepr.a

clean:
	-rm -f *~ *.o
