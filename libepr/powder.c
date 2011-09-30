/*
 * Powder integrator routines.
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

/* see misc.c for details on these */
extern int epr_phi, epr_theta, epr_misc_preload, epr_leave_fft;
extern double epr_phi_begin, epr_phi_end, epr_theta_begin, epr_theta_end;
int epr_powder_evals;
int epr_powder_fm = 10;
extern int epr_hamilton_lfollow;

static epr_type tmp_sp;

static int epr_powder_weights(double *weight, double *x, int type, int npt) {

  int i;

  if(type == 1 && (npt < 2 || npt > 6)) {
    fprintf(stderr, "libepr: npts too small or large.\n");
    return -1;
  }
  if(type == 1) {
    /* Gauss-Legendre */
    switch(npt) {
    case 2:
      x[0] = 0.5773502691;
      x[1] = -x[0];
      for(i = 0; i < npt; i++) x[i] = (x[i] + 1.0) / 2.0; /* to [0, 1] */
      weight[0] = 1.0;
      weight[1] = weight[0];
      return 1;
    case 3:
      x[0] = 0.7745966692;
      x[1] = 0.0;
      x[2] = -x[0];
      for(i = 0; i < npt; i++) x[i] = (x[i] + 1.0) / 2.0; /* to [0, 1] */
      weight[0] = 0.5555555555;
      weight[1] = 0.8888888888;
      weight[2] = weight[0];
      return 1;
    case 4:
      x[0] = 0.8611363115;
      x[1] = -x[0];
      x[2] = 0.3399810435;
      x[3] = -x[2];
      for(i = 0; i < npt; i++) x[i] = (x[i] + 1.0) / 2.0; /* to [0, 1] */
      weight[0] = 0.3478548451;
      weight[1] = weight[0];
      weight[2] = 0.6521451548;
      weight[3] = weight[2];
      return 1;
    case 5:
      x[0] = 0.9061798459;
      x[1] = -x[0];
      x[2] = 0.5384693101;
      x[3] = -x[2];
      x[4] = 0.0;
      for(i = 0; i < npt; i++) x[i] = (x[i] + 1.0) / 2.0; /* to [0, 1] */
      weight[0] = 0.2369268850;
      weight[1] = weight[0];
      weight[2] = 0.4786286704;
      weight[3] = weight[2];
      weight[4] = 0.5688888888;
      return 1;
    case 6:
      x[0] = 0.9324695142;
      x[1] = -x[0];
      x[2] = 0.6612093864;
      x[3] = -x[2];
      x[4] = 0.2386191860;
      x[5] = -x[4];
      for(i = 0; i < npt; i++) x[i] = (x[i] + 1.0) / 2.0; /* to [0, 1] */
      weight[0] = 0.1713244923;
      weight[1] = weight[0];
      weight[2] = 0.3607615730;
      weight[3] = weight[2];
      weight[4] = 0.4679139345;
      weight[5] = weight[4];
      return 1;
    }
  }
  if(type == 2) {
    /* Gauss-Chebysev */
    for (i = 1; i <= npt; i++) {
      x[i-1] = cos(((double) (2*i-1)) * M_PI / (double) (2*npt));
      weight[i-1] = M_PI / (double) npt;
    }
    /* The values are over [-1,1] */
    for(i = 0; i < npt; i++) x[i] = (x[i] + 1.0) / 2.0; /* to [0, 1] */
    return 1;
  }
  return -1; /* unknown quadrature */
}

/*
 * Function: int epr_powder_interp_spl(epr_type *, epr_sim_set *, int, int, void *, int, int, int)
 *
 * Calculate powder spectrum (2D) using eigen value (bicubic spline)
 * interpolation. The integral is obtained by integrating over the interpolant.
 * It is assumed that the order of the transitions does not change
 * within the integration element! The integration is performed by n-point
 * Gauss-Legendre quadrature. The spectra are evaluated at four points
 * within the element after which interpolation is applied in order
 * to obtain all the remaining points for n-point G-L quadrature.
 *
 * sp   = Target spectrum [input/ouput, epr_type *]
 * ss   = Simulation sets [input, epr_sim_set *]
 * sm   = Simulation model [input, int]. See epr_misc_create_sp().
 * model= Lineshape model [input, int]. See epr_misc_create_sp().
 * fltmp= Temp - See epr_misc_create_sp() [input, void *].
 * ham  = Spin Hamiltonian type [input, int].
 * tran = Transition moment type [input, int].
 * igloo= 1 = Implement IGLOO partitioning, 0 = regular partitioning [input, int].
 * nip  = Number of function evaluations performed within element (nip X nip)
 *        that is 4 + interpolated points [input, int].
 * type   = Gaussian quadrature type (0 = Legendre, 1 = Chebysev) [input, int].
 *
 *
 * Returns the number of peaks generated (int).
 *
 * NOTE: 1) phi must be in [0, 2Pi] and theta in [0, Pi].
 *       2) model must not be powder! Infinite loop may occur otherwise.
 *
 */

static int inited = 0;

#define SWAP(x,y) {tmpp = epr_misc_peaks[(x)]; epr_misc_peaks[(x)] = epr_misc_peaks[(y)]; epr_misc_peaks[(y)] = tmpp; }

