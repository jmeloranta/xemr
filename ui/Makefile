
include ../make.conf

OBJS = gui.o param.o mark.o polyn.o symmetry.o calibsig.o convol.o differences.o diffout.o position.o peakout.o convert.o linewidth.o equivalence.o simpar.o heinzer.o variables.o runsimul.o runmon.o runsim.o misc.o shift.o summary.o generic.o hyperfine.o elel.o quadrupole.o alinewidth.o selectlw.o gvalue.o elw.o runml.o spec_setup.o window.o orbital.o cmhz.o hj.o

all: libui.a

libui.a: $(OBJS)
	ar cr libui.a *.o
	ranlib libui.a

gui.o: gui.c gui.h
param.o: param.c param.h
mark.o: mark.c mark.h
polyn.o: polyn.c polyn.h
symmetry.o: symmetry.c symmetry.h
calibsig.o: calibsig.c calibsig.h
convol.o: convol.c convol.h
differences.o: differences.c differences.h 
diffout.o: diffout.c diffout.h 
position.o: position.c position.h 
peakout.o: peakout.c peakout.h 
convert.o: convert.c convert.h 
linewidth.o: linewidth.c linewidth.h
equivalence.o: equivalence.c equivalence.h 
simpar.o: simpar.c simpar.h 
heinzer.o: heinzer.c heinzer.h
variables.o: variables.c variables.h 
runsimul.o: runsimul.c runsimul.h 
runmon.o: runmon.c runmon.h 
runsim.o: runsim.c runsim.h 
misc.o: misc.c misc.h 
shift.o: shift.c shift.h 
summary.o: summary.c summary.h 
generic.o: generic.c generic.h 
hyperfine.o: hyperfine.c hyperfine.h
elel.o: elel.c elel.h 
quadrupole.o: quadrupole.c quadrupole.h 
alinewidth.o: alinewidth.c alinewidth.h 
selectlw.o: selectlw.c selectlw.h 
gvalue.o: gvalue.c gvalue.h 
elw.o: elw.c elw.h 
runml.o: runml.c runml.h 
spec_setup.o: spec_setup.c spec_setup.h 
window.o: window.c window.h 
orbital.o: orbital.c orbital.h
cmhz.o: cmhz.c cmhz.h 
hj.o: hj.c hj.h

#install: libui.a
#	cp libui.a $(ROOT)/lib
#	ranlib $(ROOT)/lib/libui.a

clean:
	-rm *.o *.a
