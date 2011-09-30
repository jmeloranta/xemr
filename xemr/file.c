/*
 * Parameter file operations
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "xemr.h"

extern int nelws, imode, ipoints;
extern double elws[], amps[];

static char buf[MAXPATHLEN], buf2[MAXPATHLEN];
static double fltmp1, fltmp2, fltmp3, fltmp4;

/*
 * Read parameters from file
 *
 */

void read_param(const char *str) {

  FILE *fp;
  int i, j, k, l, m, n, nel;
  double tmp;
  char tmp2[512];

  if(!(fp = fopen(str, "r"))) {
    fl_show_alert("Parameter file open failed.", "", "", 1);
    return;
  }
  fscanf(fp, " %d %*[^\n\r]", &nspec);
  sprintf(buf, "%d", nspec);
  fl_set_input(simparam->nspec, buf);

  fscanf(fp, " %d %d %d %d %d %d %*[^\n\r]", &i, &j, &k, &l, &m, &n);
  fl_set_choice(simparam->run, i);
  /* this param is not saved... fix it some day! */
  if(i == 5) {
    epr_fit_difl = 0.5;
    sprintf(tmp2, "%le", epr_fit_difl);
    fl_set_input(hj->diff, tmp2);
  } else {
    epr_fit_difl = 1E-5;
    sprintf(tmp2, "%le", epr_fit_difl);
    fl_set_input(runml->tolerance, tmp2);
  }

  fl_set_choice(simparam->system, j);
  fl_set_choice(simparam->hamilton, k);
  fl_set_choice(simparam->lineshape, l);
  fl_set_choice(simparam->transition, m);
  fl_set_choice(simparam->iunits, n);

  fscanf(fp, " %d %d %d %lf %lf %lf %le %*[^\n\r]", &i, &j, &k, &fltmp1, 
	 &fltmp4, &fltmp2, &fltmp3);
  sprintf(buf, "%d", i);
  fl_set_input(runsimul->resolution, buf);
  sprintf(buf, "%d", j);
  fl_set_input(simparam->target, buf);
  sprintf(buf, "%lf", fltmp1);
  fl_set_input(runsimul->width, buf);
  sprintf(buf, "%lf", fltmp4);
  fl_set_input(runsimul->center, buf);
  sprintf(buf, "%d", (int) fltmp2);
  fl_set_input(runmon->iterations, buf);
  fl_set_input(runsim->iterations, buf);
  fl_set_input(hj->maxiter, buf);
  sprintf(buf, "%lf", fltmp2);
  epr_fit_mltol = fltmp2;
  sprintf(buf, "%d", k);
  fl_set_input(runsim->restarts, buf);
  if(fltmp3 < 100.0) fltmp3 = DEF_FREQ;
  sprintf(buf, "%le", fltmp3);
  fl_set_input(runsimul->frequency, buf);

  fscanf(fp, " %d %d %*[^\n\r]", &epr_phi, &epr_theta);
  fscanf(fp, "%lf %lf %*[^\n\r]", &fltmp1, &fltmp2);
  sprintf(buf, "%lf", fltmp1);
  fl_set_input(simparam->theta, buf);
  sprintf(buf, "%lf", fltmp2);
  fl_set_input(simparam->phi, buf);

  for (i = 0; i < nspec; i++) {
    fscanf(fp, " %d %*[^\n\r]", &sims[i].auto_amp);
    fl_set_button(simparam->auto_amp, sims[i].auto_amp);
    fscanf(fp, " %lf %*[^\n\r]", &(sims[i].amp));
    fscanf(fp, " %d %*[^\n\r]", &(sims[i].ngr));
    fscanf(fp, " %lf %*[^\n\r]", &(sims[i].espin));
    nel = (int) (sims[i].espin / 0.5);
    fscanf(fp, " %lf %*[^\n\r]", &(sims[i].lg_fact));
    fscanf(fp, " %lf %le %le", &(sims[i].lw), &(sims[i].lwy), &(sims[i].lwz));
    fscanf(fp, " %*[^\n\r]");
    fscanf(fp, " %d %*[^\n\r]", &(sims[i].dlevel));
    fl_set_choice(simparam->dlevel, sims[i].dlevel);
    /* anisotropic g */
    for (j = 0; j < nel; j++) {
      for (k = 0; k < 9; k++)
	fscanf(fp, " %lf", &(sims[i].posn[j][k]));
      fscanf(fp, " %*[^\n\r]");
    }

    /* nuclear data */
    for (j = 0; j < sims[i].ngr; j++)
      fscanf(fp, " %lf %d %*[^\n\r]", &(sims[i].spin[j]), &(sims[i].nbr[j]));

    /* HFC */
    for (l = 0; l < nel; l++) 
      for (j = 0; j < sims[i].ngr; j++) {
	for(k = 0; k < 9; k++)
	  fscanf(fp, " %lf", &(sims[i].hfc[l][j][k]));
	fscanf(fp, " %*[^\n\r]");
      }

    /* asymmetric linewidth parameters */
    for (j = 0; j < sims[i].ngr; j++) {
      fscanf(fp, " %lf %*[^\n\r]", &(sims[i].b[j]));
      for (k = j; k < sims[i].ngr; k++)
	fscanf(fp, " %lf %*[^\n\r]", &(sims[i].c[j][k]));
    }

    /* atom definitions */
    for (j = 0; j < sims[i].ngr; j++)
      fscanf(fp, " %d", &(sims[i].atoms[j]));
    fscanf(fp, " %*[^\n\r]");

    /* D parameters */
    for (k = 0; k < nel; k++)
      for (l = 0; l < nel; l++) {
	for (j = 0; j < 9; j++)
	  fscanf(fp, " %le", &(sims[i].d[k][l][j]));
	fscanf(fp, " %*[^\n\r]");
      }

    /* P parameters */
    for (j = 0; j < sims[i].ngr; j++) {
      for (k = 0; k < 9; k++)
	fscanf(fp, " %le", &(sims[i].p[j][k]));
      fscanf(fp, " %*[^\n\r]");
    }

    /* angle offsets */
    fscanf(fp, " %lf %lf %*[^\n\r]", &(sims[i].o_phi), &(sims[i].o_theta));
  }

  /* Exchange simulation parameters */
  fscanf(fp, " %le %*[^\n\r]", &norris_k);
  for (i = 0; i < nspec; i++) 
    for (j = 0; j < nspec; j++)
      fscanf(fp, " %le %*[^\n\r]", &(heinzer_k[nspec*i + j]));  

  /* Variable definitions */
  fscanf(fp, " %d %*[^\n\r]", &nvars);
  bzero(var_type, sizeof(int) * MAXVAR);
  for (i = 0; i < nvars; i++) {
    vars[i].nequiv = 0;
    fscanf(fp, " %d %d %d %d %d %d %le %le %le %*[^\n\r]",
	   &(var_type[i]), &(var_set[i]), &(var_arg1[i]),
	   &(var_arg2[i]), &(var_arg3[i]), &(var_arg4[i]), &(var_mclimit[i]),
	   &(var_high[i]), &(var_low[i]));
  }
  
  /* Equivalence definitions */
  fscanf(fp, " %d %*[^\n\r]", &j);
  bzero(alias_type, sizeof(int) * NEQUIV);
  for (i = 0; i < j; i++)
    fscanf(fp, " %d %d %d %d %d %d %d %d %d %d %d %*[^\n\r]",
	   &alias_type[i], &alias_from_set[i], &alias_from_arg1[i],
	   &alias_from_arg2[i], &alias_from_arg3[i], &alias_from_arg4[i],
	   &alias_to_set[i],&alias_to_arg1[i],&alias_to_arg2[i], 
	   &alias_to_arg3[i], &alias_to_arg4[i]);
  fscanf(fp, " %lf %*[^\n\r]", &last_rms);
  /* finally ENDOR B */
  fscanf(fp, " %le %*[^\n\r]", &tmp);
  sprintf(tmp2, "%lf", tmp);
  fl_set_input(runsimul->endor_b, tmp2);
  /* and linewidth */
  for (i = 0; i < nspec; i++)
    fscanf(fp, " %le %*[^\n\r]", &sims[i].elw);
  /* read internal parameters */
  if(!feof(fp)) {
    fscanf(fp, " %le %*[^\n\r]", &epr_line_cutoff);
    fscanf(fp, " %d %*[^\n\r]", &epr_hamilton_divs);
  }
  if(!feof(fp)) {
    fscanf(fp, " %d %*[^\n\r]", &nelws);
    for (i = 0; i < nelws; i++)
      fscanf(fp, " %le %le %*[^\n\r]", elws + i, amps + i);
    fscanf(fp, " %le %*[^\n\r]", &sims[0].e_offset);
  }
  /* read orbital parameters */
  if(!feof(fp)) {
    fscanf(fp, "%d %*[^\n\r]", &i);
    fl_set_choice(simparam->orbital, i);
    for (i = 0; i < nspec; i++) {
      fscanf(fp, "%le %*[^\n\r]", &sims[i].spin_orbit);
      fscanf(fp, "%le %*[^\n\r]", &sims[i].cra1);
      fscanf(fp, "%le %*[^\n\r]", &sims[i].cra2);
      fscanf(fp, "%le %*[^\n\r]", &sims[i].cra3);
      fscanf(fp, "%le %*[^\n\r]", &sims[i].crb);
      fscanf(fp, "%d %d %*[^\n\r]", &sims[i].begin, &sims[i].end);
      fscanf(fp, "%d %*[^\n\r]", &sims[i].orbit);
      fscanf(fp, "%le %*[^\n\r]", &sims[i].spin_temp);
    }
    if(sims[0].begin == -1 && sims[0].end == -1)
      fl_set_button(simparam->active, 0);
    else
      fl_set_button(simparam->active, 1);
    sprintf(buf, "%d", sims[0].begin);
    fl_set_input(simparam->begin, buf);
    sprintf(buf, "%d", sims[0].end);
    fl_set_input(simparam->end, buf);
    sprintf(buf, "%.2lf", sims[0].spin_temp);
    fl_set_input(simparam->temp, buf);
    if(sims[0].spin_temp <= 0.0)
      fl_set_button(simparam->boltzmann, 0);
    else
      fl_set_button(simparam->boltzmann, 1);
  } else {
    fl_set_choice(simparam->orbital, 1);
    for (i = 0; i < nspec; i++) {
      sims[i].spin_orbit = sims[i].cra1 = sims[i].cra2 =
	sims[i].cra3 = sims[i].crb = 0.0;
      sims[i].orbit = 0;
      sims[i].begin = sims[i].end = -1;
      fl_set_button(simparam->active, 0);
    }
    sprintf(buf, "0");
    fl_set_input(simparam->begin, buf);
    fl_set_input(simparam->end, buf);
    fl_set_button(simparam->boltzmann, 0);
  }
  if(!feof(fp)) {
    fscanf(fp, " %d %d %d %*[^\n\r]", &imode, &ipoints, &epr_powder_fm);
    fscanf(fp, " %le %le %le %le %*[^\n\r]", &epr_phi_begin, &epr_phi_end, &epr_theta_begin, &epr_theta_end);
  } else {
    imode = 1;
    ipoints = 2;
    epr_phi_begin = epr_theta_begin = 0.0;
    epr_phi_end = epr_theta_end = M_PI / 2.0;
  }
  if(!feof(fp)) {
    fscanf(fp, " %le %*[^\n\r]", &fltmp2);
    sprintf(buf, "%le", fltmp2);
    fl_set_input(hj->diff, buf);
  } else fl_set_input(hj->diff, "0.5");
  fclose(fp);
}


