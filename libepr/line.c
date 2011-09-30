/*
 * Line shape functions.
 * Last modified: Oct-13-2009 jussi.eloranta@csun.edu
 *
 * Written by:
 * Jussi Eloranta (jussi.eloranta@csun.edu)
 *
 * Department of chemistry and biochemistry
 * California State University at Northridge
 *
 */

#include "epr.h"

#define EPS 1.0E-16

/*
 * Function: double epr_line_gauss(double amp, double lw, double bc, double b)
 * Purpose: Evaluate gaussian line at specific point.
 *
 * amp  = signal amplitude [input; double]
 * lw   = linewidth in Gauss [input; double]
 * bc   = Magnetic field at the center [input; double]
 * b    = Magnetic field at the evaluation point [input; double]
 *
 * NOTE: Magnetic field unit is Gauss.
 *
 */

inline double epr_line_gauss(double amp, double lw, double bc, double b) {

  double tmp;

  lw *= 0.58870501; /* peak-to-peak width to half-width at half-height */
  if(lw < EPS) return 0.0; /* to avoid NaN's */
  tmp = (b - bc) / lw;
  return (amp * exp(-M_LN2 * tmp * tmp));
}

/*
 * Function: double epr_line_lorentz(double amp, double lw, double bc, double b)
 * Purpose: Evaluate lorentzian line at specific point.
 *
 * amp  = signal amplitude [input; double]
 * lw   = linewidth in Gauss [input; double]
 * bc   = Magnetic field at the center [input; double]
 * b    = Magnetic field at the evaluation point [input; double]
 *
 * NOTE: Magnetic field unit is Gauss. The caller must perform the
 *       appropriate conversion before calling.
 *
 */

inline double epr_line_lorentz(double amp, double lw, double bc, double b) {

  double tmp1, tmp2;

  lw *= 0.8660254; /* peak-to-peak width to half-width at half-height */
  tmp1 = lw * lw;
  if(tmp1 < EPS) return 0.0; /* to avoid NaN's */
  tmp2 = b - bc;
  tmp2 *= tmp2;
  return (amp * tmp1 / (tmp1 + tmp2));
}

/*
 * Function: double epr_line_dgauss(double amp, double lw, double bc, double b)
 * Purpose: Evaluate 1st derivative of gaussian line at specific point.
 *
 * amp  = signal amplitude [input; double]
 * lw   = linewidth in Gauss [input; double]
 * bc   = Magnetic field at the center [input; double]
 * b    = Magnetic field at the evaluation point [input; double]
 *
 * NOTE: Magnetic field unit is Gauss.
 *
 */

inline double epr_line_dgauss(double amp, double lw, double bc, double b) {

  double tmp1, tmp2, tmp3;

  lw *= 0.58870501; /* peak-to-peak width to half-width at half-height */
  tmp1 = lw * lw;
  if(tmp1 < EPS) return 0.0; /* to avoid NaN's */
  tmp2 = b - bc;
  tmp3 = tmp2*tmp2;

  return (-amp*2.0*M_LN2*tmp2*exp(-M_LN2*tmp3/tmp1)/tmp1);
}

/*
 * Function: double epr_line_dlorentz(double amp, double lw, double bc, double b)
 * Purpose: Evaluate 1st derivative of lorentzian line at specific point.
 *
 * amp  = signal amplitude [input; double]
 * lw   = linewidth in Gauss [input; double]
 * bc   = Magnetic field at the center [input; double]
 * b    = Magnetic field at the evaluation point [input; double]
 *
 * NOTE: Magnetic field unit is Gauss. The caller must perform the
 *       appropriate conversion before calling.
 *
 */

inline double epr_line_dlorentz(double amp, double lw, double bc, double b) {

  double tmp1, tmp2, tmp3;

  lw *= 0.8660254; /* peak-to-peak width to half-width at half-height */
  tmp1 = lw * lw;
  if(tmp1 < EPS) return 0.0; /* to avoid NaN's */
  tmp2 = b - bc;
  tmp3 = tmp1 + tmp2*tmp2;

  return (-amp*2.0*tmp1*tmp2/(tmp3*tmp3));
}

