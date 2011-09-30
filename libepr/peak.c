/*
 * Peak locate functions.
 * Last modified: Oct-13-2009 jussi.eloranta@csun.edu
 *
 * The original code was written by:
 * Dr. Burkhard Kirste, Institut fuer Organische Chemie,
 * Freie Universitaet Berlin, Takustrasse 3,
 * D-1000 Berlin 33, Germany
 * 
 * Modified by:
 * Jussi Eloranta (jussi.eloranta@csun.edu)
 *
 * Department of chemistry and biochemistry
 * California State University at Northridge
 *
 */

#include "epr.h"

/*
 * Function: double epr_peak_baseline(epr_type *)
 * Purpose: Find baseline for spectrum (mean value of the Y-data).
 *
 * sp = spectrum whose baseline value is to be calulated. [input; epr_type *]
 *
 * return value: baseline Y-value.
 *
 */

double epr_peak_baseline(epr_type *sp) {

  double sum;
  int i;

  for(i = 0, sum = 0.0; i < sp->samples; i++)
    sum += sp->y[i];
  return (sum/(double)sp->samples);
}

/*
 * Function: double epr_peak_nlevel(epr_type *)
 * Purpose: Estimate noise level in the spectrum.
 *          This is done by taking the first and last ten points
 *          of the spectrum and finding the min and max Y-values.
 *          This, of course, assumes that the beginning and the end
 *          of the spectrum are "clean".
 *
 * sp    = spectrum from which the noise level is calculated.
 *         [input; epr_type *]
 *
 * return value: noise level.
 *
 */

double epr_peak_nlevel(epr_type *sp) {

  int i;
  double imin, imax, nl;

  if(sp->samples < 20) return 1;
  imin = imax = sp->y[0];
  for (i = 1; i < 10; i++) {
    if(sp->y[i] < imin) imin = sp->y[i];
    else if(sp->y[i] > imax) imax = sp->y[i];
  }
  nl = imax - imin;
  imin = imax = sp->y[sp->samples-10];
  for (i = sp->samples-9; i < sp->samples; i++) {
    if(sp->y[i] < imin) imin = sp->y[i];
    else if(sp->y[i] > imax) imax = sp->y[i];
  }
  nl = (nl + imax - imin) / 2.0;
  if (nl <= 0) nl = 1;
  return nl;
}

/*
 * Function:  int epr_peak_diffs(double *, int, double *, int *, int, double,
 *                              double)
 *
 * peaks  = array of peak positions [input; double *]
 * npeaks = number of peaks in peaks pointer array. [input; int]
 * diffs  = diffs found from peaks. [output; double *]
 * dfound = number of differences found. [output; int *]
 * ndiffs = size of diffs and dfound arrays. [input; int]
 * accur  = accuracy for choosing peaks to diffs array. [input; double]
 * max_hfc= maximum hfc value. [input; double]
 *
 * return value: number of diffs put in diffs and dfound arrays.
 *
 */

int epr_peak_diffs(double *peaks, int npeaks, double *diffs, int *dfound, int ndiffs, double accur, double max_hfc) {

  int i, j, k, l;
  double d;
  int avoid_goto_flag = 1;

  for (i = 0; i < ndiffs; i++) {
    diffs[i] = 0.0;
    dfound[i] = 0;
  }

  l = 0;
  for (i = 0; i < npeaks-1 && avoid_goto_flag; i++)
    for (j = i+1; j < npeaks; j++) {
      d = fabs(peaks[i] - peaks[j]);
      for (k = 0; k < l; k++)
	if(fabs(d - diffs[k]) < accur) {
	  dfound[k]++;
	  break;
	}
      if (k == l && d < max_hfc) {
	  diffs[l] = d;
	  dfound[l] = 1;
	  l++;
	  if (l >= ndiffs) {
	    l = ndiffs;
	    avoid_goto_flag = 0;
	    break;
	  }
	}
    }
  /* Number of peaks is not big... ;-) */
  for (i = 0; i < l-1; i++)
    for (j = i + 1; j < l; j++)
      if (diffs[i] > diffs[j]) {
	d = diffs[i];
	diffs[i] = diffs[j];
	diffs[j] = d;
	k = dfound[i];
	dfound[i] = dfound[j];
	dfound[j] = k;
      }
  return l;
}

/*
 * Function: int epr_peak_find(epr_type *, double *, double *, int, double, char)
 * Purpose: Find peak positions from spectrum.
 *
 * sp        = spectrum from which the peak positions are extracted.
 *             [input; epr_type *]
 * peaks     = array of peak positions [output; double *]
 * widths    = array of peak widths [output; double *]
 * size      = maximum size peaks array. [input; int]
 * snl       = signal to noise limit (noise is multiplied by this).
 *             [input, double]
 * method    = 0 = baseline, 1 = tangent. [input, char]
 * 
 * NOTE: This routine expects that the signal phase is:
 *       counting from the left to right the 1st derivative curve
 *       rises and then falls (ie. first max and then min).
 *       If the phase is wrong then no peaks will be found.
 *
 * return value: number of peaks found.
 *
 * TODO: peak finding for absorption peaks. This assumes 1st derivative...
 */

