
/*
 * Some useful operations for imaginary numbers (doublecomplex)
 *
 */

#include "epr.h"

inline void epr_imag_addto(doublecomplex *a, doublecomplex *b) {

  a->r += b->r;
  a->i += b->i;
}

inline void epr_imag_mult(doublecomplex *a, doublecomplex *b) {

  double tmp;

  tmp = a->r * b->r - a->i * b->i;
  a->i = a->r * b->i + a->i * b->r;
  a->r = tmp;
}

/* a = a/b */
inline void epr_imag_div(doublecomplex *a, doublecomplex *b) {

  double tmpr, tmpi, div;

  div = b->i * b->i + b->r * b->r;
  if(div == 0.0) div = EPR_EPS;
  tmpr = (a->r * b->r + a->i * b->i) / div;
  tmpi = (a->i * b->r - a->r * b->i) / div;
  a->r = tmpr;
  a->i = tmpi;
}

/*
 * Multiply a by b and put the result in a.
 * (complex version)
 *
 */

inline void epr_imag_matr_mult(doublecomplex *a, doublecomplex *b, int n) {

#if 1
  /* use BLAS */
  static doublecomplex *tmp = NULL;
  static doublecomplex beta = {0.0, 0.0}, alpha = {1.0, 0.0};
  char transa = 'n', transb = 'n';
  
  if(tmp == NULL)
    tmp = (doublecomplex *) malloc(sizeof(doublecomplex) * EPR_MAXDIM * EPR_MAXDIM);
  
  /* BLAS: matrix x matrix */
  zgemm_(&transa, &transb, &n, &n, &n, &alpha, a, &n, b, &n, &beta, tmp, &n);
  bcopy(tmp, a, sizeof(doublecomplex) * n * n);
  /* rwork & tmp never released .. */
#else
  static doublecomplex *tmp = NULL;
  doublecomplex tmp2;
  int i, j, k;

  if(tmp == NULL)
    tmp = malloc(sizeof(doublecomplex) * n);

  for (i = 0; i < n; i++) {
    bzero(tmp, sizeof(doublecomplex) * n);
    for (j = 0; j < n; j++) {
      for (k = 0; k < n; k++) {
	tmp2.r = a[EPR_INDEX(n, i, k)].r;
	tmp2.i = a[EPR_INDEX(n, i, k)].i;
	epr_imag_mult(&tmp2, &b[EPR_INDEX(n, k, j)]);
	tmp[j].r += tmp2.r;
	tmp[j].i += tmp2.i;
      }
    }
    for (j = 0; j < n; j++) {
      a[EPR_INDEX(n, i, j)].r = tmp[j].r;
      a[EPR_INDEX(n, i, j)].i = tmp[j].i;
    }
  }
  /* tmp not released */
#endif
}