/*
 * Function: epr_type *epr_line_create_add(epr_type *, double *, double *
 *                   double *, double *, int, int)
 * Purpose: create (gaussian/lorentzian) line (absorption) by addition.
 *
 * sp     = destination spectrum [input/output; epr_type *]
 * center = peak centers [input; double *]
 * lw     = line widths [input; double *]
 * lg     = Lorentz/Gauss factor [input; double *]
 * amp    = peak amplitude [input; double *]
 * n      = number of peaks to be generates [input; int]
 * dlevel = derivative level [input; int]
 *
 * If lg factor < 0.0 then a lineshape consisting of convolution of
 * Lorentzian with Gaussian is generated. The Lorentz linewidths are
 * given by fabs(lg) and the Gaussian line widths by lw. FFT method is
 * applied in this process. 
 * 
 * return value: ptr to epr_type
 *               NULL = error
 *
 */

double epr_line_cutoff = 12.0;

epr_type *epr_line_create_add(epr_type *sp, double *center, double *lw, double *lg, double *amp, int n, int dlevel) {

  int i, j;
  double step, x, u, tmp;
  double (*gls)(double, double, double, double);
  double (*lls)(double, double, double, double);

  switch(dlevel) {
  case EPR_ABSORPTION:
    gls = epr_line_gauss;
    lls = epr_line_lorentz;
    break;
  case EPR_FIRSTDERIV:
    gls = epr_line_dgauss;
    lls = epr_line_dlorentz;
    break;
  case EPR_SECONDERIV:
    /* FIXME: not yet written */
    break;
  default:
    return NULL;
  }
  step = EPR_XSTEP(sp);
  for (i = 0; i < n; i++) {
    tmp = epr_line_cutoff*lw[i];
    u = center[i] + tmp;
    j = EPR_XTOI(sp, center[i] - tmp, step);
    for (x = center[i] - tmp; x <= u; x += step, j++)
      if(j >= 0 && j < sp->samples)
	sp->y[j] += amp[i] * ((*lls)(lg[i], lw[i], center[i], x) +
			      (*gls)(1.0-lg[i], lw[i], center[i], x));
  }
  return sp;
}

/*
 * Function: epr_type *epr_line_create_heinzer_intra(epr_type *, epr_sim_set *,
 *                   double *, double, int, doublecomplex *)
 * Purpose: create n-jump linewidth by the Heinzer method. (intra)
 *
 * sp        = destination spectrum [input/ouput; epr_type *]
 * ss        = simlation parameters [input; epr_sim_set *]
 * centers   = centers for peaks [input; double *]
 * intensity = line intensity [input; double]
 * n         = number of structures [input; int]
 * k         = rate constants [input; doublecomplex *]
 *             (Note intexing!)
 *
 * return value: ptr to epr_type or NULL on error.
 * 
 * See notes below the Norris routine. Note that the relative populations
 * are calculated from the amplitudes of each site. The eqn. references
 * are to Heinzer's paper in Mol. Phys., 167-177, 22, 1971.
 * 
 * Originally this route was based on numerical diagonalization of the
 * "exchange" matrix but the LAPACK eigen value solver was not numerically
 * stable. Therefore I switched to the analytical solutions given by
 * Heinzer. Only two, three, and four sites are supported.
 *
 */

#define alpha_r(mu,nu) (x[mu].r*x[nu].r - x[mu].i*x[nu].i - k[EPR_INDEX(n,mu,nu)]*k[EPR_INDEX(n,nu,mu)])
#define alpha_i(mu,nu) (x[mu].r*x[nu].i + x[mu].i*x[nu].r)
#define beta_r(mu,ka,la) (x[mu].r*k[EPR_INDEX(n,ka,la)] - k[EPR_INDEX(n,mu,la)] * k[EPR_INDEX(n,ka,mu)])
#define beta_i(mu,ka,la) (x[mu].i*k[EPR_INDEX(n,ka,la)])

/* FIXME: optimize the code */