/*
 * Write parameters to disk
 *
 */

void write_param(char *str) {

  FILE *fp;
  int i, j, k, l, nel;
  extern double epr_fit_mltol;
  extern int epr_powder_fm;

  if(!(fp = fopen(str, "w"))) {
    fl_show_alert("Write failed!","","",1);
    return;
  }
  fprintf(fp,"%d # numer of spectra\n", nspec);
  fprintf(fp,"%d %d %d %d %d %d # run type, system type, hamilton, lineshape type, transition, IHFC units\n",
	  fl_get_choice(simparam->run), fl_get_choice(simparam->system),
	  fl_get_choice(simparam->hamilton),fl_get_choice(simparam->lineshape),
	  fl_get_choice(simparam->transition), fl_get_choice(simparam->iunits));
  switch(fl_get_choice(simparam->run)) {
  case 1:
    strcpy(buf, "0");
    break;
  case 2:
    strcpy(buf, fl_get_input(runmon->iterations));
    break;
  case 3:
    strcpy(buf, fl_get_input(runsim->iterations));
    break;
  case 4:
    sprintf(buf, "%le", epr_fit_mltol);
    break;
  case 5:
    strcpy(buf, fl_get_input(hj->maxiter));
    break;
  }
  fprintf(fp, "%d %d %d %lf %lf %lf %le # resolution, target, restarts, width, center, iterations, frequency\n",
	  atoi(fl_get_input(runsimul->resolution)),
	  atoi(fl_get_input(simparam->target)),
	  atoi(fl_get_input(runsim->restarts)),
	  atof(fl_get_input(runsimul->width)), 
	  atof(fl_get_input(runsimul->center)),
	  atof(buf),
	  atof(fl_get_input(runsimul->frequency)));
  fprintf(fp,"%d %d # number of integration steps (phi,theta)\n",
	  epr_phi, epr_theta);
  fprintf(fp,"%lf %lf # Current sample orientation (theta, phi)\n",
	  atof(fl_get_input(simparam->theta)),
	  atof(fl_get_input(simparam->phi)));
  for(i = 0; i < nspec; i++) {
    nel = (int) (sims[i].espin / 0.5);
    fprintf(fp,"%d # Automagic amplitude\n", sims[i].auto_amp);
    fprintf(fp, "%lf # Amplitude for spectrum #%d\n", sims[i].amp, i+1);
    fprintf(fp, "%d  # Number of spin groups\n", sims[i].ngr);
    fprintf(fp, "%lf # Total S\n", sims[i].espin);
    fprintf(fp, "%lf # Lorentz-Gauss factor\n", sims[i].lg_fact);
    fprintf(fp, "%lf %lf %lf # Linewidth\n", sims[i].lw, sims[i].lwy, sims[i].lwz);
    fprintf(fp, "%d  # Derivative level\n", sims[i].dlevel);
    /* position */
    for (j = 0; j < nel; j++) {
      for (k = 0; k < 9; k++)
	fprintf(fp, "%lf ", sims[i].posn[j][k]);
      fprintf(fp, "# Anisotropic g tensor\n");
    }
    /* nuclear data */
    for (j = 0; j < sims[i].ngr; j++)
      fprintf(fp, "%lf %d # nuclear spin, number of nuclei\n",
	      sims[i].spin[j], sims[i].nbr[j]);
    /* anisotropic HFC */
    for (l = 0; l < nel; l++) 
      for (j = 0; j < sims[i].ngr; j++) {
	for (k = 0; k < 9; k++)
	  fprintf(fp, "%lf ", sims[i].hfc[l][j][k]);
	fprintf(fp, "# IHFC matrix (%d,%d)\n", j+1, l+1);
      }

    /* asymmetric linewidth parameters */
    for (j = 0; j < sims[i].ngr; j++) {
      fprintf(fp, "%lf # b[%d]\n", sims[i].b[j], j+1);
      for (k = j; k < sims[i].ngr; k++)
	fprintf(fp, "%lf # c[%d][%d]\n", sims[i].c[j][k], j+1, k+1);
    }

    for (j = 0; j < sims[i].ngr; j++)
      fprintf(fp, "%d ", sims[i].atoms[j]);
    fprintf(fp, "# Atom definitions\n");

    for(k = 0; k < nel; k++)
      for (l = 0; l < nel; l++) {
	for (j = 0; j < 9; j++)
	  fprintf(fp, "%lf ", sims[i].d[k][l][j]);
	fprintf(fp, "# D matrix (%d, %d)\n", k+1, l+1);
	}

    for (j = 0; j < sims[i].ngr; j++) {
      for (k = 0; k < 9; k++)
	fprintf(fp, "%lf ", sims[i].p[j][k]);
      fprintf(fp, " # Anisotropic P for %d\n", j+1);
    }
    fprintf(fp, "%lf %lf # Phi, theta angle offsets\n", sims[i].o_phi,
	    sims[i].o_theta);
  }
  /* Exchange parameters */
  fprintf(fp, "%le # Average rate constant for Norris method\n", norris_k);
  for (i = 0; i < nspec; i++) 
    for (j = 0; j < nspec; j++)
      fprintf(fp, "%le # Heinzer rate constant [%d][%d]\n",
	      heinzer_k[nspec*i + j], i+1, j+1);
  /* Variable definitions */
  nvars = 0;
  for(i = 0; i < MAXVAR; i++)
    if(var_type[i] != TYPE_NONE) nvars++;
  fprintf(fp, "%d # Number of variables\n", nvars);
  for (i = 0; i < MAXVAR; i++)
    if(var_type[i] != TYPE_NONE)
      fprintf(fp, "%d %d %d %d %d %d %le %le %le # Variable %d\n",
	      var_type[i], var_set[i], var_arg1[i], var_arg2[i],
	      var_arg3[i], var_arg4[i], var_mclimit[i], var_high[i],
	      var_low[i], i+1);
  /* Equivalence definitions */
  for (i = j = 0; i < NEQUIV; i++)
    if(alias_type[i] != TYPE_NONE) j++;
  fprintf(fp, "%d # Number of equivalence definitions\n", j);
  for (i = 0; i < NEQUIV; i++)
    if(alias_type[i] != TYPE_NONE)
      fprintf(fp, "%d %d %d %d %d %d %d %d %d %d %d # type, from_set, from_arg1, from_arg2, from_arg3, from_arg4, to_set, to_arg1, to_arg2, to_arg3, to_arg4\n",
	      alias_type[i], alias_from_set[i], alias_from_arg1[i],
	      alias_from_arg2[i], alias_from_arg3[i], alias_from_arg4[i],
	      alias_to_set[i], alias_to_arg1[i],alias_to_arg2[i],
	      alias_to_arg3[i], alias_to_arg4[i]);
  fprintf(fp, "%lf # RMS from last fitting\n", last_rms);
  fprintf(fp, "%lf # ENDOR B\n", atof(fl_get_input(runsimul->endor_b)));
  for(i = 0; i < nspec; i++)
    fprintf(fp, "%lf # ENDOR lw for %d.\n", sims[i].elw, i+1);
  fprintf(fp, "%le # epr_line_cutoff\n", epr_line_cutoff);
  fprintf(fp, "%d # epr_hamilton_divs\n", epr_hamilton_divs);
  /* write endor amps & lws */
  fprintf(fp, "%d # Number of ENDOR lines\n", nelws);
  for (i = 0; i < nelws; i++)
    fprintf(fp, "%le %le # endor lw and amp for %d\n", elws[i], amps[i], i+1);
  fprintf(fp, "%le # ENDOR field offset\n", sims[0].e_offset);
  /* write orbital parameters */
  fprintf(fp, "%d # orbital terms\n", fl_get_choice(simparam->orbital));
  /* for backward compatibility - sigh - */
  for (i = 0; i < nspec; i++) {
    fprintf(fp, "%le # spin-orbit coupling for %d\n", sims[i].spin_orbit, i+1);
    fprintf(fp, "%le # crystal field parameter a(x) (%d)\n", sims[i].cra1, i+1);
    fprintf(fp, "%le # crystal field parameter a(y) (%d)\n", sims[i].cra2, i+1);
    fprintf(fp, "%le # crystal field parameter a(z) (%d)\n", sims[i].cra3, i+1);
    fprintf(fp, "%le # crystal field parameter b (%d)\n", sims[i].crb, i+1);
    fprintf(fp, "%d %d # active space (%d)\n", sims[i].begin, sims[i].end, i+1);
    fprintf(fp, "%d # active orbital ang. mom. (%d)\n", sims[i].orbit, i+1);
    fprintf(fp, "%le # Spin temperature\n", sims[i].spin_temp);
  }
  fprintf(fp, "%d %d %d # Powder integrator mode, gauss-points, and iterp. points\n", imode, ipoints, epr_powder_fm);
  fprintf(fp, "%le %le %le %le # phi begin, end, theta begin, end\n", epr_phi_begin,
	  epr_phi_end, epr_theta_begin, epr_theta_end);
  fprintf(fp, "%le # Hooke-Jeeves step length\n", atof(fl_get_input(hj->diff)));
  fclose(fp);
}

