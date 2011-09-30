/*
 * dialog routines
 *
 * FIXME: variables stuff looks awful. Rewrite it some day.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "xemr.h"

static const char *str;
static FL_OBJECT *tmp;
static char buf[MAXPATHLEN], buf2[MAXPATHLEN];

void upd_orbital(int page) {

  sims[page].spin_orbit = atof(fl_get_input(orbital->so));
  sims[page].cra1 = atof(fl_get_input(orbital->parama1));
  sims[page].cra2 = atof(fl_get_input(orbital->parama2));
  sims[page].cra3 = atof(fl_get_input(orbital->parama3));
  sims[page].crb = atof(fl_get_input(orbital->paramb));
  sims[page].orbit = atoi(fl_get_input(orbital->angmom));
}

void do_orbital() {

  int page = 0, upd = 1;
  char buf[256];

  fl_show_form(orbital->orbital, FL_PLACE_MOUSE, FL_TRANSIENT, "Orbital parameters");
  while(1) {
    if(upd) {
      sprintf(buf, "Current page = %d", page);
      fl_set_object_label(orbital->number, buf);
      sprintf(buf, "%le", sims[page].spin_orbit);
      fl_set_input(orbital->so, buf);
      sprintf(buf, "%le", sims[page].cra1);
      fl_set_input(orbital->parama1, buf);
      sprintf(buf, "%le", sims[page].cra2);
      fl_set_input(orbital->parama2, buf);
      sprintf(buf, "%le", sims[page].cra3);
      fl_set_input(orbital->parama3, buf);
      sprintf(buf, "%le", sims[page].crb);
      fl_set_input(orbital->paramb, buf);
      sprintf(buf, "%d", sims[page].orbit);
      fl_set_input(orbital->angmom, buf);
      upd = 0;
    }
    tmp = fl_do_forms();
    if(tmp == orbital->ok) {
      upd_orbital(page);
      fl_hide_form(orbital->orbital);
      return;      
    }
    if(tmp == orbital->cancel) {
      fl_hide_form(orbital->orbital);
      return;
    }
    if(tmp == orbital->next && page < MAXSP-1) {
      upd_orbital(page);
      upd = 1;
      page++;
    }
    if(tmp == orbital->prev && page > 0) {
      upd_orbital(page);
      upd = 1;
      page--;
    }
  }
}

void do_elw() {

  int i;
  extern double elws[], amps[];
  extern int nelws;
  char buf[256];

  fl_show_form(elw->elw, FL_PLACE_MOUSE, FL_TRANSIENT, "ENDOR lw & amp");
  if(nelws >= MAXENDOR) nelws = MAXENDOR;
  if(nelws > 2*sims[0].ngr) nelws = sims[0].ngr*2;
  for (i = 0; i < nelws; i++) {
    sprintf(buf, "%le", elws[i]);
    fl_set_input(elw->lw[i], buf);
    sprintf(buf, "%le", amps[i]);
    fl_set_input(elw->amp[i], buf);
  }
  sprintf(buf, "%le", sims[0].e_offset);
  fl_set_input(elw->offset, buf);
  while(1) {
    tmp = fl_do_forms();
    if(tmp == elw->ok) {
      for (i = 0; i < MAXENDOR; i++) {
	elws[i] = atof(fl_get_input(elw->lw[i]));
	amps[i] = atof(fl_get_input(elw->amp[i]));
	if(amps[i] == 0.0) break;
      }
      nelws = i;
      sims[0].e_offset = atof(fl_get_input(elw->offset));
      fl_hide_form(elw->elw);
      return;
    }
    if(tmp == elw->cancel) {
      fl_hide_form(elw->elw);
      return;
    }
  }
}

/* X -> MHz */
double cnv_hfc(double x, double g) {

  switch (fl_get_choice(simparam->iunits)) {
  case 1:
    return (2.802494 * x * g / EPR_GE); /* G */
  case 2:
    return (28.02494 * x * g / EPR_GE); /* mT */
  case 3:
    return x; /* MHz */
  case 4:
    return (x * EPR_CM2HZ * 1.0E-6); /* cm-1 */
  }
  return 0.0;
}

/* MHz -> X */
double ucnv_hfc(double x, double g) {

  switch (fl_get_choice(simparam->iunits)) {
  case 1:
    if(g == 0.0) return 0.0;
    return (0.3568250 * x * EPR_GE / g); /* G */
  case 2:
    if(g == 0.0) return 0.0;
    return (0.03568250 * x * EPR_GE / g); /* mT */
  case 3:
    return x; /* MHz */
  case 4:
    return (1.0E6 * x / EPR_CM2HZ); /* cm-1 */
  }
  return 0.0;
}

/* gvalue.fd */

void update_gvalue(int ns, int el) {

  int i, j;
  double e[3], o[9];
  
  e[0] = atof(fl_get_input(gvalue->g1));
  e[1] = atof(fl_get_input(gvalue->g2));
  e[2] = atof(fl_get_input(gvalue->g3));
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      o[EPR_INDEX(3,i,j)] = atof(fl_get_input(gvalue->posn[EPR_INDEX(3,i,j)]));
  epr_tensor_udiag(sims[ns].posn[el], o, e);
}

/* gvalue.fd */

void do_gvalue() {

  int i, j;
  int ns = 0, el = 0;
  double a[9], e[3], o[9];

  fl_show_form(gvalue->gvalue, FL_PLACE_MOUSE, FL_TRANSIENT,
	       "Electron g parameters");

  while (1) {
    sprintf(buf, "Electron g parameters for spectrum %d / electron %d",
	    ns + 1, el + 1);
    fl_set_object_label(gvalue->header, buf);
    bcopy(sims[ns].posn[el], a, sizeof(double) * 9);
    epr_tensor_diag(a, o, e);
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++) {
	sprintf(buf, "%lf", o[EPR_INDEX(3,i,j)]);
	fl_set_input(gvalue->posn[EPR_INDEX(3,i,j)], buf);
      }
    sprintf(buf, "%lf", e[0]);
    fl_set_input(gvalue->g1, buf);
    sprintf(buf, "%lf", e[1]);
    fl_set_input(gvalue->g2, buf);
    sprintf(buf, "%lf", e[2]);
    fl_set_input(gvalue->g3, buf);
    while(1) {
      tmp = fl_do_forms();
      if(tmp == gvalue->ok) {
	update_gvalue(ns, el);
	fl_hide_form(gvalue->gvalue);
	return;
      }
      if(tmp == gvalue->cancel) {
	fl_hide_form(gvalue->gvalue);
	return;
      }
      if(tmp == gvalue->nextsp) {
	update_gvalue(ns, el);
	if(ns < MAXSP-1) ns++;
	break;
      }
      if(tmp == gvalue->prevsp) {
	update_gvalue(ns, el);
	if(ns > 0) ns--;
	break;
      }
      if(tmp == gvalue->nextel) {
	update_gvalue(ns, el);
	if(el + 1 < EPR_MAXELS) el++;
	break;
      }
      if(tmp == gvalue->prevel) {
	update_gvalue(ns, el);
	if(el > 0) el--;
	break;
      }
    }
  }
}

/* alinewidth.fd */

void update_alinewidth(int ns) {

  sims[ns].lw = atof(fl_get_input(alinewidth->lwx));
  sims[ns].lwy = atof(fl_get_input(alinewidth->lwy));
  sims[ns].lwz = atof(fl_get_input(alinewidth->lwz));
  sims[ns].lg_fact = atof(fl_get_input(alinewidth->lg_fact));
  sims[ns].elw = atof(fl_get_input(alinewidth->elw));
}

/* alinewidth.fd */