epr_type *epr_line_create_heinzer_intra(epr_type *sp, epr_sim_set *ss, double *centers, double intensity, int n, double *k) {

  double p[4], Q, mean_amp, step, beg, end, ul, cf, cfreq;
  doublecomplex x[4], tmp1, tmp2, tmp3;
  doublecomplex A, B, C, D, E, F, G, H, K, L;
  doublecomplex EP, FP, GP, HP, KP, LP;
  int i, j, l, m;

  if(n > 4 || n < 2) return (epr_type *) NULL;

  /* relative populations */
  for (i = 0, step = 0.0; i < n; i++) step += ss[i].amp;
  beg = 1E99; end = 0.0;
  for (i = 0; i < n; i++) {
    p[i] = ss[i].amp / step;
    if(centers[i] < beg) beg = centers[i];
    if(centers[i] > end) end = centers[i];
  }
  mean_amp = step / (double) n;

  /* generate spectrum (eqn. 12) - later: do first deriv directly */
  ul = EPR_XSTEP(sp);
  beg -= ss[0].lw*epr_line_cutoff*50.0;
  end += ss[0].lw*epr_line_cutoff*50.0;
  if(beg < sp->xbeg) beg = sp->xbeg;
  if(end > sp->xend) end = sp->xend;
  for (i = 0; i < n; i++) {
    x[i].r = -EPR_LWTOIT2(ss[i].lw);
    for (j = 0; j < n; j++)
      if(i != j) x[i].r -= k[EPR_INDEX(n,i,j)];
  }
  i = EPR_XTOI(sp, beg, ul);
  j = EPR_XTOI(sp, end, ul);
  for (cf = beg; i < j; i++, cf += ul) {
    cfreq = EPR_GTOHZ(cf);
    for (l = 0; l < n; l++)
      x[l].i = cfreq - EPR_GTOHZ(centers[l]);
    switch(n) {
    case 2: /* two sites */
      tmp1.r = p[0]*(x[1].r - k[EPR_INDEX(n,1,0)]) +
	p[1]*(x[0].r -k[EPR_INDEX(n,0,1)]);
      tmp1.i = p[0]*x[1].i + p[1]*x[0].i;
      tmp2.r = x[0].r;
      tmp2.i = x[0].i;
      epr_imag_mult(&tmp2, &x[1]);
      tmp2.r -= k[EPR_INDEX(n,0,1)] * k[EPR_INDEX(n,1,0)];
      break;
    case 3: /* three sites */
      tmp1.r = p[0]*(alpha_r(1,2) - beta_r(1,2,0) - beta_r(2,1,0));
      tmp1.i = p[0]*(alpha_i(1,2) - beta_i(1,2,0) - beta_i(2,1,0));
      tmp1.r += p[1]*(alpha_r(0,2) - beta_r(0,2,1) - beta_r(2,0,1));
      tmp1.i += p[1]*(alpha_i(0,2) - beta_i(0,2,1) - beta_i(2,0,1));
      tmp1.r += p[2]*(alpha_r(0,1) - beta_r(0,1,2) - beta_r(1,0,2));
      tmp1.i += p[2]*(alpha_i(0,1) - beta_i(0,1,2) - beta_i(1,0,2));
      tmp2.r = alpha_r(0,1);
      tmp2.i = alpha_i(0,1);
      epr_imag_mult(&tmp2, &x[2]);
      tmp2.r -= k[EPR_INDEX(n,2,1)]*beta_r(0,1,2) + k[EPR_INDEX(n,2,0)]*beta_r(1,0,2);
      tmp2.i -= k[EPR_INDEX(n,2,1)]*beta_i(0,1,2) + k[EPR_INDEX(n,2,0)]*beta_i(1,0,2);
      break;
    case 4: /* four sites */
      A.r = alpha_r(2,3);
      A.i = alpha_i(2,3);
      epr_imag_mult(&A, &x[1]);
      A.r -= k[EPR_INDEX(n,3,1)]*beta_r(2,1,3) + k[EPR_INDEX(n,2,1)]*beta_r(3,1,2);
      A.i -= k[EPR_INDEX(n,3,1)]*beta_i(2,1,3) + k[EPR_INDEX(n,2,1)]*beta_i(3,1,2);

      B.r = alpha_r(2,3);
      B.i = alpha_i(2,3);
      epr_imag_mult(&B, &x[0]);
      B.r -= k[EPR_INDEX(n,0,3)]*beta_r(2,3,0) + k[EPR_INDEX(n,0,2)]*beta_r(3,2,0);
      B.i -= k[EPR_INDEX(n,0,3)]*beta_i(2,3,0) + k[EPR_INDEX(n,0,2)]*beta_i(3,2,0);

      C.r = alpha_r(0,1);
      C.i = alpha_i(0,1);
      epr_imag_mult(&C, &x[3]);
      C.r -= k[EPR_INDEX(n,1,3)]*beta_r(0,3,1) + k[EPR_INDEX(n,0,3)]*beta_r(1,3,0);
      C.i -= k[EPR_INDEX(n,1,3)]*beta_i(0,3,1) + k[EPR_INDEX(n,0,3)]*beta_i(1,3,0);

      D.r = alpha_r(0,1);
      D.i = alpha_i(0,1);
      epr_imag_mult(&D, &x[2]);
      D.r -= k[EPR_INDEX(n,2,1)]*beta_r(0,1,2) + k[EPR_INDEX(n,2,0)]*beta_r(1,0,2);
      D.i -= k[EPR_INDEX(n,2,1)]*beta_i(0,1,2) + k[EPR_INDEX(n,2,0)]*beta_i(1,0,2);

      E.r = -k[EPR_INDEX(n,1,0)]*alpha_r(2,3) + k[EPR_INDEX(n,1,2)]*beta_r(3,2,0)
	+ k[EPR_INDEX(n,1,3)]*beta_r(2,3,0);
      E.i = -k[EPR_INDEX(n,1,0)]*alpha_i(2,3) + k[EPR_INDEX(n,1,2)]*beta_i(3,2,0)
	+ k[EPR_INDEX(n,1,3)]*beta_i(2,3,0);

      F.r = -k[EPR_INDEX(n,2,0)]*alpha_r(1,3) + k[EPR_INDEX(n,2,1)]*beta_r(3,1,0)
	+ k[EPR_INDEX(n,2,3)]*beta_r(1,3,0);
      F.i = -k[EPR_INDEX(n,2,0)]*alpha_i(1,3) + k[EPR_INDEX(n,2,1)]*beta_i(3,1,0)
	+ k[EPR_INDEX(n,2,3)]*beta_i(1,3,0);

      G.r = -k[EPR_INDEX(n,3,0)]*alpha_r(1,2) + k[EPR_INDEX(n,3,1)]*beta_r(2,1,0)
	+ k[EPR_INDEX(n,3,2)]*beta_r(1,2,0);
      G.i = -k[EPR_INDEX(n,3,0)]*alpha_i(1,2) + k[EPR_INDEX(n,3,1)]*beta_i(2,1,0)
	+ k[EPR_INDEX(n,3,2)]*beta_i(1,2,0);

      H.r = -k[EPR_INDEX(n,2,1)]*alpha_r(0,3) + k[EPR_INDEX(n,0,1)]*beta_r(3,2,0)
	+ k[EPR_INDEX(n,3,1)]*beta_r(0,2,3);
      H.i = -k[EPR_INDEX(n,2,1)]*alpha_i(0,3) + k[EPR_INDEX(n,0,1)]*beta_i(3,2,0)
	+ k[EPR_INDEX(n,3,1)]*beta_i(0,2,3);

      K.r = -k[EPR_INDEX(n,3,1)]*alpha_r(0,2) + k[EPR_INDEX(n,3,0)]*beta_r(2,0,1)
	+ k[EPR_INDEX(n,3,2)]*beta_r(0,2,1);
      K.i = -k[EPR_INDEX(n,3,1)]*alpha_i(0,2) + k[EPR_INDEX(n,3,0)]*beta_i(2,0,1)
	+ k[EPR_INDEX(n,3,2)]*beta_i(0,2,1);

      L.r = -k[EPR_INDEX(n,3,2)]*alpha_r(0,1) + k[EPR_INDEX(n,3,1)]*beta_r(0,1,2)
	+ k[EPR_INDEX(n,3,0)]*beta_r(1,0,2);
      L.i = -k[EPR_INDEX(n,3,2)]*alpha_i(0,1) + k[EPR_INDEX(n,3,1)]*beta_i(0,1,2)
	+ k[EPR_INDEX(n,3,0)]*beta_i(1,0,2);

      EP.r = -k[EPR_INDEX(n,0,1)]*alpha_r(2,3) + k[EPR_INDEX(n,2,1)]*beta_r(3,0,2)
	+ k[EPR_INDEX(n,3,1)]*beta_r(2,0,3);
      EP.i = -k[EPR_INDEX(n,0,1)]*alpha_i(2,3) + k[EPR_INDEX(n,2,1)]*beta_i(3,0,2)
	+ k[EPR_INDEX(n,3,1)]*beta_i(2,0,3);

      FP.r = -k[EPR_INDEX(n,0,2)]*alpha_r(1,3) + k[EPR_INDEX(n,1,2)]*beta_r(3,0,1)
	+ k[EPR_INDEX(n,3,2)]*beta_r(1,0,3);
      FP.i = -k[EPR_INDEX(n,0,2)]*alpha_i(1,3) + k[EPR_INDEX(n,1,2)]*beta_i(3,0,1)
	+ k[EPR_INDEX(n,3,2)]*beta_i(1,0,3);

      GP.r = -k[EPR_INDEX(n,0,3)]*alpha_r(1,2) + k[EPR_INDEX(n,1,3)]*beta_r(2,0,1)
	+ k[EPR_INDEX(n,2,3)]*beta_r(1,0,2);
      GP.i = -k[EPR_INDEX(n,0,3)]*alpha_i(1,2) + k[EPR_INDEX(n,1,3)]*beta_i(2,0,1)
	+ k[EPR_INDEX(n,2,3)]*beta_i(1,0,2);

      HP.r = -k[EPR_INDEX(n,1,2)]*alpha_r(0,3) + k[EPR_INDEX(n,1,0)]*beta_r(3,0,2)
	+ k[EPR_INDEX(n,1,3)]*beta_r(0,3,2);
      HP.i = -k[EPR_INDEX(n,1,2)]*alpha_i(0,3) + k[EPR_INDEX(n,1,0)]*beta_i(3,0,2)
	+ k[EPR_INDEX(n,1,3)]*beta_i(0,3,2);

      KP.r = -k[EPR_INDEX(n,1,3)]*alpha_r(0,2) + k[EPR_INDEX(n,0,3)]*beta_r(2,1,0)
	+ k[EPR_INDEX(n,2,3)]*beta_r(0,1,2);
      KP.i = -k[EPR_INDEX(n,1,3)]*alpha_i(0,2) + k[EPR_INDEX(n,0,3)]*beta_i(2,1,0)
	+ k[EPR_INDEX(n,2,3)]*beta_i(0,1,2);

      LP.r = -k[EPR_INDEX(n,2,3)]*alpha_r(0,1) + k[EPR_INDEX(n,1,3)]*beta_r(0,2,1)
	+ k[EPR_INDEX(n,0,3)]*beta_r(1,2,0);
      LP.i = -k[EPR_INDEX(n,2,3)]*alpha_i(0,1) + k[EPR_INDEX(n,1,3)]*beta_i(0,2,1)
	+ k[EPR_INDEX(n,0,3)]*beta_i(1,2,0);

      Q = (k[EPR_INDEX(n,2,0)]*k[EPR_INDEX(n,3,1)] - k[EPR_INDEX(n,2,1)]*k[EPR_INDEX(n,3,0)])
	* (k[EPR_INDEX(n,0,3)]*k[EPR_INDEX(n,1,2)] - k[EPR_INDEX(n,0,2)]*k[EPR_INDEX(n,1,3)]);

      tmp1.r = p[0]*(A.r + E.r + F.r + G.r) + p[1]*(B.r + EP.r + H.r + K.r)
	+ p[2]*(C.r + FP.r + HP.r + L.r) + p[3]*(D.r + GP.r + KP.r + LP.r);
      tmp1.i = p[0]*(A.i + E.i + F.i + G.i) + p[1]*(B.i + EP.i + H.i + K.i)
	+ p[2]*(C.i + FP.i + HP.i + L.i) + p[3]*(D.i + GP.i + KP.i + LP.i);
      tmp2.r = alpha_r(0,1);
      tmp2.i = alpha_i(0,1);
      tmp3.r = alpha_r(2,3);
      tmp3.i = alpha_i(2,3);
      epr_imag_mult(&tmp2, &tmp3);
      tmp2.r -= k[EPR_INDEX(n,3,1)]*KP.r + k[EPR_INDEX(n,2,1)]*HP.r + 
	k[EPR_INDEX(n,3,0)]*GP.r + k[EPR_INDEX(n,2,0)]*FP.r;
      tmp2.i -= k[EPR_INDEX(n,3,1)]*KP.i + k[EPR_INDEX(n,2,1)]*HP.i + 
	k[EPR_INDEX(n,3,0)]*GP.i + k[EPR_INDEX(n,2,0)]*FP.i;
      tmp2.r += Q;      
      break;
    }
    epr_imag_div(&tmp1, &tmp2);
    sp->y[i] -= intensity * tmp1.r * mean_amp * 1.0E3;
  }
  return sp;
}

