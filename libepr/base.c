/*
 * Baseline correction functions.
 * Last modified: Oct-13-2009 jmeloranta@gmail.com
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
 * Function: epr_type *epr_base_line(epr_type *, i, j, n)
 * Purpose: Perform simple background correction (line)
 *
 * sp = spectrum to be corrected [input/output; epr_type *]
 * i  = first point for line [input; int]
 * j  = second point for line [input; int]
 * a =  number of points around i and j [input; int]
 *
 * return value: ptr to sp. Returns NULL on error.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_base_line(epr_type *sp, int i, int j, int a) {

  double ay, ly, k, b, *y;
  int n, l, mi, mj;

  if (!a) return (epr_type *) 0;

  y = sp->y;
  n = sp->samples;

  for (l = i, ay = 0.0; l < a+i; l++)
    ay += y[l];
  ay /= a;

  for (l = j - a, ly = 0.0; l < j; l++)
    ly += y[l];
  ly /= a;

  mi = i + a / 2;
  mj = j - a / 2;
  k = (ly - ay) / (mj - mi);
  b = ay - k * mi;
  for (l = 0; l < n; l++)
    y[l] -= k * l + b;

  return sp;
}

/*
 * Function: epr_type *epr_base_polyn(epr_type *, int, int *)
 * Purpose: Perform polynomial background correction
 *
 * sp = spectrum to be corrected [input/output; epr_type *]
 * d  = degree of the polynom [input; int]
 * p =  array of spectrum points (note: int!) [input; int]
 *      (must contain d+1 points)
 *
 * return value: ptr to sp (NULL on error)
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_base_polyn(epr_type *sp, int d, int *p) {

  int i, j, k;
  double x, *y;
  double a[100], b[10];

  y = sp->y;
  d++;

  if(d >= 10 || d <= 0) return (epr_type *) 0;

  for (i = 0; i < d; i++) {
    for (j = 0; j < d; j++)
      a[EPR_INDEX(d, i, j)] = 0.0;
    b[i] = 0.0;
  }

  for (i = 0; i < d; i++) {
    if (d <= 2) {
      a[EPR_INDEX(d, 1, 0)] += (double) p[i];
      a[EPR_INDEX(d, 1, 1)] += (double) p[i] * (double) p[i];
      b[1] += (double) p[i] * y[p[i]];
    } else {
      for (j = 1; j < d; j++) {
	for (k = j - 1; k <= j; k++)
	  a[EPR_INDEX(d, j, k)] += (double) pow((double) p[i], (double)(j+k));
	b[j] += y[p[i]] * (double) pow((double) p[i], (double)j);
      }
    }
    b[0] += y[p[i]];
    ++a[EPR_INDEX(d, 0, 0)];
  }
  
  if(d >= 3)
    for(i = 2; i < d; i++)
      for(j = 0; j <= i-2; j++)
	a[EPR_INDEX(d, i, j)] = a[EPR_INDEX(d, i-1, j+1)];
  
  if(d > 1)
    for(i = 1; i < d; i++)
      for(j = 0; j < i ; j++)
	a[EPR_INDEX(d, j, i)] = a[EPR_INDEX(d, i, j)];
  
  epr_misc_linear(a, d, b);
  
  for(i = 0; i < sp->samples; i++) {
    x = 0.0;
    for(j = d-1; j >= 0; j--)
      x = x * (double) i + b[j];
    y[i] -= x;
  }

  return sp;
}

/*
 * Function: epr_type *epr_base_spline(epr_type *, int, int *)
 * Purpose: Perform spline background correction
 *
 * sp = spectrum to be corrected [input/output; epr_type *]
 * d  = number of spline points [input; int]
 * p  = array of spline points (note integers!) [input; int *]
 *
 * return value: ptr to sp (NULL on error).
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_base_spline(epr_type *sp, int d, int *p) {
  
  double xa[25], ya[25], *y, tmp_y[EPR_MAX_SAMPLES];
  int i;

  if (d >= 25) return (epr_type *) 0;
  y = sp->y;

  for (i = 0; i < d; i++) {
    xa[i] = (double) p[i];
    ya[i] = y[p[i]];
  }
  epr_base_splin1(xa, ya, d, EPR_LARGE, EPR_LARGE, tmp_y);
  for (i = 0; i < sp->samples; i++)
    y[i] -= epr_base_splie1(xa, ya, tmp_y, d, (double) i);
  return sp;
}

/*
 * 1D spline generation.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

void epr_base_splin1(double *x, double *y, int n, double yp1, double ypn, double *y2) {

  int i, k;
  double p, qn, sig, un;
  static double *u;
  static int inited = 0;
  
  if(!inited) {
    u = (double *) calloc(25, sizeof(double));
    inited = 1;
  }
  if (yp1 >= EPR_LARGE) {
    y2[0] = 0.0;
    u[0] = 0.0;
  } else {
    y2[0] = -0.5;
    u[0] = (3.0 / (x[1] - x[0])) * ((y[1] - y[0]) / (x[1] - x[0]) - yp1);
  }

  for (i = 1; i < n - 1; i++) {
    sig = (x[i] - x[i-1]) / (x[i+1] - x[i-1]);
    p = sig * y2[i - 1] + 2.0;
    y2[i] = (sig - 1.0) / p;
    u[i] = (y[i + 1] - y[i]) / (x[i + 1] - x[i]) - (y[i] - y[i - 1])
      / (x[i] - x[i - 1]);
    u[i] = (6.0 * u[i] / (x[i + 1] - x[i - 1]) - sig * u[i - 1]) / p;
  }

  if (ypn >= EPR_LARGE) {
    qn = 0.0;
    un = 0.0;
  } else {
    qn = 0.5;
    un = (3.0 / (x[n - 1] - x[n - 2])) * (ypn - (y[n - 1] - y[n - 2]) / (x[n - 1] - x[n - 2]));
  }

  y2[n - 1] = (un - qn * u[n - 2]) / (qn * y2[n - 2] + 1.0);

  for (k = n - 2; k >= 0; k--)
    y2[k] = y2[k] * y2[k + 1] + u[k];
}

/*
 * 1D spline evaluation.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

double epr_base_splie1(double *xa, double *ya, double *y2a, int n, double x) {

  int klo, khi, k;
  double h, b, a;
  
  klo = 0;
  khi = n - 1;

  while (khi - klo > 1) {
    k = (khi + klo) / 2;
    if (xa[k] > x) khi = k;
    else klo = k;
  }

  h = xa[khi] - xa[klo];
  if (h == 0.0) {
    fprintf(stderr,"libepr: error in epr_base_splie1().\n");
    return (double) 0.0;
  }

  a = (xa[khi] - x) / h;
  b = (x - xa[klo]) / h;

  return a * ya[klo] + b * ya[khi] + ((a * a * a - a) * y2a[klo] + (b * b * b - b)
				    * y2a[khi]) * h * h / 6.0;
}

/*
 * 2D spline generation.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

void epr_base_splie2(double *x1a, double *x2a, double **ya, int m, int n, double **y2a) {

  int j;

  for (j = 0; j < m; j++)
    epr_base_splin1(x2a, ya[j], n, EPR_LARGE, EPR_LARGE, y2a[j]);
}

/*
 * 2D spline evaluation.
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

void epr_base_splin2(double *x1a, double *x2a, double **ya, double **y2a, int m, int n, double x1, double x2, double *y) {

  int j;
  static double *ytmp, *yytmp;
  static int inited = 0;

  if(!inited) {
    ytmp = (double *) calloc(25, sizeof(double));
    yytmp = (double *) calloc(25, sizeof(double));
    inited = 1;
  }

  for (j = 0; j < m; j++)
    yytmp[j] = epr_base_splie1(x2a, ya[j], y2a[j], n, x2);

  epr_base_splin1(x1a, yytmp, m, EPR_LARGE, EPR_LARGE, ytmp);

  *y = epr_base_splie1(x1a, yytmp, ytmp, m, x1);
}