int epr_powder_interp_spl(epr_type *sp, epr_sim_set *ss, int sm, int model, void *fltmp, int ham, int tran, int igloo, int npt, int type) {

  int npeaks = 0, i, j, k, l, m, nrp, npt2;
  double theta, phi, stheta, sphi, tmp_int, tmp, t, p, u, st_theta, st_phi;
  double cpos, camp, xstep, tmp_int2, tmp_int3, tmp_int4;
  extern epr_peak *epr_misc_peaks[];
  extern int epr_misc_npeaks[], epr_misc_noupd;
  epr_peak *tmpp;
  double xval1[EPR_MAXNS], xval2[EPR_MAXNS];
  static double *pos_deriv[EPR_MAXNS], *amp_deriv[EPR_MAXNS], *pos[EPR_MAXNS], *amp[EPR_MAXNS];
  double sst, ssp;
  double ll[3], tmp2;

  if(!inited) {
    for (i = 0; i < EPR_MAXNS; i++) {
      pos[i] = malloc(sizeof(double) * EPR_MAXNS);
      pos_deriv[i] = malloc(sizeof(double) * EPR_MAXNS);
      amp[i] = malloc(sizeof(double) * EPR_MAXNS);
      amp_deriv[i] = malloc(sizeof(double) * EPR_MAXNS);
    }
    inited = 1;
  }

  npt2 = npt * npt;
  epr_powder_evals = 0;
  epr_misc_preload = 0;
  epr_misc_noupd = 1;
  if(epr_theta < 2) stheta = 1.0;
  else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
  tmp = fabs(epr_phi_end - epr_phi_begin);
  if(epr_phi < 2) sphi = 1.0;
  else sphi = tmp / (double) (epr_phi - 1);
  xstep = EPR_XSTEP(sp);
  tmp_int = ss->amp;
  tmp_int2 = stheta / 2.0;
  tmp_int3 = sphi / 2.0;
  
  if(model == EPR_LS_DDEP) {
    tmp_sp.samples = sp->samples;
    tmp_sp.frequency = sp->frequency;
    tmp_sp.xbeg = sp->xbeg;
    tmp_sp.xend = sp->xend;
    tmp_sp.xunits = sp->xunits;
    tmp_sp.endor_b = sp->endor_b;
    bzero(tmp_sp.y, sizeof(double) * sp->samples);
  }

  st_theta = stheta / (double) (npt-1);
  st_phi = sphi / (double) (npt-1);
  ssp = sphi / ((double) (epr_powder_fm) - 1);
  sst = stheta / ((double) (epr_powder_fm) - 1);

  for (m = 1, theta = epr_theta_begin; theta <= epr_theta_end; theta += stheta) {

    if(igloo) {
      sphi = tmp / (double) m;
      st_phi = sphi / (double) (npt-1);
      tmp_int3 = sphi;
      m++;
    }

    if(sin(theta) > EPR_EPS) {

      for (phi = epr_phi_begin; phi <= epr_phi_end; phi += sphi) {
	
	/* Calculate spectra at the four corner points of the element */
	if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
	ss->theta = theta;
	for (i = 0, l = 1; i < npt; i++) {
	  ss->phi = phi;
	  xval1[i] = ss->theta;
	  for (j = 0; j < npt; j++) {
	    xval2[j] = ss->phi;
	    ss->amp = 1.0;
	    npeaks += epr_misc_create_sp(sp,ss,1,sm,model,fltmp,ham,tran,0,0);
	    SWAP(0, l);
	    epr_misc_npeaks[l] = epr_misc_npeaks[0];
	    if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
	    epr_misc_preload = 1;
	    epr_powder_evals++;
	    l++;
	    ss->phi += st_phi;
	  }
	  ss->theta += st_theta;
	}

	/* calculate cubic spline */
	for (i = 0; i < epr_misc_npeaks[0]; i++) {
	  for (l = 1, j = 0; j <= npt2; j++) {
	    if(epr_misc_npeaks[1] != epr_misc_npeaks[l]
	       || epr_misc_peaks[l][i].pos == 0.0) break;
	    l++;
	  }
	  if(j == npt2+1) {
	    fprintf(stderr,"libepr: warning - Skipping integration element: ");
	    if(epr_misc_peaks[l][i].pos == 0.0)
	      fprintf(stderr,"libepr: position out of range.\n");
	    else
	      fprintf(stderr,"libepr: missing transition.\n");
	    continue;
	  }

	  /* calculate spline (for pos & amp) */
	  /* (theta, phi) */
	  for(l = 1, j = 0; j < npt; j++)
	    for (k = 0; k < npt; k++) {
	      pos[j][k] = epr_misc_peaks[l][i].pos;
	      amp[j][k] = epr_misc_peaks[l][i].intensity;
	      l++;
	    }

	  /* calculate 2D spline */
	  epr_base_splie2(xval1, xval2, pos, npt, npt, pos_deriv);
	  epr_base_splie2(xval1, xval2, amp, npt, npt, amp_deriv);
	  
	  /* raw integration (normalization missing) */
	  for (t = 0.0; t < 1.0; t += sst)
	    for (u = 0.0; u < 1.0; u += ssp) {
	      double pp;
	      p = theta + t*stheta;
	      pp = phi + u*sphi;
	      epr_base_splin2(xval1, xval2, pos, pos_deriv, npt, npt, p, pp, &cpos);
	      epr_base_splin2(xval1, xval2, amp, amp_deriv, npt, npt, p, pp, &camp);
	      camp *= sin(p);
	      j = EPR_XTOI(sp,cpos,xstep);
	      if(j >= 0 && j < sp->samples)
		sp->y[j] += camp;
	    }
	}
	/* if field dir. dep. lw then we must do ls generation here. */
	/* actually we assume that lw is constant in one interpolation square*/
	if(model == EPR_LS_DDEP && sm < EPR_SM_EXACT_ENDOR) {
	  epr_tensor_dir(ll, phi, theta);
	  tmp2 = ss->lw;
	  ss->lw = ss->lw * ll[0] * ll[0] + ss->lwy *
	    ll[1] * ll[1] + ss->lwz * ll[2] * ll[2];
	  epr_line_create_fft(sp, ss, epr_leave_fft);
	  ss->lw = tmp2;
	  for(i = 0; i < sp->samples; i++)
	    tmp_sp.y[i] += sp->y[i];
	  bzero(sp->y, sizeof(double) * sp->samples);
	}
      }
    }

    if(igloo) {
      if(theta < M_PI/2.0) k++; /* if theta goes over [0,pi] - the IGLOO must be corrected */
      else m--;
    }
  }
  if(model == EPR_LS_DDEP)
    bcopy(tmp_sp.y, sp->y, sizeof(double) * sp->samples);

  ss->amp = tmp_int;
  epr_misc_preload = 0;
  epr_misc_noupd = 0;
  
  return npeaks;
}