/*
 * Function: epr_type *epr_line_create_norris(epr_type *, epr_sim_set *, double *, 
 *                    double *, double, int)
 * Purpose: create n-jump linewidth by the Norris equation.
 *
 * sp        = destination spectrum [input/output; epr_type *]
 * ss        = simulation parameters [input; epr_sim_set *]
 * centers   = centers for peaks [input; double *]
 * intensity = line intensity [input; double]
 * rate      = averaged rate constant [input; double]
 * n         = number jump structures [input; int]
 *
 * return value: ptr to epr_type
 *               NULL = error
 *
 * see simul.c for usage.
 * NOTE 1: Unit G is hidden all over the constants!
 */
/* TODO: write all the peak generation stuff so that the output is
 * normalized! */

/* FIXME: optimize the code */

epr_type *epr_line_create_norris(epr_type *sp, epr_sim_set *ss, double *centers, double intensity, double rate, int n) {

  int i, idx;
  double norm2, p[EPR_MAX_JUMPS], t, cpos, diff, step, beg, end, lw, mean_amp,
    invlt;
  doublecomplex f[EPR_MAX_JUMPS], ctmp, ctmp2, norm;
  double mlw = 0.0;

  if(n >= EPR_MAX_JUMPS) return NULL;
  invlt = ((double) n) * rate;
  step = EPR_XSTEP(sp);
  beg = sp->xend;
  end = sp->xbeg;
  for (norm2 = 0.0, i = 0; i < n; i++) {
    norm2 += ss[i].amp;
    if(centers[i] < beg) beg = centers[i];
    if(centers[i] > end) end = centers[i];
  }

  mean_amp = 0.0;
  for (i = 0; i < n; i++) {
    t = ss[i].lw;
    if(t > mlw) mlw = t;
    t = EPR_LWTOIT2(t) + invlt;
    f[i].r = -t;
    f[i].i = -EPR_GTOHZ(centers[i]);
    p[i] = ss[i].amp / norm2;
    mean_amp += ss[i].amp;
  }
  mean_amp /= (double) n;

  /* FIXME: this depends on the exchange rate as well ! */
  mean_amp = epr_line_cutoff*mlw*2.0;
  beg -= mean_amp;
  end += mean_amp;
  if(beg < sp->xbeg) beg = sp->xbeg;
  if(end > sp->xend) end = sp->xend;
    
  idx = EPR_XTOI(sp, beg, step);
  for (cpos = beg; cpos <= end; cpos += step, idx++) {
    norm.r = 0.0;
    norm.i = 0.0;
    for (i = 0; i < n; i++) {
      diff = EPR_GTOHZ(cpos);
      ctmp2.r = p[i];
      ctmp2.i = 0.0;
      ctmp.r = f[i].r;
      ctmp.i = f[i].i + diff;
      epr_imag_div(&ctmp2, &ctmp);
      epr_imag_addto(&norm, &ctmp2);
    }
    ctmp.r = intensity * norm.r;
    ctmp.i = intensity * norm.i;
    ctmp2.r = 1.0 + invlt * norm.r;
    ctmp2.i = invlt * norm.i;
    epr_imag_div(&ctmp, &ctmp2);
    sp->y[idx] += -1.0E6*mean_amp*ctmp.r;
  }
  return sp;
}

