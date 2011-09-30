/*
 * Misc.functions.
 * Last modified: Oct-13-2009 jussi.eloranta@csun.edu
 *
 * Written by:
 * Jussi Eloranta (jussi.eloranta@csun.edu)
 *
 * Department of Chemistry and Biochemistry
 * California State University at Norhtridge
 *
 */

#include "epr.h"

static double xwork2[2*EPR_MAXDIM];
static doublecomplex xwork[EPR_MAXDIM*512];

int epr_misc_hverbose = 0;

/*
 * Function: epr_type *epr_misc_change_resolution(epr_type *sp, int resol)
 * Purpose: change spectrum resolution by either dropping
 *          points or by duplicating them.
 *
 * sp    = spectrum
 * resol = new resolution (old and new must be multiples of each other)
 *
 * return value: ptr to epr_type (new spectrum)
 *               NULL = error
 *
 */

epr_type *epr_misc_change_resolution(epr_type *sp, int resol) {

  int skp, i, j, k;
  epr_type *new;

  if(!(sp->samples % resol) && !(resol % sp->samples)) return NULL; 
  
  new = epr_misc_new();
  epr_misc_copy(sp, new);
  if(resol == sp->samples) return new;

  new->samples = resol;
  if(sp->samples < resol) { /* increase # of points */
    skp = resol / sp->samples;
    for(i = j = 0; i < resol; i++, j += skp)
      for (k = 0; k < skp; k++)
	new->y[j+k] = sp->y[i];
  } else { /* decrease # of points */
    skp = sp->samples / resol;
    for(i = j = 0; i < resol; i++, j += skp)
      new->y[i] = sp->y[j];
  }

  return new;
}

/*
 * Function: epr_type *epr_misc_new()
 * Purpose: allocate a new spectrum
 *
 * return value: ptr to epr_type
 *               NULL = error
 *
 */

epr_type *epr_misc_new() {

  epr_type *ptr;

  if(!(ptr = (epr_type *) malloc(sizeof(epr_type)))) {
    fprintf(stderr, "Memory allocation error.\n");
    exit(1);
  }
  return ptr;
}

/*
 * Function: epr_type *epr_misc_copy(epr_type *, epr_type *)
 * Purpose: copy sp1 to sp2.
 *
 * sp1  = source spectrum [input; epr_type *]
 * sp2  = destination spectrum [output; epr_type *]
 *
 * return value: sp2
 *
 */

epr_type *epr_misc_copy(epr_type *sp1, epr_type *sp2) {

  memcpy(sp2, sp1, sizeof(epr_type));
  return sp2;
}

/*
 * Function: epr_misc_zero(epr_type *)
 * Purpose: zero spectra parameters
 *
 * sp = spectra to be zeroed [input; epr_type *]
 *
 * return value: ptr to sp.
 *
 */

epr_type *epr_misc_zero(epr_type *sp) {

  sp->fname[0] = sp->name[0] = sp->operator[0] = sp->date[0] = sp->time[0]
    = sp->solvent[0] = sp->info[0] = sp->spek_type = sp->samp_type =
    sp->xunits = (char) 0;
  sp->harmonic = sp->resonator = 1;
  sp->samples = sp->scans = 0;
  sp->temperature = sp->gain = sp->frequency = sp->power = sp->modfreq = 
    sp->timec = sp->ctime = sp->modulampl = sp->power = sp->endor_b = 
    sp->endor_mod = sp->endor_pump_freq = sp->endor_power = sp->xbeg =
    sp->xend = sp->theta = sp->phi = sp->endor_pump_power = 0.0;
  sp->sphase = 0.0;
  return sp;
}

/*
 * Function: epr_misc_zerosp(epr_type *, double from, double to)
 * Purpose: zero part of spectrum
 *
 * sp   = spectra to be zeroed [input; epr_type *]
 * from = zero from (x-axis value); 0.0 = beg [input, double]
 * to   = zero to (x-axis value); 0.0 = end [input, double]
 *
 * return value: ptr to sp.
 *
 */

epr_type *epr_misc_zerosp(epr_type *sp, double from, double to) {

  int i, j;
  double step;

  step = EPR_XSTEP(sp);
  if(from == 0.0)
    i = 0;
  else
    i = EPR_XTOI(sp,from,step);

  if(to == 0.0)
    j = sp->samples - 1;
  else
    j = EPR_XTOI(sp,to,step);

  for ( ; i <= j; i++)
    sp->y[i] = 0.0;

  return sp;
}

/*
 * Function: epr_misc_drms(epr_type *, epr_type *)
 * Purpose: calculate (squared) RMS difference of two spectra (scaled)
 *
 * sp1 = pointer to the first spectrum [input; epr_type *]
 * sp2 = pointer to the second spectrum [input; epr_type *]
 *
 * return value: (scaled) RMS difference.
 *
 */

double epr_misc_drms(epr_type *sp1, epr_type *sp2) {

  double error = 0.0, tmp;
  int i, n;

  n = (sp1->samples < sp2->samples)?sp1->samples:sp2->samples;
  for (i = 0; i < n; i++) {
    tmp = sp1->y[i] - sp2->y[i];
    error += tmp*tmp;
  }

  return error / ((double) n);
}

/*
 * Function: epr_misc_rz(epr_type *, epr_type *)
 * Purpose: remove zero areas from the beginning & end of the spectrum
 *
 * sp  = pointer to the spectrum [input/output; epr_type *]
 * thr = threshold value for zero [input, double]
 *
 * return value: ptr to spectrum.
 *
 */

epr_type *epr_misc_rz(epr_type *sp, double thr) {

  int i, beg, end;
  double tmp;
  
  if(fabs(sp->y[0]) <= thr)
    for (beg = 0; beg < sp->samples && fabs(sp->y[beg]) <= thr ; beg++);
  else beg = 0;

  if(fabs(sp->y[sp->samples - 1]) <= thr)
    for (end = sp->samples - 1; end >= 0 && fabs(sp->y[end]) <= thr; end--);
  else end = sp->samples - 1;

  for (i = 0; i < sp->samples; i++)
    if(i >= beg) sp->y[i-beg] = sp->y[i];
  tmp = EPR_ITOX(sp, end, EPR_XSTEP(sp));
  sp->xbeg = EPR_ITOX(sp, beg, EPR_XSTEP(sp));
  sp->xend = tmp;
  sp->samples = end - beg + 1;
  return sp;
}

/*
 * Function: double epr_misc_normalize(epr_type *, epr_type *)
 * Purpose: Return normalization constant by which sp1 should
 *          be multiplied in order to get sp2's intensity.
 *          (used in spectral fitting)
 *
 * sp1     = Experimental spectrum [input; epr_type *]
 * sp2     = Simulated spectrum [input; epr_type *]
 *
 * Return: Normalization constant (0.0 on error).
 *
 */

double epr_misc_normalize(epr_type *sp1, epr_type *sp2) {

  double n, s;
  int i;

  if(sp1->samples != sp2->samples) return 0.0;
  s = n = 0.0;
  for (i = 0; i < sp1->samples; i++) {
    n += sp1->y[i]*sp2->y[i];
    s += sp2->y[i]*sp2->y[i];
  }
  if(fabs(s) < EPR_EPS) s = 1.0;
  n /= s;
  return n;
}

