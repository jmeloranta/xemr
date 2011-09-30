templatedir = $(libdir)/xemr/templates
template_DATA = templates/eie.spe templates/eie.spy templates/endor.spe templates/endor.spy templates/epr.spe templates/epr.spy templates/triple-gen.spe templates/triple-gen.spy templates/triple-spec.spe templates/triple-spec.spy

calibrationdir = $(libdir)/xemr/calibration
calibration_DATA = calibration/varian-epr.set calibration/bruker-endor.set calibration/README

bin_PROGRAMS = xemr
xemr_SOURCES = main.c genutil.c varutil.c dialog.c file.c create.c setup.c measure.c xemr.h
AM_CFLAGS = -I../ui -I../libepr -DTEMPLATES=\"$(libdir)/xemr/templates\" -DCALIBRATION=\"$(libdir)/xemr/calibration\"
xemr_LDADD = -lmeas -lX11 -lXpm ../libepr/libepr.a ../ui/libui.a -llapack -lblas -lrfftw -lfftw -lfl -lforms
