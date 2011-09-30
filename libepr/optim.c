/*
 * Optimization routines.
 * Last modified: Oct-13-2009 jussi.eloranta@csun.edu
 *
 * Written by:
 * Jussi Eloranta (jussi.eloranta@csun.edu)
 *
 * Department of chemistry and biochemistry
 * California State University at Northridge
 *
 * TODO: add simulated annealing and genetic algortihm based methods.
 *
 */

#include "epr.h"

/*
 * Global variables (may be set from application).
 *
 */

double epr_optim_end = 0.1;        /* line search width */
double epr_optim_start = 0.0;      /* min. value for line search */
double epr_optim_width;            /* does this need to be global? */
double epr_optim_ltol = 1.0E-12;    /* line search accuracy */
double epr_optim_tol = 1.0E-12;     /* optimization tolerance (grad norm) */
double epr_optim_ctol;             /* reached optimization tolerance (grad norm) */
double epr_optim_maxit = 100;      /* max # of iterations in optimization */
double epr_optim_dstep = 1.0E-16;   /* difference step length */
int epr_optim_debug = 0;          /* debug optimization at specified iters */
int epr_optim_debug_line = 0;     /* debug line search at specified iters */
int epr_optim_line_maxretry = 10; /* maximum line search restart times */

/*
 * Function: epr_optim_hj(double (*f)(int n, epr_variable *x), double rho, int n,
 *                        epr_variable *x, int maxiter)
 *
 * Optimization using the method of Hooke and Jeeves.
 *
 * f       = pointer to the function to be optimized [input; double]
 * rho     = convergence parameter for optimization [input; double]
 * n       = dimension of the optimization problem [input; int]
 * x       = current optimization point [input/output; double * of length n]
 * maxiter = maximum number of iterations [input; int]
 *
 * return value: value of f at minimum.
 *
 * This implementation is heavily based on code written by
 * M. G. Johnson (see netlib).
 *
 */

/* aux routine */
static double epr_optim_bn(double (*f)(int, epr_variable *), epr_variable *x, double prevbest, int n) {

  double minf, ftmp;
  int i, j;

  minf = prevbest;
  for (i = 0; i < n; i++)
    x[i].oderiv = *(x[i].value);
  for (i = 0; i < n; i++) {
    *(x[i].value) = x[i].oderiv + x[i].deriv;
    for (j = 0; j < x[i].nequiv; j++) *(x[i].equiv[j]) = *(x[i].value);
    ftmp = (*f)(n, x);
    if(ftmp < minf) minf = ftmp;
    else {
      x[i].deriv *= -1.0;
      *(x[i].value) = x[i].oderiv + x[i].deriv;
      for (j = 0; j < x[i].nequiv; j++) *(x[i].equiv[j]) = *(x[i].value);
      ftmp = f(n, x);
      if(ftmp < minf) minf = ftmp;
      else {
	*(x[i].value) = x[i].oderiv;
	for (j = 0; j < x[i].nequiv; j++) *(x[i].equiv[j]) = *(x[i].value);
      }
    }
  }
  return minf;
}