/*
 * Function: epr_type *epr_misc_scale(epr_type *, double)
 * Purpose: Scale spectrum to interval [0, max]
 *
 * sp  = Pointer to spectrum to be scaled [input/output; epr_type *]
 * max = Y-data point maximum value [input; double]
 *
 * Return: pointer to sp. NULL pointer on error (i.e. max = 0.0).
 *
 */

epr_type *epr_misc_scale(epr_type *sp, double max) {

  int i;
  double maxy, *y, tmp;

  y = sp->y;
  maxy = fabs(y[0]);
  for (i = 1; i < sp->samples; i++) {
    tmp = fabs(y[i]);
    if(tmp > maxy) maxy = tmp;
  }
  if(maxy < EPR_EPS) return (epr_type *) NULL;
  tmp = max / maxy;
  for (i = 0; i < sp->samples; i++)
    y[i] *= tmp;
  return sp;
}

/*
 * Solve (real) linear system Ax=b using LAPACK routine.
 * Uses LU factorization:
 *
 * DGETRF, DGETRS DGECON, DGETFS, DGETRI, DGEEQU.
 *
 */

void epr_misc_linear(double *mat, int n, double *b) {

  int info, one = 1, ipiv[256];
  char trans = 'N';
  
  if(n > 256) {
    fprintf(stderr,"libepr: too large dimension in  epr_misc_linear(). Increase ipiv array in misc.c\n");
    exit(1);
  }
  /* factorize */
  dgetrf_(&n, &n, mat, &n, ipiv, &info);
  if(info != 0) fprintf(stderr,"libepr: warning - DGETRF failed.\n");
  /* solve */
  dgetrs_(&trans, &n, &one, mat, &n, ipiv, b, &n, &info);
  if(info != 0) fprintf(stderr,"libepr: warning - DGETRS failed.\n");
}

/* 
 * Solve eigen values and vectors of given Hermitean matrix
 * (using LAPACK). TODO: make the calculation of eigen vectors
 * optional. QR/QL method.
 *
 */

void epr_misc_heigen(doublecomplex *mat, int n, double *w, doublecomplex *vec) {
  
  int lwork = EPR_MAXDIM * 512, info = 0;

  zheev_("V","U",&n,mat,&n,w,&xwork,&lwork,&xwork2,&info);
  bcopy(mat,vec,sizeof(doublecomplex) * n * n);
}

/*
 * Solve the right and left eigen vectors and the corresponding
 * eigen values by reduction to the Hessenberg form. This routine
 * is just a driver for the LAPACK routeines which do the actual
 * work. TODO: limit the line generation area. Also the eigen solver
 * still suffers from numerical instability. This will show up many times
 * as unsymmetrical lineshapes. - optimize the code - 
 *
 */

void epr_misc_eigen(doublecomplex *vecr, doublecomplex *vecl, doublecomplex *w, doublecomplex *k, int n) {
  
  int ilo, ihi, i, j;
  doublecomplex tau[EPR_MAX_JUMPS];
  doublecomplex work[EPR_MAX_JUMPS*EPR_MAX_JUMPS], z[EPR_MAX_JUMPS*EPR_MAX_JUMPS];
  double rwork[EPR_MAX_JUMPS], norm;
  int lwork = EPR_MAX_JUMPS*EPR_MAX_JUMPS, info, ifaill[EPR_MAX_JUMPS],
    ifailr[EPR_MAX_JUMPS], tmp;
  logical select;
  double scale[EPR_MAX_JUMPS];

  /*  ilo = 1;
      ihi = n; */
  /* balance matrix */
  (void) zgebal_("B", &n, k, &n, &ilo, &ihi, scale, &info);
  if(info != 0)
    fprintf(stderr,"libepr: warning - Matrix balance failed (ZGEBAL).\n");

  /* First to Hessenber form LAPACK: ZGEHRD */
  /* A to H */
  (void) zgehrd_(&n, &ilo, &ihi, k, &n, tau, work, &lwork, &info);
  if(info != 0)
    fprintf(stderr, "libepr: warning - Reduction to Hessenberg form failed (ZGEHRD).\n");

  /* Call ZUNGHR to form the Hessenberg transformation matrix Q */
  /* Z <- Q */
  tmp = n * n;
  for (i = 0; i < tmp; i++) {
    z[i].r = k[i].r;
    z[i].i = k[i].i;
  }
  (void) zunghr_(&n, &ilo, &ihi, z, &n, tau, work, &lwork, &info);
  if(info != 0)
    printf("Warning: ZUNGHR failed.\n");

  /* Calculate eigenvalues using ZHSEQR */
  /* Z <- QS */
  /* H to T */
  (void) zhseqr_("S", "V", &n, &ilo, &ihi, k, &n, w, z, &n, work,
		 &lwork, &info);
  if(info != 0)
    fprintf(stderr,"libepr: warning - Calculation of eigenvalues failed (ZHSEQR).\n");

  /* Calculate eigenvectors (ZTRVEC) */
  /* Transform back to original representation by using Z */
  for(i = 0; i < tmp; i++) {
    vecr[i].r = vecl[i].r = z[i].r;
    vecr[i].i = vecl[i].i = z[i].i;
  }
  (void) ztrevc_("B", "B", &select, &n, k, &n, vecl, &n, vecr,
		 &n, &n, &n, work, rwork, &info);
  if(info != 0)
    fprintf(stderr,"libeprL warning - Calculation of eigen vectors failed (ZTREVC).\n");

  /* normalize eigen vectors to unity (is this required?) */
  for (i = 0; i < n; i++) {
    /* left */
    norm = 0.0;
    for (j = 0; j < n; j++)
      norm += vecl[j + n*i].r * vecl[j + n*i].r +
	vecl[j + n*i].i * vecl[j + n*i].i;
    norm = sqrt(norm);
    for (j = 0; j < n; j++) {
      vecl[j + n*i].r /= norm;
      vecl[j + n*i].i /= norm;
    }
    /* right */
    norm = 0.0;
    for (j = 0; j < n; j++)
      norm += vecr[j + n*i].r * vecr[j + n*i].r +
	vecr[j + n*i].i * vecr[j + n*i].i;
    norm = sqrt(norm);
    for (j = 0; j < n; j++) {
      vecr[j + n*i].r /= norm;
      vecr[j + n*i].i /= norm;
    }
  }
}

