/*
 * Convert Bruker Aspect file to epr file (only data is read).
 *
 * Usage: aspect2epr centerg rangeg aspectfile epr-file
 * centerg = g-value at the center of the spectrum
 * rangeg  = scan range (in g scale).
 *
 * (note that epr-file is only *basename*, since this format has three
 *  different files: .spe, .spx and .spy).
 *
 * Also this format does not supply all the parameters for the spetrum,
 * so this data has to be entered by hand
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
#include <stdlib.h>
#include <math.h>
#include "epr.h"

MAIN__() {} /* I have no idea why this is needed... */

int main(argc, argv) int argc; char **argv; {

  FILE *fp;
  epr_type *sp;
  register int i;
  int sign;
  unsigned char f[3];
  unsigned int num;

  if(argc != 6) {
    fprintf(stderr, "Usage: aspect2epr mw-freq B-beg B-end aspectfile eprfile\n");
    exit(1);
  }

  if(!(fp = fopen(argv[4], "r"))) {
    fprintf(stderr, "Can't open aspectfile: %s\n", argv[4]);
    exit(1);
  }

  if(!(sp = epr_misc_new())) {
    fprintf(stderr, "Memory allocation error.\n");
    exit(1);
  }
  epr_misc_zero(sp);
  sp->frequency = atof(argv[1]);
  sp->xbeg = atof(argv[2]);
  sp->xend = atof(argv[3]);
  sp->xunits = 0;
  for (i = 0; i < EPR_MAX_SAMPLES; i++) {
    if(!fread(f, 3, 1, fp)) break;
    num = f[0]; num <<= 8;
    num |= f[1]; num <<= 8;
    num |= f[2];
    sign = (f[0]&0x80)?-1:1;
    if(sign == -1) num = ~(0xff000000|num);
    sp->y[i] = (double) sign*num;
  }
  fclose(fp);
  sp->samples = i;
  strcpy(sp->fname, argv[5]);

  if(!epr_file_write(sp)) {
    fprintf(stderr, "Error writing epr file.\n");
    exit(1);
  }
  return 0;
}