double epr_optim_hj(double (*f)(int, epr_variable *), double rho, int n, epr_variable *x) {

  int i, j, iadj, iter;
  int keep, maxiter;
  double steplength, fbefore, newf, tmp;

  maxiter = epr_optim_maxit;

  for (i = 0; i < n; i++) {
    x[i].ovalue = *(x[i].value);
    x[i].deriv = fabs(*(x[i].value) * rho);
    if(x[i].deriv == 0.0) x[i].deriv = rho;
  }

  iadj = 0;
  steplength = rho;
  fbefore = (*f)(n, x);
  newf = fbefore;

  for (iter = 0; iter < maxiter && steplength > EPR_EPS; ) {
    iter++;
    iadj++;
    fprintf(stderr,"libepr: at iteration %d RMS error = %le\n", iter, fbefore);
    for (i = 0; i < n; i++) {
      *(x[i].value) = x[i].ovalue;
      for (j = 0; j < x[i].nequiv; j++) *(x[i].equiv[j]) = *(x[i].value);
    }
    /* newx contains both value and delta */
    newf = epr_optim_bn(f, x, fbefore, n);
    keep = 1;
    while(newf < fbefore && keep) {
      iadj = 0;
      for (i = 0; i < n; i++) {
	if(*(x[i].value) <= x[i].ovalue)
	  x[i].deriv = -fabs(x[i].deriv);
	else
	  x[i].deriv = fabs(x[i].deriv);
	tmp = x[i].ovalue;
	x[i].ovalue = *(x[i].value);
	*(x[i].value) = 2.0 * *(x[i].value) - tmp;
	for (j = 0; j < x[i].nequiv; j++) *(x[i].equiv[j]) = *(x[i].value);
      }
      fbefore = newf;
      newf = epr_optim_bn(f, x, fbefore, n);
      if(newf >= fbefore) break;
      keep = 0;
      for (i = 0; i < n; i++) {
	keep = 1;
	if(fabs(*(x[i].value) - x[i].ovalue) > 0.5*fabs(x[i].deriv))
	  break;
	else keep = 0;
      }
    }
    if(steplength >= EPR_EPS && newf >= fbefore) {
      steplength *= rho;
      for (i = 0; i < n; i++)
	x[i].deriv *= rho;
    }
  }
  for (i = 0; i < n; i++) {
    *(x[i].value) = x[i].ovalue;
    for (j = 0; j < x[i].nequiv; j++) *(x[i].equiv[j]) = *(x[i].value);
  }

  return newf;
}

/*
 * Function: 
 *    double epr_optim_cgrad(double (*f)(int n, epr_variable *x),
 *                           void (*grad)(int n, epr_variable *x), int n, 
 *                           epr_variable *x)
 *
 * Optimization using conjugate gradient method.
 *
 * f    = pointer to function to be optimized [input;
 *        double (*f)(int n, epr_variable *x)]
 * grad = pointer to gradient of f [input; double (*grad)(int n, epr_variable *x)]
 *        If grad is NULL then gradient is estimated using finite differences.
 * n    = dimension of the optimization problem [input; int]
 * x    = current optimization point. Vector of dimension n.
 *                                            [input/output; epr_variable *]
 * Purpose: Perform conjugate gradient optimization (min) on function (*f)()
 *          using analytical gradient (*grad)() or finite difference
 *          approximation.
 *
 * return value: value of f at minimum.
 *
 */

double epr_optim_cgrad(double (*f)(int, epr_variable *), void (*grad)(int, epr_variable *), int n, epr_variable *x) {

  double err, lambda, step, n1, n2, rval;
  int i, j, k;

  for (j = 0; j < epr_optim_maxit; j++) {
    for (i = 0; i < n; i++)
      x[i].oderiv = x[i].deriv;
    /* -gradient */
    if(grad)
      (*grad)(n, x);
    else
      epr_optim_diff(f, n, x);
    n2 = n1;
    n1 = 0.0;
    for (i = 0; i < n; i++)
      n1 += x[i].deriv * x[i].deriv;
    for (i = 0; i < n; i++) {
      /* TODO: should we add or substract the norm stuff? */
      /* which order of these two lines should be? */
      /* this may be wrong!!! Check this out! */
      x[i].deriv *= -1.0;
      if ((j % n)) x[i].deriv += x[i].oderiv * n1 / n2;
    }
    lambda = epr_optim_line(f, n, x);
    if(lambda == 0.0) break; /* stop requested by line search */
    err = 0.0;
    for (i = 0; i < n; i++) {
      double tmp;
      tmp = *(x[i].value) + x[i].deriv * lambda;
      *(x[i].value) = tmp;
      for (k = 0; k < x[i].nequiv; k++) /* update equivalent mem. locations*/
	*(x[i].equiv[k]) = tmp;
      err += x[i].deriv * x[i].deriv;       /* gradient norm */
    }
    err = sqrt(err);
    if (epr_optim_debug && !(j % epr_optim_debug))
      fprintf(stderr,"libepr: iteration %d, object function value = %lf, Gradient norm = %lf\n", j, (*f)(n,x), err);
    if(err < epr_optim_tol) break;
  }

  epr_optim_ctol = err;
  if(epr_optim_debug) fprintf(stderr,"libepr: total number of iterations = %d.\n", j);
  return (*f)(n,x);
}

