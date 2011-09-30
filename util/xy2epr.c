/*
 * Convert "XY-file" (first column is the field and second intensity)
 * to epr file
 *
 * Usage: xy2epr xyfile epr-file
 * (note that epr-file is only *basename*, since this format has three
 *  different files: .spe, .spx and .spy).
 *
 * Also this format does not supply all the parameters for the spetrum,
 * so this data has to be entered by hand.
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
#include "epr.h"

MAIN__() {} /* I have no idea why this is needed... */

int main(argc, argv) int argc; char **argv; {

  FILE *fp;
  epr_type *sp;
  double xmin = 9999.9, xmax = -xmin;
  int i;
  double x;

  if(argc != 4) {
    fprintf(stderr, "Usage: xy2epr mw-freq xyfile eprfile\n");
    exit(1);
  }

  if(!(fp = fopen(argv[2], "r"))) {
    fprintf(stderr, "Can't open xyfile: %s\n", argv[2]);
    exit(1);
  }

  if(!(sp = epr_misc_new())) {
    fprintf(stderr, "Memory allocation error.\n");
    exit(1);
  }
  epr_misc_zero(sp);
  sp->frequency = atof(argv[1]);
  for (i = 0; i < EPR_MAX_SAMPLES; i++) {
    if(fscanf(fp, " %lf %lf", &x, sp->y + i) != 2) break;
    printf("%le %le\n", x, sp->y[i]);
    if (x < xmin) xmin = x;
    if (x > xmax) xmax = x;
  }
  fclose(fp);
  sp->samples = i;

  sp->xbeg = xmin;
  sp->xend = xmax;

  strcpy(sp->fname, argv[3]);

  if(!epr_file_write(sp)) {
    fprintf(stderr, "Error writing epr file.\n");
    exit(1);
  }
  return 0;
}
