/*
 * Convolutions & filters.
 * Last modified: Oct-13-2009 jmeloranta@gmail.com
 *
 * Written by:
 * Jussi Eloranta (jmeloranta@gmail.com)
 *
 */

#include "epr.h"

static epr_type tmp_sp;

/*
 * Function: epr_type *epr_cepstr(epr_type *)
 * Purpose: Calcuate cepstrum of the given spectrum.
 *
 * sp = spectrum [input/output; epr_type *]
 *
 * returns pointer to sp.
 *
 * This routine is numerically(?) unstable. I think one should divide
 * the lineshape out in the time domain... Things usually work better
 * if spectrum resolution is high (32K!)
 *
 * The operation is FT of the log of |FT|.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

#define AVOID_INF 1E-3

epr_type *epr_cepstr(epr_type *sp) {

  int i, j;
  double len;

  epr_misc_scale(sp, 16383.0);
  epr_fft(sp);
  epr_fft_power(sp);
  for (i = 0; i < (sp->samples+1)/2; i++) {
    if(sp->y[i] > AVOID_INF) sp->y[i] = log(sp->y[i]);
    else sp->y[i] = 0.0;
    if(i % 2 != 0) sp->y[i] *= -1.0;
  }
  if(sp->samples % 2 == 0) {
    int fish = sp->samples / 2;
    if(sp->y[fish] > AVOID_INF) sp->y[fish] = log(AVOID_INF + sp->y[fish]);
    else sp->y[fish] = 0.0;
  }
  epr_fft_inv(sp);
  len = fabs(sp->xbeg - sp->xend) / 2.0;
  sp->xbeg = -len;
  sp->xend = len;

  return sp;
}

/*
 * Function: epr_type *epr_cross(epr_type *, epr_type *)
 * Purpose: Calculate crosscorrelation of two spectra using FFT method.
 *
 * sp1   = spectrum 1 (the result is written here). [input/output; epr_type *]
 * sp2   = spectrum 2. [input; epr_type *]
 *
 * Evaluate: I(sp1(y-x)*sp2(y)dy). It is assumed that sp1 and sp2 are *real*.
 *
 * return value: ptr to new sp.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_cross(epr_type *sp1, epr_type *sp2) {

  int i, j;
  doublecomplex a, b;
  double a1, a2;

  if(sp1->samples != sp2->samples) return (epr_type *) NULL;
  epr_misc_copy(sp2, &tmp_sp);
  epr_fft(sp1);
  epr_fft(&tmp_sp);
  sp1->y[0] *= tmp_sp.y[0];
  for (i = 1, j = sp1->samples-1; i < (sp1->samples+1)/2; i++, j--) {
    a.r = sp1->y[i];
    a.i = sp1->y[j];
    b.r = tmp_sp.y[i];
    b.i = -tmp_sp.y[j];
    epr_imag_mult(&a, &b);
    sp1->y[i] = a.r;
    sp1->y[j] = a.i;
    if(i % 2 != 0) {
      sp1->y[i] *= -1.0;
      sp1->y[j] *= -1.0;
    }
  }
  if(sp1->samples % 2 == 0) /* imaginary part zero */
    sp1->y[sp1->samples / 2] *= tmp_sp.y[tmp_sp.samples / 2];
  epr_fft_inv(sp1);
  a2 = (sp2->xbeg - sp1->xend)/ 2.0;
  a1 = (sp2->xend - sp1->xbeg) / 2.0;
  if(a1 > a2) {
    sp1->xbeg = a2;
    sp1->xend = a1;
  } else {
    sp1->xbeg = a1;
    sp1->xend = a2;
  }
  return sp1;
}

/*
 * Function: epr_type *epr_corr_f(epr_type *, double (*)(double,epr_type *))
 * Purpose: Calculate correlation of spectrum and some function using the
 * FFT method.
 *
 * sp = spectrum (the result is written here). [input/output; epr_type *]
 * f  = function. [input; double (*)(double)]
 *      This function takes two arguments: x-value and ptr to sp
 *
 * Evaluate: I(sp1(y-x)*f(y)dy). It is assumed that sp1 and f are *real*.
 *
 * return value: ptr to sp.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_cross_f(epr_type *sp, double (*f)(double, epr_type *)) {

  int i, j;
  double x, step;
  doublecomplex a, b;
  
  epr_fft(sp);
  step = EPR_XSTEP(sp);
  tmp_sp.xbeg = sp->xbeg;
  tmp_sp.xend = sp->xend;
  tmp_sp.samples = sp->samples;
  for (x = sp->xbeg, i = 0; i < sp->samples; i++, x += step)
    tmp_sp.y[i] = (*f)(x, sp);
  epr_fft(&tmp_sp);
  sp->y[0] *= tmp_sp.y[0];
  for (i = 1, j = sp->samples-1; i < (sp->samples+1)/2; i++, j--) {
    a.r = sp->y[i];
    a.i = -sp->y[j];
    b.r = tmp_sp.y[i];
    b.i = tmp_sp.y[j];
    epr_imag_mult(&a, &b);
    sp->y[i] = a.r;
    sp->y[j] = a.i;
  }
  if(sp->samples % 2 == 0) /* imaginary part zero */
    sp->y[sp->samples / 2] *= tmp_sp.y[sp->samples / 2];
  epr_fft_inv(sp);
  return sp;
}


