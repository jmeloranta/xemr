/*
 * Convert epr to Bruker ESP-380 file.
 *
 * Usage: epr2esp epr-file esp-file
 * (note that both esp-file and epr-file is only a *basename*)
 *
 * Last modified: Aug 25 2014 jussi.eloranta@csun.edu
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
#include "esp2epr.h"

MAIN__() {} /* I have no idea why this is needed... */

int main(argc, argv) int argc; char **argv; {

  FILE *fp;
  epr_type *sp;
  int i;
#ifdef __i386__
  union {
    int tmp;
    char ctmp[4];
  } fish;
  int tmp3;
#else
  int tmp;
#endif
  int tmp2;
  double scanrange, midrange;
  double freqbeg, freqwidth;
  char file[MAXPATHLEN], buf[EPR_IBUFLEN], unitstr[512];

  unitstr[0] = 0;
  if(argc != 3) {
    fprintf(stderr, "Usage: epr2esp eprfile espfile\n");
    exit(1);
  }

  if((sp = epr_file_read(argv[1])) == NULL) {
    fprintf(stderr, "Can't open %s.\n", argv[1]);
    exit(1);
  }

  sp->frequency /= 1.0E9;
  midrange = (sp->xend + sp->xbeg) / 2.0;
  scanrange = fabs(sp->xend - sp->xbeg);

  strcpy(file, argv[1]);
  strcat(file, ".par");
  if(!(fp = fopen(file, "w"))) {
    fprintf(stderr, "Can't open ESP380 parameter file for writing.\n");
    exit(1);
  }
  fprintf(fp, "JON %s\r", sp->operator);
  fprintf(fp, "JRE %d\r", sp->resonator);
  fprintf(fp, "JDA %s\r", sp->date);
  fprintf(fp, "JTM %s\r", sp->time);
  fprintf(fp, "JCO %s\r", sp->info);
  fprintf(fp, "JUN gauss\r");  /* for now */
  fprintf(fp, "JSD %d\r", sp->scans);
  fprintf(fp, "JEX epr\r"); /* for now */
  fprintf(fp, "TE %le\r", sp->temperature);
  fprintf(fp, "HCF %le\r", midrange);
  fprintf(fp, "HSW %le\r", scanrange);
  fprintf(fp, "MF %le\r", sp->frequency);
  fprintf(fp, "MP %le\r", sp->power);
  fprintf(fp, "RMA %le\r", sp->modulampl);
  fprintf(fp, "RRG %le\r", sp->gain);
  fprintf(fp, "RCT %le\r", sp->ctime);
  fprintf(fp, "RTC %le\r", sp->timec);
  fprintf(fp, "RMF %le\r", sp->modfreq);
  fprintf(fp, "RES %d\r", sp->samples);
  /* ENDOR stuff left out for now */
  fclose(fp);

  strcpy(file, argv[1]);
  strcat(file, ".spc");
  if(!(fp = fopen(file, "w"))) {
    fprintf(stderr, "Can't open ESP380 spectra file for writing.\n");
    exit(1);
  }
  for (i = 0; i < sp->samples; i++) {
#ifdef __i386__
    fish.tmp = (int) sp->y[i];
    tmp3 = fish.ctmp[1];
    fish.ctmp[1] = fish.ctmp[2];
    fish.ctmp[2] = tmp3;    
    tmp3 = fish.ctmp[0];
    fish.ctmp[0] = fish.ctmp[3];
    fish.ctmp[3] = tmp3;
    if(!fwrite((char *) &(fish.tmp), sizeof(int), 1, fp)) break;
#else
    tmp = (int) sp->y[i];
    if(!fwrite((char *) &tmp, sizeof(int), 1, fp)) break;
#endif
  }
  fclose(fp);

  return 0;
}