int epr_peak_find(epr_type *sp, double *peaks, double *widths, int size, double snl, char method) {

  int i, j, k, l, imax, jmax, jmax2;
  double step, *y, nl, y0, maxtg, tg;
  
  nl = epr_peak_nlevel(sp) * snl;
  y0 = epr_peak_baseline(sp);

  k = imax = 0;
  step = EPR_XSTEP(sp);
  y = sp->y;

  for (i = 1; i < sp->samples-1; i++) {
    /* test if y[i] is a local minimum (points below nl are not counted) */
    if(!(y[i] < y[i-1] && y[i+1] >= y[i] && y[imax] - y[i] > nl)) {
      /* test for local maximum */
      if (y[i] >= y[i-1] && y[i+1] < y[i]) imax = i;
      continue;
    }
    if (k >= size) return k;

    widths[k] = fabs(EPR_ITOX(sp,i,step) - EPR_ITOX(sp,imax,step)); /* peak to peak */

    /* Find the center of the peak (left side) */
    if (!method) {
      /* comparison to baseline */
      for (jmax = j = imax; j < i; j++) 
	if (fabs(y[j]) >= y0 && fabs(y[j+1]) <= y0) {
	  jmax = j;
	  break;
	}
    } else {
      /* find the max tangent value */
      maxtg = (y[imax] - y[imax+1]) / step;
      for (jmax = j = imax + 1; j < i; j++) {
	tg = (y[j] - y[j+1]) / step;
	if(tg < maxtg) {
	  jmax = j;
	  maxtg = tg;
	}
      }
    }

    /* ... (right side) */
    if(!method) {
      for (j = i; j > imax; j--)
	if(fabs(y[j]) >= y0 && fabs(y[j-1]) <= y0) break;
    } else {
      /*tangent... */
      maxtg = (y[i-1] - y[i]) / step;
      for (jmax2 = j = i - 1; j > imax; j--) {
	tg = (y[j-1] - y[j]) / step;
	if(tg > maxtg) {
	  jmax2 = j;
	  maxtg = tg;
	}
      }
      j = jmax2;
    }
    
    peaks[k] = (EPR_ITOX(sp,j,step) + EPR_ITOX(sp,jmax,step)) / 2.0;
    k++;
  }

  return k;
}

/*
 * Function: double epr_peak_int2(epr_type *, double, double, char)
 * Purpose: Double integrate over peak [begin, end]
 *
 * sp        = spectrum. [input; epr_type *]
 * begin     = starting point for integration [input; double]
 * end       = ending point for integration [input; double]
 * method    = 0 = real double integral, 1 = integration by parts. [input, char]
 * 
 * return value: integration value.
 *
 */

double epr_peak_int2(epr_type *sp, double begin, double end, char method) {
  
  double value = 0.0, *y, step;
  int ibeg, iend, i;
  double tmp_y[EPR_MAX_SAMPLES], center;
  
  y = sp->y;
  step = EPR_XSTEP(sp);
  ibeg = EPR_XTOI(sp, begin, step);
  iend = EPR_XTOI(sp, end, step);
  center = fabs(end+begin) / 2.0;
  if (method) {
    /* integrate over xbeg to xend (integration by parts) */
    for (i = ibeg; i < iend; i++)
      value -= y[i]*(center - EPR_ITOX(sp,i,step))*step;
  } else {
    for (i = 0; i < sp->samples; i++) {
      value += step * y[i];
      tmp_y[i] = value;
    }
    value = 0.0;
    for (i = ibeg; i < iend; i++)
      value += tmp_y[i];
    value *= step;
  }
  return value;
}

/*
 * Function: double epr_peak_int1(epr_type *, double, double)
 * Purpose: Integrate over [begin,end]
 *
 * sp        = spectrum. [input; epr_type *]
 * begin     = starting point for integration [input; double]
 * end       = ending point for integration [input; double]
 * 
 * return value: integration value
 *
 */

double epr_peak_int1(epr_type *sp, double begin, double end) {

  double value = 0.0, *y, step;
  int ibeg, iend, i;

  y = sp->y;
  step = EPR_XSTEP(sp);
  ibeg = EPR_XTOI(sp,begin,step);
  iend = EPR_XTOI(sp,end,step);
  for (i = ibeg; i < iend; i++)
    value += y[i];
  return value * step;
}

/*
 * Function: double epr_peak_symm(double *, int, double)
 * Purpose: To test the peak symmetry about given X-axis point.
 *
 * peaks     = array of peak positions [input; double]
 * size      = number of peaks array. [input; int]
 * center    = center of spectrum on the X-axis. [input; double]
 * 
 * return value: error on symmetry over all given peaks.
 *
 */

double epr_peak_symm(double *peaks, int size, double center) {

  int i, j, bestj;
  double dc, dcp, diff, bestdiff, error;

  for (i = 0; i < size-1; i++) {
    dc = fabs(peaks[i] - center);
    bestdiff = 9999.0;
    for (j = i + 1; j < size; j++) {
      dcp = fabs(peaks[j] - center);
      diff = fabs(dcp - dc);
      if (diff < bestdiff) {
	bestdiff = diff;
	bestj = j;
      }
    }
    if (dc < bestdiff) continue; /* center line */
    error += bestdiff;
  }
  return error / ((double)size * center);
}