/*
 * Function: epr_type *epr_conv_f(epr_type *, double (*)(double,epr_type *))
 * Purpose: Calculate convolution of spectrum and some function using the
 * FFT method.
 *
 * sp = spectrum (the result is written here). [input/output; epr_type *]
 * f  = function. [input; double (*)(double)]
 *      This function takes two arguments: x-value and ptr to sp.
 *
 * c(x[i]) = I(f(x[i]-x[j])*g(x[j]), x[j])
 *
 * return value: ptr to sp.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_conv_f(epr_type *sp, double (*f)(double, epr_type *)) {

  int i, j;
  double x, step;
  doublecomplex a, b;
  
  epr_fft(sp);
  step = EPR_XSTEP(sp);
  tmp_sp.xbeg = sp->xbeg;
  tmp_sp.xend = sp->xend;
  tmp_sp.samples = sp->samples;
  for (x = sp->xbeg, i = 0; i < sp->samples; i++, x += step)
    tmp_sp.y[i] = (*f)(x, sp);
  epr_fft(&tmp_sp);
  sp->y[0] *= tmp_sp.y[0];
  for (i = 1, j = sp->samples-1; i < (sp->samples+1)/2; i++, j--) {
    a.r = sp->y[i];
    a.i = sp->y[j];
    b.r = tmp_sp.y[i];
    b.i = tmp_sp.y[j];
    epr_imag_mult(&a, &b);
    sp->y[i] = a.r;
    sp->y[j] = a.i;
  }
  if(sp->samples % 2 == 0) /* imaginary part zero */
    sp->y[sp->samples / 2] *= tmp_sp.y[sp->samples / 2];
  epr_fft_inv(sp);
  return sp;
}

/*
 * Function: epr_type *epr_conv(epr_type *, epr_type *)
 * Purpose: Calculate convolution of two spectra using FFT method.
 *
 * sp1   = spectrum 1. (the result is written here). [input/output; epr_type *]
 * sp2   = spectrum 2. [input; epr_type *]
 *
 * c(x) = I(f(x-y)*g(y), y)
 *
 * return value: ptr to new sp.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_conv(epr_type *sp1, epr_type *sp2) {

  int i, j;
  doublecomplex a, b;

  if(sp1->samples != sp2->samples) return (epr_type *) NULL;
  epr_misc_copy(sp2, &tmp_sp);
  epr_fft(sp1);
  epr_fft(&tmp_sp);
  /* imaginary part corresponding to this element is zero */
  sp1->y[0] *= tmp_sp.y[0];
  for (i = 1, j = sp1->samples-1; i < (sp1->samples+1)/2; i++, j--) {
    a.r = sp1->y[i];
    a.i = sp1->y[j];
    b.r = tmp_sp.y[i];
    b.i = tmp_sp.y[j];
    epr_imag_mult(&a, &b);
    sp1->y[i] = a.r;
    sp1->y[j] = a.i;
    if(i % 2 != 0) {
      sp1->y[i] *= -1.0;
      sp1->y[j] *= -1.0;
    }
  }
  if(sp1->samples % 2 == 0) /* imaginary part zero corresp. to this is 0 */
    sp1->y[sp1->samples / 2] *= tmp_sp.y[sp1->samples / 2];
  epr_fft_inv(sp1);
  sp1->xbeg = sp1->xbeg + sp1->xbeg;
  sp1->xend = sp1->xend + sp2->xend;
  return sp1;
}