/*
 * Function: 
 *    double epr_optim_grad(double (*f)(int n, epr_variable *x),
 *                           void (*grad)(int n, epr_variable *x), int n, 
 *                           epr_variable *x)
 *
 * f    = pointer to function to be optimized [input;
 *        double (*f)(int n, epr_variable *x)]
 * grad = pointer to gradient of f [input; double (*grad)(int n, epr_variable *x)]
 *        If grad is NULL then gradient is estimated using finite differences.
 * n    = dimension of the optimization problem [input; int]
 * x    = current optimization point. Vector of dimension n.
 *                                            [input/output; epr_variable *]
 *
 * Purpose: Perform gradient optimization (min) on function (*f)() using
 *          analytical gradient (*grad)() or finite difference approximation.
 *
 * return value: value of f at minimum.
 *
 */

double epr_optim_grad(double (*f)(int, epr_variable *), void (*grad)(int, epr_variable *), int n, epr_variable *x) {

  double err, lambda, step, nrm;
  int i, j, k;

  for (j = 0; j < epr_optim_maxit; j++) {
    /* -gradient */
    if(grad)
      (*grad)(n, x);
    else
      epr_optim_diff(f, n, x);
    for (i = 0; i < n; i++) {
      x[i].deriv *= -1.0;
      nrm += x[i].deriv*x[i].deriv;
    }
    nrm = sqrt(nrm);
    for (i = 0; i < n; i++) x[i].deriv /= nrm;
    lambda = epr_optim_line(f, n, x);
    if(lambda == 0.0) break; /* stop requested by line search */
    err = 0.0;
    for (i = 0; i < n; i++) {
      double tmp;
      tmp = *(x[i].value) + x[i].deriv * lambda;
      *(x[i].value) = tmp;
	for (k = 0; k < x[i].nequiv; k++) /* update equivalent mem. locs */
	  *(x[i].equiv[k]) = tmp;
      err += x[i].deriv * x[i].deriv;       /* gradient norm */
    }
    err = sqrt(err);
    if (epr_optim_debug && !(j % epr_optim_debug))
      fprintf(stderr,"libepr: iteration %d, object function value = %lf, Gradient norm = %lf\n", j, (*f)(n,x), err);
    if(err < epr_optim_tol) break;
  }

  epr_optim_ctol = err;
  if(epr_optim_debug) fprintf(stderr,"libepr: total number of iterations = %d.\n", j);
  return (*f)(n,x);
}

/*
 * Function: 
 *    double epr_optim_line(double (*f)(int n, epr_variable *x), int n, epr_variable *x);
 *
 * f    = pointer to function to be optimized [input;
 *        double (*f)(int n, epr_variable *x)]
 * n    = dimension of the optimization problem [input; int]
 * x    = current optimization point. Vector of dimension n.
 *                                            [input/output; epr_variable *]
 *
 * Purpose: Perform line search from x to lambda * grad using golden crossing
 *          method.
 *
 * return value: value of the optimal step in line search.
 *
 */

/* Golden crossing constant */
#define EPR_OPTIM_CROSS 0.618033988749894903

