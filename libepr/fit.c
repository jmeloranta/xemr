/*
 * Spectrum fit routines.
 * Last modified: Oct-13-2009 jussi.eloranta@csun.edu
 *
 * Written by:
 * Jussi Eloranta (jussi.eloranta@csun.edu)
 *
 * Department of chemistry and biochemistry
 * California State University at Northridge
 *
 * For information about these routines see:
 * B. Kirste, Anal. Chim. Acta 265 (1992) 191-200.
 * 
 */

#include "epr.h"

/*
 * Function: double epr_fit_sp(epr_type *, epr_type *, epr_sim_set *, int, int)
 *
 * Purpose: Minimize RMS error of simulated and experimental spectrum
 *          (lw, intensity and peak position) using method
 *          specified with method.
 *
 * spexp  = experimental spectrum [input; epr_type *]
 * spsim  = simulated spectrum [input/output; epr_type *]
 *          (must be allocated by the caller)
 * sim    = simulation parameter array [input/output; epr_sim_set *]
 * n      = # of elements in sim array [input; int]
 * method = Fitting method:            [input; int]
 *          0 = Monte Carlo
 *          1 = Gradient method (gradient by difference)
 *          2 = Conjugate Gradient (gradient by difference)
 *          3 = Simplex
 *          4 = M-L method
 *          5 = Hooke & Jeeves
 * iter   = Maximum number of iterations [input; int]
 *
 * See also the optimization parameters in optim.c.
 *
 * return value: RMS error for fitting.
 *
 */

static epr_sim_set *tmp_set;
static int nbr_tmp_set;
static epr_type *exp_sp, *sim_sp;
static double x[EPR_MAXVARS], mc[EPR_MAXVARS];
static epr_type tmp_sp;
static void *fltmp;

int epr_simulation_model = 0; /* lw */
int epr_simulation_type = 0; /* hamilton type */
int epr_simulation_hamilton = 0; /* hamilton (gen) */
int epr_simulation_transition = 0; /* transition (gen) */
int epr_simulation_imode = 0; /* powder integrator mode */
int epr_simulation_npt = 0;  /* n-points for gauss integrator */

double epr_fit_mltol = 1E-8; /* termination tolerance */
double epr_fit_difl = 1E-8; /* difference step length */

/* Wrapper functions for Marquardt optimization */

static epr_variable *epr_fit_vars;
static int epr_fit_nc;
static double epr_fit_epr_f(int, epr_variable *);

int epr_fit_fcn(int *m, int *n, double *x, double *fvec, int *iflag) {

  int i, j;
  double norm, err, *tmp, val;

  for (i = 0; i < *n; i++) {
    *(epr_fit_vars[i].value) = x[i];
    for(j = 0; j < epr_fit_vars[i].nequiv; j++)
      *(epr_fit_vars[i].equiv[j]) = x[i];
  }

  bzero(sim_sp->y, sizeof(double)*sim_sp->samples);
  epr_misc_create_sp(sim_sp, tmp_set, nbr_tmp_set, epr_simulation_type, 
		    epr_simulation_model, fltmp, epr_simulation_hamilton,
		    epr_simulation_transition, epr_simulation_imode, epr_simulation_npt);
  if(tmp_set[0].auto_amp) {
    norm = epr_misc_normalize(exp_sp,sim_sp);
    for (i = 0; i < nbr_tmp_set; i++)
      tmp_set[i].amp *= fabs(norm);
    for (i = 0; i < sim_sp->samples; i++)
      sim_sp->y[i] *= norm;    
  }
  
  err = 0.0;
  for (i = 0; i < *m; i++) {
    fvec[i] = sim_sp->y[i] - exp_sp->y[i];
    err += fvec[i]*fvec[i];
  }
  /* limits by penalty terms */
  for (i = 0; i < *n; i++) {
    tmp = epr_fit_vars[i].value; 
    if(*tmp > epr_fit_vars[i].high || *tmp < epr_fit_vars[i].low) {
      fprintf(stderr,"libepr: (%s) Limit problem: %le -> %le (%le, %le) - penalty issued.\n",
	     epr_fit_vars[i].name, *(epr_fit_vars[i].value),
	     *tmp, epr_fit_vars[i].high, epr_fit_vars[i].low);
      val = *tmp - (*tmp > epr_fit_vars[i].high)?epr_fit_vars[i].high:
	epr_fit_vars[i].low;
      val *= val;
      for (j = 0; j < *m; j++)
	fvec[j] += val;
    }
  }

  fprintf(stderr,"libepr: %d. RMS = %lf\n", ++epr_fit_nc, err / (double) sim_sp->samples);
  return 0;
}

