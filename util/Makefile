include ../make.conf
CFLAGS += -I../libepr
PROGS = aspect2epr esp2epr glue xy2epr y2epr pc2epr epr2esp

all: $(PROGS)

aspect2epr: aspect2epr.o
	$(CC) -o aspect2epr aspect2epr.o $(LDFLAGS)

esp2epr: esp2epr.o
	$(CC) -o esp2epr esp2epr.o $(LDFLAGS)

epr2esp: epr2esp.o
	$(CC) -o epr2esp epr2esp.o $(LDFLAGS)

glue: glue.o
	$(CC) -o glue glue.o $(LDFLAGS)

xy2epr: xy2epr.o
	$(CC) -o xy2epr xy2epr.o $(LDFLAGS)

y2epr: y2epr.o
	$(CC) -o y2epr y2epr.o $(LDFLAGS)

pc2epr: pc2epr.o
	$(CC) -o pc2epr pc2epr.o $(LDFLAGS)

install: $(PROGS)
	cp $(PROGS) $(ROOT)/bin

clean:
	-rm $(PROGS) *.o