double epr_optim_line(double (*f)(int, epr_variable *), int n, epr_variable *x) {

  double err, x1, x2, f1, f2, a, b, width, val, orig_val;
  int x1flag = 1, x2flag = 1, i, j, k, l;

  orig_val = (*f)(n, x);
  epr_optim_width = fabs(epr_optim_start - epr_optim_end);
  a = epr_optim_start;
  b = epr_optim_end;
  if(b - a < 0.0) return 0.0;
  k = 1;
  do {
    if(k > 1 && epr_optim_debug_line)
      fprintf(stderr,"libepr: line search restarted k = %d.\n", k);
    for (j = 0; ; j++) {
      width = b - a;
      if(width < epr_optim_ltol) break;
      if(x2flag) {
	x2 = a + EPR_OPTIM_CROSS * width;
	for (i = 0; i < n; i++) {
	  double tmp;
	  x[i].ovalue = *(x[i].value);
	  tmp = *(x[i].value) + x2 * x[i].deriv;
	  *(x[i].value) = tmp;
	  for (l = 0; l < x[i].nequiv; l++)
	    *(x[i].equiv[l]) = tmp;
	}
	f2 = (*f)(n, x);
	for (i = 0; i < n; i++) {
	  *(x[i].value) = x[i].ovalue;
	  for (l = 0; l < x[i].nequiv; l++)
	    *(x[i].equiv[l]) = x[i].ovalue;
	}
      }
      if(x1flag) {
	x1 = b - EPR_OPTIM_CROSS * width;
	for (i = 0; i < n; i++) {
	  double tmp;
	  x[i].ovalue = *(x[i].value);
	  tmp = *(x[i].value) + x1 * x[i].deriv;
	  *(x[i].value) = tmp;
	  for (l = 0; l < x[i].nequiv; l++)
	    *(x[i].equiv[l]) = tmp;
	}
	f1 = (*f)(n, x);
	for (i = 0; i < n; i++) {
	  *(x[i].value) = x[i].ovalue;
	  for (l = 0; l < x[i].nequiv; l++)
	    *(x[i].equiv[l]) = x[i].ovalue;
	}
      }
      x1flag = x2flag = 0;
      if(epr_optim_debug_line && !(j % epr_optim_debug_line))
	fprintf(stderr,"libepr: interval [%lf,%lf] with function values [%lf,%lf].\n",
	       a, b, f1, f2);
      if(f1 < f2) {
	b = x2;
	x2 = x1;
	f2 = f1;
	x1flag = 1; /* need only x1 */
      } else {
	a = x1;
	x1 = x2;
	f1 = f2;
	x2flag = 1; /* need only x2 */
      }    
    }
    val = (f1 + f2) / 2.0;
    k++;
    a = epr_optim_start + epr_optim_width / (double) k;
    b = epr_optim_end - epr_optim_width / (double) k;
  } while (val >= orig_val  && k < epr_optim_line_maxretry);
  if(k == epr_optim_line_maxretry) return 0.0; 
  return (a + b) / 2.0;
}

/*
 * Function: 
 *    void epr_optim_diff(double (*f)(int n, epr_variable *x), int n, epr_variable *x)
 *
 * f    = pointer to function for which the gradient is estimated [input;
 *        double (*f)(int n, epr_variable *x)]
 * n    = dimension of f [input; int]
 * x    = current f evaluation point. Vector of dimension n.
 *        [input/output; epr_variable *]
 *
 * Purpose: If analytical gradient is not available then this routine can
 *          be used to estimate the gradient vector (using finite difference).
 *
 * return value: none.
 *
 */

void epr_optim_diff(double (*f)(int, epr_variable *), int n, epr_variable *x) {

  double yf1, yf2, yb1, yb2;
  int i;
  
  for (i = 0; i < n; i++) {
    *(x[i].value) += epr_optim_dstep;
    yf1 = (*f)(n, x);
    *(x[i].value) += epr_optim_dstep;
    yf2 = (*f)(n, x);
    *(x[i].value) -= 3.0 * epr_optim_dstep;
    yb1 = (*f)(n, x);
    *(x[i].value) -= epr_optim_dstep;
    yb2 = (*f)(n, x);
    *(x[i].value) += 2.0 * epr_optim_dstep;
    x[i].deriv = (yb2 - yf2 - 8.0 * (yb1 - yf1)) / 12.0;
  }
}

/*
 * Function: double epr_optim_monte_carlo(double (*f)(int n, epr_variable *x), int n,
 *                                      epr_variable *x, int iter)
 *
 * Purpose: Monte Carlo optimization routine.
 *
 * f    = pointer to function to be optimized
 *        [input; double (*f)(int, epr_variable *)]
 * n    = dimension of the optimization problem [input; int n]
 * x    = current optimization point. Vector of dimension n. [input/output;
 *        epr_variable *]
 *
 * return value: value of f at minimum.
 *
 */

