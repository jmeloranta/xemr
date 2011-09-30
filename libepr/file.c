/*
 * I/O functions.
 * Last modified: Oct-23-2009 jmeloranta@gmail.com
 *
 * Written by:
 * Teemu Lähteenmäki
 * Heikki Joela
 * Jussi Eloranta (jmeloranta@gmail.com)
 *
 */

#include "epr.h"

/* identifiers for epr file format */
struct epr_idents epr_idtbl[] = {
  "helo", EPR_NIL, (void *) 0,
  "name", EPR_STRING, (void *) 0,     /* 1 (compound name) */
  "oper", EPR_STRING, (void *) 0,     /* 2 (operator) */
  "date", EPR_STRING, (void *) 0,     /* 3 (date of creation) */
  "time", EPR_STRING, (void *) 0,     /* 4 (time of creation) */
  "reso", EPR_INT, (void *) 0,        /* 5 (resonator type) */
  "harm", EPR_INT, (void *) 0,        /* 6 (harmonic) */
  "spty", EPR_FLAG, (void *) 0,       /* 7 (spectrum type) */
  "saty", EPR_FLAG, (void *) 0,       /* 8 (sample type) */
  "solu", EPR_STRING, (void *) 0,     /* 9 (solvent or matrix) */
  "NA", EPR_INT, (void *) 0,          /* 10 not in use */
  "temp", EPR_FLOAT, (void *) 0,      /* 11 (temperature) */
  "scan", EPR_INT, (void *) 0,        /* 12 (scans) */
  "gain", EPR_FLOAT, (void *) 0,      /* 13 (gain) */
  "timc", EPR_FLOAT, (void *) 0,      /* 14 (time constant) */
  "ctim", EPR_FLOAT, (void *) 0,      /* 15 (conversion time) */
  "modu", EPR_FLOAT, (void *) 0,      /* 16 (modulation ampl) */
  "modf", EPR_FLOAT, (void *) 0,      /* 17 (modulation freq) */
  "mwfq", EPR_FLOAT, (void *) 0,      /* 18 (microwave frequency) */
  "mwpw", EPR_FLOAT, (void *) 0,      /* 19 (microwave power) */
  "endb", EPR_FLOAT, (void *) 0,      /* 20 (endor field) */
  "endm", EPR_FLOAT, (void *) 0,      /* 21 (endor modulation) */
  "trif", EPR_FLOAT, (void *) 0,      /* 22 (triple pump field) */
  "endp", EPR_FLOAT, (void *) 0,      /* 23 (endor power) */
  "trip", EPR_FLOAT, (void *) 0,      /* 24 (triple power) */
  "info", EPR_STRING, (void *) 0,     /* 25 (general info field) */
  "xbeg", EPR_FLOAT, (void *) 0,      /* 26 (X-axis start) */
  "xend", EPR_FLOAT, (void *) 0,      /* 27 (X-axis end) */
  "xuni", EPR_INT, (void *) 0,        /* 28 (X-axis type) */
  "othe", EPR_FLOAT, (void *) 0,      /* 29 (orientation theta) */
  "ophi", EPR_FLOAT, (void *) 0,      /* 30 (orientation phi) */
  "spha", EPR_FLOAT, (void *) 0,      /* 31 (signal phase) */
  (char *) 0, 0, (void *) 0
  };