/*
 * Function: epr_type *epr_line_create_fft(epr_type *sp, epr_sim_set *ss)
 * Purpose: Convolution of lineshape using the FFT method (consequence
 * of the Plancherel's theorem).
 *
 * sp    = source/destination spectrum [input/output; epr_type *]
 * ss    = simset containing params [input; epr_sim_set *]
 * flag  = 0 = perform inverse fft at end, 1 = don't do it
 *
 * return value: ptr to epr_type (NULL on error)
 *
 * NOTE: If lg_fact < 0.0 then a lineshape resulting from
 * gaussian distribution of lorentzian lines is generated.
 * (ss->lw gives the Lorentz linewidth and |ss->lg_fact| the
 *  gaussian linewidth)
 * 
 * For further explanation of this routine see:
 * B. Kirste, Anal. Chem. Acta 265 (1992) 191-200.
 *
 */

epr_type *epr_line_create_fft(epr_type *sp, epr_sim_set *ss, int flag) {

  int i, j, k, len;
  double c1, c2, c3, lf, gf, width, elw;

  elw = ss->lw;
  lf = ss->lg_fact;
  gf = 1.0 - ss->lg_fact;
  width = fabs(sp->xbeg - sp->xend);
  /* Lorentz */
  c1 = -M_PI * EPR_SQRT3 * elw / width;
  /* Gauss */
  if(ss->lg_fact < 0.0) elw = fabs(ss->lg_fact);
  c2 = -M_PI*M_PI*elw*elw / (2.0 * width * width);
  epr_fft(sp);
  /* i = 0: the real part is non-zero and imaginary is zero */
  /* no modification of this element is needed since lf + gf = 1 */
  len = (sp->samples + 1) / 2;
  if(ss->lg_fact >= 0.0) {
    for (i = 1, j = sp->samples-1; i <= len; i++, j--) {
      double tmp;
      c3 = 0.0;
      tmp = c1 * (double) i;
      if(tmp >= EPR_EXP_EPS)
	c3 += lf * exp(tmp);
      tmp = c2 * ((double) i*i);
      if(tmp >= EPR_EXP_EPS)
	c3 += gf * exp(tmp);
      sp->y[i] *= c3;
      if(j > len) sp->y[j] *= c3;
    }
  } else {
    for (i = 1, j = sp->samples-1; i <= len; i++, j--) {
      double tmp;
      c3 = 1.0;
      tmp = c1 * (double) i;
      if(tmp >= EPR_EXP_EPS)
	c3 *= exp(tmp);
      else c3 = 0.0;
      tmp = c2 * ((double) i*i);
      if(tmp >= EPR_EXP_EPS)
	c3 *= exp(tmp);
      else c3 = 0.0;
      sp->y[i] *= c3;
      if(j > len) sp->y[j] *= c3;
    }
  }

  /* derivative */
  c3 = EPR_PI2 / (double) sp->samples;
  for (k = 0; k < ss->dlevel-1; k++) {
    sp->y[0] = 0.0;
    for (i = 1, j = sp->samples-1; i < len; i++, j--) {
      double t;
      t = sp->y[i];
      /* re = -2Pi*x*im */
      sp->y[i] = -c3 * sp->y[j] * (double) i;
      /* im = 2Pi*x*re */
      sp->y[j] = c3 * t * (double) i;
    }
    if(len % 2 == 0) sp->y[sp->samples/2] = 0.0;
  }
  if(!flag) epr_fft_inv(sp);

  return sp;
}