/*
 * Function: int epr_powder_interp(epr_type *, epr_sim_set *, int, int, void *, int, int)
 *
 * Calculate powder spectrum (2D) using eigen value (linear) interpolation.
 * The integral is obtained by integrating over the interpolant.
 * It is assumed that the order of the transitions does not change
 * within the integration element!
 *
 * sp   = Target spectrum [input/ouput, epr_type *]
 * ss   = Simulation sets [input, epr_sim_set *]
 * sm   = Simulation model [input, int]. See epr_misc_create_sp().
 * model= Lineshape model [input, int]. See epr_misc_create_sp().
 * fltmp= Temp - See epr_misc_create_sp() [input, void *].
 * ham  = Spin Hamiltonian type [input, int].
 * tran = Transition moment type [input, int].
 * igloo= 1 = Implement IGLOO partitioning, 0 = regular partitioning [input, int].
 * epr_powder_fm (int, external):
 *        "fine" interpolation mesh steps [input, double].
 * 
 * Returns the number of peaks generated (int).
 *
 * NOTE: 1) phi must be in [0, 2Pi] and theta in [0, Pi].
 *       2) model must not be powder!
 *
 */

int epr_powder_interp(epr_type *sp, epr_sim_set *ss, int sm, int model, void *fltmp, int ham, int tran, int igloo) {

  int npeaks = 0, i, j, k, elaska = 1;
  double theta, phi, stheta, sphi, tmp_int, tmp, t, u, sst, ssp;
  double m1, m2, m3, m4, pos, amp, xstep, tmp_int2, tmp_int3, tmp_int4;
  extern epr_peak *epr_misc_peaks[];
  extern int epr_misc_npeaks[], epr_misc_noupd;
  epr_peak *tmpp;
  extern int epr_hamilton_lfollow;
  double ll[3], tmp2;

  epr_powder_evals = 0;
  epr_misc_preload = 0;
  epr_misc_noupd = 1;
  if(epr_theta < 2) stheta = 1.0;
  else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
  tmp = fabs(epr_phi_end - epr_phi_begin);
  if(epr_phi < 2) sphi = 1.0;
  else sphi = tmp / (double) (epr_phi - 1);
  xstep = EPR_XSTEP(sp);
  ssp = sphi / ((double) (epr_powder_fm) - 1);
  sst = stheta / ((double) (epr_powder_fm) - 1);
  tmp_int4 = ss->amp;
  tmp_int = stheta * ss->amp / (double) (epr_powder_fm * epr_powder_fm);
  tmp_int2 = sphi;

  if(model == EPR_LS_DDEP) {
    tmp_sp.samples = sp->samples;
    tmp_sp.frequency = sp->frequency;
    tmp_sp.xbeg = sp->xbeg;
    tmp_sp.xend = sp->xend;
    tmp_sp.xunits = sp->xunits;
    tmp_sp.endor_b = sp->endor_b;
    bzero(tmp_sp.y, sizeof(double) * sp->samples);
  }

  for (k = 1, theta = epr_theta_begin; theta < epr_theta_end; theta += stheta) {

    if(igloo) {
      sphi = tmp / (double) k;
      tmp_int2 = sphi;
    }

    ss->phi = epr_phi_begin;
    ss->theta = theta;
    ss->amp = sin(ss->theta);
    if(ss->amp > EPR_EPS) {
      if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2; /* fix here */
      npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
      if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
      epr_powder_evals++;
      SWAP(0,2); /* (2) = (0,0) */
      epr_misc_preload = 1;
      
      ss->theta += stheta;
      ss->amp = sin(ss->theta);
      npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
      epr_powder_evals++;
      SWAP(0,3); /* (3) = (0,stheta) */
      
      /* (2,3) = prev. points, (0,1) = new points */
      ss->amp = sin(ss->theta);
      for (phi = epr_phi_begin; phi < epr_phi_end; phi += sphi) {
	
	ss->phi = phi + sphi;
	ss->theta = theta;
	npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
	epr_powder_evals++;
	SWAP(0,1); /* (1) = (sphi,0) */
	
	ss->theta += stheta;
	if(epr_hamilton_lfollow) epr_hamilton_lfollow = 3; /* for next round */
	npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
	if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
	epr_powder_evals++; /* (0) = (sphi,stheta) */
	
	/* (linear) interpolate */
	tmp_int3 = tmp_int2 * tmp_int;
	for (i = 0; i < epr_misc_npeaks[0]; i++) {
	  if(epr_misc_peaks[0][i].pos == 0.0 || epr_misc_peaks[1][i].pos == 0.0
	     || epr_misc_peaks[2][i].pos == 0.0 || epr_misc_peaks[3][i].pos == 0.0) {
	    if(elaska) {
	      fprintf(stderr,"libepr: warning - loosing peak info.\n");
	      elaska = 0;
	    }
	    continue;
	  }
	  for (t = 0.0; t < 1.0; t += sst) /* "theta dir" */
	    for (u = 0.0; u < 1.0; u += ssp) { /* "phi dir" */
	      m1 = (1.0 - t) * (1.0 - u); /* (2) */
	      m2 = t * (1.0 - u);         /* (3) */
	      m3 = t * u;                 /* (0) */
	      m4 = (1.0 - t) * u;         /* (1) */
	      pos = m1 * epr_misc_peaks[2][i].pos + m2 * epr_misc_peaks[3][i].pos 
		+ m3 * epr_misc_peaks[0][i].pos + m4 * epr_misc_peaks[1][i].pos;
	      amp = m1 * epr_misc_peaks[2][i].intensity + m2 * epr_misc_peaks[3][i].intensity
		+ m3 * epr_misc_peaks[0][i].intensity + m4 * epr_misc_peaks[1][i].intensity;
	      amp *= tmp_int3;
	      j = EPR_XTOI(sp,pos,xstep);
	      if(j >= 0 && j < sp->samples)
		sp->y[j] += amp;
	    }
	}
	SWAP(2,1); /* (0,0) - (sphi,0) */
	SWAP(3,0); /* (0,stheta) - (sphi,stheta) */
	/* if field dir. dep. lw then we must do ls generation here. */
	/* actually we assume that lw is constant in one interpolation square*/
	if(model == EPR_LS_DDEP && sm < EPR_SM_EXACT_ENDOR) {
	  epr_tensor_dir(ll, phi, theta);
	  tmp2 = ss->lw;
	  ss->lw = ss->lw * ll[0] * ll[0] + ss->lwy *
	    ll[1] * ll[1] + ss->lwz * ll[2] * ll[2];
	  epr_line_create_fft(sp, ss, epr_leave_fft);
	  ss->lw = tmp2;
	  for(i = 0; i < sp->samples; i++)
	    tmp_sp.y[i] += sp->y[i];
	  bzero(sp->y, sizeof(double) * sp->samples);
	}
      }
    }
    if(igloo) {
      if(theta < M_PI/2.0) k++; /* if theta goes over [0,pi] - the IGLOO must be corrected */
      else k--;
    }
  }
  if(model == EPR_LS_DDEP) bcopy(tmp_sp.y, sp->y, sizeof(double) * sp->samples);
  ss->amp = tmp_int4;
  epr_misc_preload = 0;
  epr_misc_noupd = 0;
  
  return npeaks;
}

