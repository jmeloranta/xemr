/* Variable utilities for xemr */

#include "xemr.h"

/*
 * Find variable index from var_* arrays given type, set, arg1, arg2, arg3
 * and arg4.
 *
 * Returns -1 if not found.
 *
 */

int find_var(int type, int set, int arg1, int arg2, int arg3, int arg4) {

  int i;

  for (i = 0; i < MAXVAR; i++) {
    if(type == TYPE_T && var_type[i] == type) return i;
    if(type == TYPE_EOFF && var_type[i] == type) return i;
    if(set == var_set[i] && type == var_type[i]) {
      switch(type) {
      case TYPE_LG:
      case TYPE_AMP:
      case TYPE_LW:
      case TYPE_ELW:
      case TYPE_EAMP:
      case TYPE_SO:
      case TYPE_CRA1:
      case TYPE_CRA2:
      case TYPE_CRA3:
      case TYPE_CRB:
	return i;
      case TYPE_B:
      case TYPE_C:
      case TYPE_K:
	if(arg1 == var_arg1[i]) return i;
	break;
      case TYPE_D:
	if(arg1 == var_arg1[i] && arg2 == var_arg2[i]) return i;
	break;
      case TYPE_AP:
      case TYPE_POSN:
	if(arg1 == var_arg1[i] && arg2 == var_arg2[i] && arg3 == var_arg3[i])
	  return i;
	break;
      case TYPE_AD:
      case TYPE_HFC:
	if(arg1 == var_arg1[i] && arg2 == var_arg2[i] && arg3 == var_arg3[i]
	   && arg4 == var_arg4[i]) return i;
	break;
      }
    }
  }
  return -1;
}

/*
 * Return pointer to given parameter. Used in creating *real* variables
 * for optimization (which requires passing a pointer to the variable data).
 *
 * Returns NULL on error.
 *
 */

double *find_param(int type, int set, int arg1, int arg2, int arg3, int arg4) {

  switch(type) {
  case TYPE_NONE:
    return (double *) 0;
  case TYPE_LG:
    return &(sims[set].lg_fact);
  case TYPE_LW:
    return &(sims[set].lw);
  case TYPE_ELW:
    return &(sims[0].e_ilw[set]);
  case TYPE_EAMP:
    return &(sims[0].e_iamp[set]);
  case TYPE_EOFF:
    return &(sims[0].e_offset);
  case TYPE_POSN:
    return &(sims[set].posn[arg1][EPR_INDEX(3,arg2,arg3)]);
  case TYPE_AMP:
    return &(sims[set].amp);
  case TYPE_SO:
    return &(sims[set].spin_orbit);
  case TYPE_CRA1:
    return &(sims[set].cra1);
  case TYPE_CRA2:
    return &(sims[set].cra2);
  case TYPE_CRA3:
    return &(sims[set].cra3);
  case TYPE_CRB:
    return &(sims[set].crb);
  case TYPE_HFC:
    return &(sims[set].hfc[arg1][arg2][EPR_INDEX(3,arg3,arg4)]);
  case TYPE_B:
    return &(sims[set].b[arg1]);
  case TYPE_C:
    return &(sims[set].c[arg1][arg1]);	
  case TYPE_D:
    return &(sims[set].c[arg1][arg2]);
  case TYPE_K:
    return &(heinzer_k[nspec*set + arg1]);
  case TYPE_T:
    return &norris_k;
  case TYPE_AD:
    return &(sims[set].d[arg1][arg2][EPR_INDEX(3,arg3,arg4)]);
  case TYPE_AP:
    return &(sims[set].p[arg1][EPR_INDEX(3,arg2,arg3)]);
  default:
    return (double *) 0;
  }
  return (double *) 0;
}

/*
 * Clear variable settings (var_* arrays).
 *
 */

void clear_variables() {

  int i;

  for(i = 0; i < MAXVAR; i++)
    var_type[i] = TYPE_NONE;
}

/*
 * Convert xemr variable arrays into structure type variable which is
 * then used by libepr.
 *
 */