void do_alinewidth() {

  int i, j;
  int ns = 0;

  fl_show_form(alinewidth->alinewidth, FL_PLACE_MOUSE, FL_TRANSIENT,
	       "Linewidth parameters");

  while (1) {
    sprintf(buf, "Linewidth parameters for spectrum %d", ns+1);
    fl_set_object_label(alinewidth->header, buf);
    sprintf(buf, "%lf", sims[ns].lw);
    fl_set_input(alinewidth->lwx, buf);
    sprintf(buf, "%lf", sims[ns].lwy);
    fl_set_input(alinewidth->lwy, buf);
    sprintf(buf, "%lf", sims[ns].lwz);
    fl_set_input(alinewidth->lwz, buf);
    sprintf(buf, "%lf", sims[ns].elw);
    fl_set_input(alinewidth->elw, buf);
    sprintf(buf, "%lf", sims[ns].lg_fact);
    fl_set_input(alinewidth->lg_fact, buf);

    while(1) {
      tmp = fl_do_forms();
      if(tmp == alinewidth->ok) {
	update_alinewidth(ns);
	fl_hide_form(alinewidth->alinewidth);
	return;
      }
      if(tmp == alinewidth->cancel) {
	fl_hide_form(alinewidth->alinewidth);
	return;
      }
      if(tmp == alinewidth->nextsp) {
	update_alinewidth(ns);
	if(ns < MAXSP-1) ns++;
	break;
      }
      if(tmp == alinewidth->prevsp) {
	update_alinewidth(ns);
	if(ns > 0) ns--;
	break;
      }
    }
  }
}



/* quadrupole.fd */

void update_quadrupole(int ns, int nu) {

  int i, j;
  double e[3], o[9], tmpx;
  
  tmpx = EPR_GE;
  e[0] = cnv_hfc(atof(fl_get_input(quadrupole->p1)), tmpx);
  e[1] = cnv_hfc(atof(fl_get_input(quadrupole->p2)), tmpx);
  e[2] = cnv_hfc(atof(fl_get_input(quadrupole->p3)), tmpx);
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      o[EPR_INDEX(3,i,j)] = atof(fl_get_input(quadrupole->p[EPR_INDEX(3,i,j)]));
  epr_tensor_udiag(sims[ns].p[nu], o, e);
}

/* quadrupole.fd */

void do_quadrupole() {

  int i, j;
  int ns = 0, nu = 0;
  double a[9], e[3], o[9], tmpx;

  fl_show_form(quadrupole->quadrupole, FL_PLACE_MOUSE, FL_TRANSIENT,
	       "Quadrupole parameters");

  while (1) {
    sprintf(buf, "Nuclear quadrupole parameters for spectrum %d / nucleus %d",
	    ns + 1, nu + 1);
    fl_set_object_label(quadrupole->header, buf);
    bcopy(sims[ns].p[nu], a, sizeof(double) * 9);
    epr_tensor_diag(a, o, e);
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++) {
	sprintf(buf, "%lf", o[EPR_INDEX(3,i,j)]);
	fl_set_input(quadrupole->p[EPR_INDEX(3,i,j)], buf);
      }
    tmpx = EPR_GE; /* FIXME */
    sprintf(buf, "%le", ucnv_hfc(e[0], tmpx));
    fl_set_input(quadrupole->p1, buf);
    sprintf(buf, "%le", ucnv_hfc(e[1], tmpx));
    fl_set_input(quadrupole->p2, buf);
    sprintf(buf, "%le", ucnv_hfc(e[2], tmpx));
    fl_set_input(quadrupole->p3, buf);
    while(1) {
      tmp = fl_do_forms();
      if(tmp == quadrupole->ok) {
	update_quadrupole(ns,nu);
	fl_hide_form(quadrupole->quadrupole);
	return;
      }
      if(tmp == quadrupole->cancel) {
	fl_hide_form(quadrupole->quadrupole);
	return;
      }
      if(tmp == quadrupole->nextsp) {
	update_quadrupole(ns,nu);
	if(ns < MAXSP-1) ns++;
	break;
      }
      if(tmp == quadrupole->prevsp) {
	update_quadrupole(ns,nu);
	if(ns > 0) ns--;
	break;
      }
      if(tmp == quadrupole->nextnucl) {
	update_quadrupole(ns,nu);
	if(nu < 11) nu++;
	break;
      }
      if(tmp == quadrupole->prevnucl) {
	update_quadrupole(ns,nu);
	if(nu > 0) nu--;
	break;
      }
    }
  }
}


/* elel.fd */

void update_elel(int ns, int e1, int e2) {

  int i, j;
  double e[3], o[9], tmpx;
  
  tmpx = EPR_GE;
  e[0] = cnv_hfc(atof(fl_get_input(elel->d1)), tmpx);
  e[1] = cnv_hfc(atof(fl_get_input(elel->d2)), tmpx);
  e[2] = cnv_hfc(atof(fl_get_input(elel->d3)), tmpx);
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      o[EPR_INDEX(3,i,j)] = atof(fl_get_input(elel->d[EPR_INDEX(3,i,j)]));
  epr_tensor_udiag(sims[ns].d[e1][e2], o, e);
}

/* elel.fd */

void do_elel() {

  int i, j, k;
  int ns = 0, e1 = 0, e2 = 0, nel, fict = 0;
  double a[9], e[3], o[9], tmpx;

  fl_show_form(elel->elel, FL_PLACE_MOUSE, FL_TRANSIENT,
	       "Electron-Electron dipolar parameters");
  fict = fl_get_choice(simparam->system);
  while (1) {
    sprintf(buf, "Electron-electron dipolar parameters for spectrum %d (%d,%d)",ns + 1,
	    e1 + 1, e2 + 1);
    fl_set_object_label(elel->header, buf);
    /* fictious spin */
    if(fict > 4) nel = 1;
    else nel = (int) (sims[ns].espin / 0.5);
    
    bcopy(sims[ns].d[e1][e2], a, sizeof(double) * 9);
    epr_tensor_diag(a, o, e);
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++) {
	sprintf(buf, "%lf", o[EPR_INDEX(3,i,j)]);
	fl_set_input(elel->d[EPR_INDEX(3,i,j)], buf);
      }
    tmpx = EPR_GE; /* FIXME */
    sprintf(buf, "%le", ucnv_hfc(e[0], tmpx));
    fl_set_input(elel->d1, buf);
    sprintf(buf, "%le", ucnv_hfc(e[1], tmpx));
    fl_set_input(elel->d2, buf);
    sprintf(buf, "%le", ucnv_hfc(e[2], tmpx));
    fl_set_input(elel->d3, buf);
    while(1) {
      tmp = fl_do_forms();
      if(tmp == elel->ok) {
	update_elel(ns, e1, e2);
	fl_hide_form(elel->elel);
	return;
      }
      if(tmp == elel->cancel) {
	fl_hide_form(elel->elel);
	return;
      }
      if(tmp == elel->nextsp) {
	update_elel(ns, e1, e2);
	if(ns < MAXSP-1) ns++;
	break;
      }
      if(tmp == elel->prevsp) {
	update_elel(ns, e1, e2);
	if(ns > 0) ns--;
	break;
      }
      if(tmp == elel->ince1) {
	update_elel(ns, e1, e2);
	if(e1 + 1 < nel) e1++;
	break;
      }
      if(tmp == elel->dece1) {
	update_elel(ns, e1, e2);
	if(e1 > 0) e1--;
	break;
      }
      if(tmp == elel->ince2) {
	update_elel(ns, e1, e2);
	if(e2 + 1 < nel) e2++;
	break;
      }
      if(tmp == elel->dece2) {
	update_elel(ns, e1, e2);
	if(e2 > 0) e2--;
	break;
      }
      if(tmp == elel->quick) {	
	double xd, xe, xj;
	str = fl_show_input("Enter D: ", "");
	if(!str) break;
	xd = cnv_hfc(atof(str), EPR_GE);
	str = fl_show_input("Enter E: ", "");
	if(!str) break;
	xe = cnv_hfc(atof(str), EPR_GE);
	str = fl_show_input("Enter J: ", "");
	if(!str) break;
	xj = cnv_hfc(atof(str), EPR_GE);
	if(fict > 4) {
	  bzero(sims[ns].d[0][0], sizeof(double) * 3 * 3);
	  sims[ns].d[0][0][EPR_INDEX(3,0,0)] = xe - xd / 3.0 + xj;
	  sims[ns].d[0][0][EPR_INDEX(3,1,1)] = -xe - xd / 3.0 + xj;
	  sims[ns].d[0][0][EPR_INDEX(3,2,2)] = 2.0 * xd / 3.0 + xj;
	} else {
	  for (j = 0; j < nel; j++)
	    for (k = 0; k < nel; k++) {
	      bzero(sims[ns].d[j][k], sizeof(double) * 3 * 3);
	      if(j != k) {
		sims[ns].d[j][k][EPR_INDEX(3,0,0)] = xe - xd / 3.0 + xj;
		sims[ns].d[j][k][EPR_INDEX(3,1,1)] = -xe - xd / 3.0 + xj;
		sims[ns].d[j][k][EPR_INDEX(3,2,2)] = 2.0 * xd / 3.0 + xj;
	      }
	    }
	}
	break;
      }
    }
  }
}

