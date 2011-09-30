/*
 * FFT routines.
 * Last modified: Oct-13-2009 jmeloranta@gmail.com
 *
 * The real work is done by the libfftw people.
 *
 * Added simple front-ends:
 * Jussi Eloranta (jmeloranta@csun.edu)
 *
 */


#include "epr.h"
#include <rfftw.h>

static int last_fft_size_forward = -1, last_fft_size_backward = -1;
static rfftw_plan plan_forward, plan_backward;
static double tmp_storage[EPR_MAX_SAMPLES];

/*
 * Function: epr_type *epr_fft(epr_type *)
 * Purpose: Calculate Fourier transformation of spectrum.
 *
 * sp   = Fourier transform of this spectrum will be calculated
 *        [input/output; epr_type *]
 *
 * Note: The spectrum will contain now sp->samples Y-points.
 *       The lower part contains the real-part and the upper part
 *       the imaginary-part. The reverse fft will assume that the
 *       imaginary part is in upper part of the Y-data.
 *       The X-axis i not changed.
 *
 * Return value: 0          = error
 *               ptr to sp = no error
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_fft(epr_type *sp) {
  
  double tmp;

  if(sp->samples != last_fft_size_forward) {
    if(last_fft_size_forward != -1) rfftw_destroy_plan(plan_forward);
    plan_forward = rfftw_create_plan(sp->samples, FFTW_REAL_TO_COMPLEX, FFTW_ESTIMATE);
    last_fft_size_forward = sp->samples;
  }
  rfftw_one(plan_forward, sp->y, tmp_storage);
  bcopy(tmp_storage, sp->y, sizeof(double) * sp->samples);

  return sp;
}

/*
 * Function: epr_type *epr_fft_inv(epr_type *)
 * Purpose: Calculate Inverse Fourier transformation of spectrum.
 *
 * sp   = Fourier transform of this spectrum will be calculated
 *        [input/output; epr_type *]
 *
 * Note: The spectrum is assumed to contain now sp->samples Y-points.
 *       The lower part contains the real-part and the upper part
 *       the imaginary-part. The X-axis i not changed.
 *
 * Return value: 0          = error
 *               ptr to sp = no error
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_fft_inv(epr_type *sp) {

  int i;
  double tmp;

  if(sp->samples != last_fft_size_backward) {
    if(last_fft_size_backward != -1) rfftw_destroy_plan(plan_backward);
    plan_backward = rfftw_create_plan(sp->samples, FFTW_COMPLEX_TO_REAL, FFTW_ESTIMATE);
    last_fft_size_backward = sp->samples;
  }
  rfftw_one(plan_backward, sp->y, tmp_storage);
  for(i = 0; i < sp->samples; i++)
    sp->y[i] = tmp_storage[i] / (double) sp->samples;

  return sp;
}

/*
 * Function: int epr_fft_fill(epr_type *, double)
 * Purpose: Fill # of samples of spectrum to the nearest power of two.
 *          Possible new intensity values are 'values'.
 *
 * sp    = Spectrum to be filled [input/output; epr_type *]
 * value = Constant used for filling [input; double]
 *
 * return value: new # of points in spectrum.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

int epr_fft_fill(epr_type *sp, double value) {

  int i, j;
  double step;

  step = EPR_XSTEP(sp);
  for (i = 512; i < EPR_MAX_SAMPLES; i *= 2)
    if (i >= sp->samples) break;
  for (j = sp->samples; j < i; j++) sp->y[j] = value;
  sp->xend += step * (double) (i - sp->samples);
  sp->samples = i;
  return i;
}

/*
 * Function: epr_fft_zfill(epr_type *)
 * Purpose:  Zero filling
 * 
 * sp = Input spectrum [input/output]
 * resol = new resolution [input; int]
 * method = 0 = begin, 1 = middle, 2 = end.
 *
 * Returns the zero filled spectrum.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_fft_zfill(epr_type *sp, int resol, int method) {

  double w2;
  int i, j, i2;

  if(sp->samples >= resol) return sp;
  if(resol > EPR_MAX_SAMPLES) return NULL;

  w2 = EPR_XSTEP(sp) * (resol - sp->samples) / 2.0;
  switch (method) {
  case 0: /* fill at begin */
    i2 = resol - sp->samples;
    for (i = 0, j = i2; i < sp->samples; i++, j++)
      sp->y[j] = sp->y[i];
    bzero(sp->y, sizeof(double) * i2);
    sp->xbeg -= 2.0 * w2;
    sp->samples = resol;
    break;
  case 1: /* centered fill */
    sp->xend += w2;
    sp->xbeg -= w2;
    i2 = (resol - sp->samples) / 2;
    bcopy(sp->y, &tmp_storage, sizeof(double) * sp->samples);
    bzero(sp->y, sizeof(double) * resol);
    bcopy(&tmp_storage, sp->y + i2, sizeof(double) * sp->samples);
    sp->samples = resol;
    break;
  case 2: /* fill at end */
    i2 = resol - sp->samples;
    bzero(sp->y + sp->samples, sizeof(double) * i2);
    sp->xend += 2.0 * w2;
    sp->samples = resol;
    break;
  }
  return sp;
}