/*
 * Function: epr_type *epr_line_create_conv(epr_type *sp, epr_sim_set *ss)
 * Purpose: Convolution of sp with given line shape.
 *
 * sp     = source/destination spectrum [input/output; epr_type *]
 * ss     = simset containing params [input; epr_sim_set *]
 *
 * return value: ptr to epr_type (NULL on error)
 *
 */

double epr_line_tmp[EPR_MAX_SAMPLES];

epr_type *epr_line_create_conv(epr_type *sp, epr_sim_set *ss) {

  int i, j;
  double s, f, x, xi, xj, gf, lf, elw;
  double (*gls)(double, double, double, double);
  double (*lls)(double, double, double, double);

  elw = ss->lw;
  bzero(epr_line_tmp, sizeof(double)*sp->samples);
  s = EPR_XSTEP(sp);
  switch(ss->dlevel) {
  case EPR_ABSORPTION:
    gls = epr_line_gauss;
    lls = epr_line_lorentz;
    break;
  case EPR_FIRSTDERIV:
    gls = epr_line_dgauss;
    lls = epr_line_dlorentz;
    break;
  case EPR_SECONDERIV:
    /* FIXME: not yet written */
    break;
  default:
    return NULL;
  }
  xi = sp->xbeg;
  lf = ss->amp * ss->lg_fact;
  gf = ss->amp * (1.0 - ss->lg_fact);
  for(i = 0; i < sp->samples; i++, xi += s) {
    xj = sp->xbeg;
    for(j = 0; j < sp->samples; j++, xj += s) {
      if(gf > 0.0) 
	f = (*gls)(gf, elw, xj, xi);
      else
	f = 0.0;
      if(lf > 0.0)
	f += (*lls)(lf, elw, xj, xi);
      epr_line_tmp[i] += s * f * sp->y[j];
    }
  }
  bcopy(epr_line_tmp, sp->y, sp->samples * sizeof(double));

  return sp;
}

