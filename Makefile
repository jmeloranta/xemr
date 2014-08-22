all: 
	(cd libepr; make)
	(cd ui; make)
	(cd xemr; make)
	(cd util; make)

install:
	(cd libepr; make install)
	(cd xemr; make install)
	(cd util; make install)

clean:
	(cd xemr; make clean)
	(cd ui; make clean)
	(cd libepr; make clean)
	(cd util; make clean)