/*
 * Function: int epr_powder_gauss(epr_type *, epr_sim_set *, int, int, void *, int, int, int, int, int)
 *
 * Calculate powder spectrum (2D) using Gauss-X quadrature using
 * (X = Legendre or Chebysev). 
 *
 * sp   = Target spectrum [input/ouput, epr_type *]
 * ss   = Simulation sets [input, epr_sim_set *]
 * sm   = Simulation model [input, int]. See epr_misc_create_sp().
 * model= Lineshape model [input, int]. See epr_misc_create_sp().
 * fltmp= Temp - See epr_misc_create_sp() [input, void *].
 * ham  = Spin Hamiltonian type [input, int].
 * tran = Transition moment type [input, int].
 * igloo= 1 = Implement IGLOO partitioning, 0 = regular partitioning [input, int].
 * type = Weighting scheme:
 *        0 = Legendre, 1 = Chebysev [input, int].
 * npt  = Number of integration points for one element [input, int]
 * 
 * Returns the number of peaks generated (int).
 *
 * NOTE: 1) phi must be in [0, 2Pi] and theta in [0, Pi].
 *       2) model must not be powder!
 *
 */

int epr_powder_gauss(epr_type *sp, epr_sim_set *ss, int sm, int model, void *fltmp, int ham, int tran, int igloo, int type, int npt) {

  int npeaks = 0, i, j, k;
  double theta, phi, stheta, sphi, tmp_int, tmp_int2, tmp_int3, tmp, tmp2;
  double weight[128], x[128];

  if(epr_powder_weights(weight, x, type, npt) == -1) return -1;
  epr_powder_evals = 0;
  epr_misc_preload = 0;
  if(epr_theta < 2) stheta = 1.0;
  else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
  tmp = fabs(epr_phi_end - epr_phi_begin);
  if(epr_phi < 2) sphi = 1.0;
  else sphi = tmp / (double) (epr_phi - 1);
  tmp_int = ss->amp;
  tmp_int2 = stheta / 2.0;
  tmp_int3 = sphi / 2.0;
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
  for (k = 1, theta = epr_theta_begin; theta < epr_theta_end; theta += stheta) {
    if(igloo) {
      sphi = tmp / (double) k;
      tmp_int3 = sphi / 2.0;
    }
    if(sin(theta) > EPR_EPS) {
      for (phi = epr_phi_begin; phi < epr_phi_end; phi += sphi) {
	for (i = 0; i < npt; i++) {
	  for (j = 0; j < npt; j++) {
	    ss->phi = phi + sphi * x[j];
	    ss->theta = theta + stheta * x[i]; /* x scaled on [0,1] */
	    ss->amp = tmp_int * sin(ss->theta) * weight[i] * weight[j] * tmp_int2 * tmp_int3;
	    npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
	    epr_misc_preload = 1;
	    epr_powder_evals++;
	  }
	}
      }
    }
    if(igloo) {
      if(theta < M_PI/2.0) k++; /* if theta goes over [0,pi] - the IGLOO must be corrected */
      else k--;
    }
  }
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
  ss->amp = tmp_int;
  epr_misc_preload = 0;

  return npeaks;
}