/*
 * Function: void epr_misc_create_sp(epr_type *, epr_sim_set *, int, int, int,
 *                                  double *, int, int, int, int)
 *
 * Purpose: Front end for spectrum simulation.
 *
 * sp   = Target spectrum [input/output; epr_type *]
 * sims = Array of simulation sets [input; epr_sim_set *]
 * ns   = Number of sets in sims [input; int]
 * sm   = Simulation model - see below [input; int]
 * model= Lineshape model - see below [input; int]
 * fltmp= Optional double pointer type argument [input; double *]
 * ham  = Hamilton (see below) [input; int]
 * tran = Transition moment operator [input; int]
 * imode= Powder integrator model (I_GAUSS_... and I_SIMPSON, I_TRASIM, etc.)
 * npt  = Number of points in quadrature.
 * 
 *
 * Variables e, ne, t, and nt are only referenced if SM_EXACT is used.
 *
 * The available simulation models:
 *
 * EPR_SM_1STORDER         First order stick spectrum.
 * EPR_SM_2NDORDER         Second order stick spectrum. (MISSING!)
 * EPR_SM_EXACT            Exact simulation based on the numerical solution of the
 *                     spin Hamiltonian. (look for epr_hamilton_divs)
 *
 * The minor and major modes may be combined by summation. The minor modes are:
 * EPR_LS_NORMAL           Normal Lorentz or Gauss lineshape.
 * EPR_LS_ASYMMLW          Asymmetric m_I dependend Lorentz or Gauss lineshape.
 * EPR_LS_NORRIS           Exchange lineshape using the Norris equation.
 * EPR_LS_HEINZER_INTRA    Exchange lineshape using the Heinzer intra method.
 * EPR_LS_HEINZER_INTER    Exchange lineshape using the Heinzer inter method.
 *
 * The major modes are:
 * EPR_LS_ANISOTROPIC      Simulate using anisotropic and isotropic couplings
 *                     present (includes angluar dependence of theta and phi).
 *                     Note that the isotropic g is NOT added to the trace of
 *                     g matrix.
 * EPR_LS_POWDER           Generate powder lineshape using isotropic and aniso-
 *                     tropic couplings (+ anisotropic g).
 *
 * Global variables:
 *
 * epr_phi         = number of phi steps in powder integration (0 = use IGLOO).
 * epr_theta       = number of theta steps in powder integration.
 * epr_phi_begin   = begin phi angle for powder integrator
 * epr_phi_end     = end phi angle for powder integrator
 * epr_theta_begin = begin theta angle for powder integrator
 * epr_theta_end   = end theta angle for powder integrator
 * epr_leave_fft   = if 1 then the spectrum is left in the frequency domain
 *                  otherwise the inverse fft is performed.
 *
 */


int epr_phi = 20, epr_theta = 20, epr_leave_fft = 0;
double epr_phi_begin = 0.0, epr_phi_end = M_PI_2,
  epr_theta_begin = 0.0, epr_theta_end = M_PI_2;

epr_peak *epr_misc_peaks[EPR_MAXNS]; /* these may be used in powder.c */
int epr_misc_npeaks[EPR_MAXNS];
static epr_oper hamil[EPR_MAXOPER], transition[EPR_MAXOPER], transition_n[EPR_MAXOPER];
static int el_spin, el_orb, nucl, nh;
/* ugh.. this is kind of ugly.. but works .. */
int epr_misc_fictious = 0, epr_misc_orbit = 0;
int epr_misc_preload = 0, epr_misc_noupd = 0;

#define SWAP(x,y) {tmpp = epr_misc_peaks[(x)]; epr_misc_peaks[(x)] = epr_misc_peaks[(y)]; epr_misc_peaks[(y)] = tmpp; tmp3 = epr_misc_npeaks[(x)]; epr_misc_npeaks[(x)] = epr_misc_npeaks[(y)]; epr_misc_npeaks[(y)] = tmp3; }