/* hyperfine.fd */

void update_hyperfine(int ns, int nu, int el) {

  int i, j;
  double e[3], o[9], tmpx;
  
  tmpx = sims[ns].posn[el][EPR_INDEX(3,0,0)];
  e[0] = cnv_hfc(atof(fl_get_input(hyperfine->a1)), tmpx);
  e[1] = cnv_hfc(atof(fl_get_input(hyperfine->a2)), tmpx);
  e[2] = cnv_hfc(atof(fl_get_input(hyperfine->a3)), tmpx);
  for (i = 0; i < 3; i++)
    for (j = 0; j < 3; j++)
      o[EPR_INDEX(3,i,j)] = atof(fl_get_input(hyperfine->a[EPR_INDEX(3,i,j)]));
  epr_tensor_udiag(sims[ns].hfc[el][nu], o, e);
}

/* hyperfine.fd */

void do_hyperfine() {

  int i, j;
  int ns = 0, nu = 0, el = 0;
  double a[9], e[3], o[9], tmpx;

  fl_show_form(hyperfine->hyperfine, FL_PLACE_MOUSE, FL_TRANSIENT,
	       "Hyperfine parameters");

  while (1) {
    sprintf(buf, "Hyperfine parameters for spectrum %d / nucleus %d / electron %d",
	    ns + 1, nu + 1, el + 1);
    fl_set_object_label(hyperfine->header, buf);
    bcopy(sims[ns].hfc[el][nu], a, sizeof(double) * 9);
    epr_tensor_diag(a, o, e);
    for (i = 0; i < 3; i++)
      for (j = 0; j < 3; j++) {
	sprintf(buf, "%lf", o[EPR_INDEX(3,i,j)]);
	fl_set_input(hyperfine->a[EPR_INDEX(3,i,j)], buf);
      }
    tmpx = sims[ns].posn[el][EPR_INDEX(3,0,0)]; /* FIXME */
    sprintf(buf, "%le", ucnv_hfc(e[0], tmpx));
    fl_set_input(hyperfine->a1, buf);
    sprintf(buf, "%le", ucnv_hfc(e[1], tmpx));
    fl_set_input(hyperfine->a2, buf);
    sprintf(buf, "%le", ucnv_hfc(e[2], tmpx));
    fl_set_input(hyperfine->a3, buf);
    while(1) {
      tmp = fl_do_forms();
      if(tmp == hyperfine->ok) {
	update_hyperfine(ns,nu, el);
	fl_hide_form(hyperfine->hyperfine);
	return;
      }
      if(tmp == hyperfine->cancel) {
	fl_hide_form(hyperfine->hyperfine);
	return;
      }
      if(tmp == hyperfine->nextsp) {
	update_hyperfine(ns,nu, el);
	if(ns < MAXSP-1) ns++;
	break;
      }
      if(tmp == hyperfine->prevsp) {
	update_hyperfine(ns,nu, el);
	if(ns > 0) ns--;
	break;
      }
      if(tmp == hyperfine->nextnucl) {
	update_hyperfine(ns,nu, el);
	if(nu < 11) nu++;
	break;
      }
      if(tmp == hyperfine->prevnucl) {
	update_hyperfine(ns,nu, el);
	if(nu > 0) nu--;
	break;
      }
      if(tmp == hyperfine->ince) {
	update_hyperfine(ns, nu, el);
	if(el + 1 < EPR_MAXELS) el++;
	break;
      }
      if(tmp == hyperfine->dece) {
	update_hyperfine(ns, nu, el);
	if(el > 0) el--;
	break;
      }	
    }
  }
}


/* generic.fd */

void update_generic(int ns) {

  int ngr, i;
  double tmp;
  
  sims[ns].espin = atof(fl_get_input(generic->espin));
  sims[ns].o_theta = atof(fl_get_input(generic->theta_offset));
  sims[ns].o_phi = atof(fl_get_input(generic->phi_offset));

  for (ngr = 0; ngr < 12; ngr++) {
    if(!(sims[ns].nbr[ngr] = atoi(fl_get_input(generic->nbr[ngr])))) break;
    i = sims[ns].atoms[ngr] = fl_get_choice(generic->atom[ngr])-1;
    sims[ns].spin[ngr] = epr_atoms[i].spin;
  }
  sims[ns].ngr = ngr;
  sims[ns].amp = atof(fl_get_input(generic->amplitude));
}

/* generic.fd */

void do_generic() {

  int i, j, ns = 0;

  fl_show_form(generic->generic, FL_PLACE_MOUSE, FL_TRANSIENT,
	       "Generic Hamilton paramters");

  while(1) {
    sprintf(buf, "Generic paramters for spectrum %d", ns + 1);
    fl_set_object_label(generic->label, buf);
    
    sprintf(buf, "%lf", sims[ns].espin);
    fl_set_input(generic->espin, buf);
    sprintf(buf, "%lf", sims[ns].amp);
    fl_set_input(generic->amplitude, buf);
    sprintf(buf, "%lf", sims[ns].o_theta);
    fl_set_input(generic->theta_offset, buf);
    sprintf(buf, "%lf", sims[ns].o_phi);
    fl_set_input(generic->phi_offset, buf);
  
    for (i = 0; i < sims[ns].ngr; i++) {
      sprintf(buf, "%d", sims[ns].nbr[i]);
      fl_set_input(generic->nbr[i], buf);
      fl_set_choice(generic->atom[i], sims[ns].atoms[i]+1);
    }
    for (; i < 12; i++) {
      fl_set_object_label(generic->info[i], "");
      fl_set_input(generic->nbr[i], "");
      fl_set_choice(generic->atom[i], 0);
    }
    
    while(1) {
      for (i = 0; i < 12; i++) {
	j = fl_get_choice(generic->atom[i]) - 1;
	if(j >= 0) {
	  sprintf(buf, "s = %lf, a = %lf, gn = %lf",
		  epr_atoms[j].spin, epr_atoms[j].abundance, epr_atoms[j].gn);
	  fl_set_object_label(generic->info[i], buf);
	}
      }
      tmp = fl_do_forms();
      if(tmp == generic->ok) {
	update_generic(ns);
	fl_hide_form(generic->generic);
	return;
      }
      if(tmp == generic->cancel) {
	fl_hide_form(generic->generic);
	return;
      }
      if(tmp == generic->nextsp) {
	update_generic(ns);
	if(ns < MAXSP-1) ns++;
	break;
      }
      if(tmp == generic->prevsp) {
	update_generic(ns);
	if(ns > 0) ns--;
	break;
      }
    }
  }
}

/*
 * Rate constants (Heinzer.fd)
 *
 */