/*
 * Function: int epr_powder_simpson(epr_type *, epr_sim_set *, int, int, void *, int, int)
 *
 * Calculate powder spectrum (2D) using combined the Simpson's rule.
 *
 * sp   = Target spectrum [input/ouput, epr_type *]
 * ss   = Simulation sets [input, epr_sim_set *]
 * sm   = Simulation model [input, int]. See epr_misc_create_sp().
 * model= Lineshape model [input, int]. See epr_misc_create_sp().
 * fltmp= Temp - See epr_misc_create_sp() [input, void *].
 * ham  = Spin Hamiltonian type [input, int].
 * tran = Transition moment type [input, int].
 * 
 * Returns the number of peaks generated (int).
 *
 * NOTE: 1) This routine does not implement IGLOO partitioning.
 *       2) phi must be in [0, 2Pi] and theta in [0, Pi].
 *       3) model must not be powder!
 *
 * Convergence: O(1/n^4).
 *
 */

int epr_powder_simpson(epr_type *sp, epr_sim_set *ss, int sm, int model, void *fltmp, int ham, int tran) {

  int npeaks = 0, i, j;
  double theta, phi, stheta, sphi, tmp_int, co, tmp_int2;
  extern int epr_hamilton_lfollow;

  epr_powder_evals = 0;
  epr_misc_preload = 0;
  if(epr_theta < 2) stheta = 1.0;
  else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
  if(epr_phi < 2) sphi = 1.0;
  else sphi = fabs(epr_phi_end - epr_phi_begin) / (double) (epr_phi - 1);
  co = stheta * sphi;
  tmp_int = ss->amp * co;
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
  for (i = 0, theta = epr_theta_begin; theta <= epr_theta_end; theta += stheta, i++) {
    ss->amp = tmp_int * sin(theta);
    if(ss->amp < EPR_EPS) continue;
    if(i == 0 || i == epr_theta) ss->amp *= 1.0 / 3.0;
    else if(i % 2) ss->amp *= 4.0 / 3.0;
    else ss->amp *= 2.0 / 3.0;
    for (j = 0, phi = epr_phi_begin; phi <= epr_phi_end; phi += sphi, j++) {
      ss->theta = theta;
      ss->phi = phi;
      tmp_int2 = ss->amp;
      if(j == 0 || j == epr_theta) ss->amp *= 1.0 / 3.0;
      else if(j % 2) ss->amp *= 4.0 / 3.0;
      else ss->amp *= 2.0 / 3.0;
      npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
      epr_misc_preload = 1;
      epr_powder_evals++;
      ss->amp = tmp_int2;
    }
  }
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
  epr_misc_preload = 0;
  ss->amp = tmp_int / co;

  return npeaks;
}


/*
 * Function: int epr_powder_trasim(epr_type *, epr_sim_set *, int, int, void *, int, int)
 *
 * Calculate powder spectrum (2D) using combined trapezoidal & Simpson rule.
 *
 * sp   = Target spectrum [input/ouput, epr_type *]
 * ss   = Simulation sets [input, epr_sim_set *]
 * sm   = Simulation model [input, int]. See epr_misc_create_sp().
 * model= Lineshape model [input, int]. See epr_misc_create_sp().
 * fltmp= Temp - See epr_misc_create_sp() [input, void *].
 * ham  = Spin Hamiltonian type [input, int].
 * tran = Transition moment type [input, int].
 * 
 * Returns the number of peaks generated (int).
 *
 * NOTE: 1) This routine does not implement IGLOO partitioning.
 *       2) phi must be in [0, 2Pi] and theta in [0, Pi].
 *       3) model must not be powder!
 *
 * Convergence: O(1/n^3).
 *
 */