/*
 * Function: epr_type *epr_line_asymmlw(epr_type *, epr_sim_set *, epr_peak *, int)
 *
 * Purpose: Generate peaks with asymmetric lw (nuclear spin dependent).
 *
 * sp = Target spectrum [input/output; epr_type *]
 * ss = Simulation set (for lw, lg_fact, etc.) [input; epr_sim_set *]
 * pe = Peak array [input; epr_peak *]
 * np = Number of peaks [input; int]
 *
 * Returns pointer to the target spectrum.
 *
 */

epr_type *epr_line_asymmlw(epr_type *sp, epr_sim_set *ss, epr_peak *pe, int np) {

  int i, j, k, l;
  double nlw, elw, l_dir[3], tmp;

  elw = ss->lw;
  for (i = 0; i < np; i++) {
    nlw = elw;
    for (k = 0; k < ss->ngr; k++) {
      nlw += ss->b[k] * pe[i].mi[k];
      for (l = k; l < ss->ngr; l++)
	nlw += ss->c[k][l] * pe[i].mi[k] * pe[i].mi[l];
    }
    if(nlw < 0.0) nlw = 0.0;
    tmp = pe[i].lw;
    pe[i].lw = nlw;
    /* FIXME: this routimne should take peaks as args ! */
    epr_line_create_add(sp, &(pe[i].pos), &nlw, &(ss->lg_fact),
		       &(pe[i].intensity), 1, ss->dlevel);
    pe[i].lw = tmp;
  }
  return sp;
}

