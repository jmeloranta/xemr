/*
 * Convert "our pc format" to epr file.
 *
 * "our pc format" := One file contains MW frequency value
 * and field values (.x file) and the other intensity values (.y file).
 *
 * Usage: pc2epr pcfile epr-file
 * (note that pcfile and epr-file contain only the *basename*)
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
#include "epr.h"

MAIN__() {} /* I have no idea why this is needed... */

int main(argc, argv) int argc; char **argv; {

  FILE *fp1, *fp2;
  char fname1[MAXPATHLEN], fname2[MAXPATHLEN];
  epr_type *sp;
  double xmin = 99999.9, xmax = -xmin;
  double x;
  int i;

  if(argc != 3) {
    fprintf(stderr, "Usage: pc2epr pcfile eprfile\n");
    exit(1);
  }

  strcpy(fname1, argv[1]);
  strcat(fname1, ".x");
  strcpy(fname2, argv[1]);
  strcat(fname2, ".y");

  if(!(fp1 = fopen(fname1, "r"))) {
    fprintf(stderr, "Can't open pcfile: %s.x\n", argv[1]);
    exit(1);
  }
  if(!(fp2 = fopen(fname2, "r"))) {
    fprintf(stderr, "Can't open pcfile: %s.y\n", argv[1]);
    exit(1);
  }

  if(!(sp = epr_misc_new())) {
    fprintf(stderr, "Memory allocation error.\n");
    exit(1);
  }
  epr_misc_zero(sp);
  if(fscanf(fp1, " %f", &(sp->frequency)) != 1) {
    fprintf(stderr, "Failed to read the MW frequency.\n");
    exit(1);
  }
  for (i = 0; i < EPR_MAX_SAMPLES; i++) {
    if(fscanf(fp1, " %lf", &x) != 1 ||
       fscanf(fp2, " %lf", sp->y + i) != 1) break;
    if (x < xmin) xmin = x;
    if (x > xmax) xmax = x;
  }
  fclose(fp1);
  fclose(fp2);
  sp->samples = i;
  strcpy(sp->fname, argv[2]);
  sp->xbeg = xmin;
  sp->xend = xmax;
  sp->xunits = 0;

  if(!epr_file_write(sp)) {
    fprintf(stderr, "Error writing epr file.\n");
    exit(1);
  }
  return 0;
}