int epr_powder_trasim(epr_type *sp, epr_sim_set *ss, int sm, int model, void *fltmp, int ham, int tran) {

  int npeaks = 0, i, j;
  double theta, phi, stheta, sphi, tmp_int, co, tmp_int2;
  extern int epr_hamilton_lfollow;

  epr_powder_evals = 0;
  epr_misc_preload = 0;
  if(epr_theta < 2) stheta = 1.0;
  else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
  if(epr_phi < 2) sphi = 1.0;
  else sphi = fabs(epr_phi_end - epr_phi_begin) / (double) (epr_phi - 1);
  co = stheta * sphi;
  tmp_int = ss->amp * co;
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
  for (i = 0, theta = epr_theta_begin; theta <= epr_theta_end; theta += stheta, i++) {
    ss->amp = tmp_int * sin(theta);
    if(ss->amp < EPR_EPS) continue;
    if(i == 0 || i == epr_theta) ss->amp *= 5.0 / 12.0;
    else if(i == 1 || i == epr_theta-1) ss->amp *= 13.0 / 12.0;
    for (j = 0, phi = epr_phi_begin; phi <= epr_phi_end; phi += sphi, j++) {
      ss->theta = theta;
      ss->phi = phi;
      tmp_int2 = ss->amp;
      if(j == 0 || j == epr_phi) ss->amp *= 5.0 / 12.0;
      else if(j == 1 || j == epr_phi-1) ss->amp *= 13.0 / 12.0;
      npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
      epr_misc_preload = 1;
      epr_powder_evals++;
      ss->amp = tmp_int2;
    }
  }
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
  epr_misc_preload = 0;
  ss->amp = tmp_int / co;

  return npeaks;
}

/*
 * Function: int epr_powder_trapez(epr_type *, epr_sim_set *, int, int, void *, int, int)
 *
 * Calculate powder spectrum using the Trapezoidal rule (2D).
 *
 * sp   = Target spectrum [input/ouput, epr_type *]
 * ss   = Simulation sets [input, epr_sim_set *]
 * sm   = Simulation model [input, int]. See epr_misc_create_sp().
 * model= Lineshape model [input, int]. See epr_misc_create_sp().
 * fltmp= Temp - See epr_misc_create_sp() [input, void *].
 * ham  = Spin Hamiltonian type [input, int].
 * tran = Transition moment type [input, int].
 * 
 * Returns the number of peaks generated (int).
 *
 * NOTE: 1) This routine does not implement IGLOO partitioning.
 *       2) phi must be in [0, 2Pi] and theta in [0, Pi].
 *       3) model must not be powder!
 *
 * Convergence: O(1/n^2).
 *
 */

int epr_powder_trapez(epr_type *sp, epr_sim_set *ss, int sm, int model, void *fltmp, int ham, int tran) {

  int npeaks = 0;
  double theta, phi, stheta, sphi, tmp_int, co, tmp_int2;
  extern int epr_hamilton_lfollow;

  epr_powder_evals = 0;
  epr_misc_preload = 0;
  if(epr_theta < 2) stheta = 1.0;
  else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
  if(epr_phi < 2) sphi = 1.0;
  else sphi = fabs(epr_phi_end - epr_phi_begin) / (double) (epr_phi - 1);
  co = stheta * sphi;
  tmp_int = ss->amp * co;
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
  for (theta = epr_theta_begin; theta <= epr_theta_end; theta += stheta) {
    ss->amp = tmp_int * sin(theta);
    if(ss->amp < EPR_EPS) continue;
    if(theta == epr_theta_begin || theta == epr_theta_end) ss->amp *= 0.5;
    for (phi = epr_phi_begin; phi <= epr_phi_end; phi += sphi) {
      ss->theta = theta;
      ss->phi = phi;
      tmp_int2 = ss->amp;
      if(phi == epr_phi_begin || phi == epr_phi_end) ss->amp *= 0.5;
      npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
      epr_misc_preload = 1;
      epr_powder_evals++;
      ss->amp = tmp_int2;
    }
  }
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
  epr_misc_preload = 0;
  ss->amp = tmp_int / co;
  return npeaks;
}

/*
 * Function: int epr_powder_axial(epr_type *, epr_sim_set *, int, int, void *, int, int)
 *
 * Calculate powder spectrum for axial symmetry case (ie. 1D).
 *
 * sp   = Target spectrum [input/ouput, epr_type *]
 * ss   = Simulation sets [input, epr_sim_set *]
 * sm   = Simulation model [input, int]. See epr_misc_create_sp().
 * model= Lineshape model [input, int]. See epr_misc_create_sp().
 * fltmp= Temp - See epr_misc_create_sp() [input, void *].
 * ham  = Spin Hamiltonian type [input, int].
 * tran = Transition moment type [input, int].
 * 
 * Returns the number of peaks generated (int).
 *
 * NOTE: Phi is held constant and theta is integrated from (beg->end).
 *
 */
 
int epr_powder_axial(epr_type *sp, epr_sim_set *ss, int sm, int model, void *fltmp, int ham, int tran) {

  int npeaks = 0;
  double theta, stheta, tmp_int;
  extern int epr_hamilton_lfollow, epr_misc_noupd;

  epr_misc_preload = 0;
  epr_powder_evals = 0;
  epr_misc_noupd = 0;
  if(epr_theta < 2) stheta = 1.0;
  else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
  ss->phi = epr_phi_begin;
  tmp_int = ss->amp * stheta;
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
  for (theta = epr_theta_begin; theta <= epr_theta_end; theta += stheta) {
    ss->amp = tmp_int * sin(theta);
    if(ss->amp < EPR_EPS) continue;
    ss->theta = theta;
    npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
    epr_misc_preload = 1;
    epr_powder_evals++;
  }
  if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
  ss->amp = tmp_int / stheta;
  epr_misc_preload = 0;
  return npeaks;
}