/*
 * Function: epr_fft_window(epr_type *, double, double)
 * Purpose: Perform simple windowing in the Fourier space.
 *
 * sp = Spectrum (real at low part - imaginary at high part) [input/output;
 *      epr_type *]
 * t  = Windowing type [input, int]
 * a  = Parameter a [input; double]
 * b  = Parameter b [input; double]
 *
 * Returns windowed spectrum.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

inline double epr_fft_win1(double x, double a, double b, double samples) {

  return exp(x / a - x * x / b);
}

inline double epr_fft_win2(double x, double a, double b, double samples) {

  return 1.0 - fabs(2.0 * x - 1.0);
  /*  return 1.0 - fabs((x - 0.5*(samples-1.0)) / (0.5 * (samples - 1.0))); */
}

inline double epr_fft_win3(double x, double a, double b, double samples) {

  return 0.5 * (1.0 - cos(2.0 * M_PI * x));
  /*  return 0.5 * (1.0 - cos(2.0 * M_PI * x / (samples - 1.0))); */
}

inline double epr_fft_win4(double x, double a, double b, double samples) {

  return 1.0 - pow(2.0 * x - 1.0, 2.0);
  /*  return 1.0 - pow((x - 0.5 * (samples - 1.0)) / (0.5 * (samples + 1)), 2.0); */
}

inline double epr_fft_win5(double x, double a, double b, double samples) {

  return 0.54 - 0.46 * cos(2.0 * M_PI * x);
  /*  return 0.54 - 0.46 * cos(2.0 * M_PI * x / (samples - 1.0)); */
}

inline double epr_fft_win6(double x, double a, double b, double samples) {

  return 0.42 - 0.5 * cos(2.0 * M_PI * x) + 0.08 * cos(4.0 * M_PI * x);
  /*  return 0.42 - 0.5 * cos(2.0 * M_PI * x / (samples - 1.0)) + 0.08 * cos(4.0 * M_PI * x / (samples - 1.0)); */
}

inline double epr_fft_win7(double x, double a, double b, double samples) {

  return 1.0 - fabs(2.0 * x - 1.0);
  /*  return 1.0 - fabs((x - 0.5 * (samples - 1.0)) / (0.5 * (samples + 1.0))); */
}

epr_type *epr_fft_window(epr_type *sp, int t, double a, double b) {

  int i, j;
  double (*fnc)(double, double, double, double), x, s;

  switch(t) {
  case EPR_WN_EXPONENTIAL:
    fnc = epr_fft_win1;
    break;
  case EPR_WN_TRIANGULAR:
    fnc = epr_fft_win2;
    break;      
  case EPR_WN_HANNING:
    fnc = epr_fft_win3;
    break;      
  case EPR_WN_WELCH:
    fnc = epr_fft_win4;
    break;      
  case EPR_WN_HAMMING:
    fnc = epr_fft_win5;
    break;      
  case EPR_WN_BLACKMAN:
    fnc = epr_fft_win6;
    break;      
  case EPR_WN_PARZEN:
    fnc = epr_fft_win7;
    break;      
  }
  
  x = 0.0;
  s = EPR_XSTEP(sp) / 2.0;
  sp->y[0] *= (*fnc)(x, a, b, (double) sp->samples);
  for (i = 1, j = sp->samples-1; i < (sp->samples+1)/2; i++, j--) {
    x += s;
    sp->y[i] *= (*fnc)(x, a, b, (double) sp->samples);
    sp->y[j] *= (*fnc)(x, a, b, (double) sp->samples);
  }
  x += s;
  if(sp->samples % 2 == 0)
    sp->y[sp->samples / 2] *= (*fnc)(x, a, b, sp->samples);
  
  return sp;
}

/*
 * Function: epr_fft_power(epr_type *)
 * Purpose: Calculate power spectrum.
 *
 * sp = Spectrum (real at low part - imaginary at high part) [input/output;
 *      epr_type *]
 *
 * Returns power spectrum.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_fft_power(epr_type *sp) {

  int i, j;

  tmp_storage[0] = sp->y[0] * sp->y[0];
  for(i = 1, j = sp->samples-1; i < (sp->samples+1)/2; i++, j--) {
    tmp_storage[i] = sp->y[i] * sp->y[i] + sp->y[j] * sp->y[j];
    tmp_storage[j] = 0.0;
  }
  if(sp->samples % 2 == 0)
    tmp_storage[sp->samples / 2] = sp->y[sp->samples / 2]
      * sp->y[sp->samples / 2];
  bcopy(tmp_storage, sp->y, sizeof(double) * sp->samples);

  return sp;
}
