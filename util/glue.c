/*
 * Join given spectra to yield an overall sum spectrun.
 *
 * Usage: glue spectrum1 spectrum2 ...
 *
 * Last modified: Oct-14-2009 jussi.eloranta@csun.edu
 *
 * Written by:
 * Jussi Eloranta (jussi.eloranta@csun.edu)
 *
 * Department of chemistry and biochemistry
 * California State University at Northridge
 *
 */

#include <stdio.h>
#include <math.h>
#include "epr.h"

#define MAXSP 512

int main(int argc, char **argv) {

  epr_type *sp[MAXSP];
  epr_type sp_new;
  double tmp, step, step2;
  int i, j, k, jini, tmp3;

  if(argc == 1) {
    fprintf(stderr, "Usage: glue espfile1 espfile2 ...\n");
    exit(1);
  }

  /* read in */
  for (i = 1; i < argc; i++) {
    fprintf(stderr, "Reading spectrum %s.\n", argv[i]);
    sp[i-1] = epr_file_read(argv[i]);    
  }
  step = fabs(sp[0]->xend - sp[0]->xbeg) / (double) sp[0]->samples;
  
  /* sanity checks */
  for (i = 2; i < argc; i++) {
    if(sp[0]->samples != sp[i-1]->samples) {
      fprintf(stderr, "Warning: Wrong number of samples in %s (adjusting to %d).\n", argv[i], sp[0]->samples);
      sp[i-1] = epr_misc_change_resolution(sp[i-1], sp[0]->samples); /* TODO: free the old spectrum */
    }
    if(sp[0]->gain != sp[i-1]->gain) fprintf(stderr, "Warning: different gain coefficients.\n");
  }

  /* Correct x-axis for frequency fluctuations and gain */
  /* note that the frequency must appear correctly in the spe file + field corrected */
  for (i = 2; i < argc; i++) {
    sp[i-1]->xbeg *= (sp[0]->frequency / sp[i-1]->frequency);
    sp[i-1]->xend *= (sp[0]->frequency / sp[i-1]->frequency);
    tmp = sp[0]->gain / sp[i-1]->gain;
    for (j = 0; j < sp[i-1]->samples; j++)
      sp[i-1]->y[j] *= tmp;
  }

  /* use linear interpolation to make the spectral (mag. field) step size equal to sp[0] */
  for (i = 2; i < argc; i++) {
    step2 = fabs(sp[i-1]->xend - sp[i-1]->xbeg) / (double) sp[i-1]->samples;
    sp[i-1]->samples = (int) (fabs(sp[i-1]->xend - sp[i-1]->xbeg) / step);
    sp[i-1]->xend = sp[i-1]->xbeg + step * sp[i-1]->samples;
    bcopy(sp[i-1]->y, sp_new.y, sizeof(double) * EPR_MAX_SAMPLES);
    tmp3 = sp[i-1]->samples;
    for (j = 0; j < sp[i-1]->samples; j++) {
      tmp = step * (double) j;
      k = (int) (tmp / step2);
      if(k >= tmp3) k = tmp3 - 1;
      sp[i-1]->y[j] = sp_new.y[k];
    }
  }

  /* construct new spectrum */
  epr_misc_copy(sp[0], &sp_new);
  sp_new.xbeg = sp[0]->xbeg;
  sp_new.xend = sp[argc-2]->xend;
  sp_new.samples = 1 + (int) (fabs(sp_new.xend - sp_new.xbeg) / step);
  bzero(sp_new.y, sizeof(double) * EPR_MAX_SAMPLES);
  for (i = 1; i < argc; i++) {
    jini = (int) (fabs(sp[i-1]->xbeg - sp_new.xbeg) / step);
    for (j = jini; j < jini + sp[i-1]->samples; j++)
      sp_new.y[j] += sp[i-1]->y[j - jini];
  }

  /* write new spectrum */
  strcpy(sp_new.fname, "glue");
  epr_file_write(&sp_new);

  return 0;
}
