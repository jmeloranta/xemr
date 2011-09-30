/*
 * Spectrum manipulation.
 * Last modified: Oct-13-2009 jmeloranta@gmail.com
 *
 * Written by:
 * Jussi Eloranta (jmeloranta@gmail.com)
 *
 * Integration and differentation part of the code was borrowed from (manip.c):
 * Dr. Burkhard Kirste, Institut fuer Organische Chemie,
 * Freie Universitaet Berlin, Takustrasse 3,
 * D-1000 Berlin 33, Germany
 *
 */

#include "epr.h"

/*
 * Function: double epr_calc_add(epr_type *, epr_type *, mult, flag)
 * Purpose: add, sp1 = sp1 + mult * sp2
 *
 * sp1  = spectra one [input/output; epr_type *]
 * sp2  = spectra two (which will be multiplied by mult) [input; epr_type *]
 * mult = multiplier for sp2 (+ for addition, - for subtraction) [input; double]
 * flag = 1 is direct, 0 is use g-scale, 2 = x-axis search
 *
 * The result will be written to sp1.
 *
 * Warning: don't use the same sp on both sp1 and sp2! If you want to do this
 *          then allocate memory for both sp separately.
 *
 * return value: relative error caused by the operation or -1.0 for error.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

double epr_calc_add(epr_type *sp1, epr_type *sp2, double mult, int flag) {

  double error, s1, s2, p1, e;
  int i, j;

  if(sp1 == sp2) return -1.0; /* src and dest the same */

  s1 = EPR_XSTEP(sp1);
  s2 = EPR_XSTEP(sp2);

  if(!flag && (sp1->xunits != EPR_UNIT_GV || sp2->xunits != EPR_UNIT_GV)) return -1.0;

  switch(flag) {

  case 0: /* g */
    e = EPR_M2G(sp2->xend, sp2->frequency);
    for (i = 0, error = 0.0; i < sp1->samples; i++) {

      p1 = EPR_M2G(EPR_ITOX(sp1, i, s1), sp1->frequency);
      if(p1 < e) break;
      
      for (j = 0; j < sp2->samples - 1; j++)
	if (p1 < EPR_M2G(EPR_ITOX(sp2, j, s2), sp2->frequency) &&
	    p1 >= EPR_M2G(EPR_ITOX(sp2, j+1, s2), sp2->frequency)) break;
      
      sp1->y[i] += (sp2->y[j] + sp2->y[j+1]) * mult / 2.0;
      error += fabs(p1 - EPR_M2G(EPR_ITOX(sp2, j, s2), sp2->frequency));
    }
    return (error / (double) sp1->samples);

  case 1: /* direct */
    for (i = 0; i < MIN(sp1->samples, sp2->samples); i++)
      sp1->y[i] += mult * sp2->y[i];
    return 0.0; /* error not defined in this case */

  case 2: /* x-axis */
    for (i = 0, error = 0.0; i < sp1->samples; i++) {
      p1 = EPR_ITOX(sp1, i, s1);
      if(p1 > sp2->xend) break;
      
      for (j = 0; j < sp2->samples - 1; j++)
	if (p1 > EPR_ITOX(sp2, j, s2) && p1 <= EPR_ITOX(sp2, j + 1, s2)) break;
      
      sp1->y[i] += (sp2->y[j] + sp2->y[j + 1]) * mult / 2.0;
      error += fabs(p1 - EPR_ITOX(sp2, j, s2));
    }
    return (error / (double)sp1->samples);
  }
}

