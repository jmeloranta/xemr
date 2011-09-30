AM_CFLAGS = -I../libepr

bin_PROGRAMS = aspect2epr esp2epr glue xy2epr y2epr pc2epr
aspect2epr_SOURCES = aspect2epr.c
aspect2epr_LDADD = -lmeas -lX11 -lXpm ../libepr/libepr.a ../ui/libui.a -llapack -lblas -lrfftw -lfftw -lfl -lforms
esp2epr_SOURCES = esp2epr.c esp2epr.h
esp2epr_LDADD = -lmeas -lX11 -lXpm ../libepr/libepr.a ../ui/libui.a -llapack -lblas -lrfftw -lfftw -lfl -lforms
glue_SOURCES = glue.c
glue_LDADD = -lmeas -lX11 -lXpm ../libepr/libepr.a ../ui/libui.a -llapack -lblas -lrfftw -lfftw -lfl -lforms
pc2epr_SOURCES = pc2epr.c
pc2epr_LDADD = -lmeas -lX11 -lXpm ../libepr/libepr.a ../ui/libui.a -llapack -lblas -lrfftw -lfftw -lfl -lforms
xy2epr_SOURCES = xy2epr.c
xy2epr_LDADD = -lmeas -lX11 -lXpm ../libepr/libepr.a ../ui/libui.a -llapack -lblas -lrfftw -lfftw -lfl -lforms
y2epr_SOURCES = y2epr.c
y2epr_LDADD = -lmeas -lX11 -lXpm ../libepr/libepr.a ../ui/libui.a -llapack -lblas -lrfftw -lfftw -lfl -lforms

