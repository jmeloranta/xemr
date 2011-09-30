/*
 * Common tensor routines (or actually matrix routines..).
 * Last modified: Oct-13-2009 jmeloranta@gmail.com
 *
 * Written by:
 * Jussi Eloranta (jmeloranta@gmail.com)
 *
 */

#include "epr.h"

/*
 * Function: void epr_tensor_mult(double *a, double *b, double *c)
 *
 * Purpose: Multiply tensor a and b and put the result in c.
 *
 * a  = Tensor a [input; double *]
 * b  = Tensor a [input; double *]
 *
 * NOTE: Everything is 3x3 !
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

inline void epr_tensor_mult(double *a, double *b, double *c) {

  int i, j, k;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      for (k = 0; k < 3; k++)
	c[EPR_INDEX(3, i, j)] = a[EPR_INDEX(3, i, k)] * b[EPR_INDEX(3, k, j)];
}

/*
 * Function: void epr_tensor_dir(double *l, double phi, double theta)
 *
 * Purpose: Calculate projection vector l.
 *
 * l     = Projection vector [output; double *]
 * phi   = Phi angle [input; double]
 * theta = Theta angle [input; double]
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

inline void epr_tensor_dir(double *l, double phi, double theta) {
 
  l[0] = sin(theta) * cos(phi);
  l[1] = sin(theta) * sin(phi);
  l[2] = cos(theta);
}
  
/*
 * Function: double epr_tensor_project(double *t, double *l)
 *
 * Purpose: Calculate projection of tensor along specified direction.
 *
 * t     = Tensor to be projected [input; double *]
 * l     = Projection vector [input; double *]
 *
 * Returns the projection value in the direction of l.
 *
 * NOTE: 3x3 only !
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

inline double epr_tensor_project(double *t, double *l) {

  int i, j;
  double tmp1[3], tmp2[3];

  tmp1[0] = tmp1[1] = tmp1[2] = 0.0;
  tmp2[0] = tmp2[1] = tmp2[2] = 0.0;

  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      tmp1[i] += l[j] * t[EPR_INDEX(3, j, i)];
  
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      tmp2[i] += t[EPR_INDEX(3, i, j)] * l[j];

  return sqrt(tmp1[0] * tmp2[0] + tmp1[1] * tmp2[1] + tmp1[2] * tmp2[2]);
}

/*
 * Function: double epr_tensor_project2(double *t, double *u, double *l)
 *
 * Purpose: Calculate projection of product of two tensors along l.
 *          That is sqrt(l.t.u.u.t.l / l.t.t.l).
 *          (applied, for example, in sqrt(l.g.A.A.g.l / (l.g.g.l)))
 *
 * t     = 1. Tensor to be projected [input; double *]
 * u     = 2. Tensor to be projected [input; double *]
 * l     = Projection vector [input; double *]
 *
 * Returns the projection value in the direction of  l.
 *
 * NOTE: 3x3 only !
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

inline double epr_tensor_project2(double *t, double *u, double *l) {

  int i, j;
  double tmp1[3], tmp2[3], tmp3[3], tmp4[3], tmp5;

  tmp1[0] = tmp1[1] = tmp1[2] = 0.0;
  tmp2[0] = tmp2[1] = tmp2[2] = 0.0;
  tmp3[0] = tmp3[1] = tmp3[2] = 0.0;
  tmp4[0] = tmp4[1] = tmp4[2] = 0.0;

  /* l.t */
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      tmp1[i] += l[j] * t[EPR_INDEX(3, j, i)];
  
  /* t.l */
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      tmp2[i] += t[EPR_INDEX(3, i, j)] * l[j];

  /* l.t.t.l */
  tmp5 = 0.0;
  for (i = 0; i < 3; i++)
    tmp5 += tmp1[i] * tmp2[i];

  /* tmp1.A */
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      tmp3[i] += tmp1[j] * u[EPR_INDEX(3, j, i)];

  /* A.tmp2 */
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      tmp4[i] += u[EPR_INDEX(3, i, j)] * tmp2[j];

  /* sqrt(tmp3.tmp4 / tmp5) */
  return sqrt((tmp3[0]*tmp4[0] + tmp3[1]*tmp4[1] + tmp3[2]*tmp4[2]) / tmp5);
}

/*
 * Function: void epr_tensor_diag(double *a, double *o, double *e)
 *
 * Purpose: Diagonalize 3x3 tensor.
 *
 * a = 3x3 tensor [input; double *]
 * o = 3x3 unitary rotation matrix [output; double *]
 * e = principal values for tensor [output; double *]
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

void epr_tensor_diag(double *a, double *o, double *e) {

  doublecomplex t[9], u[9];
  int i;

  for (i = 0; i < 9; i++) {
    t[i].r = a[i];
    t[i].i = 0.0;
  }

  (void) epr_misc_heigen(t, 3, e, u);
  
  for (i = 0; i < 9; i++)
    o[i] = u[i].r;
}

/*
 * Function: void epr_tensor_udiag(double *a, double *o, double *e)
 *
 * Purpose: "undiagonalize" 3x3 tensor, ie. return to the original axis
 *          system after epr_tensor_diag().
 *
 * a = 3x3 tensor [input; double *]
 * o = 3x3 unitary rotation matrix [output; double *]
 * e = principal values for tensor [output; double *]
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

void epr_tensor_udiag(double *a, double *o, double *e) {

  int i, j, k;
  
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++) {
      a[EPR_INDEX(3,i,j)] = 0.0;
      for (k = 0; k < 3; k++)
	a[EPR_INDEX(3,i,j)] += o[EPR_INDEX(3,i,k)] * e[k] * o[EPR_INDEX(3,j,k)];
    }
}
