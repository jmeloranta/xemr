#include "xemr.h"

/*
 * Find first free page.
 *
 */

int first_free_page() {

  int i;

  for (i = 0; i < MAXSP; i++)
    if(!sp[i]) break;
  return i;
}

/* 
 * Convert units from internal representation to ASCII.
 *
 */

char *units(int i) {

  switch (i) {
  case EPR_UNIT_G:
    return "G";
  case EPR_UNIT_MT:
    return "mT";
  case EPR_UNIT_MHZ:
    return "MHz";
  case EPR_UNIT_GHZ:
    return "GHz";
  case EPR_UNIT_GV:
    return "g-value";
  case EPR_UNIT_S:
    return "s";
  case EPR_UNIT_ARB:
    return "Arbitary";
  default:
    return "unknown";
  }
}
