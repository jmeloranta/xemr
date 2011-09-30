/*
 * Convert Bruker ESP-380 to epr file.
 *
 * Usage: esp2epr esp-file epr-file
 * (note that both esp-file and epr-file is only a *basename*)
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
#include "esp2epr.h"

MAIN__() {} /* I have no idea why this is needed... */

int main(argc, argv) int argc; char **argv; {

  FILE *fp;
  epr_type *sp;
  struct epr_idents *ptr;
  register int i;
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
  char exptype[20];
  double scanrange, midrange;
  double freqbeg, freqwidth;
  char file[MAXPATHLEN], buf[EPR_IBUFLEN], unitstr[512];

  unitstr[0] = 0;
  if(argc != 3) {
    fprintf(stderr, "Usage: esp2epr espfile eprfile\n");
    exit(1);
  }

  strcpy(file, argv[1]);
  strcat(file, ".par");
  if(!(fp = fopen(file, "r"))) {
    fprintf(stderr, "Can't open ESP380 parameter file.\n");
    exit(1);
  }

  if(!(sp = epr_misc_new())) {
    fprintf(stderr, "Memory allocation error.\n");
    exit(1);
  }
  epr_misc_zero(sp);

  esptbl[0].ptr = (void *) sp->operator;
  esptbl[1].ptr = (void *) &(sp->resonator);
  esptbl[2].ptr = (void *) sp->date;
  esptbl[3].ptr = (void *) sp->time;
  esptbl[4].ptr = (void *) sp->info;
  esptbl[5].ptr = (void *) &unitstr;
  esptbl[6].ptr = (void *) &(sp->scans);
  esptbl[7].ptr = (void *) exptype;
  esptbl[8].ptr = (void *) &(sp->temperature);
  esptbl[9].ptr = (void *) &midrange;
  esptbl[10].ptr = (void *) &scanrange;
  esptbl[11].ptr = (void *) &(sp->frequency);
  esptbl[12].ptr = (void *) &(sp->power);
  esptbl[13].ptr = (void *) &(sp->modulampl);
  esptbl[14].ptr = (void *) &(sp->gain);
  esptbl[15].ptr = (void *) &(sp->ctime);
  esptbl[16].ptr = (void *) &(sp->timec);
  esptbl[17].ptr = (void *) &(sp->modfreq);
  esptbl[18].ptr = (void *) &tmp2;
  esptbl[19].ptr = (void *) &(sp->endor_b);
  esptbl[20].ptr = (void *) &freqbeg;
  esptbl[21].ptr = (void *) &freqwidth;
  esptbl[22].ptr = (void *) &(sp->endor_mod);
  esptbl[23].ptr = (void *) &(sp->endor_pump_freq);
  esptbl[24].ptr = (void *) &(sp->endor_power);
  esptbl[25].ptr = (void *) &(sp->endor_pump_power);

  /* FIXME: add endor signal phase to xemr - read EPH */

  while(epr_file_fgets(buf, sizeof(buf), fp)) {
    for (ptr = esptbl; ptr->id; ptr++) {
      if(!strncasecmp(buf, ptr->id, strlen(ptr->id)) && ptr->ptr) {
	printf("%s\n", buf);
	switch (ptr->type) {
	case EPR_NIL:
	  break;
	case EPR_STRING:
	  strcpy((char *) ptr->ptr, buf+strlen(ptr->id)+1);
	  break;
	case EPR_FLOAT:
	  *((double *) ptr->ptr) = (double) atof(buf+strlen(ptr->id)+1);
	  break;
	case EPR_INT:
	  *((int *) ptr->ptr) = (int) atoi(buf+strlen(ptr->id)+1);
	  break;
	case EPR_FLAG:
	  *((char *) ptr->ptr) = (char) atoi(buf+strlen(ptr->id)+1);
	  break;
	}
      }
    }
  }
  fclose(fp);

  strcpy(file, argv[1]);
  strcat(file, ".spc");
  if(!(fp = fopen(file, "r"))) {
    fprintf(stderr, "Can't open ESP380 spectra file.\n");
    exit(1);
  }
  for (i = 0; i < EPR_MAX_SAMPLES; i++) {
#ifdef __i386__
    if(!fread((char *) &(fish.tmp), sizeof(int), 1, fp)) break;
    tmp3 = fish.ctmp[0];
    fish.ctmp[0] = fish.ctmp[3];
    fish.ctmp[3] = tmp3;
    tmp3 = fish.ctmp[1];
    fish.ctmp[1] = fish.ctmp[2];
    fish.ctmp[2] = tmp3;
    sp->y[i] = (double) fish.tmp;
#else
    if(!fread((char *) &tmp, sizeof(int), 1, fp)) break;
    sp->y[i] = (double) tmp;
#endif
  }
  fclose(fp);
  sp->samples = i;
  printf("%d\n", sp->samples);
  if(sp->samples != tmp2) {
    if(!tmp2) fprintf(stderr, "Warning: Resolution not defined.\n");
    else fprintf(stderr, "Warning: resolution differs from # of data points.\n");
  }
  sp->frequency *= 1.0E9;
  if(!strcasecmp(exptype, "ENDOR")) {
    sp->xbeg = freqbeg;
    sp->xend = freqbeg + freqwidth;
  } else {
    sp->xbeg = midrange - scanrange / 2.0;
    sp->xend = midrange + scanrange / 2.0;
  }
  if(*unitstr) {
    if(!strcasecmp(unitstr, "gauss")) sp->xunits = 0;
    if(!strcasecmp(unitstr, "mhz")) sp->xunits = 2;
    if(!strcasecmp(unitstr, "sec")) sp->xunits = 4;
  } else sp->xunits = 0; /* default is Gauss */

  strcpy(sp->fname, argv[2]);

  strcpy(sp->name, argv[1]);

  if(!epr_file_write(sp)) {
    fprintf(stderr, "Error writing epr file.\n");
    exit(1);
  }
  return 0;
}