/* from MINPACK - see minpack subdirectory */
double epr_fit_marq(int nvars, epr_variable *vars) {

  int i, j, m, n, info, nprint = 0;
  int *iwa, lwa, maxfev, mode = 1, nfev;
  double fvec[EPR_MAX_SAMPLES], err = 0.0, xtol, ftol, *wa, factor = 1.0;
  double *x, gtol, epsfcn;

  m = sim_sp->samples;
  n = nvars;
  epr_fit_vars = vars;
  xtol = epr_fit_mltol;
  ftol = epr_fit_mltol;
  epr_fit_nc = 0;
  gtol = 0.0;
  maxfev = 200 * (n + 1);
  epsfcn = epr_fit_difl;
  
  if(!(iwa = (int *) malloc(sizeof(int) * n))) {
    printf("Out of memory in M-L procedure (iwa).\n");
    return -1.0;
  }
  bzero(iwa, sizeof(int) * n);
  if(!(wa = (double *) malloc(sizeof(double) * m*n+5*m*n))) {
    printf("Out of memory in M-L procedure (wa).\n");
    free(iwa);
    return -1.0;
  }
  bzero(wa, sizeof(double) * n);
  if(!(x = (double *) malloc(sizeof(double) * n))) {
    printf("Out of memory in M-L procedure (x).\n");
    free(iwa);
    free(wa);
    return -1.0;
  }
  
  for (i = 0; i < n; i++)
    x[i] = *(vars[i].value);

  lmdif_(epr_fit_fcn, &m, &n, x, fvec, &xtol, &ftol, &gtol, &maxfev, &epsfcn, wa,
	 &mode, &factor, &nprint, &info, &nfev, wa + m + 5*n, &m, iwa,
	 wa + n, wa + 2*n, wa + 3*n, wa + 4*n, wa + 5*n);
  /*  lmdif1_(epr_fit_fcn, &m, &n, x, fvec, &tol, &info, iwa, wa, &lwa); */

  switch(info) {
  case 0: 
    fprintf(stderr,"libepr: M-L - improper input parameters.\n");
    return -1.0;
  case 1:
    fprintf(stderr,"libepr: M-L - relative error in the sum of squares at most %le.\n", epr_fit_mltol);
    break;
  case 2:
    fprintf(stderr,"libepr: M-L - relative error between x and solution at most %le.\n", epr_fit_mltol);
    break;
  case 3:
    fprintf(stderr,"libepr: M-L - relative error between x and solution, and sum of squares at most %le\n", epr_fit_mltol);
    break;
  case 4:
    fprintf(stderr,"libepr: M-L - fvec is orthogonal to the columns of the Jacobian to mach. prec.\n");
    break;
  case 5:
    fprintf(stderr,"libepr: M-L - number of calls to object function exceeded %d.\n", 200*(n+1));
    break;
  case 6:
  case 7:
    fprintf(stderr,"libepr: M-L - tolerance %le is too small. No further improvment possible.\n", epr_fit_mltol);
    break;
  case 8:
    fprintf(stderr,"libepr: M-L - gtol is too small.\n");
    break;
  default:
    fprintf(stderr,"libepr: M-L - unknown error.\n");
    return -1.0;
  }

  for (i = 0; i < n; i++) {
    *(vars[i].value) = x[i];
    for(j = 0; j < epr_fit_vars[i].nequiv; j++)
      *(epr_fit_vars[i].equiv[j]) = x[i];    
  }  

  err = epr_fit_epr_f(n, vars); /* to get the spectrum with final params */
  
  free(iwa);
  free(wa);
  free(x);
  return err;
}