void convert_variables() {

  extern int nvars;
  int i;
  double *ptr;

  nvars = 0;
  for (i = 0; i < MAXVAR; i++)
    if(var_type[i] != TYPE_NONE) {
      if((ptr = find_param(var_type[i], var_set[i], var_arg1[i], var_arg2[i],
			   var_arg3[i], var_arg4[i])) != (double *) 0) {
	switch (var_type[i]) {
	case TYPE_LG:
	  sprintf(vars[nvars].name, "LG-factor(%d)", var_set[i]+1);
	  break;
	case TYPE_LW:
	  sprintf(vars[nvars].name, "Linewidth(%d)", var_set[i]+1);
	  break;
	case TYPE_ELW:
	  sprintf(vars[nvars].name, "ENDOR linewidth(%d)", var_set[i]+1);
	  break;
	case TYPE_EAMP:
	  sprintf(vars[nvars].name, "ENDOR amplitude(%d)", var_set[i]+1);
	  break;
	case TYPE_EOFF:
	  sprintf(vars[nvars].name, "ENDOR field offset(%d)",var_set[i]+1);
	  break;
	case TYPE_POSN:
	  sprintf(vars[nvars].name, "Position(%d,%d,%d,%d)", var_set[i]+1,
		  var_arg1[i]+1, var_arg2[i]+1, var_arg3[i]+1);
	  break;
	case TYPE_AMP:
	  sprintf(vars[nvars].name, "Amplitude(%d).", var_set[i]+1);
	  break;
	case TYPE_SO:
	  sprintf(vars[nvars].name, "Spin-orbit(%d).", var_set[i]+1);
	  break;
	case TYPE_CRA1:
	  sprintf(vars[nvars].name, "Crystal field A-x(%d).\n", var_set[i]+1);
	  break;
	case TYPE_CRA2:
	  sprintf(vars[nvars].name, "Crystal field A-y(%d).\n", var_set[i]+1);
	  break;
	case TYPE_CRA3:
	  sprintf(vars[nvars].name, "Crystal field A-z(%d).\n", var_set[i]+1);
	  break;
	case TYPE_CRB:
	  sprintf(vars[nvars].name, "Crystal field B(%d).\n", var_set[i]+1);
	  break;
	case TYPE_HFC:
	  sprintf(vars[nvars].name, "HFC(%d,%d,%d,%d,%d)", var_set[i]+1,
		  var_arg1[i]+1, var_arg2[i]+1, var_arg3[i]+1, var_arg4[i]+1);
	  break;
	case TYPE_B:
	  sprintf(vars[nvars].name, "B(%d,%d)", var_set[i]+1, var_arg1[i]+1);
	  break;
	case TYPE_C:
	  sprintf(vars[nvars].name, "C(%d,%d,%d)", var_set[i]+1, var_arg1[i]+1,
		  var_arg2[i]+1);
	  break;
	case TYPE_T:
	  sprintf(vars[nvars].name, "Norris");
	  break;
	case TYPE_K:
	  sprintf(vars[nvars].name, "Heinzer(%d,%d)", var_set[i]+1, var_arg1[i]+1);
	  break;
	case TYPE_AD:
	  sprintf(vars[nvars].name, "D(%d,%d,%d,%d,%d)", var_set[i]+1,
		  var_arg1[i]+1, var_arg2[i]+1, var_arg3[i]+1, var_arg4[i]+1);
	  break;
	case TYPE_AP:
	  sprintf(vars[nvars].name, "P(%d,%d,%d,%d)", var_set[i]+1,
		  var_arg1[i]+1, var_arg2[i]+1, var_arg3[i]+1);
	  break;
	}
	vars[nvars].value = ptr;
	vars[nvars].nequiv = 0;
	vars[nvars].high = var_high[i];
	vars[nvars].low = var_low[i];
	if(active[i])
	  vars[nvars++].mc_limit = var_mclimit[i];	
	else
	  vars[nvars++].mc_limit = 0.0;
      } else {
	printf("Warning: variable reference to non-existing parameter:\n");
	printf("   Type = %d, Set = %d, arg1 = %d, arg2 = %d, arg3 = %d, arg4 = %d\n",
	       var_type[i], var_set[i]+1, var_arg1[i]+1, var_arg2[i]+1,
	       var_arg3[i]+1, var_arg4[i]+1);
      }
    }
}