/*
 * Function: char *epr_file_fgets(char *, int, FILE *)
 * Purpose: fgets function that accepts both ctrl-j and ctrl-m as a line
 *          delimiter.
 *
 * buf    = buffer where data is returned. [output; char *]
 * len    = length of buf. [input; int]
 * stream = stream from which the data is to be read. [input; FILE *]
 * 
 * return value = If data was available the return value is buf (char *).
 *                If data was not available the return value is 0 (char *).
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

char *epr_file_fgets(char *buf, int len, FILE *strm) {

  int ch;
  char *ptr;

  ptr = buf;
  while((ch = fgetc(strm)) != EOF) {
    len--;
    if(ch == 13 || ch == 10 || !len) {
      *ptr = 0;
      return buf;
    }
    *ptr++ = ch;
  }

  return 0;
}

/*
 * Function: epr_type *epr_file_write(epr_type *)
 * Purpose: write data and parameter files.
 *
 * sp = structure containing the spectra data [input; epr_type *]
 *
 * return value: 0        = error
 *               non-zero = pointer to epr_type structure
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_file_write(epr_type *sp) {

  FILE *fp;
  char fname[MAXPATHLEN];
  int i;

  strcpy(fname, sp->fname);
  strcat(fname, ".spe");
  if(!(fp = fopen(fname, "w")))
    return 0;
  fprintf(fp, "helo\n");
  if(sp->name[0])
    fprintf(fp, "name %s\n", sp->name);
  if(sp->operator[0])
    fprintf(fp, "oper %s\n", sp->operator);
  if(sp->date[0])
    fprintf(fp, "date %s\n", sp->date);
  if(sp->time[0])
    fprintf(fp, "time %s\n", sp->time);
  fprintf(fp, "reso %d\n", sp->resonator);
  fprintf(fp, "harm %d\n", sp->harmonic);
  fprintf(fp, "spty %d\n", sp->spek_type);
  fprintf(fp, "saty %d\n", sp->samp_type);
  if(sp->solvent[0])
    fprintf(fp, "solu %s\n", sp->solvent);
  fprintf(fp, "temp %le\n", sp->temperature);
  fprintf(fp, "scan %d\n", sp->scans);
  fprintf(fp, "gain %le\n", sp->gain);
  fprintf(fp, "timc %le\n", sp->timec);
  fprintf(fp, "ctim %le\n", sp->ctime);
  fprintf(fp, "modu %le\n", sp->modulampl);
  fprintf(fp, "modf %le\n", sp->modfreq);
  fprintf(fp, "mwfq %le\n", sp->frequency);
  fprintf(fp, "mwpw %le\n", sp->power);
  fprintf(fp, "endb %le\n", sp->endor_b);
  fprintf(fp, "endm %le\n", sp->endor_mod);
  fprintf(fp, "trif %le\n", sp->endor_pump_freq);
  fprintf(fp, "endp %le\n", sp->endor_power);
  fprintf(fp, "trip %le\n", sp->endor_pump_power);
  if(sp->info[0])
    fprintf(fp, "info %s\n", sp->info);
  fprintf(fp, "xbeg %le\n", sp->xbeg);
  fprintf(fp, "xend %le\n", sp->xend);
  fprintf(fp, "xuni %d\n", sp->xunits);
  fprintf(fp, "othe %le\n", sp->theta);
  fprintf(fp, "ophi %le\n", sp->phi);
  fprintf(fp, "spha %le\n", sp->sphase);
  fclose(fp);

  fname[strlen(fname)-1] = 'y';
  if(!(fp = fopen(fname, "w")))
    return 0;
  for (i = 0; i < sp->samples; i++)
    fprintf(fp, "%lf\n", sp->y[i]);
  fclose(fp);
  
  return sp;
}

/*
 * Function: epr_type *epr_file_read(char *)
 * Purpose: read data and parameters from file.
 *
 * file  = path name to file (without extensions). [input; char *]
 *
 * return value: 0        = error
 *               non-zero = pointer to epr_type structure
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

epr_type *epr_file_read(char *file) {

  struct epr_idents *ptr;
  int i;
  char buf[EPR_IBUFLEN], fname[MAXPATHLEN];
  FILE *fp;
  epr_type *sp;

  strcpy(fname, file);
  strcat(fname, ".spe");
  if(!(fp = fopen(fname, "r")) || !(sp = epr_misc_new()))
    return 0;
  epr_idtbl[0].ptr = (void *) 0;
  epr_idtbl[1].ptr = (void *) sp->name;
  epr_idtbl[2].ptr = (void *) sp->operator;
  epr_idtbl[3].ptr = (void *) sp->date;
  epr_idtbl[4].ptr = (void *) sp->time;
  epr_idtbl[5].ptr = (void *) &(sp->resonator);
  epr_idtbl[6].ptr = (void *) &(sp->harmonic); 
  epr_idtbl[7].ptr = (void *) &(sp->spek_type); 
  epr_idtbl[8].ptr = (void *) &(sp->samp_type); 
  epr_idtbl[9].ptr = (void *) sp->solvent;
  epr_idtbl[11].ptr = (void *) &(sp->temperature);
  epr_idtbl[12].ptr = (void *) &(sp->scans);
  epr_idtbl[13].ptr = (void *) &(sp->gain);
  epr_idtbl[14].ptr = (void *) &(sp->timec);
  epr_idtbl[15].ptr = (void *) &(sp->ctime);
  epr_idtbl[16].ptr = (void *) &(sp->modulampl);
  epr_idtbl[17].ptr = (void *) &(sp->modfreq);
  epr_idtbl[18].ptr = (void *) &(sp->frequency);
  epr_idtbl[19].ptr = (void *) &(sp->power);
  epr_idtbl[20].ptr = (void *) &(sp->endor_b);
  epr_idtbl[21].ptr = (void *) &(sp->endor_mod);
  epr_idtbl[22].ptr = (void *) &(sp->endor_pump_freq);
  epr_idtbl[23].ptr = (void *) &(sp->endor_power);
  epr_idtbl[24].ptr = (void *) &(sp->endor_pump_power);
  epr_idtbl[25].ptr = (void *) sp->info;
  epr_idtbl[26].ptr = (void *) &(sp->xbeg);
  epr_idtbl[27].ptr = (void *) &(sp->xend);
  epr_idtbl[28].ptr = (void *) &(sp->xunits);
  epr_idtbl[29].ptr = (void *) &(sp->theta);
  epr_idtbl[30].ptr = (void *) &(sp->phi);
  epr_idtbl[31].ptr = (void *) &(sp->sphase);

  strcpy(sp->fname, file);
  while(epr_file_fgets(buf, sizeof(buf), fp)) {
    for (ptr = epr_idtbl; ptr->id; ptr++)
      if(!strncasecmp(buf, ptr->id, strlen(ptr->id)) && ptr->ptr) {
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
  fclose(fp);
  fname[strlen(fname)-1] = 'y';
  if(!(fp = fopen(fname, "r"))) {
    free(sp);
    return 0;
  }
  for (i = 0; i < EPR_MAX_SAMPLES; i++)
    if(fscanf(fp, " %lf", sp->y + i) != 1) break;
  fclose(fp);
  sp->samples = i;
  
  if (sp->xbeg == sp->xend) {
    free(sp);
    return 0;
  }

  return sp;
}

/*
 * Function: void epr_file_export(char *, epr_type *)
 * Purpose: Export spectrum to x, y ASCII format.
 *
 * file = Output file [input; char *]
 * sp   = Spectrum to be exported [input; epr_type *]
 *
 * Reviewed: Nov-12-09 (jme).
 *
 */

void epr_file_export(char *file, epr_type *sp) {
  
  FILE *fp;
  int i;
  
  if(!(fp = fopen(file, "w"))) {
    return;
  }

  for (i = 0; i < sp->samples; i++)
    fprintf(fp, "%le %le\n", sp->xbeg + EPR_XSTEP(sp) * i, sp->y[i]);

  fclose(fp);

  return;
}