/*
 * Function: epr_type *epr_calc_convert_scale(epr_type *, int)
 * Purpose: Convert X-axis scale to given units.
 *
 * sp   = Spectrum to be converted [input/output; epr_type *]
 * int  = New axis scale units [input; int]
 *
 * Returns ptr to sp, NULL on error.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_conv_scale(epr_type *sp, int units) {

  double tmp;

  /* remember to keep a correct ordering of UNIT_* definitions */
  if(sp->xunits > EPR_UNIT_GV || units > EPR_UNIT_GV) return (epr_type *) NULL;
  switch(units) {
  case EPR_UNIT_G: /* current -> Gauss */
    /* Gauss to Gauss is NOP */
    if(sp->xunits == EPR_UNIT_MT) { /* mT -> G */
      sp->xbeg *= 1.0E1;
      sp->xend *= 1.0E1;
    }
    if(sp->xunits == EPR_UNIT_MHZ) { /* MHz -> G */
      tmp = EPR_H / (EPR_M2G(1.0E-4 * sp->frequency, 1.0E6 * (sp->xbeg + sp->xend) / 2.0) 
		     * EPR_GAUSS_BE);
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    if(sp->xunits == EPR_UNIT_GHZ) { /* GHz -> G */
      tmp = EPR_H / (EPR_M2G(1.0E-4*sp->frequency, 1.0E9 * (sp->xbeg + sp->xend) / 2.0) 
		     * EPR_GAUSS_BE);
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    if(sp->xunits == EPR_UNIT_GV) { /* g-scale to G */
      tmp = EPR_H * sp->frequency / EPR_GAUSS_BE;
      sp->xbeg = tmp / sp->xend;
      sp->xend = tmp / sp->xbeg;
    }
    break;

  case EPR_UNIT_MT: /* current -> mT */
    if(sp->xunits == EPR_UNIT_G) { /* G -> mT */
      sp->xbeg *= 1.0E-1;
      sp->xend *= 1.0E-1;
    }
    /* mT -> mT is NOP */
    if(sp->xunits == EPR_UNIT_MHZ) { /* MHz -> mT */
      tmp = 1.0E3 * EPR_H / (EPR_M2G(1.0E-4*sp->frequency,
				 1.0E6 * (sp->xbeg + sp->xend) / 2.0) * EPR_SI_BE);
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    if(sp->xunits == EPR_UNIT_GHZ) { /* GHz -> mT */
      tmp = 1.0E3 * EPR_H / (EPR_M2G(1.0E-4*sp->frequency, 1.0E9 * (sp->xbeg + sp->xend) / 2.0) 
		     * EPR_SI_BE);
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    if(sp->xunits == EPR_UNIT_GV) { /* g-scale -> mT */
      tmp = 1.0E3 * EPR_H * sp->frequency / EPR_SI_BE;
      sp->xbeg = tmp / sp->xend;
      sp->xend = tmp / sp->xbeg;
    }
    break;

  case EPR_UNIT_MHZ: /* current -> MHz */
    if(sp->xunits == EPR_UNIT_G) { /* G -> MHz */
      tmp = 1.0E-6 * EPR_M2G((sp->xbeg + sp->xend) / 2.0, sp->frequency) *
	EPR_GAUSS_BE / EPR_H;
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    if(sp->xunits == EPR_UNIT_MT) { /* mT -> MHz */
      tmp = 1.0E-6 * EPR_M2G(10.0*(sp->xbeg + sp->xend) / 2.0, sp->frequency) *
	EPR_SI_BE / EPR_H;
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    /* MHz -> MHz is NOP */
    if(sp->xunits == EPR_UNIT_GHZ) { /* GHz -> MHz */
      sp->xbeg *= 1.0E-3;
      sp->xend *= 1.0E-3;
    }
    if(sp->xunits == EPR_UNIT_GV) { /* g-scale -> MHz */
      tmp = 1.0E-6 * EPR_SI_BE * sp->frequency / EPR_H;
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    break;

  case EPR_UNIT_GHZ: /* current -> GHz */
    if(sp->xunits == EPR_UNIT_G) { /* G -> GHz */
      tmp = 1.0E-9 * EPR_M2G((sp->xbeg + sp->xend) / 2.0, sp->frequency) *
	EPR_GAUSS_BE / EPR_H;
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    if(sp->xunits == EPR_UNIT_MT) { /* mT -> GHz */
      tmp = 1.0E-9 * EPR_M2G(10.0*(sp->xbeg + sp->xend) / 2.0, sp->frequency) *
	EPR_SI_BE / EPR_H;
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    if(sp->xunits == EPR_UNIT_MHZ) { /* GHz -> GHz */
      sp->xbeg *= 1.0E3;
      sp->xend *= 1.0E3;
    }
    /* GHz -> GHz is NOP */
    if(sp->xunits == EPR_UNIT_GV) { /* g-scale -> GHz */
      tmp = 1.0E-9 * EPR_SI_BE * sp->frequency / EPR_H;
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    break;

  case EPR_UNIT_GV: /* current -> g-scale */
    if(sp->xunits == EPR_UNIT_G) { /* Gauss -> g-scale */
      tmp = EPR_H * sp->frequency / EPR_GAUSS_BE;
      sp->xbeg = tmp / sp->xend;
      sp->xend = tmp / sp->xbeg;
    }
    if(sp->xunits == EPR_UNIT_MT) { /* mT -> g-scale */
      tmp = EPR_H * sp->frequency / (EPR_SI_BE * 1.0E-3);
      sp->xbeg = tmp / sp->xend;
      sp->xend = tmp / sp->xbeg;
    }
    if(sp->xunits == EPR_UNIT_MHZ) { /* MHz -> g-scale */
      tmp = 1.0E-6 * EPR_H / (EPR_SI_BE * sp->frequency);
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    if(sp->xunits == EPR_UNIT_GHZ) { /* GHz -> g-scale */
      tmp = 1.0E-9 * EPR_H / (EPR_SI_BE * sp->frequency);
      sp->xbeg *= tmp;
      sp->xend *= tmp;
    }
    /* g-scale -> g-scale is NOP */
    break;
  }
  return sp;
}
