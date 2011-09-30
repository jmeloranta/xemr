/*
 * Baseline correction functions.
 * Last modified: Oct-23-2009 jmeloranta@gmail.com
 *
 * Written by:
 * Jussi Eloranta (jmeloranta@gmail.com)
 *
 * Most of the code was borrowed from (manip.c):
 * Dr. Burkhard Kirste, Institut fuer Organische Chemie,
 * Freie Universitaet Berlin, Takustrasse 3,
 * D-1000 Berlin 33, Germany
 *
 */

#include "epr.h"

/*
 * Function: epr_type *epr_smooth_symmetry(epr_type *, double center)
 * Purpose: Smooth spectrum to be symmetrical around some center
 *
 * sp     = spectrum to be symmetrized [input/output; epr_type *]
 *          0.0 for automatic center.
 * center = center for symmetrization [input; double]
 *
 * NOTE: this routine is critical for the baseline to be at zero.
 *
 * return value: ptr to sp
 * 
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_smooth_symmetry(epr_type *sp, double center) {

  int i, j;
  double val;

  if(center == 0.0) j = sp->samples / 2;
  else j = EPR_XTOI(sp, center, EPR_XSTEP(sp));
  if(j >= sp->samples) return (epr_type *) 0;

  for (i = 0; i < j; i++) {
    val = (fabs(sp->y[i]) + fabs(sp->y[2 * j - i])) / 2.0;
    sp->y[i] = val * EPR_SIGN(sp->y[i]);
    sp->y[2 * j - i] = val * EPR_SIGN(sp->y[2 * j - i]);
  }

  return sp;
}

/*
 * Function: epr_type *epr_smooth_9pt(epr_type *)
 * Purpose: Perform nine point smooth on spectrum
 *
 * sp = spectrum to be smoothed [input/output; epr_type *]
 *
 * return value: ptr to sp
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_smooth_9pt(epr_type *sp) {
  
  int i, n;
  double *y, x;
  double yy[EPR_MAX_SAMPLES];

  n = sp->samples;
  y = sp->y;

  for(i = 4; i < n - 4; i++) {
    x = -21.0 * (y[i - 4] + y[i + 4]);
    x += 14.0 * (y[i - 3] + y[i + 3]); 
    x += 39.0 * (y[i - 2] + y[i + 2]); 
    x += 54.0 * (y[i - 1] + y[i + 1]) + 59.0 * y[i];  
    yy[i] = x / 231.0;
  }
  
  for(i = 2; i < 4; i++) {
    x = -3.0 * (y[i - 2] + y[i + 2]);
    x += 12.0 * (y[i - 1] + y[i + 1]) + 17.0 * y[i];
    yy[i] = x / 35.0;
  }
  
  for(i = n - 4; i < n - 2; i++) {
    x = -3.0 * (y[i - 2] + y[i + 2]);
    x += 12.0 * (y[i - 1] + y[i + 1]) + 17.0 * y[i];
    yy[i] = x / 35.0;
  }
  
  yy[1] = (y[0] + y[1] + y[2]) / 3.0;
  yy[n - 2] = (y[n - 3] + y[n - 2] + y[n - 1]) / 3.0;
  yy[0] = y[0];
  yy[n - 1] = y[n - 1];
  memcpy(y, yy, n * sizeof(double));

  return sp;
}
