include ../make.conf

CFLAGS += -I../ui -I../libepr -DTEMPLATES=\"$(ROOT)/lib/xemr/templates\" -DCALIBRATION=\"$(ROOT)/lib/xemr/calibration\"

OBJS = create.o dialog.o file.o genutil.o main.o measure.o setup.o varutil.o

xemr: $(OBJS)
	$(CC) -o xemr $(OBJS) $(LDFLAGS)

install: xemr
	cp xemr $(ROOT)/bin
	-mkdir $(ROOT)/lib/xemr
	-mkdir $(ROOT)/lib/xemr/templates
	-mkdir $(ROOT)/lib/xemr/calibration
	cp templates/* $(ROOT)/lib/xemr/templates
	cp calibration/* $(ROOT)/lib/xemr/calibration

clean:
	-rm xemr *.o