int epr_misc_create_sp(epr_type *sp, epr_sim_set *sims, int ns, int sm, int model, void *fltmp, int ham, int tran, int imode, int npt) {

  int i, j, k, l, m, n, o, p, total = 0, nt, nnt, nel, tmp3;
  int ac[EPR_MAXELS];
  double l_dir[3], stheta, sphi, lp_dir[3], lpp_dir[3];
  double theta, phi, intens, a1, a2, a3, tmp, tmp2;
  double a1p, a2p, a3p;
  static int inited = 0;
  extern int epr_hamilton_constant, epr_powder_evals, epr_hamilton_lfollow;
  extern int epr_hamilton_mode, epr_hamilton_strict;
  epr_type tmp_sp;
  static int prev_nh = 0;
  extern FILE *epr_hamilton_br;
  extern double epr_hamilton_from, epr_hamilton_to, epr_hamilton_freq;
  static int bt = 0;
  double post[EPR_MAXPEAKS];
  epr_peak *tmpp;

  if(!inited) {
    for (i = 0; i < EPR_MAXNS; i++)
      epr_misc_peaks[i] = (epr_peak *) malloc(sizeof(epr_peak) * EPR_MAXPEAKS);
    inited = 1;
  }
  for (i = 0; i < ns; i++) epr_misc_npeaks[i] = 0;

  /* energy vs. angle visualization - for other visualization stuff
   * see hamilton.c */
  if(!bt && epr_hamilton_mode == EPR_SH_ANGLE) {
    if(model >= EPR_LS_POWDER) model -= EPR_LS_POWDER;
    if(epr_theta < 2) stheta = 1.0;
    else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
    if(epr_phi < 2) sphi = 1.0;
    else sphi = fabs(epr_phi_end - epr_phi_begin) / (double) (epr_phi - 1);
    epr_misc_noupd = 1;
    bt = 1;
    tmp_sp.xbeg = epr_hamilton_from;
    tmp_sp.xend = epr_hamilton_to;
    tmp_sp.frequency = epr_hamilton_freq;
    epr_hamilton_mode = EPR_SH_NORMAL;
    i = 1;
    if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
    phi = 0.0; /* for now - later ask for angle */
    for (theta = epr_theta_begin; theta <= epr_theta_end; theta += stheta) {
      sims->theta = theta;
      sims->phi = phi;
      (void) epr_misc_create_sp(&tmp_sp, sims, 1, sm, model, fltmp, ham, tran, 0, 0);
      if(i == EPR_MAXNS-1) {
	printf("Out of peak storage (increase MAXNS).\n");
	return -1;
      }
      SWAP(0,i);
      i++;
      if(epr_hamilton_lfollow) {
	if (!(i % 10)) epr_hamilton_lfollow = 2;
	else epr_hamilton_lfollow = 1;
      }
    }
    if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
    j = 1;
    for (k = 0; k < EPR_MAXPEAKS; k++) {
      l = 0;
      for (j = 1; j < i; j++) {
	if(epr_misc_peaks[j][k].pos == 0.0) continue;
	if(k >= epr_misc_npeaks[j]) continue;
	l = 1;
	fprintf(epr_hamilton_br, "%le %le\n", epr_misc_peaks[j][k].pos,
		180.0*(epr_theta_begin + stheta * (double) (j-1)) / M_PI);
      }
      if(l) fprintf(epr_hamilton_br, "**\n");
    }
    epr_misc_noupd = 0;
    bt = 0;
    return 0;
  }

  if(epr_hamilton_br && model >= EPR_LS_POWDER) model -= EPR_LS_POWDER;
  if(model >= EPR_LS_POWDER && !epr_hamilton_br) {
    if(epr_theta_end > M_PI || epr_theta_begin < 0.0) {
      fprintf(stderr, "epr_theta out of valid range [0,pi].\n");
      return -1;
    }
    if(epr_phi_end > 2.0 * M_PI || epr_phi_begin < 0.0) {
      fprintf(stderr, "epr_phi out of valid range [0,2pi].\n");
      return -1;
    }
    tmp_sp.samples = sp->samples;
    tmp_sp.frequency = sp->frequency;
    tmp_sp.xbeg = sp->xbeg;
    tmp_sp.xend = sp->xend;
    tmp_sp.xunits = sp->xunits;
    tmp_sp.endor_b = sp->endor_b;
    model -= EPR_LS_POWDER;
    if(model == EPR_LS_NORMAL) {
      tmp3 = EPR_LS_NORMAL;
      model = EPR_LS_STICK;
    } else tmp3 = 2000; /* ;-) */

    epr_hamilton_strict = 1;
    /* NOTE: the powder functions will call back this routine for single crystal spectra */
    for (i = 0; i < ns; i++) {
      bzero(&(tmp_sp.y), tmp_sp.samples * sizeof(double));      
      switch(imode) {
      case EPR_I_GAUSS_LEGENDRE:
	total += epr_powder_gauss(&tmp_sp, sims+i, sm, model, fltmp, ham, tran, 0, 1, npt);
	break;
      case EPR_I_GAUSS_LEGENDRE_IGLOO:
	total += epr_powder_gauss(&tmp_sp, sims+i, sm, model, fltmp, ham, tran, 1, 1, npt);
	break;
      case EPR_I_GAUSS_CHEBYSEV:
	total += epr_powder_gauss(&tmp_sp, sims+i, sm, model, fltmp, ham, tran, 0, 2, npt);
	break;
      case EPR_I_GAUSS_CHEBYSEV_IGLOO:
	total += epr_powder_gauss(&tmp_sp, sims+i, sm, model, fltmp, ham, tran, 1, 2, npt);
	break;
      case EPR_I_SIMPSON:
	total += epr_powder_simpson(&tmp_sp, sims+i, sm, model, fltmp, ham, tran);
	break;
      case EPR_I_TRASIM:
	total += epr_powder_trasim(&tmp_sp, sims+i, sm, model, fltmp, ham, tran);
	break;
      case EPR_I_TRAPEZ:
	total += epr_powder_trapez(&tmp_sp, sims+i, sm, model, fltmp, ham, tran);
	break;
      case EPR_I_INTERP:
	total += epr_powder_interp(&tmp_sp, sims+i, sm, model, fltmp, ham, tran, 0);
	break;	
      case EPR_I_INTERP_IGLOO:
	total += epr_powder_interp(&tmp_sp, sims+i, sm, model, fltmp, ham, tran, 1);
	break;	
      case EPR_I_INTERP_SPL:
	total += epr_powder_interp_spl(&tmp_sp, sims+i, sm, model, fltmp, ham, tran, 0, npt, 1);
	break;	
      case EPR_I_INTERP_SPL_IGLOO:
	total += epr_powder_interp_spl(&tmp_sp, sims+i, sm, model, fltmp, ham, tran, 1, npt, 1);
	break;	
      case EPR_I_AXIAL:
	total += epr_powder_axial(&tmp_sp, sims+i, sm, model, fltmp, ham, tran);
	break;
      case EPR_I_INTERP_AXIAL:
	total += epr_powder_axial_interp(&tmp_sp, sims+i, sm, model, fltmp, ham, tran);
	break;
      case EPR_I_INTERP_AXIAL_SPL:
	total += epr_powder_axial_spl(&tmp_sp, sims+i, sm, model, fltmp, ham, tran, npt);
	break;
      }
      
      if(model == EPR_LS_STICK && tmp3 == EPR_LS_NORMAL) {
	if(sm == EPR_SM_EXACT_ENDOR) {
	  tmp2 = sims[i].lw;
	  sims[i].lw = sims[i].elw;
	}
	epr_line_create_fft(&tmp_sp, sims+i,epr_leave_fft);
	if(sm == EPR_SM_EXACT_ENDOR) sims[i].lw = tmp2;
      }
      for (j = 0; j < tmp_sp.samples; j++)
	sp->y[j] += tmp_sp.y[j];
      epr_misc_preload = 0;
    }
    fprintf(stderr,"libepr: %d evaluations.\n", epr_powder_evals);
    epr_hamilton_strict = 0;
    return total;
  }
  
  /* having peak arrays makes things generic but requires large peak arrays */
  for (i = 0; i < ns; i++) {
    if(sims[i].amp < EPR_EPS) {
      fprintf(stderr, "libepr: warning - skipping too small intensity spectrum.\n");
      continue;
    }
    if(sp) {
      tmp_sp.samples = sp->samples;
      tmp_sp.frequency = sp->frequency;
      tmp_sp.xbeg = sp->xbeg;
      tmp_sp.xend = sp->xend;
      tmp_sp.xunits = sp->xunits;
      tmp_sp.endor_b = sp->endor_b;
      if(!epr_misc_noupd) bzero(&(tmp_sp.y), tmp_sp.samples * sizeof(double)); 
    }
    /* Produce stick spectrum */
    switch (sm) {
    case EPR_SM_1STORDER:
      epr_misc_npeaks[i] = epr_simul_stick(&tmp_sp, sims + i, epr_misc_peaks[i]);
      total += epr_misc_npeaks[i];
      break;
    case EPR_SM_1STENDOR:
      epr_misc_npeaks[i] = epr_simul_stick_endor(&tmp_sp, sims + i, epr_misc_peaks[i]);
      total += epr_misc_npeaks[i];
      break;
    case EPR_SM_EXACT:       /* EPR spectrum */
    case EPR_SM_EXACT_ENDOR: /* ENDOR spectrum */
      /* To do generate approximate nuclear labels for transitions */
      nel = (int) (sims[i].espin / 0.5);
      nh = nt = nnt = 0;
      theta = sims[i].theta + sims[i].o_theta; /* incl. offsets */
      phi = sims[i].phi + sims[i].o_phi;
      epr_tensor_dir(l_dir, phi, theta);
      
      /* el. spins: el_spin->el_orb, el. orb. mom.: el_orb -> nucl,
	 nuclear spins: nucl -> ... */
      el_spin = 0;
      if(ham & EPR_HM_FICTIOUS) {
	epr_misc_fictious = 1;
	if((ham & EPR_HM_BL) || (ham & EPR_HM_SL) || (ham & EPR_HM_CRYSTAL)) {
	  el_orb = 1;
	  nucl = 2;
	  epr_misc_orbit = 1;
	} else {
	  el_orb = nucl = 1;
	  epr_misc_orbit = 0;
	}
      } else {
	epr_misc_fictious = 0;
	if((ham & EPR_HM_BL) || (ham & EPR_HM_SL) || (ham & EPR_HM_CRYSTAL)) {
	  el_orb = nel;
	  nucl = el_orb + nel;
	  epr_misc_orbit = 1;
	} else {
	  el_orb = nucl = nel;
	  epr_misc_orbit = 0;
	}
      }
      
      if(ham & EPR_HM_GBS) { /* electron Zeeman */
	/* Here we rotate the magnetic field (ie. the direction cosine l) */
	/* The transition moment operator must be modified accordingly */
	/* b * B * l.g.S */
	/* (usage of EPR_INDEX: row first then column) */
	/* Zeeman for each electron */
	for (j = el_spin; j < el_orb; j++) {
	  a1 = l_dir[0] * sims[i].posn[j][EPR_INDEX(3,0,0)] +
	    l_dir[1] * sims[i].posn[j][EPR_INDEX(3,1,0)] +
	    l_dir[2] * sims[i].posn[j][EPR_INDEX(3,2,0)];
	  a2 = l_dir[0] * sims[i].posn[j][EPR_INDEX(3, 0, 1)] +
	    l_dir[1] * sims[i].posn[j][EPR_INDEX(3, 1, 1)] +
	    l_dir[2] * sims[i].posn[j][EPR_INDEX(3, 2, 1)];
	  a3 = l_dir[0] * sims[i].posn[j][EPR_INDEX(3, 0, 2)] +
	    l_dir[1] * sims[i].posn[j][EPR_INDEX(3, 1, 2)] +
	    l_dir[2] * sims[i].posn[j][EPR_INDEX(3, 2, 2)];
	  a1 *= EPR_GAUSS_BE / EPR_H;
	  a2 *= EPR_GAUSS_BE / EPR_H;
	  a3 *= EPR_GAUSS_BE / EPR_H;
	  if(fabs(a1) > EPR_EPS) {
	    hamil[nh].n[0] = j;
	    hamil[nh].m = a1;
	    hamil[nh].d[0] = 0; /* Sx */
	    hamil[nh].e[0] = 1;
	    hamil[nh].bdeg = 1; /* multiplied by B */
	    hamil[nh].nbr = 1;
	    if(epr_misc_hverbose) fprintf(stderr,"libepr: electron Zeeman (x) = %le\n",a1);
	    nh++;
	  }
	  if(fabs(a2) > EPR_EPS) {
	    hamil[nh].n[0] = j;
	    hamil[nh].m = a2;
	    hamil[nh].d[0] = 1; /* Sy */
	    hamil[nh].e[0] = 1;
	    hamil[nh].bdeg = 1; /* multiplied by B */
	    hamil[nh].nbr = 1;
	    if(epr_misc_hverbose) fprintf(stderr,"libepr: electron Zeeman (y) = %le\n",a2);
	    nh++;
	  }
	  if(fabs(a3) > EPR_EPS) {
	    hamil[nh].n[0] = j;
	    hamil[nh].m = a3;
	    hamil[nh].d[0] = 2; /* Sz */
	    hamil[nh].e[0] = 1;
	    hamil[nh].bdeg = 1; /* multiplied by B */
	    hamil[nh].nbr = 1;
	    if(epr_misc_hverbose) fprintf(stderr,"libepr: electron Zeeman (z) = %le\n",a3);
	    nh++;
	  }
	}
      }
      
      if(ham & EPR_HM_GBI) { /* Nuclear Zeeman */
	l = nucl;
	for (j = 0; j < sims[i].ngr; j++) {
	  for (k = 0; k < sims[i].nbr[j]; k++) {
	    /* gN isotropic */
	    tmp = epr_atoms[sims[i].atoms[j]].gn * EPR_GAUSS_BN / EPR_H;
	    for (m = 0; m < 3; m++) {
	      hamil[nh].n[0] = l;
	      hamil[nh].m = tmp * l_dir[m];
	      hamil[nh].d[0] = m; /* Im (m = x, y, z) */
	      hamil[nh].e[0] = 1;
	      hamil[nh].bdeg = 1;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		if(epr_misc_hverbose)
		  fprintf(stderr,"libepr: nuclear Zeeman (%d) = %le\n",m,hamil[nh].m);
		nh++;
	      }
	    }
	    l++;
	  }
	}
      }

      if(ham & EPR_HM_BL) { /* "Zeeman" for orbital ang. mom. */
	tmp = EPR_GAUSS_BE / EPR_H;
	for (p = el_orb; p < nucl; p++) {
	  for (k = 0; k < 3; k++) {
	    hamil[nh].n[0] = p;
	    hamil[nh].m = tmp * l_dir[k];
	    hamil[nh].d[0] = k; /* Lk (k = x, y, z) */
	    hamil[nh].e[0] = 1;
	    hamil[nh].bdeg = 1; /* multiplied by B */
	    hamil[nh].nbr = 1;
	    if(fabs(hamil[nh].m) > EPR_EPS) {
	      if(epr_misc_hverbose) fprintf(stderr,"libepr: orbital Zeeman (%d) = %le\n",
					  k, hamil[nh].m);
	      nh++;
	    }
	  }
	}
      }
      
      if(!epr_misc_preload) { /* these do not depend on B */
	prev_nh = 0;
	if(ham & EPR_HM_SL) {
	  /* interaction of each spin to the ang. mom (spin-own-orbit) */
	  /* spin-other-orbit ignored */
	  for (p = el_spin; p < el_orb; p++) {
	    for (m = 0; m < 3; m++) {
	      hamil[nh].n[0] = p; /* el spin */
	      hamil[nh].n[1] = el_orb+p; /* ang. mom. spin */
	      hamil[nh].m = sims[i].spin_orbit * EPR_CM2HZ;/* in cm-1 - conv to Hz */
	      hamil[nh].d[0] = m;
	      hamil[nh].d[1] = m;
	      hamil[nh].e[0] = 1;
	      hamil[nh].e[1] = 1;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 2;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		if(epr_misc_hverbose)
		  fprintf(stderr,"libepr: ang. mom. coupling for electron %d (dir = %d): %le.\n",p,m,hamil[nh].m);
		nh++;
		prev_nh++;
	      }
	    }
	  }
	}
	
	if(ham & EPR_HM_CRYSTAL) {
	  double lahna = sims[i].orbit;
	  double ahven1 = sims[i].cra1 * EPR_CM2HZ;
	  double ahven2 = sims[i].cra2 * EPR_CM2HZ;
	  double ahven3 = sims[i].cra3 * EPR_CM2HZ;
	  double siika = sims[i].crb * EPR_CM2HZ;
	  lahna = lahna * (lahna + 1.0);
	  for (j = el_orb; j < nucl; j++) {
	    /* cubic crystal field (operates on orb. ang. mom. only) */
	    if(sims[i].crb != 0.0) {
	      /* 1. (35/20) * beta_c Lz^4 */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = 35.0 * siika / 20.0;
	      hamil[nh].d[0] = 2; /* z */
	      hamil[nh].e[0] = 4;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	      /* 2. (1/20) * beta_c * (-30 * L * (L + 1) + 25)* Lz^2 */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = (1.0/20.0) * siika * (25.0 - 30.0 * lahna);
	      hamil[nh].d[0] = 2; /* z */
	      hamil[nh].e[0] = 2;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	      /* 3. (1/20) * beta_c * (-6 * L * (L + 1) + 3*(L*(L+1))^2) * I */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = (1.0/20.0) * siika * lahna * (3.0 * lahna - 6.0);
	      hamil[nh].d[0] = 5; /* identity */
	      hamil[nh].e[0] = 1;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	      /* 4. (1/8) * beta_c * L+^4 */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = (1.0 / 8.0) * siika;
	      hamil[nh].d[0] = 3;
	      hamil[nh].e[0] = 4;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	      /* 5. (1/8) * beta_c * L-^4 */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = (1.0 / 8.0) * siika;
	      hamil[nh].d[0] = 4;
	      hamil[nh].e[0] = 4;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	    }
	    if(sims[i].cra1 != 0.0) {
	      /* tetragonal field terms (z-direction) */
	      /* 3 * alpha * Lx^2 */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = 3.0 * ahven1;
	      hamil[nh].d[0] = 0;
	      hamil[nh].e[0] = 2;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	      /* -alpha * L * (L + 1) * I */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = -ahven1 * lahna;
	      hamil[nh].d[0] = 5;
	      hamil[nh].e[0] = 1;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	    }
	    if(sims[i].cra2 != 0.0) {
	      /* tetragonal field terms (y-direction) */
	      /* 3 * alpha * Ly^2 */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = 3.0 * ahven2;
	      hamil[nh].d[0] = 1;
	      hamil[nh].e[0] = 2;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	      /* -alpha * L * (L + 1) * I */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = -ahven2 * lahna;
	      hamil[nh].d[0] = 5;
	      hamil[nh].e[0] = 1;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	    }
	    if(sims[i].cra3 != 0.0) {
	      /* tetragonal field terms (z-direction) */
	      /* 3 * alpha * Lz^2 */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = 3.0 * ahven3;
	      hamil[nh].d[0] = 2;
	      hamil[nh].e[0] = 2;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	      /* -alpha * L * (L + 1) * I */
	      hamil[nh].n[0] = j;
	      hamil[nh].m = -ahven3 * lahna;
	      hamil[nh].d[0] = 5;
	      hamil[nh].e[0] = 1;
	      hamil[nh].bdeg = 0;
	      hamil[nh].nbr = 1;
	      if(fabs(hamil[nh].m) > EPR_EPS) {
		nh++;
		prev_nh++;
	      }
	    }
	  }
	}

	if(ham & EPR_HM_ASI) { /* hyperfine coupling */
	  /* interaction of each electron to magnetic nuclei are treated */
	  for (p = el_spin; p < el_orb; p++) {
	    l = nucl; /* nuclear spins start at nucl */
	    for (j = 0; j < sims[i].ngr; j++) {
	      for (k = 0; k < sims[i].nbr[j]; k++) {
		for (m = 0; m < 3; m++)
		  for (n = 0; n < 3; n++) {
		    hamil[nh].n[0] = p; /* el spin */
		    hamil[nh].n[1] = l; /* nu spin */
		    hamil[nh].m = sims[i].hfc[p][j][EPR_INDEX(3,m,n)] * 1.0E6; /* Amn */
		    hamil[nh].d[0] = m;
		    hamil[nh].d[1] = n;
		    hamil[nh].e[0] = 1;
		    hamil[nh].e[1] = 1;
		    hamil[nh].bdeg = 0;
		    hamil[nh].nbr = 2;
		    if(fabs(hamil[nh].m) > EPR_EPS) {
		      if(epr_misc_hverbose)
			fprintf(stderr,"libepr: hyperfine (%d,%d) for electron %d and nucleus group %d = %le\n",m,n,p,j,hamil[nh].m);
		      nh++;
		      prev_nh++;
		    }
		  }
		l++;
	      }
	    }
	  }
	}
	
	if(ham & EPR_HM_SDS) {
	  /* Si.D.Sj between each electron */
	  for (o = el_spin; o < el_orb; o++)
	    for (p = el_spin; p < el_orb; p++) 
	      for (k = 0; k < 3; k++) 
		for (j = 0; j < 3; j++) {
		  /* Sk.Sj */
		  hamil[nh].n[0] = o;
		  hamil[nh].n[1] = p;
		  hamil[nh].m = sims[i].d[o][p][EPR_INDEX(3,k,j)] * 1.0E6; /* Dkj */
		  hamil[nh].d[0] = k;
		  hamil[nh].d[1] = j;
		  hamil[nh].e[0] = 1;
		  hamil[nh].e[1] = 1;
		  hamil[nh].bdeg = 0;
		  hamil[nh].nbr = 2;
		  if(fabs(hamil[nh].m) > EPR_EPS) {
		    if(epr_misc_hverbose)
		      fprintf(stderr,"libepr: SDS(%d,%d) component (%d,%d) = %le\n",o,p,k,j,
			     hamil[nh].m);
		    nh++;
		    prev_nh++;
		  }
		}
	}
	
	if(ham & EPR_HM_IPI) {
	  l = nucl;
	  for (j = 0; j < sims[i].ngr; j++) {
	    for (k = 0; k < sims[i].nbr[j]; k++) {
	      for (m = 0; m < 3; m++)
		for (n = 0; n < 3; n++) {
		  hamil[nh].n[0] = l;
		  hamil[nh].n[1] = l;
		  hamil[nh].m = sims[i].p[j][EPR_INDEX(3,m,n)] * 1.0E6; /* Pmn */
		  hamil[nh].d[0] = m;
		  hamil[nh].d[1] = n;
		  hamil[nh].e[0] = 1;
		  hamil[nh].e[1] = 1;
		  hamil[nh].bdeg = 0;
		  hamil[nh].nbr = 2;
		  if(fabs(hamil[nh].m) > EPR_EPS) {
		    nh++;
		    prev_nh++;
		  }
		}
	      l++;
	    }
	  }
	}
      } else nh += prev_nh;

      lp_dir[0] = -l_dir[1]; /* unit vector perpendicular to field */
      lp_dir[1] = l_dir[0];
      lp_dir[2] = 0.0;
      tmp = sqrt(lp_dir[0] * lp_dir[0] + lp_dir[1] * lp_dir[1]);
      if(fabs(tmp) <= EPR_EPS) {
	lp_dir[0] = 1.0;
	lp_dir[1] = 0.0;
      } else {
	lp_dir[0] /= tmp;
	lp_dir[1] /= tmp;
      }
      if(sm == EPR_SM_EXACT_ENDOR) {
	/* pick a vector perpendicular to both l_dir and lp_dir */
	lpp_dir[0] = l_dir[1] * lp_dir[2] - l_dir[2] * lp_dir[1];
	lpp_dir[1] = l_dir[2] * lp_dir[0] - l_dir[0] * lp_dir[2];
	lpp_dir[2] = l_dir[0] * lp_dir[1] - l_dir[1] * lp_dir[0];
      } else {
	lpp_dir[0] = lp_dir[0];
	lpp_dir[1] = lp_dir[1];
	lpp_dir[2] = lp_dir[2];
      }

      if(tran & EPR_TR_S_90) {
	for (j = el_spin; j < el_orb; j++) {
	  a1p = lp_dir[0] * sims[i].posn[j][EPR_INDEX(3,0,0)] +
	    lp_dir[1] * sims[i].posn[j][EPR_INDEX(3,1,0)] +
	    lp_dir[2] * sims[i].posn[j][EPR_INDEX(3,2,0)];
	  a2p = lp_dir[0] * sims[i].posn[j][EPR_INDEX(3, 0, 1)] +
	    lp_dir[1] * sims[i].posn[j][EPR_INDEX(3, 1, 1)] +
	    lp_dir[2] * sims[i].posn[j][EPR_INDEX(3, 2, 1)];
	  a3p = lp_dir[0] * sims[i].posn[j][EPR_INDEX(3, 0, 2)] +
	    lp_dir[1] * sims[i].posn[j][EPR_INDEX(3, 1, 2)] +
	    lp_dir[2] * sims[i].posn[j][EPR_INDEX(3, 2, 2)];      
	  /* Sx */
	  transition[nt].n[0] = j;
	  transition[nt].m = a1p;
	  transition[nt].d[0] = 0;
	  transition[nt].e[0] = 1;
	  transition[nt].bdeg = 0;
	  transition[nt].nbr = 1;
	  if(fabs(transition[nt].m) > EPR_EPS) {
	    if(epr_misc_hverbose) fprintf(stderr,"libepr: transition mom. for electron %d (x) = %le\n",
					j, transition[nt].m);
	    nt++;
	  }
	  /* Sy */
	  transition[nt].n[0] = j;
	  transition[nt].m = a2p;
	  transition[nt].d[0] = 1;
	  transition[nt].e[0] = 1;
	  transition[nt].bdeg = 0;
	  transition[nt].nbr = 1;
	  if(fabs(transition[nt].m) > EPR_EPS) {
	    if(epr_misc_hverbose) fprintf(stderr,"libepr: transition mom. for electron (y) = %le\n",
					j, transition[nt].m);
	    nt++;
	  }
	  /* Sz */
	  transition[nt].n[0] = j;
	  transition[nt].m = a3p;
	  transition[nt].d[0] = 2;
	  transition[nt].e[0] = 1;
	  transition[nt].bdeg = 0;
	  transition[nt].nbr = 1;
	  if(fabs(transition[nt].m) > EPR_EPS) {
	    if(epr_misc_hverbose) fprintf(stderr,"libepr: transition mom. for electron (z) = %le\n",
					j,transition[nt].m);
	    nt++;
	  }
	}
      }

      if(tran & EPR_TR_S_0) {
	for (j = el_spin; j < el_orb; j++) {
	  a1 = l_dir[0] * sims[i].posn[j][EPR_INDEX(3,0,0)] +
	    l_dir[1] * sims[i].posn[j][EPR_INDEX(3,1,0)] +
	    l_dir[2] * sims[i].posn[j][EPR_INDEX(3,2,0)];
	  a2 = l_dir[0] * sims[i].posn[j][EPR_INDEX(3, 0, 1)] +
	    l_dir[1] * sims[i].posn[j][EPR_INDEX(3, 1, 1)] +
	    l_dir[2] * sims[i].posn[j][EPR_INDEX(3, 2, 1)];
	  a3 = l_dir[0] * sims[i].posn[j][EPR_INDEX(3, 0, 2)] +
	    l_dir[1] * sims[i].posn[j][EPR_INDEX(3, 1, 2)] +
	    l_dir[2] * sims[i].posn[j][EPR_INDEX(3, 2, 2)];      
	  /* Sx */
	  transition[nt].n[0] = j;
	  transition[nt].m = a1;
	  transition[nt].d[0] = 0;
	  transition[nt].e[0] = 1;
	  transition[nt].bdeg = 0;
	  transition[nt].nbr = 1;
	  if(fabs(transition[nt].m) > EPR_EPS) nt++;
	  /* Sy */
	  transition[nt].n[0] = j;
	  transition[nt].m = a2;
	  transition[nt].d[0] = 1;
	  transition[nt].e[0] = 1;
	  transition[nt].bdeg = 0;
	  transition[nt].nbr = 1;
	  if(fabs(transition[nt].m) > EPR_EPS) nt++;
	  /* Sz */
	  transition[nt].n[0] = j;
	  transition[nt].m = a3;
	  transition[nt].d[0] = 2;
	  transition[nt].e[0] = 1;
	  transition[nt].bdeg = 0;
	  transition[nt].nbr = 1;
	  if(fabs(transition[nt].m) > EPR_EPS) nt++;
	}
      }

      if(tran & EPR_TR_I_90) {
	l = nucl;
	for (j = 0; j < sims[i].ngr; j++)
	  for (k = 0; k < sims[i].nbr[j]; k++) {
	    /* gN isotropic */
	    tmp = epr_atoms[sims[i].atoms[j]].gn * EPR_GAUSS_BN / EPR_GAUSS_BE;
	    transition[nt].n[0] = l;
	    transition[nt].m = tmp * lp_dir[0];
	    transition[nt].d[0] = 0; /* Ix */
	    transition[nt].e[0] = 1;
	    transition[nt].bdeg = 0;
	    transition[nt].nbr = 1;
	    if(fabs(transition[nt].m) > EPR_EPS) nt++;
	    if(sm == EPR_SM_EXACT_ENDOR) {
	      transition_n[nnt].n[0] = l;
	      transition_n[nnt].m = tmp * lpp_dir[0];
	      transition_n[nnt].d[0] = 0; /* Ix */
	      transition_n[nnt].e[0] = 1;
	      transition_n[nnt].bdeg = 0;
	      transition_n[nnt].nbr = 1;
	      if(fabs(transition_n[nnt].m) > EPR_EPS) nnt++;
	    }
	    transition[nt].n[0] = l;
	    transition[nt].m = tmp * lp_dir[1];
	    transition[nt].d[0] = 1; /* Iy */
	    transition[nt].e[0] = 1;
	    transition[nt].bdeg = 0;
	    transition[nt].nbr = 1;
	    if(fabs(transition[nt].m) > EPR_EPS) nt++;
	    if(sm == EPR_SM_EXACT_ENDOR) {
	      transition_n[nnt].n[0] = l;
	      transition_n[nnt].m = tmp * lpp_dir[1];
	      transition_n[nnt].d[0] = 1; /* Iy */
	      transition_n[nnt].e[0] = 1;
	      transition_n[nnt].bdeg = 0;
	      transition_n[nnt].nbr = 1;
	      if(fabs(transition_n[nnt].m) > EPR_EPS) nnt++;
	    }
	    transition[nt].n[0] = l;
	    transition[nt].m = tmp * lp_dir[2];
	    transition[nt].d[0] = 2; /* Iz */
	    transition[nt].e[0] = 1;
	    transition[nt].bdeg = 0;
	    transition[nt].nbr = 1;
	    if(fabs(transition[nt].m) > EPR_EPS) nt++;
	    if(sm == EPR_SM_EXACT_ENDOR) {
	      transition_n[nnt].n[0] = l;
	      transition_n[nnt].m = tmp * lpp_dir[2];
	      transition_n[nnt].d[0] = 2; /* Iz */
	      transition_n[nnt].e[0] = 1;
	      transition_n[nnt].bdeg = 0;
	      transition_n[nnt].nbr = 1;
	      if(fabs(transition_n[nnt].m) > EPR_EPS) nnt++;
	    }
	    l++;
	  }
      }

      if(tran & EPR_TR_I_0) {
	l = nucl;
	for (j = 0; j < sims[i].ngr; j++)
	  for (k = 0; k < sims[i].nbr[j]; k++) {
	    /* gN isotropic */
	    tmp = epr_atoms[sims[i].atoms[j]].gn * EPR_GAUSS_BN / EPR_GAUSS_BE;
	    transition[nt].n[0] = l;
	    transition[nt].m = tmp * l_dir[0];
	    transition[nt].d[0] = 0; /* Ix */
	    transition[nt].e[0] = 1;
	    transition[nt].bdeg = 0;
	    transition[nt].nbr = 1;
	    if(fabs(transition[nt].m) > EPR_EPS) nt++;
	    if(sm == EPR_SM_EXACT_ENDOR) {
	      transition_n[nnt].n[0] = l;
	      transition_n[nnt].m = tmp * lp_dir[0];
	      transition_n[nnt].d[0] = 0; /* Ix */
	      transition_n[nnt].e[0] = 1;
	      transition_n[nnt].bdeg = 0;
	      transition_n[nnt].nbr = 1;
	      if(fabs(transition_n[nnt].m) > EPR_EPS) nnt++;
	    }
	    transition[nt].n[0] = l;
	    transition[nt].m = tmp * l_dir[1];
	    transition[nt].d[0] = 1; /* Iy */
	    transition[nt].e[0] = 1;
	    transition[nt].bdeg = 0;
	    transition[nt].nbr = 1;
	    if(fabs(transition[nt].m) > EPR_EPS) nt++;
	    if(sm == EPR_SM_EXACT_ENDOR) {
	      transition_n[nnt].n[0] = l;
	      transition_n[nnt].m = tmp * lp_dir[1];
	      transition_n[nnt].d[0] = 1; /* Iy */
	      transition_n[nnt].e[0] = 1;
	      transition_n[nnt].bdeg = 0;
	      transition_n[nnt].nbr = 1;
	      if(fabs(transition_n[nnt].m) > EPR_EPS) nnt++;
	    }

	    transition[nt].n[0] = l;
	    transition[nt].m = tmp * l_dir[2];
	    transition[nt].d[0] = 2; /* Iz */
	    transition[nt].e[0] = 1;
	    transition[nt].bdeg = 0;
	    transition[nt].nbr = 1;
	    if(fabs(transition[nt].m) > EPR_EPS) nt++;
	    if(sm == EPR_SM_EXACT_ENDOR) {
	      transition_n[nnt].n[0] = l;
	      transition_n[nnt].m = tmp * lp_dir[2];
	      transition_n[nnt].d[0] = 2; /* Iz */
	      transition_n[nnt].e[0] = 1;
	      transition_n[nnt].bdeg = 0;
	      transition_n[nnt].nbr = 1;
	      if(fabs(transition_n[nnt].m) > EPR_EPS) nnt++;
	    }
	    l++;
	  }
      }
      if(tran & EPR_TR_CONSTANT) epr_hamilton_constant = 1;
      else epr_hamilton_constant = 0;
      if(sm == EPR_SM_EXACT)
	epr_misc_npeaks[i] += epr_hamilton_generate(sims + i, &tmp_sp, hamil, nh,
					  transition, nt, epr_misc_peaks[i]);
      else {
	epr_misc_npeaks[i] += epr_hamilton_endor(sims + i, &tmp_sp, hamil, nh,
				       transition, nt, transition_n, nnt,
				       epr_misc_peaks[i]);
	sp->xunits = 2; /* MHz */
      }
      total += epr_misc_npeaks[i];
      break;
    }
  }
  if(!sp) return 0; /* only BR graphs etc. use this. 1st order needs
		       sp structure ! */
  if(epr_misc_noupd) return total; /* powder.c / interpolation ! */

  /* generate lineshape */
  epr_misc_gls(sp, &tmp_sp, sims, ns, sm, model, fltmp);

  return total;
}