/*
 * Function: epr_type *epr_line_norris(epr_type *, epr_sim_set *, epr_peak [][], int, int,
 *                                   double)
 *
 * Purpose: Generate (intra) molecular exchange using the Norris eqn.
 *
 * sp    = Target spectrum [input/output; epr_type *]
 * ss    = Simulation set [input; epr_sim_set *]
 * pe    = Peak array [input; epr_peak [][]]
 * ns    = Number of species in exchange [input; int]
 * np    = Number of peaks [input; int]
 * rate  = Average rate constant [input; double]
 *
 * Returns pointer to the spectrum. NOTE: The peaks are assumed to be
 * sorted such that there is a direct mapping among peak sets with respect
 * to the nuclear spin configurations.
 *
 */

epr_type *epr_line_norris(epr_type *sp, epr_sim_set *ss, epr_peak *pe[], int ns, int np, double rate) {

  int i, j;
  double centers[EPR_MAX_JUMPS], intensity;
  epr_peak *tmp;
  
  tmp = pe[0];
  for (i = 0; i < np; i++) {
    intensity = tmp[i].intensity;
    for (j = 0; j < ns; j++) {
      tmp = pe[j];
      centers[j] = tmp[i].pos;
    }
    (void) epr_line_create_norris(sp, ss, centers, intensity, rate, ns);
  }
  if(ss[0].dlevel == EPR_FIRSTDERIV || ss[0].dlevel == EPR_SECONDERIV)
    epr_calc_diff(sp);
  if(ss[0].dlevel == EPR_SECONDERIV) epr_calc_diff(sp);
  return sp;
}

/*
 * Function: epr_type *epr_line_heinzer_intra(epr_type *, epr_sim_set *, epr_peak *, int,
 *                                          int, double *)
 *
 * Purpose: Generate (intra) molecular exchange using the Heinzer method.
 *
 * sp    = Target spectrum [input/output; epr_type *]
 * ss    = Simulation set [input; epr_sim_set *]
 * pe    = Peak array [input; epr_peak *]
 * ns    = Number of species in exchange [input; int]
 * np    = Number of peaks [input; int]
 * rate  = Rate constant array (2 dim) [input; double *]
 *
 * Returns pointer to the spectrum.
 *
 */

epr_type *epr_line_heinzer_intra(epr_type *sp, epr_sim_set *ss, epr_peak *pe[], int ns, int np, double *rate) {
  
  int i, j, k;
  double centers[EPR_MAX_JUMPS], intensity;
  epr_peak *tmp;
  
  tmp = pe[0];
  for (i = 0; i < np; i++) {
    intensity = tmp[i].intensity;
    for (j = 0; j < ns; j++) {
      tmp = pe[j];
      centers[j] = tmp[i].pos;
    }
    (void) epr_line_create_heinzer_intra(sp, ss, centers, intensity, ns, rate);
  }
  if(ss[0].dlevel == EPR_FIRSTDERIV || ss[0].dlevel == EPR_SECONDERIV)
    epr_calc_diff(sp);
  if(ss[0].dlevel == EPR_SECONDERIV) epr_calc_diff(sp);
  return sp;
}