/*
 * Function: int epr_powder_axial_interp(epr_type *, epr_sim_set *, int, int, void *, int, int)
 *
 * Calculate powder spectrum for axial symmetry case (i.e. 1D) using
 * linear interpolation.
 *
 * sp   = Target spectrum [input/ouput, epr_type *]
 * ss   = Simulation sets [input, epr_sim_set *]
 * sm   = Simulation model [input, int]. See epr_misc_create_sp().
 * model= Lineshape model [input, int]. See epr_misc_create_sp().
 * fltmp= Temp - See epr_misc_create_sp() [input, void *].
 * ham  = Spin Hamiltonian type [input, int].
 * tran = Transition moment type [input, int].
 * 
 * Returns the number of peaks generated (int).
 *
 * NOTE: Phi is held constant and theta is integrated from (beg->end).
 *       epr_powder_fm defines the number of interpolation steps.
 *
 */
 
int epr_powder_axial_interp(epr_type *sp, epr_sim_set *ss, int sm, int model, void *fltmp, int ham, int tran) {

  int npeaks = 0, i, j;
  double theta, stheta, tmp_int, xstep, sst, t, pos, amp, tmp_int2;
  extern int epr_hamilton_lfollow;
  extern epr_peak *epr_misc_peaks[];
  extern int epr_misc_npeaks[], epr_misc_noupd;
  epr_peak *tmpp;
  double ll[3], tmp2;

  epr_misc_preload = 0;
  epr_powder_evals = 0;
  epr_misc_noupd = 1;
  xstep = EPR_XSTEP(sp);
  if(epr_theta < 2) stheta = 1.0;
  else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
  sst = stheta / ((double) (epr_powder_fm) - 1);
  ss->phi = epr_phi_begin;
  tmp_int2 = ss->amp;
  tmp_int = ss->amp * stheta / (double) epr_powder_fm;

  if(model == EPR_LS_DDEP) {
    tmp_sp.samples = sp->samples;
    tmp_sp.frequency = sp->frequency;
    tmp_sp.xbeg = sp->xbeg;
    tmp_sp.xend = sp->xend;
    tmp_sp.xunits = sp->xunits;
    tmp_sp.endor_b = sp->endor_b;
    bzero(tmp_sp.y, sizeof(double) * sp->samples);
  }

  for (theta = epr_theta_begin; theta < epr_theta_end; theta += stheta) {
    ss->amp = sin(theta);
    if(ss->amp < EPR_EPS) continue;
    if(!epr_powder_evals) {
      ss->theta = theta;
      if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
      npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
      if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
      epr_powder_evals++;
      epr_misc_preload = 1;
      SWAP(0,1);
    }
    ss->theta = theta + stheta;
    ss->amp = sin(ss->theta);
    if(epr_hamilton_lfollow) epr_hamilton_lfollow = 3;
    npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
    if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
    epr_powder_evals++;
    /* interpolate 0 <-> 1 */
    for (i = 0; i < epr_misc_npeaks[0]; i++) {
      /* if out of range then skip */
      if(epr_misc_peaks[0][i].pos == 0.0 || epr_misc_peaks[1][i].pos == 0.0)
	continue;
      for (t = 0.0; t < 1.0; t += sst) {
	pos = (1.0 - t) * epr_misc_peaks[0][i].pos + t * epr_misc_peaks[1][i].pos;
	amp = (1.0 - t) * epr_misc_peaks[0][i].intensity + t * epr_misc_peaks[1][i].intensity;
	amp *= tmp_int;
	j = EPR_XTOI(sp,pos,xstep);
	if(j >= 0 && j < sp->samples)
	  sp->y[j] += amp;
      }
    }
    SWAP(0,1);
    /* if field dir. dep. lw then we must do ls generation here. */
    /* actually we assume that lw is constant in one interpolation square*/
    if(model == EPR_LS_DDEP && sm < EPR_SM_EXACT_ENDOR) {
      epr_tensor_dir(ll, ss->phi, theta);
      tmp2 = ss->lw;
      ss->lw = ss->lw * ll[0] * ll[0] + ss->lwy *
	ll[1] * ll[1] + ss->lwz * ll[2] * ll[2];
      epr_line_create_fft(sp, ss, epr_leave_fft);
      ss->lw = tmp2;
      for(i = 0; i < sp->samples; i++)
	tmp_sp.y[i] += sp->y[i];
      bzero(sp->y, sizeof(double) * sp->samples);
    }
  }
  if(model == EPR_LS_DDEP) bcopy(tmp_sp.y, sp->y, sizeof(double) * sp->samples);
  ss->amp = tmp_int2;
  epr_misc_preload = 0;
  epr_misc_noupd = 0;
  return npeaks;
}

/*
 * Function: int epr_powder_axial_spl(epr_type *, epr_sim_set *, int, int, void *, int, int)
 *
 * Calculate powder spectrum for axial symmetry case (i.e. 1D) using
 * cubic spline interpolation.
 *
 * sp    = Target spectrum [input/output; epr_type *]
 * ss    = Simulation sets [input, epr_sim_set *]
 * sm    = Simulation model [input, int]
 * model = Lineshape model [input, int]
 * fltmp = Temp - See epr_misc_create_sp() [inputm void *]
 * ham   = Spin Hamiltonian type [input, int]
 * tran  = Transition moment type [input, int]
 *
 * Returns the number of peaks generated (int).
 *
 * Note: phi is held constant while theta is used for integration.
 *
 */