void do_heinzer() {

  int dummy = 1;
  int i, j, k, l;
  
  fl_show_form(heinzer->heinzer, FL_PLACE_MOUSE, FL_TRANSIENT, "Edit rate constants");

  while (1) {
    if(dummy) {
      for (i = 0; i < 4; i++)
	for(j = 0; j < 4; j++)
	  if(i != j) fl_set_input(heinzer->k[4*i+j], "");
      for(i = 0; i < nspec; i++) {
	for (j = 0; j < nspec; j++) {
	  sprintf(buf, "%.3le", heinzer_k[nspec*i + j]);
	  if(i != j) fl_set_input(heinzer->k[4*i + j], buf);
	}
      } 
    }
    dummy = 0;
    tmp = fl_do_forms();
    if(tmp == heinzer->ok) {
      fl_hide_form(heinzer->heinzer);
      for (i = 0; i < nspec; i++) {
	for(j = 0; j < nspec; j++) {
	  if(i != j) {
	    if(!(str = fl_get_input(heinzer->k[4*i + j])))
	      return; /* FIXME:dialog */
	    heinzer_k[nspec*i + j] = atof(str);
	  }
	}
      }
      return;
    }
    if(tmp == heinzer->cancel) {
      fl_hide_form(heinzer->heinzer);
      return;
    }
    if(tmp == heinzer->zero) {
      bzero(heinzer_k, sizeof(double) * 4 * 4);
      dummy = 1;
    }
  }
}

/*
 * Update asymm. linewidth parameters (see below) 
 *
 */

void update_asymmlw(int page, int ns) {

  int i, j, k, l;

  if(!(str = fl_get_input(linewidth->a))) return; /* FIXME: error dialog */
  sims[ns].lw = atof(str);
  for (i = 0; i < 5; i++) {
    k = 5 * page + i;
    if(!(str = fl_get_input(linewidth->b[i]))) return; /* FIXME ... */
    sims[ns].b[k] = atof(str);
    if(!(str = fl_get_input(linewidth->c[i]))) return; /* FIXME ... */
    sims[ns].c[k][k] = atof(str);
  }
}

/*
 * asymm. linewidth parameters (linewidth.fd)
 *
 */

void do_asymmlw() {

  int page = 0, ns = 0, dummy = 1;
  int i, j, k, l;

  fl_show_form(linewidth->linewidth, FL_PLACE_MOUSE, FL_TRANSIENT,
	       "Asymm. Linewidths");
  while (1) {
    if(dummy) {
      sprintf(buf, "Linewidth parameters for spectrum #%d", ns+1);
      fl_set_object_label(linewidth->header, buf);
      sprintf(buf, "%lf", sims[ns].lw);
      fl_set_input(linewidth->a, buf);
      for (i = 0; i < 5; i++) {
	k = 5 * page + i;
	sprintf(buf, "%d.", k+1);
	fl_set_object_label(linewidth->lab1[i], buf);
	fl_set_object_label(linewidth->lab2[i], buf);
	if(k < sims[ns].ngr) {
	  sprintf(buf, "%lf", sims[ns].b[k]);
	  fl_set_input(linewidth->b[i], buf);
	  sprintf(buf, "%lf", sims[ns].c[k][k]);
	  fl_set_input(linewidth->c[i], buf);
	} else {
	  fl_set_input(linewidth->b[i], "");
	  fl_set_input(linewidth->c[i], "");
	}
      }
      dummy = 0;
    }
    tmp = fl_do_forms();
    if(tmp == linewidth->cancel) {
      fl_hide_form(linewidth->linewidth);
      return;
    }
    if(tmp == linewidth->ok) {
      update_asymmlw(page, ns);
      fl_hide_form(linewidth->linewidth);
      return;      
    }
    if(tmp == linewidth->zero) {
      sims[ns].lw = 0.0;
      for (i = 0; i < sims[ns].ngr; i++) {
	sims[ns].b[i] = 0.0;
	sims[ns].c[i][i] = 0.0;
	for (j = i+1; j < sims[ns].ngr; j++)
	  sims[ns].c[i][j] = 0.0;
      }
      dummy = 1;
    }
    if(tmp == linewidth->nextsp && ns < nspec-1) {
      update_asymmlw(page, ns);
      dummy = 1;
      ns++;
    }
    if(tmp == linewidth->prevsp && ns > 0) {
      update_asymmlw(page, ns);
      dummy = 1;
      ns--;
    }
    if(tmp == linewidth->nextpg && page < MAXSP) {
      update_asymmlw(page, ns);
      dummy = 1;
      page++;
    }
    if(tmp == linewidth->prevpg && page > 0) {
      update_asymmlw(page, ns);
      dummy = 1;
      page--;
    }
    if(tmp == linewidth->editd) {
      if(!(str = fl_show_input("Enter matrix indices i and j", "1 2")))
	continue;
      sscanf(str, "%d %d", &i, &j);
      if(j == i) {
	fl_show_alert("The diagonal elements are contained in vector C.",
		      "","",1);
	continue;
      }
      if(j < i) {
	k = i;
	i = j;
	j = k;
      }
      sprintf(buf, "%lf", sims[ns].c[i-1][j-1]);
      sprintf(buf2, "Enter new value for D[%d][%d]", i, j);
      if(!(str = fl_show_input(buf2, buf))) continue;
      sims[ns].c[i-1][j-1] = atof(str);
    }
  }
}

/*
 * Update equivalences from equivalence.fd dialog box (page = equ_page!)
 *
 */

void update_equivalences(int equ_page) {

  int pos;
  int i, j, k, l;

  for (i = 0, pos = equ_page*7; i < 7; i++, pos++) {
    if(pos >= NEQUIV) return; /* add error dialog later (FIXME) */
    alias_type[pos] = fl_get_choice(equivalence->property[i])-1;
    str = fl_get_input(equivalence->from_set[i]);
    if(!str) break;
    alias_from_set[pos] = atoi(str)-1;
    str = fl_get_input(equivalence->from_index1[i]);
    if(!str) break;
    alias_from_arg1[pos] = atoi(str)-1;
    str = fl_get_input(equivalence->from_index2[i]);
    if(!str) break;
    alias_from_arg2[pos] = atoi(str)-1;
    str = fl_get_input(equivalence->from_index3[i]);
    if(!str) break;
    alias_from_arg3[pos] = atoi(str)-1;
    str = fl_get_input(equivalence->from_index4[i]);
    if(!str) break;
    alias_from_arg4[pos] = atoi(str)-1;

    str = fl_get_input(equivalence->to_set[i]);
    if(!str) break;
    alias_to_set[pos] = atoi(str)-1;
    str = fl_get_input(equivalence->to_index1[i]);
    if(!str) break;
    alias_to_arg1[pos] = atoi(str)-1;
    str = fl_get_input(equivalence->to_index2[i]);
    if(!str) break;
    alias_to_arg2[pos] = atoi(str)-1;
    str = fl_get_input(equivalence->to_index3[i]);
    if(!str) break;
    alias_to_arg3[pos] = atoi(str)-1;
    str = fl_get_input(equivalence->to_index4[i]);
    if(!str) break;
    alias_to_arg4[pos] = atoi(str)-1;
  }
}

/*
 * Update variables from variables.fd dialog box (page = var_page!)
 *
 */

void update_variables(int var_page) {

  int i, j, k, l;
  int pos;
  double *ptr;

  for (i = 0, pos = var_page*10; i < 10; i++, pos++) {
    if(pos >= MAXVAR) return; /* Add error dialog later (FIXME) */
    var_type[pos] = fl_get_choice(variables->type[i])-1;
    str = fl_get_input(variables->set[i]);
    if(!str) break;
    var_set[pos] = atoi(str) - 1;
    str = fl_get_input(variables->arg1[i]);
    if(!str) break;
    var_arg1[pos] = atoi(str) - 1;
    str = fl_get_input(variables->arg2[i]);
    if(!str) break;
    var_arg2[pos] = atoi(str) - 1;
    str = fl_get_input(variables->arg3[i]);
    if(!str) break;
    var_arg3[pos] = atoi(str) - 1;
    str = fl_get_input(variables->arg4[i]);
    if(!str) break;
    var_arg4[pos] = atoi(str) - 1;
    str = fl_get_input(variables->mc_limit[i]);
    if(!str) break;
    var_mclimit[pos] = atof(str);
    str = fl_get_input(variables->high[i]);
    if(!str) break;
    var_high[pos] = atof(str);
    str = fl_get_input(variables->low[i]);
    if(!str) break;
    var_low[pos] = atof(str);
    active[pos] = fl_get_button(variables->active[i]);
    if(var_type[pos] == TYPE_HFC) {
      var_high[pos] = cnv_hfc(var_high[pos], EPR_GE);
      var_low[pos] = cnv_hfc(var_low[pos], EPR_GE);
    }
  }
}