/*
 * Print parameters to printer
 *
 */

void print_param() {

  FILE *fp;
  int i, j, k, l, m, nel;
  extern double ucnv_hfc(double, double), elws[], amps[];
  extern int page;

  if(!(fp = fopen("/tmp/param.txt", "w"))) {
    fl_show_alert("Write failed!","","",1);
    return;
  }
  fprintf(fp, "Parameter set name: %s\n", last_param_file);
  fprintf(fp, "RMS = %lf\n", last_rms);
  if(sp[page]) {
    fprintf(fp, "Experimental spectrum name: %s\n", sp[page]->fname);
    fprintf(fp, "Microwave frequency = %le Hz.\n", sp[page]->frequency);
  }
  fprintf(fp, "Sample orientation = (%lf, %lf).\n", sims[0].phi, sims[0].theta);
  fprintf(fp,"Number of spectra: %d\n", nspec);
  for(i = 0; i < nspec; i++) {
    fprintf(fp, "\n---------------- SPECTRUM #%d ------------------\n", i+1);
    fprintf(fp, "Amplitude: %lf ", sims[i].amp);
    if(sims[i].auto_amp) fprintf(fp, "(Automatically adjusted amplitude)\n");
    else fprintf(fp, "\n");
    fprintf(fp, "Number of groups: %d\n", sims[i].ngr);
    fprintf(fp, "Active space: %d %d\n", sims[i].begin, sims[i].end);
    fprintf(fp, "Spin temperature: %le\n", sims[i].spin_temp);
    switch(fl_get_choice(simparam->system)) {
    case 1:
      fprintf(fp, "Oriented or isotropic EPR spectrum.\n");
      fprintf(fp, "Phi offset = %lf, theta offset = %lf.\n", sims[i].o_theta,
	      sims[i].o_phi);
      break;
    case 2:
      fprintf(fp, "Non-oriented EPR spectrum.\n");
      break;
    case 3:
      fprintf(fp, "Non-oriented EPR spectrum (const. lw).\n");
      break;
    case 4:
      fprintf(fp, "Oriented ENDOR spectrum with B_endor = %lf.\n", sp[i]->endor_b);
      break;
    case 5:
      fprintf(fp, "Non-oriented ENDOR spectrum with B_endor = %lf.\n", sp[i]->endor_b);
      break;
    case 6:
      fprintf(fp, "Non-oriented ENDOR spectrum (const. lw) with B_endor = .\n",
	      sp[i]->endor_b);
      break;
    }
    fprintf(fp, "Hamiltonian model: ");
    switch(fl_get_choice(simparam->hamilton)) {
    case 1:
      fprintf(fp, "1st order\n");
      break;
    case 2:
      fprintf(fp, "g.B.S + A.S.I\n");
      break;
    case 3:
      fprintf(fp, "g.B.S + A.S.I + S.D.S\n");
      break;
    case 4:
      fprintf(fp, "g.B.S + A.S.I + I.P.I\n");
      break;
    case 5:
      fprintf(fp, "g.B.S + A.S.I + S.D.S + I.P.I\n");
      break;
    case 6:
      fprintf(fp, "g.B.S + gN.B.I + A.S.I\n");
      break;
    case 7:
      fprintf(fp, "g.B.S + gN.B.I + A.S.I + S.D.S\n");
      break;
    case 8:
      fprintf(fp, "g.B.S + gN.B.I + A.S.I + I.P.I\n");
      break;
    case 9:
      fprintf(fp, "g.B.S + gN.B.I + A.S.I + S.D.S + I.P.I\n");
      break;
    }
    fprintf(fp, "Transition moment: ");
    switch (fl_get_choice(simparam->transition)) {
    case 1: 
      fprintf(fp, "B1*g*b*S (90o)\n");
      break;
    case 2:
      fprintf(fp, "-B1*gN*bn*I (90o)\n");
      break;
    case 3:
      fprintf(fp, "B1*g*b*S - B1*gN*bN*I (90o)\n");
      break;
    case 4:
      fprintf(fp, "B1*g*b*S (0o)\n");
      break;
    case 5:
      fprintf(fp, "-B1*gN*bn*I (0o)\n");
      break;
    case 6:
      fprintf(fp, "B1*g*b*S - B1*gN*bN*I (0o)\n");
      break;
    case 7:
      fprintf(fp, "Constant\n");
      break;
    }
    fprintf(fp, "Orbital settings: ");
    switch (fl_get_choice(simparam->orbital)) {
    case 1:
      /* none */
      fprintf(fp, "None.\n");
      break;
    case 2:
      /* B.L */
      fprintf(fp, "B.L (ang. mom. = %d).\n", sims[i].orbit);
      break;
    case 3:
      /* B.L + crystal */
      fprintf(fp, "B.L + CF. Ang. mom. = %d, oct = %le, tetr = (%le,%le,%le).\n",
	      sims[i].orbit, sims[i].crb, sims[i].cra1, sims[i].cra2,
	      sims[i].cra3);
      break;
    case 4:
      /* B.L + aS.L */
      fprintf(fp, "B.L + aS.L. Ang. mom. = %d, so = %le.\n",
	      sims[i].orbit, sims[i].spin_orbit);
      break;
    case 5:
      /* B.L + aS.L + crystal */
      fprintf(fp, "B.L + aS.L + CF. Ang. mom. = %d, so = %le,\n",
	      sims[i].orbit, sims[i].spin_orbit);
      fprintf(fp, "oct = %le and tetr = (%le,%le,%le).\n", sims[i].crb,
	      sims[i].cra1, sims[i].cra2, sims[i].cra3);
      break;
    case 6:
      /* aS.L */
      fprintf(fp, "aS.L. Ang. mom. = %d, so = %le).\n",
	      sims[i].orbit, sims[i].spin_orbit);
      break;
    case 7:
      /* aS.L + crystal */
      fprintf(fp, "aS.L + CF. Ang. mom. = %d, so = %le, oct = %le,\n",
	      sims[i].orbit, sims[i].spin_orbit, sims[i].crb);
      fprintf(fp, "tetr = (%le,%le,%le).\n", sims[i].cra1, sims[i].cra2,
	      sims[i].cra3);
      break;
    case 8:
      /* crystal */
      fprintf(fp, "CF. Ang. mom. = %d, oct = %le, tetr = (%le,%le,%le).\n",
	      sims[i].orbit, sims[i].crb, sims[i].cra1, sims[i].cra2,
	      sims[i].cra3);
      break;
    }

    if(fl_get_choice(simparam->hamilton) == 1 && 
       fl_get_choice(simparam->system) >= 3) {
      fprintf(fp, "ENDOR field offset: %le.\n", sims[0].e_offset);
      fprintf(fp, "ENDOR linewidths and amplitudes (high, low field):\n");
      for (j = 0; j < 2*sims[0].ngr; j += 2)
	fprintf(fp, "%le %le, %le %le\n", elws[j], amps[j], elws[j+1], amps[j+1]);
    } else fprintf(fp, "Linewidth: %lf (G)\n", sims[i].lw);
    fprintf(fp, "Lineshape: ");
    switch(fl_get_choice(simparam->lineshape)) {
    case 1: /* stick */
      fprintf(fp, "Stick.\n");
      break;
    case 2: /* normal */
      fprintf(fp, "Lorentzian/Gaussian with ratio = %lf.\n", sims[i].lg_fact);
      break;
    case 3: /* asymm. lw */
      fprintf(fp, "Asymmetric Lorentz.\n");
      fprintf(fp, "The linewidth parameters are:\n");
      for (j = 0; j < sims[i].ngr; j++) {
	fprintf(fp, "B(%d) = %lf\n", j+1, sims[i].b[j]);
	for (k = j; k < sims[i].ngr; k++)
	  fprintf(fp, "C(%d,%d) = %lf\n", j+1, k+1, sims[i].c[j][k]);
      }
      break;
      /* 4 and 5 are delt elsewehere */
    }
    switch(sims[i].dlevel) {
    case EPR_ABSORPTION:
      fprintf(fp, "Absorption.\n");
      break;
    case EPR_FIRSTDERIV:
      fprintf(fp, "1st derivative.\n");
      break;
    case EPR_SECONDERIV:
      fprintf(fp, "2nd derivative.\n");
      break;
    }
    nel = (int) (sims[i].espin / 0.5);
    fprintf(fp, "Number of unpaired electrons: %d.\n", nel);
    for (j = 0; j < nel; j++) {
      fprintf(fp, "g-tensor for electron #%d:\n", j+1);
      for (k = 0; k < 3; k++) {
	for (l = 0; l < 3; l++)
	  fprintf(fp, " %lf", sims[i].posn[j][EPR_INDEX(3,k,l)]);
	fprintf(fp, "\n");
      }
    }

    fprintf(fp, "\nAtom\tIsotope\tSpin\t\tNumber\n");
    for(j = 0; j < sims[i].ngr; j++)
      fprintf(fp, "%s\t%d\t%.1lf\t\t%d\n", epr_atoms[sims[i].atoms[j]].symbol, 
	      epr_atoms[sims[i].atoms[j]].isotope, sims[i].spin[j],
	      sims[i].nbr[j]);
    fprintf(fp, "\n");

    switch (fl_get_choice(simparam->iunits)) {
    case 1:
      fprintf(fp, "NOTE: Units in Gauss.\n");
      break;
    case 2:
      fprintf(fp, "NOTE: Units in mT.\n");
      break;
    case 3:
      fprintf(fp, "NOTE: Units in MHz.\n");
      break;
    }
    
    for (m = 0; m < nel; m++) {
      fprintf(fp,"*** Hyperfine coupling for electron %d.\n", m+1);
      for (j = 0; j < sims[i].ngr; j++) {
	fprintf(fp,"Hyperfine coupling tensor for nucleus %d:\n", j+1);
	for (k = 0; k < 3; k++) {
	  for (l = 0; l < 3; l++)
	    fprintf(fp, "%lf ", ucnv_hfc(sims[i].hfc[m][j][EPR_INDEX(3,k,l)],
		    sims[i].posn[m][EPR_INDEX(3,0,0)])); /* FIXME */
	  fprintf(fp,"\n");
	}
      }
    }
    
    switch(fl_get_choice(simparam->lineshape)) {
    case 4: /* Norris */
      fprintf(fp, "Exchange model based on Norris equation.\n");
      fprintf(fp, "Average lifetime = %le\n", norris_k);
      break;
    case 5: /* Heinzer intra */
      fprintf(fp, "Intramolecular exchange model based on Heinzer method.\n");
      fprintf(fp, "The rate constant matrix is:\n");
      for (i = 0; i < nspec; i++) {
	for (j = 0; j < nspec; j++)
	  fprintf(fp, "%le ", heinzer_k[nspec*i+j]);
	fprintf(fp, "\n");
      }
      break;
    case 6: /* Heinzer inter - NYI */
      break;
    }
  }

  switch(fl_get_choice(simparam->hamilton)) {
  case 3:
  case 5: /* S.D.S */
  case 7:
  case 9:    
    for (j = 0; j < nel; j++) {
      for (k = 0; k < nel; k++) {
	fprintf(fp,"Electron-electron coupling tensor for %d and %d.\n", j+1,k+1);
	for (l = 0; l < 3; l++) {
	  for (m = 0; m < 3; m++)
	    fprintf(fp, "%lf ", ucnv_hfc(sims[i].d[j][k][EPR_INDEX(3,l,m)], EPR_GE));
	  fprintf(fp,"\n");
	}
      }
    }
    break;
  }

  switch(fl_get_choice(simparam->hamilton)) {
  case 3:
  case 5: /* I.P.I */
  case 7:
  case 9:
    for (j = 0; j < sims[i].ngr; j++) {
      fprintf(fp,"Nuclear-nuclear quadrupole coupling tensor for %d.\n", j+1);
      for (l = 0; l < 3; l++) {
	for (m = 0; m < 3; m++)
	  fprintf(fp, "%lf ", ucnv_hfc(sims[i].p[j][EPR_INDEX(3,l,m)], EPR_GE));
	fprintf(fp,"\n");
      }
    }
    break;
  }

  fclose(fp);
  system("lpr -r /tmp/param.txt");
}

