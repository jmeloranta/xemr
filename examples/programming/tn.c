/*
 * An example of using Xemr's programming interface (LIBEPR).
 *
 * This program simulates superhyperfine couplings of atoms
 * doped in Kr or Xe. Correct amounts of isotopic nuclei are
 * included.
 *
 * Jussi Eloranta (22 Dec 1998).
 *
 */

#include <stdio.h>
#include <math.h>
#include <eprlib.h>

/* site information (6 = oct, 12 = subst) */
#define NN 12

/* spectral parameters (hfc of A in MHz! - B is derived from this) */
#define HFC_PER 66.6
#define HFC_PAR 135.8
#define LW 2.0
#define FI 0.0

/* probability threshold */
#define THR 1.0E-3

/* matrix */
/* #define KR */
#define XE

#ifdef KR
/* Kr: spin 9/2 */
#define SA 4.5
#define TA 0.115
#define GA 0.215704
#define NI 1
#endif

#ifdef XE
/* Xe: spin 1/2 */
#define SA 0.5
/* normal */
/* #define TA 0.264 */
/* Morton & Preston */
#define TA 0.37
#define GA 1.55595
/* Xe: spin 3/2 */
#define SB 1.5
/* normal */
/* #define TB 0.212 */
/* Morton & Preston */
#define TB 0.017
#define GB 0.461240
#define NI 2
#endif

MAIN__() { };

double alle(int i, int j) {

  double a = i, b = j, tmp, diff = i - j;
  int k;

  for (k = 2, tmp = 1.0; k <= a; k++)
    tmp *= (double) k;
  a = tmp;

  for (k = 2, tmp = 1.0; k <= b; k++)
    tmp *= (double) k;
  b = tmp;
  
  for (k = 2, tmp = 1.0; k <= diff; k++)
    tmp *= (double) k;
  diff = tmp;

  return (a / (b * diff));
}


main() {

  int m, n;
  double prob, nrm = 0.0, scale;
  sp_type *sp;
  sim_set sim;
  extern int sp_phi, sp_theta;

  sp_phi = 0;
  sp_theta = 40;
#if NI == 2
  scale = GB / GA;
#else
  scale = 0.0;
#endif
  sp = sp_misc_new();
  sp_misc_zero(sp);
  sp_misc_zerosp(sp,0.0,0.0);
  sp->xbeg = 2900.0; sp->xend = 3300.0;
  sp->frequency = 9.4E9;
  sp->samples = 8192;
  strcpy(sp->fname, "test");
  sp_file_write(sp);
  sim.units = 2;
  sim.ngr = NI;
  for(m = 0; m < 9; m++)
    sim.hfc[0][0][m] = sim.hfc[0][1][m] = 0.0;
  sim.hfc[0][0][0] = HFC_PAR; /* A */
  sim.hfc[0][0][4] = HFC_PER;
  sim.hfc[0][0][8] = HFC_PER;
#if NI == 2
  sim.hfc[0][1][0] = HFC_PAR * scale; /* B */
  sim.hfc[0][1][4] = HFC_PER * scale;
  sim.hfc[0][1][8] = HFC_PER * scale;
#endif
  sim.spin[0] = SA;
#if NI == 2
  sim.spin[1] = SB;
#endif
  sim.espin = 0.5;
  for(m = 0; m < 9; m++)
    sim.posn[0][m] = 0.0;
  sim.posn[0][0] = 2.18;
  sim.posn[0][4] = 2.18;
  sim.posn[0][8] = 2.18;
  sim.lw = LW;
  sim.lg_fact = 1.0;
  sim.dlevel = 2;
  sim.phi = 0.0;
  sim.theta = 0.0;
#if NI == 1
  for (m = 0; m <= NN; m++) {
    prob = alle(NN,m) * pow(TA,(double) m) * 
      pow(1.0 - TA, (double) (NN-m));
    printf("P(%d) = %le\n", m,prob);
    nrm += prob;
    sim.amp = prob*100.0;
    sim.nbr[0] = m; /* SA */
    sim.lw = LW + FI*((double) m) / (double) NN;
    if(prob >= THR)
      sp_misc_create_sp(sp, &sim, 1, SM_1STORDER, LS_NORMAL+LS_POWDER, (void *) 0, I_INTERP_AXIAL_SPL, 4);
  }
#else
  for (m = 0; m <= NN; m++)
    for (n = 0; n+m <= NN; n++) {
      prob = alle(NN,n) * alle(NN-n,m) * pow(TA,(double) m) *
	pow(TB, (double) n) * pow(1.0 - TA - TB, (double) (NN-m-n));
      printf("P(%d,%d) = %le\n", m,n,prob);
      nrm += prob;
      sim.amp = prob*100.0;
      sim.nbr[0] = m; /* SA */
      sim.nbr[1] = n; /* SB */
      if(prob >= THR)
	sp_misc_create_sp(sp, &sim, 1, SM_1STORDER, LS_NORMAL+LS_POWDER, (void *) 0, 0, 0, I_INTERP_AXIAL_SPL, 4);
    }
#endif  
  sp_file_write(sp);
  printf("Normalization = %le (should be unity!)\n", nrm);
}