int epr_powder_axial_spl(epr_type *sp, epr_sim_set *ss, int sm, int model, void *fltmp, int ham, int tran, int nip) {

  int npeaks = 0, i, j, nrp;
  double theta, phi, stheta, sphi, tmp_int, tmp, t, u, st_theta, st_phi;
  double cpos, camp, xstep, tmp_int2, sst;
  extern epr_peak *epr_misc_peaks[];
  extern int epr_misc_npeaks[], epr_misc_noupd;
  epr_peak *tmpp;
  double xval1[EPR_MAXNS], xval2[EPR_MAXNS];
  static double *pos_deriv[EPR_MAXNS], *amp_deriv[EPR_MAXNS], *pos[EPR_MAXNS], *amp[EPR_MAXNS];
  extern int epr_hamilton_lfollow;
  double ll[3], tmp2;

  if(!inited) {
    for (i = 0; i < EPR_MAXNS; i++) {
      pos[i] = malloc(sizeof(double) * EPR_MAXNS);
      pos_deriv[i] = malloc(sizeof(double) * EPR_MAXNS);
      amp[i] = malloc(sizeof(double) * EPR_MAXNS);
      amp_deriv[i] = malloc(sizeof(double) * EPR_MAXNS);
    }
    inited = 1;
  }
  epr_powder_evals = 0;
  epr_misc_preload = 0;
  epr_misc_noupd = 1;
  if(epr_theta < 2) stheta = 1.0;
  else stheta = fabs(epr_theta_end - epr_theta_begin) / (double) (epr_theta - 1);
  sst = stheta / ((double) (epr_powder_fm) - 1);
  st_theta = stheta / ((double) nip - 1);
  xstep = EPR_XSTEP(sp);
  ss->phi = epr_phi_begin;
  tmp_int2 = ss->amp;
  tmp_int = ss->amp * stheta / (double) epr_powder_fm;

  if(model == EPR_LS_DDEP) {
    tmp_sp.samples = sp->samples;
    tmp_sp.frequency = sp->frequency;
    tmp_sp.xbeg = sp->xbeg;
    tmp_sp.xend = sp->xend;
    tmp_sp.xunits = sp->xunits;
    tmp_sp.endor_b = sp->endor_b;
    bzero(tmp_sp.y, sizeof(double) * sp->samples);
  }

  for (theta = epr_theta_begin; theta <= epr_theta_end; theta += stheta) {
    if(sin(theta) > EPR_EPS) {
      /* calculate function at the given points */
      ss->theta = theta;
      for (i = 0; i < nip; i++) {
	ss->amp = sin(ss->theta);
	if(!i && epr_hamilton_lfollow) epr_hamilton_lfollow = 2; /* fix order */
	npeaks += epr_misc_create_sp(sp, ss, 1, sm, model, fltmp, ham, tran, 0, 0);
	if(!i && epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
	SWAP(0,i+1);
	epr_misc_preload = 1;
	epr_powder_evals++;
	xval1[i] = ss->theta;
	ss->theta += st_theta;
      }

      /* calculate cubic spline */
      for (i = 0; i < epr_misc_npeaks[0]; i++) {
	/* skip if out of range */
	for (j = 1; j < nip; j++)
	  if(epr_misc_peaks[j][i].pos == 0.0) break;
	if(j != nip) continue; /* out of range */
	
	/* calculate spline (for pos & amp) */
	for (j = 1; j <= nip; j++) {/* j = 1 - see SWAP above */
	  pos[0][j-1] = epr_misc_peaks[j][i].pos;
	  amp[0][j-1] = epr_misc_peaks[j][i].intensity;
	}
	/* 2e30 -> normal spline boundary condition */
	epr_base_splin1(xval1, pos[0], nip, 2e30, 2e30, pos_deriv[0]);
	epr_base_splin1(xval1, amp[0], nip, 2e30, 2e30, amp_deriv[0]);
	
	/* interpolate & integrate through the inner region */
	for (t = 0.0; t < 1.0; t += sst) {
	  cpos = epr_base_splie1(xval1, pos[0], pos_deriv[0], nip, theta+t*stheta);
	  camp = epr_base_splie1(xval1, amp[0], amp_deriv[0], nip, theta+t*stheta);
	  camp *= tmp_int;
	  j = EPR_XTOI(sp,cpos,xstep);
	  if(j >= 0 && j < sp->samples)
	    sp->y[j] += camp;
	}
      }
      /* if field dir. dep. lw then we must do ls generation here. */
      /* actually we assume that lw is constant in one interpolation square*/
      if(model == EPR_LS_DDEP && sm < EPR_SM_EXACT_ENDOR) {
	epr_tensor_dir(ll, ss->phi, theta);
	tmp2 = ss->lw;
	ss->lw = ss->lw * ll[0] * ll[0] + ss->lwy *
	  ll[1] * ll[1] + ss->lwz * ll[2] * ll[2];
	epr_line_create_fft(sp, ss, epr_leave_fft);
	ss->lw = tmp2;
	for(i = 0; i < sp->samples; i++)
	  tmp_sp.y[i] += sp->y[i];
	bzero(sp->y, sizeof(double) * sp->samples);
      }    
    }
  }
  if(model == EPR_LS_DDEP) bcopy(tmp_sp.y, sp->y, sizeof(double) * sp->samples);
  ss->amp = tmp_int2;
  epr_misc_preload = 0;
  epr_misc_noupd = 0;
  
  return npeaks;
}