/* Wrapper function for optimization routines */

static double epr_fit_epr_f(int n, epr_variable *x) {

  int i, j;
  double norm;

  bzero(sim_sp->y, sizeof(double)*sim_sp->samples);
  epr_misc_create_sp(sim_sp, tmp_set, nbr_tmp_set, epr_simulation_type, 
		    epr_simulation_model, fltmp, epr_simulation_hamilton,
		    epr_simulation_transition, epr_simulation_imode, epr_simulation_npt);
  if(tmp_set[0].auto_amp) {
    norm = epr_misc_normalize(exp_sp,sim_sp);
    for (i = 0; i < nbr_tmp_set; i++)
      tmp_set[i].amp *= fabs(norm);
    for (j = 0; j < sim_sp->samples; j++)
      sim_sp->y[j] *= norm;
  }
  
  return epr_misc_drms(sim_sp, exp_sp);
}

/*
 * Function: double epr_fit_sp(epr_type *, epr_type *, epr_sim_set *, int, epr_variables *,
 *                            int, int, double *)
 *
 * Purpose: Fit experimental and simulated spectra given suitable initial guess
 *          in epr_sim_set.
 *
 * exsp      = Pointer to experimental spectrum [input; epr_type *]
 * sisp      = Pointer to simulated spectrum [output; epr_type *]
 * set       = Array of simulation sets (dim n) [input/output; epr_sim_set *]
 * n         = Number of spectra in simulation [input; int]
 * vars      = Array of variables in optimization [input/output; epr_variable *]
 * nvars     = Number of variables in optimization [input; int]
 * methods   = Optimization method [input; int]
 * extdata   = External data which depends on method [input/output; double *]
 *
 * NOTE: remember to set epr_hamilton_model, epr_simulation_model, and possibly
 *       e, ne, t, nt.
 *
 * Returns: RMS error of after the optimization.
 *
 */

double epr_fit_sp(epr_type *exsp, epr_type *sisp, epr_sim_set *set, int n, epr_variable *vars, int nvars, int method, void *extdata) {

  epr_fft_fill(exsp, 0.0); /* If FFT lineshape generation is used then the simulated gets filled too ! */
  exp_sp = exsp;
  sim_sp = sisp;
  tmp_set = set;
  nbr_tmp_set = n;
  tmp_sp.samples = sim_sp->samples = exp_sp->samples;
  tmp_sp.frequency = sim_sp->frequency = exp_sp->frequency;
  tmp_sp.xbeg = sim_sp->xbeg = exp_sp->xbeg;
  tmp_sp.xend = sim_sp->xend = exp_sp->xend;
  tmp_sp.xunits = sim_sp->xunits = exp_sp->xunits;
  tmp_sp.endor_b = sim_sp->endor_b = exp_sp->endor_b;
  fltmp = extdata;

  switch(method) {
  case 0:
    (void) epr_optim_monte_carlo(epr_fit_epr_f, nvars, vars);
    break;
  case 1:
    (void) epr_optim_grad(epr_fit_epr_f, NULL, nvars, vars);
    break;
  case 2:
    (void) epr_optim_cgrad(epr_fit_epr_f, NULL, nvars, vars);
    break;
  case 3:
    (void) epr_optim_simplex(epr_fit_epr_f, nvars, vars);
    break;
  case 4:
    return epr_fit_marq(nvars, vars); /* MINPACK routine */
    break;
  case 5:
    return epr_optim_hj(epr_fit_epr_f, epr_fit_difl, nvars, vars);
    break;
  }

  return epr_fit_epr_f(nvars, vars);
}