void dump_variables() {

  int i;
  double *ptr;

  for (i = 0; i < MAXVAR; i++) {
    if(var_type[i] != TYPE_NONE) {
      ptr = find_param(var_type[i], var_set[i], var_arg1[i], var_arg2[i],
		       var_arg3[i], var_arg4[i]);
      if(!ptr) {
	printf("Warning: NULL variable!\n");
	continue;
      }
      printf("%d %d %d %d %d %lf\n", var_type[i], var_set[i], var_arg1[i],
	     var_arg2[i], var_arg3[i], var_arg4[i], *ptr);
    }
  }
}

/* returns 1 if alias, 0 if not */
int isalias(int type, int set, int arg1, int arg2, int arg3, int arg4) {
  
  int i;

  if(type == TYPE_T) return 0; /* Norris rate can't be alias */
  if(type == TYPE_EOFF) return 0; /* endor offset can't be alias */
  for (i = 0; i < NEQUIV; i++)
    if(alias_type[i] == type && alias_to_set[i] == set) {
      switch(alias_type[i]) {
      case TYPE_LG:
      case TYPE_AMP:
      case TYPE_LW:
      case TYPE_ELW:
      case TYPE_EAMP:
      case TYPE_SO:
      case TYPE_CRA1:
      case TYPE_CRA2:
      case TYPE_CRA3:
      case TYPE_CRB:
	return 1;
      case TYPE_B:
      case TYPE_C:
      case TYPE_K:
	if(arg1 == alias_to_arg1[i]) return 1;
	break;
      case TYPE_D:
	if(arg1 == alias_to_arg1[i] && arg2 == alias_to_arg2[i]) return 1;
	break;
      case TYPE_AD:
      case TYPE_POSN:
	if(arg1 == alias_to_arg1[i] && arg2 == alias_to_arg2[i] &&
	   arg3 == alias_to_arg3[i]) return 1;
	break;
      case TYPE_HFC:
      case TYPE_AP:
	if(arg1 == alias_to_arg1[i] && arg2 == alias_to_arg2[i] &&
	   arg3 == alias_to_arg3[i] && arg4 == alias_to_arg4[i]) return 1;
	break;
      }
    }
  return 0;
}

void convert_aliases() {

  int i, j;
  double *ptr;

  for (i = 0; i < NEQUIV; i++) {
    if(alias_type[i] != TYPE_NONE) {
#if 0
      printf("type = %d.\n", alias_type[i]);
      printf("FROM: set=%d, arg1=%d, arg2=%d, arg3=%d, arg4=%d.\n",
	     alias_from_set[i], alias_from_arg1[i], alias_from_arg2[i],
	     alias_from_arg3[i], alias_from_arg4[i]);
      printf("TO: set=%d, arg1=%d, arg2=%d, arg3=%d, arg4=%d.\n",
	     alias_to_set[i], alias_to_arg1[i], alias_to_arg2[i],
	     alias_to_arg3[i], alias_to_arg4[i]);
#endif
      if((j = find_var(alias_type[i], alias_from_set[i], alias_from_arg1[i],
		       alias_from_arg2[i], alias_from_arg3[i], 
		       alias_from_arg4[i])) != -1) {
	ptr = find_param(alias_type[i], alias_to_set[i], alias_to_arg1[i],
			 alias_to_arg2[i], alias_to_arg3[i],
			 alias_to_arg4[i]);
	vars[j].equiv[vars[j].nequiv++] = ptr;
      } else printf("Warning: Alias definition not found.\n");  
    }
  }
}

/*
 * Clear variable settings (var_* arrays).
 *
 */

void clear_equivalences() {

  int i;

  for(i = 0; i < NEQUIV; i++)
    alias_type[i] = TYPE_NONE;
}
