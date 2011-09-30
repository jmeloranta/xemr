/*
 * functions.
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

/*
 * Function: int epr_simul_stick(epr_type *sp, epr_sim_set *, epr_peak *)
 *
 * Purpose: Construct 1st order stick spectrum.
 *
 * sp   = Target spectrum [input; epr_type *]
 * ss   = Pointer to epr_sim_set [input; epr_sim_set *]
 * peaks= Peak array [output; epr_peak *]
 *
 * Returns number of peaks generated.
 *
 */

#define NSPIN 12
#define NBR   12
#define TSPIN (NSPIN * NBR + 1)

static int iarray[NSPIN][NBR][TSPIN];

/* This was borrowed from eprft by B. Kirste */

static void epr_simul_int() {

  double spin, cur_int, pt[TSPIN];
  int i, j, k, l, m, n, tot_spin, nbr, idx;

  /* S = 0.5 */
  for (nbr = 0; nbr < NBR; nbr++) {
    tot_spin = nbr + 2;
    for (i = 0; i < tot_spin; i++) {
      cur_int = 1.0;
      for (k = 1; k <= i; k++)
	cur_int *= (((double) tot_spin) - ((double) k)) / ((double) k);
      iarray[0][nbr][i] = (int) cur_int;
    }
  }
  /* S = 1.0 */
  for (nbr = 0; nbr < NBR; nbr++) {
    for (i = 1; i < TSPIN; i++)
      iarray[1][nbr][i] = 0;
    iarray[1][nbr][0] = 1;
    for (i = 1; i <= nbr+1; i++) {
      j = 2*i;
      for (k = j; k >= i; k--) {
	iarray[1][nbr][k] += iarray[1][nbr][k-1];
	if(k > 1)
	  iarray[1][nbr][k] += iarray[1][nbr][k-2];
      }
      for (k = 0; k < i; k++)
	iarray[1][nbr][k] = iarray[1][nbr][j - k];
    }
  }
  /* S > 1.0 */
  for (spin = 1.5; spin <= (double) (NSPIN/2); spin += 0.5) {
    for (nbr = 0; nbr < NBR; nbr++) {
      tot_spin = ((int) (2.0 * spin)) * (nbr + 1) + 1;
      idx = ((int) (2.0 * spin)) - 1;
      for (i = 0; i < tot_spin; i++)
	iarray[idx][nbr][i] = 0;
      j = (int) (2.0*spin);
      k = j + 1;
      for (i = 0; i < k; i++)
	iarray[idx][nbr][i] = 1;
      for (i = 2; i <= nbr+1; i++) {
	k += j;
	for (l = 1; l < k; l++) {
	  cur_int = 0.0;
	  for (m = 0; m <= j; m++) {
	    n = l - m;
	    if(n >= 0) cur_int += iarray[idx][nbr][n];
	  }
	  pt[l] = cur_int;
	}
	for (l = 1; l < k; l++)
	  iarray[idx][nbr][l] = pt[l];
      }
    }
  }
}

static int inited = 0;
static double hfc[EPR_MAXGROUPS];

int epr_simul_stick(epr_type *sp, epr_sim_set *ss, epr_peak *peaks) {

  int i, j, k, l;
  double *spin, posn, lw;
  double intensity, *y, step, nlw, norm;
  int *nbr, ngr, len, idx, stoi;
  int cur_spin[256], max_spin[256], npeaks;
  double wd, l_dir[3];
  
  if(!inited) {
    epr_simul_int();
    inited = 1;
  }
  y = sp->y;
  len = sp->samples;
  step = EPR_XSTEP(sp);
  ngr = ss->ngr;
  nbr = ss->nbr;
  spin = ss->spin;
  /* Effective field approach */
  epr_tensor_dir(l_dir, ss->phi + ss->o_phi, ss->theta + ss->o_theta);
  posn = epr_tensor_project(ss->posn[0], l_dir);
  /* All couplings are in MHz */
  for (i = 0; i < ngr; i++)
    hfc[i] = epr_tensor_project2(ss->posn[0], ss->hfc[0][i], l_dir)
      * 0.3568250 * EPR_GE / posn; /* to Gauss */
  posn = EPR_G2M(posn,sp->frequency);
  npeaks = 1;
  wd = 0.0;
  norm = 1.0;
  for (j = 0; j < ngr; j++) {
    cur_spin[j] = 0;
    max_spin[j] = ((int) (2.0 * nbr[j] * spin[j])) + 1;
    npeaks *= max_spin[j];
    wd += 2.0 * spin[j] * nbr[j] * hfc[j];
    norm *= pow(2.0 * spin[j] + 1.0, (double) nbr[j]);
  }
  intensity = ss->amp / (double) norm; /* normalize */
  posn -= wd / 2.0; /* to the first peak */
  npeaks = 0;

  do {

    for (k = 0; k < ngr; k++)
      peaks[npeaks].mi[k] = cur_spin[k] - spin[k] * nbr[k];
    peaks[npeaks].pos = posn;
    peaks[npeaks].intensity = intensity;
    npeaks++;
    
    for (j = 0; j < ngr; j++) {
      if(++cur_spin[j] < max_spin[j]) {
	posn += hfc[j];
	i = ((int) (2.0*spin[j])) - 1;
	k = nbr[j] - 1;
	l = cur_spin[j];
	intensity *= ((double) iarray[i][k][l]) / (double) iarray[i][k][l-1];
	break;
      }
      posn -= hfc[j] * (((double) max_spin[j]) - 1.0);
      cur_spin[j] = 0;
    }
  } while (j != ngr);

  return npeaks;
}

/*
 * Simple stick ENDOR simulation according to the 1st order.
 *
 * sp    = Target spectrum [epr_type *, input]
 * ss    = Simulation set [epr_sim_set *, input]
 * peaks = Peak array [epr_peak *, output]
 *
 * Returns the number of peaks generated.
 *
 * NOTE: Isotropic only !
 *
 */

int epr_simul_stick_endor(epr_type *sp, epr_sim_set *ss, epr_peak *peaks) {

  double *y, *amp, cf, pos, hfc, offset;
  int i, len, ngr, npeaks;

  y = sp->y;
  len = sp->samples;
  ngr = ss->ngr;
  npeaks = 0;
  amp = ss->e_iamp;
  offset = ss->e_offset; /* in MHz */
  /* TODO: do some unit checking ! */
  for (i = 0; i < ngr; i++) {
    cf = 1.0E-6*epr_atoms[ss->atoms[i]].gn * EPR_GAUSS_BN * sp->endor_b / EPR_H
      + offset;
    hfc = fabs(ss->hfc[0][i][0]);
    if(cf > hfc/2.0) {
      peaks[npeaks].pos = cf + hfc/2.0;
      peaks[npeaks].intensity = amp[npeaks];
      npeaks++;
      peaks[npeaks].pos = cf - hfc/2.0;
      peaks[npeaks].intensity = amp[npeaks];
      npeaks++;
    } else {
      peaks[npeaks].pos = hfc/2.0 + cf;
      peaks[npeaks].intensity = amp[npeaks];
      npeaks++;
      peaks[npeaks].pos = hfc/2.0 - cf;
      peaks[npeaks].intensity = amp[npeaks];
      npeaks++;
    }
  }
  return npeaks;
}