double epr_optim_monte_carlo(double (*f)(int n, epr_variable *x), int n, epr_variable *x) {

  int i, j, k;
  double ival, v, v2;

  srand48((long) time(0));
  ival = (*f)(n, x);
  for (i = 0; i < (int) epr_optim_maxit; i++) {
    for (k = 0; k < n; k++) {
      double tmp;
      v = 2.0 * (drand48() - 0.5) * x[k].mc_limit;
      x[k].ovalue = *(x[k].value);
      tmp = *(x[k].value) + v;
      if(tmp <= x[k].high && tmp >= x[k].low) {
	*(x[k].value) = tmp;
	for (j = 0; j < x[k].nequiv; j++)
	  *(x[k].equiv[j]) = tmp;
      } else fprintf(stderr,"libepr: (%s) Limit problem: %le -> %le (%le, %le).\n",
		   x[k].name, *(x[k].value), tmp, x[k].high, x[k].low);

    }
    v2 = (*f)(n, x);
    if (v2 >= ival) {
      for (k = 0; k < n; k++) {
	*(x[k].value) = x[k].ovalue;
	for (j = 0; j < x[k].nequiv; j++)
	  *(x[k].equiv[j]) = x[k].ovalue;
      }
    } else {
      ival = v2;
      if(epr_optim_debug)
	fprintf(stderr,"libepr: iteration %d, object function = %lf.\n", i, ival);
    }
  }
  return ival;
}

/*
 * Function: double epr_optim_simplex(double (*f)(int n, epr_variable *x), int n,
 *                  epr_variable *x, int iter)
 *
 * Purpose: Simplex optimization routine.
 *
 * f    = pointer to function to be optimized
 *                                     [input; double (*)(int, epr_variable *)]
 * n    = dimension of the optimization problem [input, int]
 * x    = epr_variable array [input/output; epr_variable *]
 * iter = maximum number of iterations [input; int]
 * 
 * return value: value of f at minimum.
 *
 */