/*
 * Process equivalence dialog (equivalence.fd)
 *
 */

void do_equivalences() {

  int equ_page = 0, update = 1;
  int i, j, k, l, m, n, nel;

  fl_show_form(equivalence->equivalence, FL_PLACE_MOUSE, FL_TRANSIENT,"Equivalence");
  while(1) {
    /* show the current settings */
    for(j = 0, i = equ_page*7; i < (equ_page+1)*7; i++, j++) {
      if(update) {
	sprintf(buf, "%d.", i+1);
	fl_set_object_label(equivalence->lab[j], buf);
	if(alias_type[i] != TYPE_NONE) {
	  fl_set_choice(equivalence->property[j], alias_type[i]+1);
	  sprintf(buf, "%d", alias_from_set[i]+1);
	  fl_set_input(equivalence->from_set[j], buf);
	  sprintf(buf, "%d", alias_from_arg1[i]+1);
	  fl_set_input(equivalence->from_index1[j], buf);
	  sprintf(buf, "%d", alias_from_arg2[i]+1);
	  fl_set_input(equivalence->from_index2[j], buf);
	  sprintf(buf, "%d", alias_from_arg3[i]+1);
	  fl_set_input(equivalence->from_index3[j], buf);
	  sprintf(buf, "%d", alias_from_arg4[i]+1);
	  fl_set_input(equivalence->from_index4[j], buf);
	  sprintf(buf, "%d", alias_to_set[i]+1);
	  fl_set_input(equivalence->to_set[j], buf);
	  sprintf(buf, "%d", alias_to_arg1[i]+1);
	  fl_set_input(equivalence->to_index1[j], buf);
	  sprintf(buf, "%d", alias_to_arg2[i]+1);
	  fl_set_input(equivalence->to_index2[j], buf);
	  sprintf(buf, "%d", alias_to_arg3[i]+1);
	  fl_set_input(equivalence->to_index3[j], buf);
	  sprintf(buf, "%d", alias_to_arg4[i]+1);
	  fl_set_input(equivalence->to_index4[j], buf);
	} else {
	  fl_set_choice(equivalence->property[j], TYPE_NONE+1);
	  fl_set_input(equivalence->from_set[j],"");
	  fl_set_input(equivalence->from_index1[j],"");
	  fl_set_input(equivalence->from_index2[j],"");
	  fl_set_input(equivalence->from_index3[j],"");
	  fl_set_input(equivalence->from_index4[j],"");
	  fl_set_input(equivalence->to_set[j],"");
	  fl_set_input(equivalence->to_index1[j],"");
	  fl_set_input(equivalence->to_index2[j],"");
	  fl_set_input(equivalence->to_index3[j],"");
	  fl_set_input(equivalence->to_index4[j],"");
	}
      }
      if(fl_get_choice(equivalence->property[j])-1 != TYPE_NONE) {
	switch(fl_get_choice(equivalence->property[j])-1) {
	case TYPE_LG:
	case TYPE_AMP:
	case TYPE_T:
	case TYPE_LW:
	case TYPE_SO:
	case TYPE_CRA1:
	case TYPE_CRA2:
	case TYPE_CRA3:
	case TYPE_CRB:
	  fl_set_object_label(equivalence->from_index1[j], "NA");
	  fl_set_object_label(equivalence->to_index1[j], "NA");
	  fl_set_object_label(equivalence->from_index2[j], "NA");
	  fl_set_object_label(equivalence->to_index2[j], "NA");
	  fl_set_object_label(equivalence->from_index3[j], "NA");
	  fl_set_object_label(equivalence->to_index3[j], "NA");
	  fl_set_object_label(equivalence->from_index4[j], "NA");
	  fl_set_object_label(equivalence->to_index4[j], "NA");
	  break;
	case TYPE_B:
	case TYPE_C:
	  fl_set_object_label(equivalence->from_index1[j], "i");
	  fl_set_object_label(equivalence->to_index1[j], "i");
	  fl_set_object_label(equivalence->from_index2[j], "NA");
	  fl_set_object_label(equivalence->to_index2[j], "NA");
	  fl_set_object_label(equivalence->from_index3[j], "NA");
	  fl_set_object_label(equivalence->to_index3[j], "NA");
	  fl_set_object_label(equivalence->from_index4[j], "NA");
	  fl_set_object_label(equivalence->to_index4[j], "NA");
	  break;
	case TYPE_D:
	case TYPE_K:
	  fl_set_object_label(equivalence->from_index1[j], "i");
	  fl_set_object_label(equivalence->to_index1[j], "i");
	  fl_set_object_label(equivalence->from_index2[j], "j");
	  fl_set_object_label(equivalence->to_index2[j], "j");
	  fl_set_object_label(equivalence->from_index3[j], "NA");
	  fl_set_object_label(equivalence->to_index3[j], "NA");
	  fl_set_object_label(equivalence->from_index4[j], "NA");
	  fl_set_object_label(equivalence->to_index4[j], "NA");
	  break;
	case TYPE_POSN:
	  fl_set_object_label(equivalence->from_index1[j], "el");
	  fl_set_object_label(equivalence->to_index1[j], "el");
	  fl_set_object_label(equivalence->from_index2[j], "i");
	  fl_set_object_label(equivalence->to_index2[j], "i");
	  fl_set_object_label(equivalence->from_index3[j], "j");
	  fl_set_object_label(equivalence->to_index3[j], "j");
	  fl_set_object_label(equivalence->from_index4[j], "NA");
	  fl_set_object_label(equivalence->to_index4[j], "NA");
	  break;
	case TYPE_HFC:
	  fl_set_object_label(equivalence->from_index1[j], "el");
	  fl_set_object_label(equivalence->to_index1[j], "el");
	  fl_set_object_label(equivalence->from_index2[j], "nu");
	  fl_set_object_label(equivalence->to_index2[j], "nu");
	  fl_set_object_label(equivalence->from_index3[j], "i");
	  fl_set_object_label(equivalence->to_index3[j], "i");
	  fl_set_object_label(equivalence->from_index4[j], "j");
	  fl_set_object_label(equivalence->to_index4[j], "j");
	  break;
	case TYPE_AD:
	  fl_set_object_label(equivalence->from_index1[j], "el");
	  fl_set_object_label(equivalence->to_index1[j], "el");
	  fl_set_object_label(equivalence->from_index2[j], "el");
	  fl_set_object_label(equivalence->to_index2[j], "el");
	  fl_set_object_label(equivalence->from_index3[j], "i");
	  fl_set_object_label(equivalence->to_index3[j], "i");
	  fl_set_object_label(equivalence->from_index4[j], "j");
	  fl_set_object_label(equivalence->to_index4[j], "j");
	  break;
	case TYPE_AP:
	  fl_set_object_label(equivalence->from_index1[j], "nu");
	  fl_set_object_label(equivalence->to_index1[j], "nu");
	  fl_set_object_label(equivalence->from_index2[j], "i");
	  fl_set_object_label(equivalence->to_index2[j], "i");
	  fl_set_object_label(equivalence->from_index3[j], "j");
	  fl_set_object_label(equivalence->to_index3[j], "j");
	  fl_set_object_label(equivalence->from_index4[j], "NA");
	  fl_set_object_label(equivalence->to_index4[j], "NA");
	  break;
	}
      } else {
	fl_set_object_label(equivalence->from_index1[j], "NA");
	fl_set_object_label(equivalence->to_index1[j], "NA");
	fl_set_object_label(equivalence->from_index2[j], "NA");
	fl_set_object_label(equivalence->to_index2[j], "NA");
	fl_set_object_label(equivalence->from_index3[j], "NA");
	fl_set_object_label(equivalence->to_index3[j], "NA");
	fl_set_object_label(equivalence->from_index4[j], "NA");
	fl_set_object_label(equivalence->to_index4[j], "NA");
      }
    }
    tmp = fl_do_forms();
    update = 0;
    if(tmp == equivalence->cancel) {
      fl_hide_form(equivalence->equivalence);
      return;
    }
    if(tmp == equivalence->ok) {
      fl_hide_form(equivalence->equivalence);
      update_equivalences(equ_page);
      return;
    }
    if(tmp == equivalence->prev) {
      update_equivalences(equ_page);
      equ_page--;
      if(equ_page < 0) equ_page = 0;
      update = 1;
    }
    if(tmp == equivalence->next) {
      update_equivalences(equ_page);
      equ_page++;
      if(6 + equ_page*7 >= NEQUIV) equ_page--;
      update = 1;
    }
    if(tmp == equivalence->clear) {
      clear_equivalences();
      update = 1;
    }
    if(tmp == equivalence->dflis || tmp == equivalence->dflendor) {
      /* position equivalences */
      for (j = k = 0; k < nspec; k++) {
	nel = (int) (sims[k].espin / 0.5);
	for (l = 0; l < nel; l++)
	  for (m = 1; m < 3; m++) {
	    alias_type[j] = TYPE_POSN;
	    alias_from_set[j] = k;
	    alias_from_arg1[j] = l;
	    alias_from_arg2[j] = alias_from_arg3[j] = 0;
	    alias_from_arg4[j] = -1;
	    alias_to_set[j] = k;
	    alias_to_arg1[j] = l;
	    alias_to_arg2[j] = alias_to_arg3[j] = m;
	    alias_to_arg4[j] = -1;
	    j++;
	  }
      }
      /* D equivalences */
      m = fl_get_choice(simparam->hamilton);
      if(m == 3 || m == 5 || m == 7 || m == 9) {
	for (k = 0; k < nspec; k++) {
	  nel = (int) (sims[k].espin / 0.5);
	  for (l = 0; l < nel; l++)
	    for (m = 0; m < nel; m++)
	      for (n = 1; n < 3; n++) {
		alias_type[j] = TYPE_AD;
		alias_from_set[j] = k;
		alias_from_arg1[j] = l;
		alias_from_arg2[j] = m;
		alias_from_arg3[j] = alias_from_arg4[j] = 0;
		alias_to_set[j] = k;
		alias_to_arg1[j] = l;
		alias_to_arg2[j] = m;
		alias_to_arg3[j] = alias_to_arg4[j] = n;
		j++;
	      }
	}
      }
      if(m == 4 || m == 5 || m == 8 || m == 9) {
	/* P equivalences */
	for (k = 0; k < nspec; k++) {
	  for (m = 0; m < sims[k].ngr; m++)
	    for (n = 1; n < 3; n++) {
	      alias_type[j] = TYPE_AP;
	      alias_from_set[j] = k;
	      alias_from_arg1[j] = m;
	      alias_from_arg2[j] = alias_from_arg3[j] = 0;
	      alias_from_arg4[j] = -1;
	      alias_to_set[j] = k;
	      alias_to_arg1[j] = m;
	      alias_to_arg2[j] = alias_to_arg3[j] = n;
	      alias_to_arg4[j] = -1;
	    j++;
	    }
	}
      }
      /* hfc equivalences */
      for (k = 0; k < nspec; k++) {
	nel = (int) (sims[k].espin / 0.5);
	for (l = 0; l < nel; l++)
	  for (m = 0; m < sims[k].ngr; m++)
	    for (n = 1; n < 3; n++) {
	      alias_type[j] = TYPE_HFC;
	      alias_from_set[j] = k;
	      alias_from_arg1[j] = l;
	      alias_from_arg2[j] = m;
	      alias_from_arg3[j] = alias_from_arg4[j] = 0;
	      alias_to_set[j] = k;
	      alias_to_arg1[j] = l;
	      alias_to_arg2[j] = m;
	      alias_to_arg3[j] = alias_to_arg4[j] = n;
	      j++;
	    }
      }
      update = 1;
    }
  }
}