/*
 * Function: double epr_calc_mult(epr_type *, mult)
 * Purpose: multiply, sp = sp * mult
 *
 * sp   = spectra one [input/output; epr_type *]
 * mult = multiplier for sp1 [input; epr_type *]
 *
 * The result is written to sp.
 *
 * return value: mult
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

double epr_calc_mult(epr_type *sp, double mult) {

  int i;

  for (i = 0; i < sp->samples; i++)
    sp->y[i] *= mult;
  return mult;
}

/*
 * Function: double epr_calc_addc(epr_type *, double c)
 * Purpose: add, sp = sp + c
 *
 * sp   = spectra one [input/output; epr_type *]
 * c    = constant to be added [input; epr_type *]
 *
 * The result is written to sp.
 *
 * return value: c
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

double epr_calc_addc(epr_type *sp, double c) {

  int i;

  for (i = 0; i < sp->samples; i++)
    sp->y[i] += c;
  return c;
}

/*
 * Function: epr_type *epr_calc_diff(epr_type *)
 * Purpose: differentiate the spectrum
 *
 * sp  = spectra one [input/output; epr_type *]
 *
 * The result is written to sp.
 *
 * return value: pointer to sp if differentation was done
 *               0 if too few points.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

static double epr_calc_tmp[EPR_MAX_SAMPLES];

epr_type *epr_calc_diff(epr_type *sp) {

  int i, n;
  double *y, step;

  y = sp->y;
  n = sp->samples;
  if(n < 6) return (epr_type *) 0; /* not enough data points */
  step = 2.0 * EPR_XSTEP(sp);
  for (i = 1; i < n - 1; i++)
    epr_calc_tmp[i] = (y[i + 1] - y[i - 1]) / step;
  epr_calc_tmp[n - 1] = epr_calc_tmp[n - 2];
  epr_calc_tmp[0] = epr_calc_tmp[1];
  bcopy(epr_calc_tmp, sp->y, sizeof(double) * sp->samples);
  return sp;
}

/*
 * Function: epr_type *epr_calc_integ(epr_type *)
 * Purpose: integrate the spectrum
 *
 * sp  = spectra one [input/output; epr_type *]
 *
 * The result is written to sp.
 *
 * return value: pointer to sp
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_calc_integ(epr_type *sp) {

  int i, n;
  double *y, t, x, step;

  y = sp->y;
  n = sp->samples;
  x = y[0];
  step = EPR_XSTEP(sp);

  for (i = 1; i < n; i++) {
    t = y[i-1] + step * (y[i] + x) / 2.0;
    x = y[i];
    y[i] = t;
  }

  return sp;
}

/*
 * Function: double *epr_calc_max(epr_type *)
 * Purpose: find maximum intensity value (absolute value)
 *
 * sp  = spectra one [input; epr_type *]
 *
 * return value: maximum value.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

double epr_calc_max(epr_type *sp) {

  int i;
  double *y, maxval = 0.0, t;

  y = sp->y;
  for (i = 0; i < sp->samples; i++)
    if((t = fabs(y[i])) > maxval) maxval = t;
  return maxval;
}

/*
 * Function: double epr_calc_rot_c(epr_type *sp, double c, double a, ws)
 * Purpose: Given the line width parameter c and the a = Tr(A.A)
 *          of given (dipolar) hyperfine coupling, this routine calculates
 *          the rotation correlation time for the radical.
 *
 * c  = Line width parameter c [input; double]
 * a  = Tr(A.A) that is the tensor inner product of the hyperfine
 *      tensor A with itself [input; double]
 * ws = Microwave frequency, or actually the electron Larmour freq.
 *      [input; double]
 *
 * Returns: < 0 if more than one root was found or no roots were found.
 *          otherwise the rotation correlation time is returned.
 *
 * This routine is based on the eqn. (9.2.50) of N. Atherton's book.
 *
 * NOTE: A is traceless hyperfine matrix.
 *       Remember the validity of the Redfield theory! See Atherton's
 *       book.
 *
 * Returns -1 on error.
 *
 * TODO: Review not done (jme). Check against the book.
 *
 */