double epr_optim_simplex(double (*f)(int n, epr_variable *x), int n, epr_variable *x) {

  epr_variable *tmp[512], *p, *trial;
  int i, j, k, l, wi;
  double y, yt, values[512], wf, bf, last_wf, last_bf;

  /* memory allocation & initial simplex building */
  if (n > 512) return -1.0; /* too big... */
  if(!(p = (epr_variable *) malloc(sizeof(epr_variable)*n))) {
    fprintf(stderr, "libepr: out of memory\n");
    return -1.0;
  }
  if(!(trial = (epr_variable *) malloc(sizeof(epr_variable)*n))) {
    fprintf(stderr, "libepr: out of memory\n");
    return -1.0;
  }
  for (i = 0; i < n; i++) {
    if(!(p[i].value = (double *) malloc(sizeof(double)))) {
      fprintf(stderr, "libepr: out of memory\n");
      return -1.0;
    }
    if(!(trial[i].value = (double *) malloc(sizeof(double)))) {
      fprintf(stderr, "libepr: out of memory\n");
      return -1.0;
    }
  }
  for (i = 0; i < n; i++) {
    *(trial[i].value) = *(x[i].value);
    trial[i].high = x[i].high;
    trial[i].low = x[i].low;
    trial[i].mc_limit = x[i].mc_limit;
  }

  /* Initial simplex */
  for (i = 0; i < n+1; i++) {
    if(!(tmp[i] = (epr_variable *) malloc(sizeof(epr_variable) * n))) {
      fprintf(stderr, "libepr: out of memory\n");
      return -1.0;
    }
    for (j = 0; j < n; j++) {
      if(!((tmp[i]+j)->value = (double *) malloc(sizeof(double)))) {
	fprintf(stderr, "libepr: out of memory\n");
	return -1.0;
      }
      if(i > 0) {
	k = 0;
	do {
	  y = *(trial[j].value) + 2.0 * (drand48() - 0.5) * x[j].mc_limit;
	  if(y > trial[j].high || y < trial[j].low) {
	    fprintf(stderr,"libepr: %d. (%s) Limit problem: %le -> %le (%le, %le).\n", k+1, 
		   x[j].name, *(x[j].value), y, x[j].high, x[j].low);
	    k++;
	    continue;
	  } 
	  break;
	} while (k < 20);
	if(k == 20)
	  return -1.0; /* problem with input data ! */
      } else y = *(x[j].value); /* starting point in simplex */
      *(x[j].value) = *((tmp[i]+j)->value) = y;
      for (l = 0; l < x[j].nequiv; l++) /* update equivalnces */
	*(x[j].equiv[l]) = y;
    }
    values[i] = (*f)(n, x);
  }

  /* simplex iterations */
  last_bf = last_wf = -1.0;
  for (i = 0; i < epr_optim_maxit; i++) {

    /* worst point */
    last_bf = bf; last_wf = wf;
    bf = wf = values[0];
    wi = 0;
    for (j = 1; j < n+1; j++) {
      if(values[j] > wf) {
	wi = j;
	wf = values[j];
      }
      if(values[j] < bf) bf = values[j];
    }

    /* "check for degeneration" */
    if(last_bf == bf && last_wf == wf) {
      if(epr_optim_debug) fprintf(stderr,"libepr: simplex degenerated!\n");
      break;
    }
    if(!(i % epr_optim_debug)) 
      fprintf(stderr,"libepr: iteration %d (%lf,%lf)\n", i, bf, wf);

    for (j = 0; j < n; j++) { /* for each coord */
      /* centroid */
      *(p[j].value) = 0.0;
      for (k = 0; k < n+1; k++) /* for each point */
	if(k != wi)
	  *(p[j].value) += *((tmp[k]+j)->value);
      *(p[j].value) /= (double) n;
    }

    for (j = 0; j < n; j++) {
      *(x[j].value) = 2.0 * (*(p[j].value)) - *((tmp[wi]+j)->value);
      for (l = 0; l < x[j].nequiv; l++)
	*(x[j].equiv[l]) = *(x[j].value);
    }
    y = (*f)(n, x);

    /* trial better than other points ? */
    for (j = 0; j < n+1; j++)
      if(j != wi && y > values[j])
	break;

    if(j == n+1) { /* R best (1st case) */
      /* try extension */
      for (j = 0; j < n; j++) {
	*(x[j].value) = 3.0 * (*(p[j].value)) - 2.0 * (*((tmp[wi]+j)->value));
	for (l = 0; l < x[j].nequiv; l++)
	  *(x[j].equiv[l]) = *(x[j].value);
      }
      if((yt = (*f)(n,x)) < y) {
	for (j = 0; j < n; j++)
	  *((tmp[wi]+j)->value) = *(x[j].value);
	values[wi] = yt;
      } else {
	for (j = 0; j < n; j++)
	  *((tmp[wi]+j)->value) = 2.0 * (*(p[j].value)) - *((tmp[wi]+j)->value);
	values[wi] = y;
      }
    } else {
      for (j = 0; j < n+1; j++)
	if(j != wi && y < values[j])
	  break;
      if(j != n+1) { /* mixed (2nd case) */
	for(j = 0; j < n; j++)
	  *((tmp[wi]+j)->value) = *(x[j].value);
	values[wi] = y;
      } else { /* contract (3rd case) */
	double ctmp;
	if(y < values[wi]) ctmp = 0.5; else ctmp = -0.5;
	for(j = 0; j < n; j++) {
	  *(x[j].value) = *((tmp[wi]+j)->value) = *(p[j].value) +
	    ctmp*(*(p[j].value) - *((tmp[wi]+j)->value));
	  for (l = 0; l < x[j].nequiv; l++)
	    *(x[j].equiv[l]) = *(x[j].value);
	}
	values[wi] = (*f)(n,x);
      }
    }
  }
  wi = 0;
  wf = values[0];
  for (i = 1; i < n+1; i++)
    if(values[i] < wf) {
      wf = values[i];
      wi = i;
    }

  for(i = 0; i < n; i++) {
    *(x[i].value) = *((tmp[wi]+i)->value);
    for (l = 0; l < x[i].nequiv; l++)
      *(x[i].equiv[l]) = *(x[i].value);
    x[i].high = trial[i].high;
    x[i].low = trial[i].low;
    x[i].mc_limit = trial[i].mc_limit;
  }

  for(i = 0; i < n; i++) {
    free(p[i].value);
    free(trial[i].value);
  }
  free(p);
  free(trial);
  for(i = 0; i < n+1; i++) {
    for(j = 0; j < n; j++)
      free((tmp[i]+j)->value);
    free(tmp[i]);
  }
  return bf;
}