/*
 * Process variables dialog (variables.fd)
 *
 */

void do_variables() {

  int var_page = 0, update = 1;
  int i, j, k, l, m, n, nel, iflag;
  extern int nelws;
  double *ptr;
  char buf[128];

  fl_show_form(variables->variables, FL_PLACE_MOUSE,FL_TRANSIENT,"Variables");
  while(1) {
    /* show the current settings */
    for (j = 0, i = var_page*10; i < (var_page+1)*10; i++, j++) {
      if(update) {
	sprintf(buf, "%d.", i+1);
	fl_set_object_label(variables->lab[j], buf);
	fl_set_button(variables->active[j], active[i]);
	if((ptr = find_param(var_type[i], var_set[i], var_arg1[i], var_arg2[i],
			     var_arg3[i], var_arg4[i])) == NULL) {
	  sprintf(buf, "");
	} else {
	  /* convert to display units */
	  if(var_type[i] == TYPE_HFC || var_type[i] == TYPE_AD ||
	     var_type[i] == TYPE_AP)
	    sprintf(buf, "%le", ucnv_hfc(*ptr, EPR_GE));
	  else
	    sprintf(buf, "%le", *ptr);
	}
	if(var_type[i] != TYPE_NONE) {
	  fl_set_object_label(variables->value[j], buf);
	  fl_set_choice(variables->type[j], var_type[i]+1);
	  sprintf(buf, "%d", var_set[i]+1);
	  fl_set_input(variables->set[j], buf);
	  sprintf(buf, "%d", var_arg1[i]+1);
	  fl_set_input(variables->arg1[j], buf);
	  sprintf(buf, "%d", var_arg2[i]+1);
	  fl_set_input(variables->arg2[j], buf);
	  sprintf(buf, "%d", var_arg3[i]+1);
	  fl_set_input(variables->arg3[j], buf);
	  sprintf(buf, "%d", var_arg4[i]+1);
	  fl_set_input(variables->arg4[j], buf);
	  sprintf(buf, "%3.5lf", var_mclimit[i]);
	  fl_set_input(variables->mc_limit[j], buf);
	  if(var_type[i] == TYPE_HFC || var_type[i] == TYPE_AD ||
	     var_type[i] == TYPE_AP) {
	    sprintf(buf, "%.3le", ucnv_hfc(var_high[i], EPR_GE));
	    fl_set_input(variables->high[j], buf);
	    sprintf(buf, "%.3le", ucnv_hfc(var_low[i], EPR_GE));
	    fl_set_input(variables->low[j], buf);
	  } else {
	    sprintf(buf, "%.3le", var_high[i]);
	    fl_set_input(variables->high[j], buf);
	    sprintf(buf, "%.3le", var_low[i]);
	    fl_set_input(variables->low[j], buf);
	  }
	} else {
	  fl_set_choice(variables->type[j], TYPE_NONE+1);
	  fl_set_input(variables->set[j], "");
	  fl_set_input(variables->arg1[j], "");
	  fl_set_input(variables->arg2[j], "");
	  fl_set_input(variables->arg3[j], "");
	  fl_set_input(variables->arg4[j], "");
	  fl_set_input(variables->mc_limit[j], "");
	  fl_set_input(variables->high[j], "");
	  fl_set_input(variables->low[j], "");	
	}
      }
      if(fl_get_choice(variables->type[j])-1 != TYPE_NONE) {
	switch(fl_get_choice(variables->type[j])-1) {
	case TYPE_NONE:
	case TYPE_LG:
	case TYPE_AMP:
	case TYPE_T:
	case TYPE_LW:
	case TYPE_SO:
	case TYPE_CRA1:
	case TYPE_CRA2:
	case TYPE_CRA3:
	case TYPE_CRB:
	  fl_set_object_label(variables->arg1[j], "NA");
	  fl_set_object_label(variables->arg2[j], "NA");
	  fl_set_object_label(variables->arg3[j], "NA");
	  fl_set_object_label(variables->arg4[j], "NA");
	  break;
	case TYPE_B:
	case TYPE_C:
	case TYPE_ELW:
	case TYPE_EAMP:
	  fl_set_object_label(variables->arg1[j], "i");
	  fl_set_object_label(variables->arg2[j], "NA");
	  fl_set_object_label(variables->arg3[j], "NA");
	  fl_set_object_label(variables->arg4[j], "NA");
	  break;
	case TYPE_D:
	case TYPE_K:
	  fl_set_object_label(variables->arg1[j], "i");
	  fl_set_object_label(variables->arg2[j], "j");
	  fl_set_object_label(variables->arg3[j], "NA");
	  fl_set_object_label(variables->arg4[j], "NA");
	  break;
	case TYPE_POSN:
	  fl_set_object_label(variables->arg1[j], "el");
	  fl_set_object_label(variables->arg2[j], "i");
	  fl_set_object_label(variables->arg3[j], "j");
	  fl_set_object_label(variables->arg4[j], "NA");
	  break;
	case TYPE_HFC:
	  fl_set_object_label(variables->arg1[j], "el");
	  fl_set_object_label(variables->arg2[j], "nu");
	  fl_set_object_label(variables->arg3[j], "i");
	  fl_set_object_label(variables->arg4[j], "j");
	  break;
	case TYPE_AD:
	  fl_set_object_label(variables->arg1[j], "el");
	  fl_set_object_label(variables->arg2[j], "el");
	  fl_set_object_label(variables->arg3[j], "i");
	  fl_set_object_label(variables->arg4[j], "j");
	  break;
	case TYPE_AP:
	  fl_set_object_label(variables->arg1[j], "nu");
	  fl_set_object_label(variables->arg2[j], "i");
	  fl_set_object_label(variables->arg3[j], "j");
	  fl_set_object_label(variables->arg4[j], "NA");
	  break;
	}
      } else {
	fl_set_object_label(variables->arg1[j], "NA");
	fl_set_object_label(variables->arg2[j], "NA");
	fl_set_object_label(variables->arg3[j], "NA");
	fl_set_object_label(variables->arg4[j], "NA");
      }
    }
    tmp = fl_do_forms();
    update = 0;
    if(tmp == variables->cancel) {
      fl_hide_form(variables->variables);
      return;
    }
    if(tmp == variables->ok) {
      fl_hide_form(variables->variables);
      update_variables(var_page);
      return;
    }
    if(tmp == variables->dflendor) {
      /* amps & lws */
      k = 0;
      for(i = 0; i < nelws; i++) {
	var_type[k] = TYPE_ELW;
	var_set[k] = i;
	var_arg1[k] = -1;
	var_arg2[k] = -1;
	var_arg3[k] = -1;
	var_arg4[k] = -1;
	var_high[k] = 2.0;
	var_mclimit[k] = 0.1;
	var_low[k++] = 0.0;
      }
      for(i = 0; i < nelws; i++) {
	var_type[k] = TYPE_EAMP;
	var_set[k] = i;
	var_arg1[k] = -1;
	var_arg2[k] = -1;
	var_arg3[k] = -1;
	var_arg4[k] = -1;
	var_high[k] = 2.0;
	var_mclimit[k] = 0.1;
	var_low[k++] = 0.0;
      }
      for (i = 0; i < sims[0].ngr; i++)
	if(!isalias(TYPE_HFC, 0, 0, i, 0, 0)) {
	  var_type[k] = TYPE_HFC;
	  var_set[k] = 0;
	  var_arg1[k] = 0;
	  var_arg2[k] = i;
	  var_arg3[k] = 0;
	  var_arg4[k] = 0;
	  var_mclimit[k] = 0.03 * *(find_param(var_type[k], var_set[k],
					       var_arg1[k],var_arg2[k],
					       var_arg3[k],var_arg4[k]));
	  var_high[k] = 10.0;
	  if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 4.0;
	  var_low[k++] = 0.0;
	}
      /* ENDOR field offset */
      var_set[k] = -1;
      var_type[k] = TYPE_EOFF;
      var_arg1[k] = -1;
      var_arg2[k] = -1;
      var_arg3[k] = -1;
      var_arg4[k] = -1;
      var_high[k] = 2.0;
      var_mclimit[k] = 0.1;
      var_low[k++] = -2.0;

      var_page = 0;
      update = 1;
    }

    if(tmp == variables->dflan || tmp == variables->dflis) {
      k = 0;
      iflag = (tmp == variables->dflis)?1:0;
      /* Mean lifetime for Norris exchange - common for all */
      /* can't be alias */
      if(fl_get_choice(simparam->lineshape) == 4) {
	var_type[k] = TYPE_T;
	var_set[k] = -1;
	var_arg1[k] = -1;
	var_arg2[k] = -1;
	var_arg3[k] = -1;
	var_arg4[k] = -1;
	var_high[k] = 1.0E10;
	var_mclimit[k] = 1.0E1;
	var_low[k++] = 0.0;
      }
      for (i = 0; i < nspec; i++) {
	if(fl_get_choice(simparam->lineshape) == 3) {
	  /* Anisotropic broadening parameter b */
	  for (j = 0; j < sims[i].ngr; j++) {
	    if(!isalias(TYPE_B, i, j, -1, -1, -1)) {
	      var_type[k] = TYPE_B;
	      var_set[k] = i;
	      var_arg1[k] = j;
	      var_arg2[k] = -1;
	      var_arg3[k] = -1;
	      var_arg4[k] = -1;
	      var_mclimit[k] = 0.003 * *(find_param(var_type[k], var_set[k],
						    var_arg1[k], var_arg2[k],
						    var_arg3[k], var_arg4[k]));
	      var_high[k] = 1.0;
	      if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 40.0;
	      if(var_mclimit[k] < 0.0) var_mclimit[k] = -var_mclimit[k];
	      var_low[k++] = -1.0;
	    }
	  }
	  for (j = 0; j < sims[i].ngr; j++) {
	    if(!isalias(TYPE_C, i, j, -1, -1, -1)) {
	      var_type[k] = TYPE_C;
	      var_set[k] = i;
	      var_arg1[k] = j;
	      var_arg2[k] = -1;
	      var_arg3[k] = -1;
	      var_arg4[k] = -1;
	      var_mclimit[k] = 0.003 * *(find_param(var_type[k], var_set[k],
						    var_arg1[k], var_arg2[k],
						    var_arg3[k], var_arg4[k]));
	      var_high[k] = 1.0;
	      if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 40.0;
	      if(var_mclimit[k] < 0.0) var_mclimit[k] = -var_mclimit[k];
	      var_low[k++] = -1.0;
	    }
	  }
	  for (j = 0; j < sims[i].ngr; j++) {
	    for (l = 0; l < sims[i].ngr; l++) {
	      if(!isalias(TYPE_D, i, j, l, -1, -1)) {
		var_type[k] = TYPE_D;
		var_set[k] = i;
		var_arg1[k] = j;
		var_arg2[k] = l;
		var_arg3[k] = -1;
		var_arg4[k] = -1;
		var_mclimit[k] = 0.003 * *(find_param(var_type[k], var_set[k],
						      var_arg1[k], var_arg2[k],
						      var_arg3[k], var_arg4[k]));
		var_high[k] = 1.0;
		if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 40.0;
		if(var_mclimit[k] < 0.0) var_mclimit[k] = -var_mclimit[k];
		var_low[k++] = -1.0;
	      }
	    }
	  }
	}

	/* lw */
	if(!isalias(TYPE_LW, i, -1, -1, -1, -1)) {
	  var_type[k] = TYPE_LW;
	  var_set[k] = i;
	  var_arg1[k] = -1;
	  var_arg2[k] = -1;
	  var_arg3[k] = -1;
	  var_arg4[k] = -1;
	  var_mclimit[k] = 0.03 * *(find_param(var_type[k], var_set[k],
					       var_arg1[k], var_arg2[k],
					       var_arg3[k], var_arg4[k]));
	  var_high[k] = 5.0;
	  if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 4.0;
	  var_low[k++] = 0.0;
	}

	/* position */
	nel = (int) (sims[i].espin / 0.5);
	if(iflag) {
	  /* to aid isotropic simulations we choose only gxx */
	  /* remember to se equivs to gyy and gzz ! */
	  for (j = 0; j < nel; j++)
	    if(!isalias(TYPE_POSN, i, j, 0, 0, -1)) {
	      var_type[k] = TYPE_POSN;
	      var_set[k] = i;
	      var_arg1[k] = j;
	      var_arg2[k] = 0;
	      var_arg3[k] = 0;
	      var_arg4[k] = -1;
	      var_mclimit[k] = 1.0E-5;
	      var_high[k] = 8.0;
	      var_low[k++] = 0.5;
	    }	  
	} else {
	  for (j = 0; j < nel; j++)
	    for (l = 0; l < 3; l++)
	      for (m = 0; m < 3; m++)
		if(!isalias(TYPE_POSN, i, j, l, m, -1)) {
		  var_type[k] = TYPE_POSN;
		  var_set[k] = i;
		  var_arg1[k] = j;
		  var_arg2[k] = l;
		  var_arg3[k] = m;
		  var_arg4[k] = -1;
		  var_mclimit[k] = 1.0E-5;
		  var_high[k] = 8.0;
		  var_low[k++] = 0.5;
		}
	}

	/* anisotropic S (S.D.S) */
	j = fl_get_choice(simparam->hamilton);
	if(j == 3 || j == 5 || j == 7 || j == 9) {
	  nel = (int) (sims[i].espin / 0.5);
	  if(iflag) {
	    /* isotropic case: Dxx only and equivs to Dyy and Dzz */
	    for (j = 0; j < nel; j++)
	      for (n = 0; n < nel; n++)
		if(!isalias(TYPE_AD, i, j, n, 0, 0)) {
		  var_type[k] = TYPE_AD;
		  var_set[k] = i;
		  var_arg1[k] = j;
		  var_arg2[k] = n;
		  var_arg3[k] = 0;
		  var_arg4[k] = 0;
		  var_mclimit[k] = 0.03 * *(find_param(var_type[k], var_set[k],
						       var_arg1[k],var_arg2[k],
						       var_arg3[k], var_arg4[k]));
		  var_high[k] = 10.0;
		  if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 4.0;
		  var_low[k++] = 0.0;
		}
	  } else {
	    for (j = 0; j < nel; j++)
	      for (n = 0; n < nel; n++)
		for (l = 0; l < 3; l++)
		  for (m = 0; m < 3; m++)
		    if(!isalias(TYPE_AD, i, j, n, l, m)) {
		      var_type[k] = TYPE_AD;
		      var_set[k] = i;
		      var_arg1[k] = j;
		      var_arg2[k] = n;
		      var_arg3[k] = l;
		      var_arg4[k] = m;
		      var_mclimit[k] = 0.03 * *(find_param(var_type[k], var_set[k],
							   var_arg1[k],var_arg2[k],
							   var_arg3[k], var_arg4[k]));
		      var_high[k] = 10.0;
		      if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 4.0;
		      var_low[k++] = 0.0;
		    }
	  }
	}

	/* Anisotropic hyperfine couplings */
	nel = (int) (sims[i].espin / 0.5);
	if(iflag) {
	  /* isotropic: Axx and equivs to Ayy and Azz */
	  for (n = 0; n < nel; n++)
	    for (j = 0; j < sims[i].ngr; j++)
	      if(!isalias(TYPE_HFC, i, n, j, 0, 0)) {
		var_type[k] = TYPE_HFC;
		var_set[k] = i;
		var_arg1[k] = n;
		var_arg2[k] = j;
		var_arg3[k] = 0;
		var_arg4[k] = 0;
		var_mclimit[k] = 0.03 * *(find_param(var_type[k], var_set[k],
						     var_arg1[k],var_arg2[k],
						     var_arg3[k],var_arg4[k]));
		var_high[k] = 10.0;
		if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 4.0;
		var_low[k++] = 0.0;
	      }
	} else {
	  for (n = 0; n < nel; n++)
	    for (j = 0; j < sims[i].ngr; j++)
	      for (l = 0; l < 3; l++)
		for (m = 0; m < 3; m++)
		  if(!isalias(TYPE_HFC, i, n, j, l, m)) {
		    var_type[k] = TYPE_HFC;
		    var_set[k] = i;
		    var_arg1[k] = n;
		    var_arg2[k] = j;
		    var_arg3[k] = l;
		    var_arg4[k] = m;
		    var_mclimit[k] = 0.03 * *(find_param(var_type[k], var_set[k],
							 var_arg1[k],var_arg2[k],
							 var_arg3[k],var_arg4[k]));
		    var_high[k] = 10.0;
		    if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 4.0;
		    var_low[k++] = 0.0;
		  }
	}
	
	j = fl_get_choice(simparam->hamilton);
	if(j == 4 || j == 5 || j == 8 || j == 9) { /* exact only */
	  if(iflag) {
	    /* isotropic: Pxx only and equivs to Pyy, Pzz. */
	    for (j = 0; j < sims[i].ngr; j++)
	      if(!isalias(TYPE_AP, i, j, 0, 0, -1)) {		   
		var_type[k] = TYPE_AP;
		var_set[k] = i;
		var_arg1[k] = j;
		var_arg2[k] = 0;
		var_arg3[k] = 0;
		var_arg4[k] = -1;
		var_mclimit[k] = 0.03 * *(find_param(var_type[k], var_set[k],
						     var_arg1[k],var_arg2[k],
						     var_arg3[k], var_arg4[k]));
		var_high[k] = 10.0;
		if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 4.0;
		var_low[k++] = 0.0;
	      }
	  } else {
	    for (j = 0; j < sims[i].ngr; j++)
	      for (l = 0; l < 3; l++)
		for (m = 0; m < 3; m++)
		  if(!isalias(TYPE_AP, i, j, l, m, -1)) {		   
		    var_type[k] = TYPE_AP;
		    var_set[k] = i;
		    var_arg1[k] = j;
		    var_arg2[k] = l;
		    var_arg3[k] = m;
		    var_arg4[k] = -1;
		    var_mclimit[k] = 0.03 * *(find_param(var_type[k], var_set[k],
							 var_arg1[k],var_arg2[k],
							 var_arg3[k], var_arg4[k]));
		    var_high[k] = 10.0;
		    if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 4.0;
		    var_low[k++] = 0.0;
		  }
	  }
	}

	/* amplitude */
	if(!fl_get_button(simparam->auto_amp)) {
	  if(!isalias(TYPE_AMP, i, -1, -1, -1, -1)) {
	    var_type[k] = TYPE_AMP;
	    var_set[k] = i;
	    var_arg1[k] = -1;
	    var_arg2[k] = -1;
	    var_arg3[k] = -1;
	    var_arg4[k] = -1;
	    var_mclimit[k] = 0.03 * *(find_param(var_type[k], var_set[k],
						 var_arg1[k], var_arg2[k],
						 var_arg3[k], var_arg4[k]));
	    var_high[k] = 60000.0;
	    if(var_mclimit[k] == 0.0) var_mclimit[k] = var_high[k] / 4.0;
	    var_low[k++] = 0.0;
	  }
	}

	/* Exchange rate constants */
	j = fl_get_choice(simparam->lineshape);
	if (j == 5 || j == 6) {
	  for (j = 0; j < nspec; j++) {
	    if(i != j && !isalias(TYPE_K, i, j, -1, -1, -1)) {
	      var_type[k] = TYPE_K;
	      var_set[k] = i;
	      var_arg1[k] = j;
	      var_arg2[k] = -1;
	      var_arg3[k] = -1;
	      var_arg4[k] = -1;
	      var_high[k] = 1.0E10;
	      var_mclimit[k] = 1.0E1;
	      var_low[k++] = 0.0;
	    }
	  }
	}
      }
      var_page = 0;
      update = 1;
    }
    if(tmp == variables->clear) {
      clear_variables();
      update = 1;
    }
    if(tmp == variables->next) {
      update_variables(var_page);
      var_page++;
      if(var_page*10 >= MAXVAR)
	var_page--;
      update = 1;
    }
    if(tmp == variables->prev) {
      update_variables(var_page);
      var_page--;
      if(var_page < 0) var_page = 0;
      update = 1;
    }
  }
  fl_hide_form(variables->variables);
}
