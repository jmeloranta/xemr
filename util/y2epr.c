/*
 * Convert "Y-file" (the only column is intensity) to epr file.
 *
 * Usage: y2epr sfield srange yfile epr-file
 * centerg = g-value at center of the spectrum
 * rangeg  = scan range
 *
 * (note that epr-file is only *basename*, since this format has three
 *  different files: .spe, .spx and .spy).
 *
 * Also this format does not supply all the parameters for the spetrum,
 * so this data has to be entered by hand
 *
 * Last modified: Oct-13-2009 jussi.eloranta@csun.edu
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
  int i;
  
  if(argc != 6) {
    fprintf(stderr, "Usage: y2epr mw-freq B-beg B-end yfile eprfile\n");
    exit(1);
  }
  
  
  if(!(fp = fopen(argv[4], "r"))) {
    fprintf(stderr, "Can't open yfile: %s\n", argv[4]);
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
  for (i = 0; i < EPR_MAX_SAMPLES; i++)
    if(fscanf(fp, " %lf", sp->y + i) != 1) break;
  fclose(fp);
  sp->samples = i;

  strcpy(sp->fname, argv[5]);
  
  if(!epr_file_write(sp)) {
    fprintf(stderr, "Error writing epr file.\n");
    exit(1);
  }
  return 0;
}