/*
 * Generate lineshape.
 *
 */

inline void epr_misc_gls(epr_type *sp, epr_type *tmp_sp, epr_sim_set *sims, int ns, int sm, int model, void *fltmp) {

  int i, j, m;
  double gc1, tmp, tmp2;

  /* lineshape generation */
  switch (model) {
  case EPR_LS_NORMAL:
  case EPR_LS_STICK:
  case EPR_LS_DDEP:
    for (i = 0; i < ns; i++) {
      if(fabs(sims[i].amp) < EPR_EPS) continue;
      bzero(&tmp_sp->y, sizeof(double)*tmp_sp->samples);
      gc1 = EPR_XSTEP(tmp_sp);
      for (j = 0; j < epr_misc_npeaks[i]; j++) {
	if(epr_misc_peaks[i][j].pos == 0.0) continue;
	m = EPR_XTOI(tmp_sp, epr_misc_peaks[i][j].pos, gc1);
	if(m >= 0 && m < tmp_sp->samples) {
	  if(sm == EPR_SM_1STENDOR) {
	    double lg = 1.0;
	    epr_line_create_add(tmp_sp, &epr_misc_peaks[i][j].pos, &sims[i].e_ilw[j],&lg,
			       &epr_misc_peaks[i][j].intensity, 1, sims[i].dlevel);
	  } else tmp_sp->y[m] += epr_misc_peaks[i][j].intensity;
	}
      }
      if(sm != EPR_SM_1STENDOR && (model == EPR_LS_NORMAL || model == EPR_LS_DDEP)) {
	tmp2 = sims[i].lw;
	if(sm == EPR_SM_EXACT_ENDOR) sims[i].lw = sims[i].elw;
	if(model == EPR_LS_DDEP) {
	  double ll[3];
	  epr_tensor_dir(ll, sims[i].phi, sims[i].theta);
	  sims[i].lw = sims[i].lw * ll[0] * ll[0] + sims[i].lwy *
	    ll[1] * ll[1] + sims[i].lwz * ll[2] * ll[2];
	}
	epr_line_create_fft(tmp_sp, sims + i, epr_leave_fft);
	tmp = 1.0E8;
	sims[i].lw = tmp2;
      } else {
	tmp = 1.0;
	if(epr_leave_fft) epr_fft_inv(tmp_sp); /* scale ? */
      }
      for (j = 0; j < tmp_sp->samples; j++)
	sp->y[j] += tmp_sp->y[j]*tmp;
    }
    return;
  case EPR_LS_ASYMMLW:
    if(sm != EPR_SM_1STORDER) return;
    for (i = 0; i < ns; i++) {
      if(sims[i].amp < EPR_EPS) continue;
      epr_line_asymmlw(tmp_sp, sims + i, epr_misc_peaks[i], epr_misc_npeaks[i]);
    }
    break;
  case EPR_LS_NORRIS:
    if(sm == EPR_SM_EXACT_ENDOR) return; /* ? */
    epr_line_norris(tmp_sp, sims, epr_misc_peaks, ns, epr_misc_npeaks[0], *((double *) fltmp));
    break;
  case EPR_LS_HEINZER_INTRA:
    if(sm == EPR_SM_EXACT_ENDOR) return; /* ? */
    epr_line_heinzer_intra(tmp_sp, sims, epr_misc_peaks, ns, epr_misc_npeaks[0], (double *) fltmp);
    break;
  }

  /* sum to total spectrum */
  for (j = 0; j < sp->samples; j++)
    sp->y[j] += tmp_sp->y[j];
}
