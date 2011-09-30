/*
 * This file is part of esp2epr.c.
 *
 * The following table is for an ESP spectrum.
 *
 */

struct epr_idents esptbl[] = {
  "JON", EPR_STRING, (void *) 0,     /* 0 (operator) */
  "JRE", EPR_INT, (void *) 0,        /* 1 (resonator) */
  "JDA", EPR_STRING, (void *) 0,     /* 2 (acq date) */
  "JTM", EPR_STRING, (void *) 0,     /* 3 (acq time) */
  "JCO", EPR_STRING, (void *) 0,     /* 4 (comment)  */
  "JUN", EPR_STRING, (void *) 0,     /* 5 (units) */
  "JSD", EPR_INT, (void *) 0,        /* 6 (# scans) */
  "JEX", EPR_STRING, (void *) 0,     /* 7 (type of experiment) */
  "TE",  EPR_FLOAT, (void *) 0,      /* 8 (temperature) */
  "HCF", EPR_FLOAT, (void *) 0,      /* 9 (center field) */
  "HSW", EPR_FLOAT, (void *) 0,      /* 10 (sweep width) */
  "MF",  EPR_FLOAT, (void *) 0,      /* 11 (microwave freq) */
  "MP",  EPR_FLOAT, (void *) 0,      /* 12 (microwave power) */
  "RMA", EPR_FLOAT, (void *) 0,      /* 13 (modulation amplitude) */
  "RRG", EPR_FLOAT, (void *) 0,      /* 14 (receiver gain) */
  "RCT", EPR_FLOAT, (void *) 0,      /* 15 (conversion time) */
  "RTC", EPR_FLOAT, (void *) 0,      /* 16 (time constant) */
  "RMF", EPR_FLOAT, (void *) 0,      /* 17 (modulation freq) */
  "RES", EPR_INT, (void *) 0,        /* 18 (resolution) */
  "EMF", EPR_FLOAT, (void *) 0,      /* 19 (ENMR field) */
  "ESF", EPR_FLOAT, (void *) 0,      /* 20 (ENMR start freq) */
  "ESW", EPR_FLOAT, (void *) 0,      /* 21 (ENMR sweep width) */
  "EFD", EPR_FLOAT, (void *) 0,      /* 22 (ENMR modulation) */
  "EPF", EPR_FLOAT, (void *) 0,      /* 23 (ENMR pump freq) */
  "ESP", EPR_FLOAT, (void *) 0,      /* 24 (ENMR power) */
  "EPP", EPR_FLOAT, (void *) 0,      /* 25 (ENMR pump power) */
  (char *) 0, 0, (void *) 0
  };