/*
 * Function: double epr_fit_line(epr_type *, double *, double *, double *, double *, double *, int, int)
 *
 * Purpose: Calculate Lorentzian and Gaussian factors + amplitude of a peak
 * centered at cent.
 *
 * sp   = pointer to the spectrum containing the line [input; epr_type *]
 * cent = position of the center of the peak [input/output; double *]
 * lf   = Lorentz factor [input/output; double *]
 * gf   = Gaussian factor [input/output; double *]
 * lw   = Linewdith [input/output; double *]
 * amp  = Amplitude [input/output; double *]
 * dl   = Derivative level [input; int]
 * met  = Optimization method [input; int] (0 = MC, 1 = Grad, 2 = CG).
 *
 * Return value: ptr to sp (epr_type *) containing the peak.
 *
 */

/* Wrapper function for optimization routines */

static int epr_fit_dl;

static double epr_fit_line_f(int n, epr_variable *x) {

  int i;
  epr_sim_set tmp;

  bzero(sim_sp->y, sizeof(double)*sim_sp->samples);
  i = EPR_XTOI(sim_sp, *(x[0].value), EPR_XSTEP(sim_sp));
  if(i >= 0 && i < sim_sp->samples)
    sim_sp->y[i] = 1.0;

  bzero(tmp.posn, sizeof(double) * 9);
  tmp.posn[0][EPR_INDEX(3,0,0)] = tmp.posn[0][EPR_INDEX(3,1,1)] = tmp.posn[0][EPR_INDEX(3,2,2)] =
    *(x[0].value);
  tmp.lg_fact = *(x[1].value);
  tmp.lw = *(x[2].value);
  tmp.dlevel = epr_fit_dl;

  (void) epr_line_create_fft(sim_sp, &tmp, 0);
  return epr_misc_drms(sim_sp, exp_sp);
}

#define MCLIMIT 0.01

epr_type *epr_fit_line(epr_type *sp, double *cent, double *lf, double *gf, double *amp, double *lw, int dl, int met) {

  epr_variable x[5];

  exp_sp = sp;
  sim_sp = epr_misc_new();
  sim_sp->samples = exp_sp->samples;
  sim_sp->frequency = exp_sp->frequency;
  sim_sp->xbeg = exp_sp->xbeg;
  sim_sp->xend = exp_sp->xend;
  sim_sp->xunits = exp_sp->xunits;
  epr_fit_dl = dl;
  x[0].value = cent;
  x[0].high = sp->xend;
  x[0].low = sp->xbeg;
  x[0].mc_limit = MCLIMIT;
  x[1].value = lf;
  x[1].high = 1.0;
  x[1].low = 0.0;
  x[1].mc_limit = MCLIMIT;
  x[2].value = gf;
  x[2].high = 1.0;
  x[2].low = 0.0;
  x[2].mc_limit = MCLIMIT;
  x[3].value = amp;
  x[3].high = 1E10;
  x[3].low = 0.0;
  x[3].mc_limit = 100.0*MCLIMIT;
  x[4].value = lw;
  x[4].high = 1E3;
  x[4].low = 0.0;
  x[4].mc_limit = MCLIMIT;
  switch(met) {
  case 0:
    (void) epr_optim_monte_carlo(epr_fit_line_f, 5, x);
    break;
  case 1:
    (void) epr_optim_grad(epr_fit_line_f, NULL, 5, x);
    break;
  case 2:
    (void) epr_optim_cgrad(epr_fit_line_f, NULL, 5, x);
    break;
  case 3:
    (void) epr_optim_simplex(epr_fit_line_f, 5, x);
    break;
  default:
    return (epr_type *) NULL;
  }

  (void) epr_fit_line_f(5, x);
  
  return sim_sp;
}