double epr_calc_rot_c(epr_type *sp, double c, double a, double ws) {

  double a1, a2, a3, di;
  double q, r, s, t, d, tmp;

  /* eqn. 9.2.50 is in energy units ! */
  switch(sp->xunits) {
    /* Check these conversions!  (FIXME) */
  case EPR_UNIT_MT:
    c *= 1E-3 * EPR_SI_GE_NRAD;                   /* 1/T2 */
    a *= EPR_PI42 * 2.802494 * 2.802494 * 1E14;     /* To Hz */
    break;
  case EPR_UNIT_G:
    c *= EPR_GAUSS_GE_NRAD;                       /* 1/T2 */
    a *= EPR_PI42 * 2.802494 * 2.802494 * 1E12;     /* To Hz */
    break;
  case EPR_UNIT_MHZ:
    c *= 1E6;
    c = 1.0 / c;
    break;
  case EPR_UNIT_GHZ:
    c *= 1E9;
    c = 1.0 / c;
    break;
  case EPR_UNIT_S:
  case EPR_UNIT_GV:
  case EPR_UNIT_ARB:
    return -1.0;
  }

  di = -ws * ws / 12.0;
  a1 = (ws * ws * c / a) / di;
  a2 = (1.0 / 60.0 - 1.0 / 12.0) / di;
  a3 = (c / a) / di;

  q = (3.0 * a2 - a1 * a1) / 9.0;
  r = (9.0 * a1 * a2 - 27.0 * a3 - 2.0 * a1 * a1 * a1) / 54.0;
  d = q * q * q + r * r;
  if(d <= 0) return -1.0; /* roots are unequal */

  tmp = sqrt(q * q * q + r * r);
  s = pow(r + tmp, 1.0 / 3.0);
  if(r - tmp >= 0.0)
    t = pow(r - tmp, 1.0 / 3.0);
  else
    t = -pow(tmp - r, 1.0 / 3.0);
  return (s + t - a1 / 3.0); /* rest of the roots are imagnary */
}

/*
 * Function: double epr_calc_rot_b(epr_type *sp, double b, double ag, ws, bc)
 * Purpose: Given line width parameter b and the a = Tr(g.A)
 *          of given hyperfine coupling and g-tensors, this routine
 *          calculates the rotation correlation time for the radical.
 *
 * b  = Line width parameter b (input; double)
 * ag = Tr(g.A) that is the tensor inner product of the hyperfine
 *      tensor A with the anisotropic g-tensor (input; double)
 * ws = Microwave frequency, or actually the electron Larmour freq.
 *      (input; double)
 * bc = Magnetic filed value (input; double).
 *
 * Returns: < 0 if more than one root was found or no roots were found.
 *          otherwise the rotation correlation time is returned.
 *
 * This routine is based on the eqn. (9.2.49) of N. Atherton's book.
 *
 * NOTE: both hyperfine matrix and the g-tensor are traceless.
 *
 * THIS HAS NOT BEEN TESTED!!!!!!!!
 *
 * TODO: review not complete (jme).
 *
 */

double epr_calc_rot_b(epr_type *sp, double b, double ag, double ws, double bc) {

  double a1, a2, a3, di;
  double q, r, s, t, d, co;

  b *= 2.0 * M_PI * 2.802494 * 1E6;
  co = ag * EPR_GAUSS_GE_NRAD * EPR_GAUSS_UE * bc / EPR_HBAR;
  di = -(4.0/15.0) * ws * ws;
  a1 = b * ws * ws / (co * di);
  a2 = b * (-3.0 / 15.0) / (di * co);
  a3 = b / (co * di);

  q = (3.0 * a2 - a1 * a1) / 9.0;
  r = (9.0 * a1 * a2 - 27.0 * a3 - 2.0 * a1 * a1 * a1) / 54.0;
  d = q * q * q + r * r;
  if(d <= 0) return -1.0; /* roots are unequal */

  s = pow(r + sqrt(q * q * q + r * r), 3.0);
  t = pow(r - sqrt(q * q * q + r * r), 3.0);
  return (s + t - a1 / 3.0); /* rest of the roots are imagnary */
}
