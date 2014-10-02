/*
 * xemr - Xforms interface to libepr.
 *
 * jussi.eloranta@csun.edu
 *
 */

#include <stdio.h>
#include <math.h>
#include <signal.h>
#include <unistd.h>
#include <time.h>
#include <setjmp.h>

#include "xemr.h"

#include "forms.h"
#include "gui.h"
#include "param.h"
#include "mark.h"
#include "polyn.h"
#include "symmetry.h"
#include "calibsig.h"
#include "convol.h"
#include "differences.h"
#include "diffout.h"
#include "position.h"
#include "peakout.h"
#include "convert.h"
#include "linewidth.h"
#include "alinewidth.h"
#include "equivalence.h"
#include "variables.h"
#include "simpar.h"
#include "runsimul.h"
#include "runmon.h"
#include "runsim.h"
#include "misc.h"
#include "hyperfine.h"
#include "generic.h"
#include "elel.h"
#include "quadrupole.h"
#include "selectlw.h"
#include "heinzer.h"
#include "shift.h"
#include "summary.h"
#include "gvalue.h"
#include "elw.h"
#include "runml.h"
#include "spec_setup.h"
#include "window.h"
#include "orbital.h"
#include "cmhz.h"
#include "hj.h"

#include "xemr.xpm"

#include <X11/keysymdef.h>

/* Form pointers */
FD_gui *gui;        /* GUI pointer */
FD_params *params;  /* parameter page pointer */
FD_mark *mark;      /* mark window */
FD_polyn *polyn;    /* polyn degree window */
FD_symmetry *symmetry; /* symmetry dialog */
FD_calibsig *calibsig; /* signal calibration window */
FD_convol *convol;     /* convolution dialog */
FD_differences *differences; /* differences dialog */
FD_diffout *diffout; /* differences output window */
FD_position *position; /* peak position analysis window */
FD_peakout *peakout; /* peak output window */
FD_convert *convert;   /* convert format dialog */
FD_linewidth *linewidth; /* linewidth editor */
FD_equivalence *equivalence; /* equivalence editor */
FD_simparam *simparam;   /* simulation parameters dialog */
FD_variables *variables; /* variables dialog */
FD_runsimul *runsimul;   /* simulation parameter dialog */
FD_runmon *runmon;       /* monte carlo fitting parameter dialog */
FD_runsim *runsim;       /* simplex fitting parameter dialog */
FD_misc *misc;           /* Numerical parameters for libepr */
FD_heinzer *heinzer;     /* Rate constants for Heinzer simulation */
FD_hyperfine *hyperfine; /* hyperfine parameters */
FD_generic *generic;     /* generic Hamilton params */
FD_elel *elel;           /* electron-electron params */
FD_quadrupole *quadrupole; /* nucl. quadrupole */
FD_alinewidth *alinewidth; /* linewidth */
FD_selectlw *selectlw;         /* select dialog */
FD_shift *shift;         /* spectrum shifting dialog */
FD_summary *summary;     /* page summary dialog */
FD_gvalue *gvalue;       /* g matrix dialog */
FD_elw *elw;             /* ENDOR lw & amp */
FD_runml *runml;         /* M-L params */
FD_spec_setup *spec_setup; /* spectrometer calibration setup */
FD_window *window;       /* Windowing dialog */
FD_orbital *orbital;     /* ang. mom. params */
FD_cmhz *cmhz;           /* cm-1 or GHz ? */
FD_hj *hj;               /* Hooke-Jeeves dialog */

/* Global variables */
int page = 0;       /* current spectrum page */
int last_page = 0;
epr_type *sp[MAXSP]; /* spectrum array */
char saved[MAXSP];  /* is the spectrum saved or not */
double c1 = 0.0, c2 = 0.0;      /* cursors */
int overlay[MAXOVER]; /* numbers of spectra to overlay */
double bg_marks[MAXMARKS];
int nbg_marks = 0;
char bg_marks_active = 0, reset_yscale = 0, reset_xscale = 0;
double norris_k, *heinzer_k;
int nspec;           /* Number of spectra */
epr_variable vars[MAXVAR];    /* Variables to be optimized */
epr_sim_set sims[MAXSIMSETS];  /* Simulation sets */

/* variables by page */
int var_type[MAXVAR], var_set[MAXVAR], var_arg1[MAXVAR], var_arg2[MAXVAR],
  var_arg3[MAXVAR], var_arg4[MAXVAR], active[MAXVAR];
double var_mclimit[MAXVAR], var_high[MAXVAR], var_low[MAXVAR];

/* aliases (equivalences) from is the existing var and to is not... */
int alias_type[NEQUIV], alias_from_set[NEQUIV], alias_from_arg1[NEQUIV],
  alias_from_arg2[NEQUIV], alias_from_arg3[NEQUIV], alias_from_arg4[NEQUIV],
  alias_to_set[NEQUIV], alias_to_arg1[NEQUIV], alias_to_arg2[NEQUIV],
  alias_to_arg3[NEQUIV], alias_to_arg4[NEQUIV];

char last_param_file[MAXPATHLEN];
double last_rms = 0.0;
int nvars = 0;
int imode = 0, ipoints = 0;

/* Global "tmp" variables */
static char buf[MAXPATHLEN], buf2[MAXPATHLEN], buf3[MAXPATHLEN], *str3;
static const char *str, *str2;
static double fltmp1, fltmp2, fltmp3;
static FL_OBJECT *tmp, *obj;

/* 1st order endor stuff */
double elws[MAXENDOR];
double amps[MAXENDOR];
int nelws = 0;

double last_endor_b = 0.0, last_triple_b = 0.0;

int cancel_flag = 0;

jmp_buf env;

/*
 * Show interaction.
 *
 */

double interact(int what, char *msg) {

  if(what == 0) {
    fl_show_message(msg, "", "");
    return 0.0;
  }
  if(!(str = fl_show_input(msg, ""))) return 0.0;
  return atof(str);
}

/*
 * Update spectrum during measurement.
 *
 */

int update_spectrum(epr_type *sp, int pos, int sweep) {

  static float x, y;
  double step;
  extern void draw_spectrum();
  int tmp;

  tmp = 4 * sp->samples / 1024;
  if(!tmp) tmp = 1;
  if(!(pos % tmp)) {
    draw_spectrum();
    sprintf(buf, "Scans %d / %d", sweep+1, sp->scans);
    fl_set_object_label(gui->scans, buf);
    (void) fl_check_forms(); 
    if(cancel_flag) {
      tmp = cancel_flag;
      cancel_flag = 0;
      return tmp;
    } else return 0;
  }
  return 0;
}

/*
 * Draw spectrum.
 *
 */

void draw_spectrum() {

  static float x[EPR_MAX_SAMPLES], y[EPR_MAX_SAMPLES];
  float step;
  float wx, wy;
  int i, j, k, l;

  obj = gui->spectrum;
  if(!sp[page]) {
    float x, y;
    fl_set_xyplot_data(obj, &x, &y, 0, "", "X", "I");
    fl_set_object_posthandler(obj, action_spectrum);
    return;
  }
  step = EPR_XSTEP(sp[page]);
  for (i = 0; i < sp[page]->samples; i++) {
    x[i] = ((float) sp[page]->xbeg) + ((float) i)*step;
    y[i] = (float) sp[page]->y[i];
  }
  fl_set_xyplot_data(obj, x, y, i, sp[page]->fname, units(sp[page]->xunits), "I");
  if(reset_xscale) {
    reset_xscale = 0;
    fl_set_xyplot_xbounds(obj, 1.0, 1.0);
  }    
  if(reset_yscale) {
    reset_yscale = 0;
    fl_set_xyplot_ybounds(obj, 1.0, 1.0);
  }
  /* Possible overlay table + maxoverlays is 32 in xyplot! - fix this later */
  /* redraw cursors */
  for (i = 0; i < MAXOVER; i++) {
    fl_delete_xyplot_overlay(obj, i+3);
    if(overlay[i] != -1 && sp[overlay[i]]) {
      k = overlay[i];
      step = EPR_XSTEP(sp[k]);
      for (j = 0; j < sp[k]->samples; j++) {
	x[j] = ((float) sp[k]->xbeg) + ((float) j)*step;
	y[j] = (float) sp[k]->y[j];
      }
      fl_add_xyplot_overlay(obj, i+3, x, y, j, FL_BLACK+i+1);
    }
  }
  fl_set_object_posthandler(obj, action_spectrum);
}

/*
 * Add overlays.
 *
 * NOTE:  Overlay numbers: 1 = left curs, 2 = right curs.
 * TODO: return value ?
 */

int action_spectrum(FL_OBJECT *obj, int ev, FL_Coord mx, FL_Coord my, int key, void *xev) {

  float x, y;
  static float wx, wy, min, max;
  static int speed = 0;
  int i;
  char tbuf[256];
  static double step;

  switch (ev) {
  case FL_KEYBOARD:
    if(key >= XK_F1 && key <= XK_F12) {
      last_page = page;
      page = key - XK_F1;
      if(sp[page]) {
	fl_set_xyplot_xbounds(gui->spectrum, sp[page]->xbeg, sp[page]->xend);
	reset_yscale = 1;
      }
      draw_spectrum();
      sprintf(buf, "Page = %d", page);
      fl_set_object_label(gui->spectrum, buf);
      return;
    }
    if(key == XK_Insert) {
      speed++;
      if(speed > 5) speed = 0;
      sprintf(tbuf, "Speed = %d", speed);
      fl_set_object_label(gui->speed, tbuf);
      if(!sp[page]) return;
      step = EPR_XSTEP(sp[page]) * pow(10.0, (double) speed);
      return;
    }
    if(key == XK_Left) {
      if(!page) return;
      last_page = page;
      page--;
      if(sp[page]) {
	fl_set_xyplot_xbounds(gui->spectrum, sp[page]->xbeg, sp[page]->xend);
	reset_yscale = 1;
      }
      draw_spectrum();
      sprintf(buf, "Page = %d", page);
      fl_set_object_label(gui->spectrum, buf);
      return;
    }
    if(key == XK_Right) {
      if(page == MAXSP-1) return;
      last_page = page;
      page++;
      if(sp[page]) {
	fl_set_xyplot_xbounds(gui->spectrum, sp[page]->xbeg, sp[page]->xend);
	reset_yscale = 1;
      }
      draw_spectrum();
      sprintf(buf, "Page = %d", page);
      fl_set_object_label(gui->spectrum, buf);
      return;
    }
    if(key == XK_Page_Down || key == XK_Down) {
      if(!sp[page]) return;
      fl_get_xyplot_ybounds(gui->spectrum, &min, &max);
      min *= 2.0;
      max *= 2.0;
      fl_set_xyplot_ybounds(gui->spectrum, min, max);      
      return;
    }
    if(key == XK_Page_Up || key == XK_Up) {
      if(!sp[page]) return;
      fl_get_xyplot_ybounds(gui->spectrum, &min, &max);
      min /= 2.0;
      max /= 2.0;
      fl_set_xyplot_ybounds(gui->spectrum, min, max);      
      return;
    }
    if(key == XK_Home) {
      if(!sp[page]) return;
      if(c1 != 0.0 && c2 != 0.0 && c1 < c2) {
	fl_set_xyplot_xbounds(gui->spectrum, c1, c2);
	c1 = c2 = 0.0;
      }
      return;
    }
    if(key == XK_End) {
      if(sp[page])
	fl_set_xyplot_xbounds(gui->spectrum, sp[page]->xbeg, sp[page]->xend);      
      return;
    }
    if(key == XK_Pause) { /* NMR (and MW) fields off */
      /* TODO: instrument_fields_off(); */
      return;
    }
    if(key == 127) { /* this appears to be Del ?! */
      if(!sp[page]) return;
      if(sp[page]->spek_type == 0) {
	last_endor_b = sp[page]->endor_b = c1;
	sprintf(buf, "%le", c1);
	fl_set_input(params->endor_b, buf);
	sprintf(buf, "ENDOR field = %le", c1);
	fl_set_object_label(gui->endor_field, buf);
      } else if(sp[page]->spek_type == 1) {
	last_triple_b = sp[page]->endor_pump_freq = c1;
	sprintf(buf, "%le", c1);
	fl_set_input(params->endor_pump_freq, buf);
	sprintf(buf, "TRIPLE general freq = %le", c1);
	fl_set_object_label(gui->triple_freq, buf);
      }
      return;
    }
#if 0
    /* what is this??? */
    obj = gui->spectrum;
    fl_delete_xyplot_overlay(obj, key);
    fl_get_xyplot_ybounds(obj, &x, &y);
    fl_add_xyplot_overlay(obj, key, &wx, &y, 1, (key==1)?FL_BLUE:FL_GREEN);
    fl_set_xyplot_overlay_type(obj, key, FL_IMPULSE_XYPLOT);
#endif
    break;
  case FL_PUSH:
  case FL_MOUSE:
    if(!sp[page]) return;
    if(key == 2) {
      if(c1 != 0.0 && c2 != 0.0) {
	sprintf(buf, "Diff. = %lf, middle = %lf", fabs(c1-c2), (c1+c2)/2.0);
	fl_xyplot_s2w(obj, mx, my, &wx, &wy);
	fl_show_oneliner(buf, mx + obj->form->x, my + obj->form->y);
      } else fl_xyplot_s2w(obj, mx, my, &wx, &wy);      
    } else {
      double xyzzy; /* A bug in egcs or glibc */
      fl_xyplot_s2w(obj, mx, my, &wx, &wy);
      i = EPR_XTOI(sp[page], wx, EPR_XSTEP(sp[page]));
      if(i < 0) i = 0;
      if(i > sp[page]->samples-1) i = sp[page]->samples-1;
      xyzzy = EPR_M2G((double)wx, sp[page]->frequency);
      sprintf(buf, "x = %f, g = %lf, y = %lf",
	      wx, xyzzy, sp[page]->y[i]);
      fl_show_oneliner(buf, mx + obj->form->x, my + obj->form->y);
    }
    break;
  case FL_RELEASE:
    if(!sp[page]) return;
    fl_hide_oneliner();
    if(key == 1 || key == 3) {
      if (key == 3) {
	key = 2;
	c2 = wx;
      } else c1 = wx;
      fl_delete_xyplot_overlay(obj, key);
      fl_get_xyplot_ybounds(obj, &x, &y);
      fl_add_xyplot_overlay(obj, key, &wx, &y, 1, (key==1)?FL_BLUE:FL_GREEN);
      fl_set_xyplot_overlay_type(obj, key, FL_IMPULSE_XYPLOT);
    } else {
      if(!bg_marks_active)
	return;
      if(nbg_marks < MAXMARKS)
	bg_marks[nbg_marks++] = wx;
      else fprintf(stderr, "Maximum number of marks set.\n");
    }
    break;
  }
  return 0;
}


/*
 * Callback routines for File menu.
 *
 */

void callback_file(FL_OBJECT *obj, long parm) {

  int i, j, k, l;

  str = fl_get_menu_text(obj);
  if(!str) return; /* nothing selected! */

  /* File->New spectrum */
  if(!strcmp(str, "New spectrum")) {
    if(!saved[page])
      if(!fl_show_question("Spectrum on the current page is not saved. Are you sure?", 0)) return;
    saved[page] = 1;
    if(sp[page]) free(sp[page]);
    sp[page] = epr_misc_new();
    strcpy(sp[page]->fname, "noname");
    reset_yscale = reset_xscale = 1;
    c1 = c2 = 0.0;
    draw_spectrum();
    return;
  }

  if(!strcmp(str, "Clear spectrum")) {
    if(!sp[page]) return;
    for (i = 0; i < sp[page]->samples; i++)
      sp[page]->y[i] = 0.0;
    c1 = c2 = 0.0;
    draw_spectrum();
  }

  /* File->New page */
  if(!strcmp(str, "New page")) {
    sprintf(buf, "Enter new page (current = %d): ", page);
    sprintf(buf3,"%d", last_page);
    str2 = fl_show_input(buf, buf3);
    if(!str2) return;
    i = atoi(str2);
    if(i >= 0 && i < MAXSP) {
      last_page = page;
      page = i;
      if(sp[page]) {
	fl_set_xyplot_xbounds(gui->spectrum, sp[page]->xbeg, sp[page]->xend);
	reset_yscale = 1;
      }
      draw_spectrum();
      sprintf(buf, "Page = %d", page);
      fl_set_object_label(gui->spectrum, buf);
    } else fl_show_alert("Invalid page number","","",1);
  }

  /* File->Load template spectrum */
  if(!strcmp(str, "Load template spectrum")) {
    if(!saved[page])
      if(!fl_show_question("Spectrum on the current page is not saved. Are you sure?", 0)) return;
    
    str = fl_show_fselector("Load template spectrum", TEMPLATES, "*.spe", "");
    if(!str) return;
    strcpy(buf, str);
    str3 = buf + strlen(buf) - 4;
    if(!strcmp(str3, ".spe")) *str3 = (char) 0;
    if(sp[page]) free(sp[page]);
    sp[page] = epr_file_read(buf);
    saved[page] = 1;
    reset_xscale = reset_yscale = 1;
    draw_spectrum();
    return;
  }

  /* File->Load spectrum */
  if(!strcmp(str, "Load spectrum")) {
    if(!saved[page])
      if(!fl_show_question("Spectrum on the current page is not saved. Are you sure?", 0)) return;
    
    str = fl_show_fselector("Load spectrum", ".", "*.spe", "");
    if(!str) return;
    strcpy(buf, str);
    str3 = buf + strlen(buf) - 4;
    if(!strcmp(str3, ".spe")) *str3 = (char) 0;
    if(sp[page]) free(sp[page]);
    sp[page] = epr_file_read(buf);
    saved[page] = 1;
    if((str3 = strrchr(buf, '/'))) {
      *str3 = 0;
      chdir(buf);
    }
    reset_xscale = reset_yscale = 1;
    draw_spectrum();
    return;
  }

  /* File->Load many spectra */
  if(!strcmp(str, "Load many spectra")) {
    if(!(str = fl_show_input("Enter base expression for spectra", "")))
      return;
    strcpy(buf, str);
    if((str3 = strchr(buf, '.'))) *str3 = (char) 0;
    i = epr_database_load_many(sp, MAXSP, buf, ".");
    sprintf(buf, "%d spectra loaded on pages 0 thru %d",
	    i, i-1);
    fl_show_message(buf, "","");
    reset_xscale = reset_yscale = 1;
    draw_spectrum();
    return;
  }

  /* File->Save spectrum */
  if(!strcmp(str, "Save spectrum")) {
    if(!sp[page]) {
      fl_show_alert("No spectrum to save!","","",1);
      return;
    }
    strcpy(buf, sp[page]->fname);
    if(*buf) strcat(buf, ".spe");
    str = fl_show_fselector("Save spectrum", ".", "*.spe", buf);
    if(!str) return;
    if(access(str, 0) != -1)
      if(!fl_show_question("File exists. Do you want to overwrite?", 0))
	return;
    strcpy(buf, str);
    str3 = buf + strlen(buf) - 4;
    if(!strcmp(str3, ".spe")) *str3 = (char) 0;
    strcpy(sp[page]->fname, buf);
    if(!epr_file_write(sp[page]))
       fl_show_alert("Save failed!", "", "", 1);
    else saved[page] = 1;    
    if((str3 = strrchr(buf, '/'))) {
      *str3 = 0;
      chdir(buf);
    }
    /* New: automatically convert to bruker format too */
    sprintf(buf, "epr2esp %s %s", sp[page]->fname, sp[page]->fname);
    system(buf);
    return;
  }

  if(!strcmp(str, "Search spectrum")) {
    int hits;
    char buf[128];
    if(sp[0]) {
      fl_show_alert("This function may over write from page 0 ->",
		    "Clear all pages and re-enter.","",1);
      return;
    }
    if(!(str = fl_show_input("Enter directory to search", "")))
      return;
    if(access(str, R_OK|X_OK) < 0) {
      fl_show_alert("Directory not found","","",1);
      return;
    }
    if(!(str2 = fl_show_input("Enter search string", "")))
      return;
    hits = epr_database_lookup(sp, sizeof(sp), (char *) str2, (char *) str);
    sprintf(buf, "%d hits found. Spectra loaded from page 0 ->", hits);
    fl_show_message(buf, "","");
    return;
  }

  if(!strcmp(str, "Convert")) {
    /* dialog: convert.fd */
    fl_show_form(convert->convert, FL_PLACE_MOUSE,FL_TRANSIENT,"Format");
    while(fl_do_forms() != convert->ok);
    fl_hide_form(convert->convert);
    switch (fl_get_choice(convert->format)) {
    case 1:
      if(!(str = fl_show_fselector("Select Bruker ESP file", ".", "*.par","")))
	return;
      strcpy(buf, str);
      if((str3 = strrchr(buf, '.'))) *str3 = (char) 0;
      sprintf(buf2, "esp2epr %s %s", buf, buf);
      system(buf2);
      break;
    case 2:
      if(!(str = fl_show_fselector("Select Bruker Aspect file", ".", "*","")))
	return;
      strcpy(buf, str);
      /*       if((str3 = strrchr(buf, '.'))) *str3 = (char) 0; */
      sprintf(buf2, "aspect2epr %le 3330 3350 %s %s", DEF_FREQ, buf, buf);
      system(buf2);
      break;
    case 3:
      if(!(str = fl_show_fselector("Select EPR-IN file", ".", "*.x","")))
	return;
      strcpy(buf, str);
      if((str3 = strrchr(buf, '.'))) *str3 = (char) 0;
      sprintf(buf2, "pc2epr %s %s", buf, buf);
      system(buf2);
      break;      
    case 4:
      if(!(str = fl_show_fselector("Select ASCII XY-file", ".", "*","")))
	return;
      strcpy(buf, str);
      if((str3 = strrchr(buf, '.'))) *str3 = (char) 0;
      sprintf(buf2, "xy2epr %le %s.xy %s", DEF_FREQ, buf, buf);
      system(buf2);
      break;
    case 5:
      if(!(str = fl_show_fselector("Select ASCII Y-file", ".", "*","")))
	return;
      strcpy(buf, str);
      if((str3 = strrchr(buf, '.'))) *str3 = (char) 0;
      sprintf(buf2, "y2epr %le 3330 3350 %s.y %s", DEF_FREQ, buf, buf);
      system(buf2);
      break;
    }
    fl_show_message("Conversion done.","When loading the file remember to",
		   "check the parameters!");
    return;
  }

  if(!strcmp(str, "Export ASCII file")) {
    if(!sp[page]) {
      fl_show_alert("No spectrum to export!","","",1);
      return;
    }
    strcpy(buf, sp[page]->fname);
    if(*buf) strcat(buf, ".asc");
    str = fl_show_fselector("Save spectrum", ".", "*.asc", buf);
    if(!str) return;
    if(access(str, 0) != -1)
      if(!fl_show_question("File exists. Do you want to overwrite?", 0))
	return;
    epr_file_export((char *) str, sp[page]);
    return;
  }
  
  if(!strcmp(str, "About Xemr")) {
    fl_show_message("Xemr v0.8", "jussi.eloranta@csun.edu","");
    return;
  }
      
  if(!strcmp(str, "Exit Xemr")) {
    for(i = 0; i < MAXSP; i++)
      if(sp[page] && !saved[page]) {
	if(!fl_show_question("Unsaved spectra exist! Do you really want to quit?",0)) return;
	else exit(0);
      }
    exit(0);
  }
}

void init_params() {
  
  double cf, sw;

  strcpy(sp[page]->fname,fl_get_input(params->file));
  strcpy(sp[page]->name,fl_get_input(params->name));
  strcpy(sp[page]->operator, fl_get_input(params->operator));
  strcpy(sp[page]->info, fl_get_input(params->info));
  strcpy(sp[page]->date, fl_get_input(params->date));
  strcpy(sp[page]->time, fl_get_input(params->time));
  sp[page]->harmonic = atoi(fl_get_input(params->harmonic));
  sp[page]->spek_type = fl_get_choice(params->type)-1;
  sp[page]->resonator = atoi(fl_get_input(params->resonator));
  strcpy(sp[page]->solvent, fl_get_input(params->solvent));
  sp[page]->temperature = atof(fl_get_input(params->temp));
  sp[page]->scans = atoi(fl_get_input(params->scans));
  sp[page]->gain = atof(fl_get_input(params->gain));
  sp[page]->timec = atof(fl_get_input(params->timec));
  sp[page]->ctime = atof(fl_get_input(params->ctime));
  sp[page]->modfreq = atof(fl_get_input(params->modf));
  if(sp[page]->modfreq == 0.0) sp[page]->modfreq = 100.0;
  sp[page]->modulampl = atof(fl_get_input(params->moda));
  if(sp[page]->modulampl == 0.0) sp[page]->modulampl = 0.05;
  sp[page]->frequency = atof(fl_get_input(params->freq));
  if(sp[page]->frequency == 0.0) sp[page]->frequency = DEF_FREQ;
  sp[page]->power = atof(fl_get_input(params->power));
  cf = atof(fl_get_input(params->cf));
  sw = atof(fl_get_input(params->sw));
  sp[page]->xbeg = cf - sw / 2.0;
  sp[page]->xend = cf + sw / 2.0;
  sp[page]->endor_b = atof(fl_get_input(params->endor_b));
  sp[page]->endor_mod = atof(fl_get_input(params->endor_mod));
  if(sp[page]->endor_mod == 0.0) sp[page]->endor_mod = 10.0;
  sp[page]->endor_pump_freq = atof(fl_get_input(params->endor_pump_freq));
  switch(fl_get_choice(params->resolution)) {
  case 1:
    sp[page]->samples = 64;
    break;
  case 2:
    sp[page]->samples = 128;
    break;
  case 3:
    sp[page]->samples = 256;
    break;
  case 4:
    sp[page]->samples = 512;
    break;
  case 5:
    sp[page]->samples = 1024;
    break;
  case 6:
    sp[page]->samples = 2048;
    break;
  case 7:
    sp[page]->samples = 4096;
    break;
  case 8:
    sp[page]->samples = 8192;
    break;
  case 9:
    sp[page]->samples = 16384;
    break;
  case 10:
    sp[page]->samples = 32768;
    break;
  case 11:
    sp[page]->samples = 65536;
    break;
  default:
    sp[page]->samples = 1024;
  }
  sp[page]->endor_power = atof(fl_get_input(params->endor_power));
  if(sp[page]->endor_power < 0.0) sp[page]->endor_power = 60.0;
  sp[page]->endor_pump_power = atof(fl_get_input(params->triple_power));
  if(sp[page]->endor_pump_power < 0.0) sp[page]->endor_pump_power = 60.0;
  sp[page]->xunits = fl_get_choice(params->units)-1;
  sp[page]->theta = atof(fl_get_input(params->theta));
  sp[page]->phi = atof(fl_get_input(params->phi));
  sp[page]->sphase = atof(fl_get_input(params->sphase));
}

void callback_edit(FL_OBJECT *obj, long parm) {

  int i, j, k, l;
  double mk, mb, mc;

  str = fl_get_menu_text(obj);
  if(!str) return;
  if(!strcmp(str, "Set center field")) {
    if(c1 == 0.0) return;
    mk = fabs(sp[page]->xend - sp[page]->xbeg);
    sp[page]->xbeg = c1 - mk;
    sp[page]->xend = c1 + mk;
    return;
  }
  if(!strcmp(str, "Set scan width")) {
    if(c1 == 0.0 || c1 == 0.0) return;
    mk = (sp[page]->xend + sp[page]->xbeg) / 2.0;
    mb = fabs(c1 - c2) / 2.0;
    sp[page]->xbeg = mk - mb;
    sp[page]->xend = mk + mb;
    return;
  }

  if(!strcmp(str, "Zero between cursors")) {
    if(c1 != 0.0 && c2 != 0.0)
      if(c1 < c2) {
	epr_misc_zerosp(sp[page], c1, c2);
	draw_spectrum();
	saved[page] = 0;
      }
    return;
  }
  if(!strcmp(str, "Shrink spectrum")) {
    if(c1 != 0.0 && c2 != 0.0 && c1 < c2) {
      int i, j, k;
      j = EPR_XTOI(sp[page], c1, EPR_XSTEP(sp[page]));
      k = EPR_XTOI(sp[page], c2, EPR_XSTEP(sp[page]));
      for (i = j; i <= k; i++)
	sp[page]->y[i-j] = sp[page]->y[i];
      sp[page]->samples = k - j;
      sp[page]->xbeg = c1;
      sp[page]->xend = c2;
      saved[page] = 0;
      draw_spectrum();
    }
    return;
  }
  if(!strcmp(str, "Extend spectrum")) {
    char buf[256];
    if(!sp[page]) return;
    sprintf(buf, "%d", sp[page]->samples);
    if(!(str = fl_show_input("Enter new resolution", buf))) return;
    epr_fft_zfill(sp[page], atoi(str), 1);
    draw_spectrum();
    return;
  }
  if(!strcmp(str, "Copy current page")) {
    str = fl_show_input("Enter target page", "1");
    i = atoi(str);
    if(i == page) {
      fl_show_message("Target page can not be the current page!","","");
      return;
    }
    if(!sp[i]) sp[i] = epr_misc_new();
    epr_misc_copy(sp[page], sp[i]);
    return;
  }
  if(!strcmp(str, "Parameters")) {
    if(!sp[page]) sp[page] = epr_misc_new();
    fl_set_input(params->file, sp[page]->fname);
    fl_set_input(params->name, sp[page]->name);
    fl_set_input(params->operator, sp[page]->operator);
    fl_set_input(params->info, sp[page]->info);
    fl_set_input(params->date, sp[page]->date);
    fl_set_input(params->time, sp[page]->time);
    sprintf(buf, "%d", sp[page]->harmonic);
    fl_set_input(params->harmonic, buf);
    fl_set_choice(params->type, sp[page]->spek_type+1);
    sprintf(buf, "%d", sp[page]->resonator);
    fl_set_input(params->resonator, buf);
    fl_set_input(params->solvent, sp[page]->solvent);
    sprintf(buf, "%lf", sp[page]->temperature);
    fl_set_input(params->temp, buf);
    sprintf(buf,"%d", sp[page]->scans);
    fl_set_input(params->scans, buf);
    sprintf(buf, "%le", sp[page]->gain);
    fl_set_input(params->gain, buf);
    sprintf(buf, "%le", sp[page]->timec);
    fl_set_input(params->timec, buf);
    sprintf(buf, "%le", sp[page]->ctime);
    fl_set_input(params->ctime, buf);
    sprintf(buf, "%lf", sp[page]->modfreq);
    fl_set_input(params->modf, buf);
    sprintf(buf, "%lf", sp[page]->modulampl);
    fl_set_input(params->moda, buf);
    sprintf(buf, "%le", sp[page]->frequency);
    fl_set_input(params->freq, buf);
    sprintf(buf, "%lf", sp[page]->power);
    fl_set_input(params->power, buf);
    mk = (sp[page]->xbeg + sp[page]->xend) / 2.0;
    mb = fabs(sp[page]->xbeg - sp[page]->xend);
    sprintf(buf, "%lf", mk);
    fl_set_input(params->cf, buf);
    sprintf(buf, "%lf", mb);
    fl_set_input(params->sw, buf);
    sprintf(buf, "%lf", sp[page]->endor_b);
    fl_set_input(params->endor_b, buf);
    sprintf(buf, "%lf", sp[page]->endor_mod);
    fl_set_input(params->endor_mod, buf);
    sprintf(buf, "%lf", sp[page]->endor_pump_freq);
    fl_set_input(params->endor_pump_freq, buf);
    switch(sp[page]->samples) {
    case 64:
      fl_set_choice(params->resolution, 1);
      break;
    case 128:
      fl_set_choice(params->resolution, 2);
      break;
    case 256:
      fl_set_choice(params->resolution, 3);
      break;
    case 512:
      fl_set_choice(params->resolution, 4);
      break;
    case 1024:
      fl_set_choice(params->resolution, 5);
      break;
    case 2048:
      fl_set_choice(params->resolution, 6);
      break;
    case 4096:
      fl_set_choice(params->resolution, 7);
      break;
    case 8192:
      fl_set_choice(params->resolution, 8);
      break;
    case 16384:
      fl_set_choice(params->resolution, 9);
      break;
    case 32768:
      fl_set_choice(params->resolution, 10);
      break;
    case 65536:
      fl_set_choice(params->resolution, 11);
      break;
    default:
      fl_set_choice(params->resolution, 5);
    }
    sprintf(buf, "%lf", sp[page]->endor_power);
    fl_set_input(params->endor_power, buf);
    sprintf(buf, "%lf", sp[page]->endor_pump_power);
    fl_set_input(params->triple_power, buf);
    fl_set_choice(params->units, sp[page]->xunits+1);
    sprintf(buf, "%lf", sp[page]->theta);
    fl_set_input(params->theta, buf);
    sprintf(buf, "%lf", sp[page]->phi);
    fl_set_input(params->phi, buf);
    sprintf(buf, "%lf", sp[page]->sphase);
    fl_set_input(params->sphase, buf);
    
    fl_show_form(params->params, FL_PLACE_MOUSE,FL_TRANSIENT,"Parameters");
    while (1) {
      if((tmp = fl_do_forms()) == params->cancel) {
	fl_hide_form(params->params);
	return;
      }
      if(tmp == params->accept) break;
    }
    init_params();    
    fl_hide_form(params->params);
    reset_yscale = reset_xscale = 1;
    draw_spectrum();
    return;
  }
  if(!strcmp(str, "Show page summary")) {
    /* summary.fd */
    fl_clear_browser(summary->txt);
    for (i = 0; i < MAXSP; i++)
      if(sp[i]) {
	sprintf(buf, "Page %d: %s\n", i, sp[i]->fname);
	fl_add_browser_line(summary->txt, buf);
      }
    fl_show_form(summary->summary, FL_FREE_SIZE, FL_TRANSIENT, "Page summary");
    while(fl_do_forms() != summary->ok);
    fl_hide_form(summary->summary);
    return;
  }
  if(!strcmp(str, "Shift spectrum")) {
    double xstep, ystep, istep, z, cur_ypos, cur_int;
    int target;
    epr_type tmpsp;
    str = fl_show_input("Enter target page", "1");
    if(!str) return;
    target = atoi(str);
    if(!sp[target]) return;
    (void) epr_misc_copy(sp[target], &tmpsp);
    fl_show_form(shift->shift, FL_PLACE_MOUSE,FL_TRANSIENT,"Spectrum shift");
    xstep = 10.0 * fabs(sp[target]->xbeg - sp[target]->xend)
      / (double) sp[target]->samples;
    ystep = 0.0;
    for (i = 0; i < sp[target]->samples; i++)
      if((z = fabs(sp[target]->y[i])) > ystep) ystep = z;
    ystep /= 100.0;
    istep = 1.1;
    sprintf(buf, "%lf", xstep);
    fl_set_input(shift->xstep, buf);
    sprintf(buf, "%lf", ystep);
    fl_set_input(shift->ystep, buf);
    sprintf(buf, "%lf", istep);
    fl_set_input(shift->istep, buf);
    cur_ypos = cur_int = 0.0;
    saved[page] = 0;
    while(1) {
      sprintf(buf, "Center B = %lf", (z = fabs(sp[target]->xbeg +
					       sp[target]->xend) / 2.0));
      fl_set_object_label(shift->btxt, buf);
      sprintf(buf, "Center g = %lf", EPR_M2G(z, sp[target]->frequency));
      fl_set_object_label(shift->gtxt, buf);
      sprintf(buf, "Y shift = %lf", cur_ypos);
      fl_set_object_label(shift->ytxt, buf);
      sprintf(buf, "Intensity = %lf", cur_int);
      fl_set_object_label(shift->itxt, buf);
      xstep = atof(fl_get_input(shift->xstep));
      ystep = atof(fl_get_input(shift->ystep));
      istep = atof(fl_get_input(shift->istep));
      if((tmp = fl_do_forms()) == shift->exit) {
	fl_hide_form(shift->shift);
	return;
      }
      if(tmp == shift->reset)
	epr_misc_copy(&tmpsp, sp[target]);
      if(tmp == shift->dxpos) {
	sp[target]->xbeg -= xstep;
	sp[target]->xend -= xstep;
      }
      if(tmp == shift->ixpos) {
	sp[target]->xbeg += xstep;
	sp[target]->xend += xstep;
      }
      if(tmp == shift->iypos) {
	(void) epr_calc_addc(sp[target], ystep);
	cur_ypos += ystep;
      }
      if(tmp == shift->dypos) {
	(void) epr_calc_addc(sp[target], -ystep);
	cur_ypos -= ystep;
      }
      if(tmp == shift->iint) {
	(void) epr_calc_mult(sp[target], istep);
	cur_int += istep;
      }
      if(tmp == shift->dint) {
	if(istep == 0.0) continue;
	(void) epr_calc_mult(sp[target], 1.0 / istep);
	cur_int -= istep;
      }
      draw_spectrum();
    }
    return;
  }
  if(!strcmp(str, "Field calibration")) {
    if(!sp[page]) return;
    str = fl_show_input("(ax^2+bx+c) Enter a: ", "0.0");
    if(!str) return;
    mc = atof(str);
    str = fl_show_input("(ax^2+bx+c) Enter b: ", "1.002429");
    if(!str) return;
    mk = atof(str);
    str = fl_show_input("(ax^2+bx+c) Enter c: ", "-0.07581");
    if(!str) return;
    mb = atof(str);
    sp[page]->xbeg = mc * pow(sp[page]->xbeg,2.0) + mk * sp[page]->xbeg + mb;
    sp[page]->xend = mc * pow(sp[page]->xend,2.0) + mk * sp[page]->xend + mb;
    saved[page] = 0;
    reset_yscale = reset_xscale = 1;
    draw_spectrum();
    return;
  }
}

void callback_view(FL_OBJECT *obj, long parm) {

  int i, j, k, l;
  
  str = fl_get_menu_text(obj);
  if(!str) return;
  if(!strcmp(str, "ZoomX")) {
    if(c1 != 0.0 && c2 != 0.0 && c1 < c2) {
      fl_set_xyplot_xbounds(gui->spectrum, c1, c2);
      c1 = c2 = 0.0;
    }
    return;
  }
  if(!strcmp(str, "UnzoomX")) {
    if(sp[page])
      fl_set_xyplot_xbounds(gui->spectrum, sp[page]->xbeg, sp[page]->xend);
    return;
  }
  if(!strcmp(str, "ZoomY")) {
    float min, max;
    fl_get_xyplot_ybounds(gui->spectrum, &min, &max);
    min /= 2.0;
    max /= 2.0;
    fl_set_xyplot_ybounds(gui->spectrum, min, max);
    return;
  }
  if(!strcmp(str, "UnzoomY")) {
    reset_yscale = 1;
    draw_spectrum();
    return;
  }
  if(!strcmp(str, "Add overlay")) {
    int i,j;
    str2 = fl_show_input("Enter page number to add: ", "0");
    if(!str2) return;
    i = atoi(str2);
    if(i >= 0 && i < EPR_MAX_SAMPLES) {
      for(j = 0; j < MAXOVER; j++)
	if(overlay[j] == -1) break;
      if(j == MAXOVER) {
	fl_show_alert("Overlay table full!","","",1);
	return;
      }
      overlay[j] = i;
      draw_spectrum();
      return;
    } else {
      fl_show_alert("Invalid page number","","",1);
      return;
    }
  }
  if(!strcmp(str, "Delete overlay")) {
    int i,j;
    str2 = fl_show_input("Enter page number to delete (all = -1): ", "0");
    if(!str2) return;
    i = atoi(str2);
    for(j = 0; j < MAXOVER; j++)
      if(i == overlay[j] || i == -1) overlay[j] = -1;
#if 0
    if(i == -1)
      fl_show_alert("Overlay table deleted.","","",1);
#endif
    draw_spectrum();
    return;
  }
}

double width, cst;

double step_func(double x, epr_type *sp) {

  double center;

  center = bg_marks[0];
  if(x < center - width / 2.0 || x > center + width / 2.0)
    return 0.0;
  return cst;
}

double friedrich_func(double x, epr_type *sp) {

  double center;

  center = bg_marks[0];
  if(x < center - width / 2.0 || x > center + width / 2.0)
    return 0.0;
  return (cst * exp(-1.0/(1.0 - x*x)));
}

void callback_calc(FL_OBJECT *obj, long param) {

  double x, y, z, z2;
  int i, j, k, l;

  str = fl_get_menu_text(obj);
  if(!str) return;

  if(!strcmp(str, "Calc LW B")) {
    str2 = fl_show_input("Enter magnetic field (in G): ", "3300");
    if(!str2) return;
    x = atof(str2);
    str2 = fl_show_input("Enter value of Tr(g.A) (in G): ", "");
    if(!str2) return;
    y = atof(str2);
    str2 = fl_show_input("Enter microwave frequency (in GHz): ", "9");
    if(!str2) return;
    z = atof(str2) * 1.0E9;
    str2 = fl_show_input("Enter the value of the constant B (in G): ", "");
    if(!str2) return;
    z2 = atof(str2);
    z2 = epr_calc_rot_b(sp[page],z2, y, z, x);
    sprintf(buf, "Rotation correlation = %le\n", z2);
    fl_show_message(buf, "", "");
    return;
  }
  if(!strcmp(str, "Calc LW C")) {
    str2 = fl_show_input("Enter value of Tr(A.A) (in G^2): ", "");
    if(!str2) return;
    y = atof(str2);
    str2 = fl_show_input("Enter microwave frequency (in GHz): ", "9");
    if(!str2) return;
    z = atof(str2) * 1.0E9;
    str2 = fl_show_input("Enter the value of the constant C (in G): ","");
    if(!str2) return;
    z2 = atof(str2);
    z2 = epr_calc_rot_c(sp[page],z2, y, z);
    sprintf(buf, "Rotation correlation = %le\n", z2);
    fl_show_message(buf, "", "");
    return;
  }
  if(!sp[page]) return;
  if(!strcmp(str, "Add spectra direct")) {
    str2 = fl_show_input("Enter page for second spectrum: ", "1");
    if(!str2) return;
    i = atoi(str2);
    if(!sp[i]) {
      fl_show_alert("Page empty!","","",1);
      return;
    }
    str2 = fl_show_input("Enter multiplier for second spectrum: ", "-1.0");
    if(!str2) return;
    x = atof(str2);
    x = epr_calc_add(sp[page],sp[i], x, 1);
    draw_spectrum();
    sprintf(buf, "RMS difference = %lf",x);
    fl_show_message(buf, "", "");
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "Add spectra")) {
    str2 = fl_show_input("Enter page for second spectrum: ", "1");
    if(!str2) return;
    i = atoi(str2);
    if(!sp[i]) {
      fl_show_alert("Page empty!","","",1);
      return;
    }
    str2 = fl_show_input("Enter multiplier for second spectrum: ", "-1.0");
    if(!str2) return;
    x = atof(str2);
    x = epr_calc_add(sp[page],sp[i], x, 2);
    draw_spectrum();
    sprintf(buf, "RMS difference = %lf",x);
    fl_show_message(buf, "", "");
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "Multiply by constant")) {
    str2 = fl_show_input("Enter constant multiplier: ", "-1.0");
    if(!str2) return;
    x = atof(str2);
    epr_calc_mult(sp[page], x);
    draw_spectrum();
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "Add constant")) {
    str2 = fl_show_input("Enter constant: ", "0");
    if(!str2) return;
    x = atof(str2);
    epr_calc_addc(sp[page], x);
    draw_spectrum();
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "Differentiate")) {
    epr_calc_diff(sp[page]);
    draw_spectrum();
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "Integrate")) {
    epr_calc_integ(sp[page]);
    draw_spectrum();
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "9 point smooth")) {
    epr_smooth_9pt(sp[page]);
    saved[page] = 0;
    draw_spectrum();
    return;
  }
  if(!strcmp(str, "Symmetry smooth")) {
    fl_show_form(symmetry->symmetry,FL_FREE_SIZE,FL_TRANSIENT,"Symmetry");
    bg_marks_active = 1;
    nbg_marks = 0;
    while(1) {
      if((tmp = fl_do_forms()) == symmetry->cancel) {
	fl_hide_form(symmetry->symmetry);
	return;
      }
      if(tmp == symmetry->ok) break;
    }
    bg_marks_active = 0;
    fl_hide_form(symmetry->symmetry);
    epr_smooth_symmetry(sp[page], bg_marks[0]);
    nbg_marks = 0;
    saved[page] = 0;
    draw_spectrum();
    return;
  }
  if(!strcmp(str, "Forward FFT")) {
    /* epr_fft_fill(sp[page], 0.0); */
    epr_fft(sp[page]);
    saved[page] = 0;
    draw_spectrum();
    return;
  }
  if(!strcmp(str, "Power spectrum")) {
    if(!sp[page]) return;
    epr_fft_power(sp[page]);
    saved[page] = 0;
    draw_spectrum();
    return;
  }
  if(!strcmp(str, "Reverse FFT")) {
    /* epr_fft_fill(sp[page], 0.0); */
    epr_fft_inv(sp[page]);
    saved[page] = 0;
    draw_spectrum();
    return;
  }  
  if(!strcmp(str, "Windowing")) {
    fl_show_form(window->window, FL_PLACE_MOUSE, FL_TRANSIENT, "Windowing");
    while(1) {
      if((tmp = fl_do_forms()) == window->cancel) {
	fl_hide_form(window->window);
	return;
      }
      if(tmp == window->ok) break;
    }
    fl_hide_form(window->window);
    (void) epr_fft_window(sp[page], fl_get_choice(window->type) - 1,
			 atof(fl_get_input(window->parametera)),
			 atof(fl_get_input(window->parameterb)));
    saved[page] = 0;
    draw_spectrum();
    return;
  }
  if(!strcmp(str, "Convolution")) {
    epr_sim_set temp;
    /* dialog: convol.fd */
    fl_show_form(convol->convol, FL_PLACE_MOUSE,FL_TRANSIENT,"Convolution");
    while(1) {
      if((tmp = fl_do_forms()) == convol->cancel) {
	fl_hide_form(convol->convol);
	return;
      }
      if(tmp == convol->ok) break;
    }
    fl_hide_form(convol->convol);
    j =  fl_get_choice(convol->type);
    switch(j) {
    case 1: /* Lorentz (absorption) lineshape */
      temp.lg_fact = 1.0;
      str2 = fl_show_input("Enter linewidth (G): ","");
      if(!str2) return;
      temp.lw = atof(str2);
      temp.dlevel = EPR_ABSORPTION;
      (void *) epr_line_create_fft(sp[page], &temp, 0);
      break;
    case 2: /* Gaussian (absorption) lineshape */
      temp.lg_fact = 0.0;
      str2 = fl_show_input("Enter linewidth (G): ","");
      if(!str2) return;
      temp.lw = atof(str2);
      temp.dlevel = EPR_ABSORPTION;
      (void *) epr_line_create_fft(sp[page], &temp, 0);
      break;
    case 3: /* Another spectrum */
      str2 = fl_show_input("Enter page for 2nd spectrum", "0");
      if(!str2) return;
      i = atoi(str2);
      if(i < 0 || i >= MAXSP) {
	fl_show_message("Page number too large.","","");
	return;
      }
      if(sp[i]) epr_conv(sp[page], sp[i]);
      break;
    }
    draw_spectrum();
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "Cross/Autocorrelation")) {
    str2 = fl_show_input("Enter page for 2nd spectrum", "0");
    if(!str2) return;
    i = atoi(str2);
    if(i < 0 || i >= MAXSP) {
      fl_show_message("Page number too large.","","");
      return;
    }
    if(sp[i]) epr_cross(sp[page], sp[i]);
    draw_spectrum();
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "Cepstral analysis")) {
    if(!sp[page]) return;
    epr_cepstr(sp[page]);
    draw_spectrum();
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "Auto scale")) {
    if(!sp[page]) return;
    (void) epr_misc_scale(sp[page], 16383.0);
    draw_spectrum();
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "RMS difference")) {
    str2 = fl_show_input("Enter page for 2nd spectrum", "0");
    if(!str2) return;
    i = atoi(str2);
    if(i < 0 || i >= MAXSP) {
      fl_show_message("Page number too large.","","");
      return;
    }
    if(sp[i]) {
      sprintf(buf, "RMS difference = %le\n", epr_misc_drms(sp[page], sp[i]));
      fl_show_message(buf, "", "");
    }
  }
}

void callback_baseline(FL_OBJECT *obj, long param) {
  
  int degree, marks[MAXMARKS];
  double step;
  int i, j, k, l;

  str = fl_get_menu_text(obj);
  if(!str) return;
  
  if(!sp[page]) return;
  if(!strcmp(str, "Calculate noise level")) {
    step = epr_peak_nlevel(sp[page]);
    sprintf(buf, "Noise level = %lf", step);
    fl_show_message(buf, "","");
    return;
  }
  if(!strcmp(str, "Mean value subtract")) {
    epr_peak_baseline(sp[page]);
    saved[page] = 0;
    draw_spectrum();
    return;
  }
  if(!strcmp(str, "Spline correct")) {
    fl_show_form(mark->mark, FL_FREE_SIZE,FL_TRANSIENT,"Mark");
    bg_marks_active = 1;
    nbg_marks = 0;
    while(fl_do_forms() != mark->ok);
    fl_hide_form(mark->mark);
    bg_marks_active = 0;
    if(nbg_marks == 0) return;
    step = EPR_XSTEP(sp[page]);
    for (i = 0; i < nbg_marks; i++)
      marks[i] = EPR_XTOI(sp[page], bg_marks[i], step);
    (void) epr_base_spline(sp[page], nbg_marks, marks);
    draw_spectrum();
    nbg_marks = 0;
    saved[page] = 0;
    return;
  }
  if(!strcmp(str, "Polynomial correct")) {
    fl_show_form(mark->mark, FL_FREE_SIZE, FL_TRANSIENT,"Mark");
    bg_marks_active = 1;
    nbg_marks = 0;
    while(fl_do_forms() != mark->ok);
    fl_hide_form(mark->mark);
    bg_marks_active = 0;
    if(nbg_marks == 0) return;
    fl_set_input(polyn->degree, "1");
    fl_show_form(polyn->polyn, FL_FREE_SIZE, FL_TRANSIENT,"Degree");
    while((tmp = fl_do_forms()) != polyn->cont)
      if(tmp == polyn->cancel) {
	nbg_marks = 0;
	fl_hide_form(polyn->polyn);
	return;
      }
    fl_hide_form(polyn->polyn);
    degree = atoi(fl_get_input(polyn->degree));
    if(degree < 0 || degree > 9 || degree > nbg_marks) {
      fl_show_message("Invalid input values.","","");
      nbg_marks = 0;
      return;
    }
    step = EPR_XSTEP(sp[page]);
    for (i = 0; i < nbg_marks; i++)
      marks[i] = EPR_XTOI(sp[page], bg_marks[i], step);
    if(nbg_marks < degree+1) {
      fl_show_message("Insufficient number of points.","","");
      nbg_marks = 0;
      return;
    }
    (void) epr_base_polyn(sp[page], degree, marks);
    draw_spectrum();
    nbg_marks = 0;
    saved[page] = 0;
    return;
  }
}

void callback_measure(FL_OBJECT *obj, long param) {

  char buf[256];
  FL_OBJECT *timer;
  float x[EPR_MAX_SAMPLES], y[EPR_MAX_SAMPLES];
  double step, delay;
  int i, j, k, l;

  str = fl_get_menu_text(obj);
  if(!str) return;

  if(!strcmp(str, "Measure EPR")) {
    if(!sp[page]) {
      sp[page] = epr_misc_new();
      init_params();
    }
    if(sp[page]->spek_type != 0) {
      fl_show_message("Wrong mode in spectral parameters.", "", "");
      return;
    }
    if(sp[page]->xbeg == 0.0 || sp[page]->xend == 0.0) {
      fl_show_message("Field begin/end are zero?","","");
      return;
    }
    if(sp[page]->scans == 0) {
      fl_show_message("Number of scans is zero?","","");
      return;
    }
    if(sp[page]->samples == 0) {
      fl_show_message("Number of data points is zero?","","");
      return;
    }

    delay = atof(fl_get_input(params->delay)); /* in sec */
    if(delay > 0.0 && sp[page]->xbeg != sp[page]->xend) {
      l = page + sp[page]->scans; /* scan to subsequent pages */
      sp[page]->scans = 1;
    } else l = page + 1;
   
    fl_show_object(gui->mprog);
    for (k = page ; k < l; k++) {
      if(k > page) {
	if(!sp[k]) sp[k] = epr_misc_new();
	epr_misc_copy(sp[page], sp[k]);
      }
      epr_misc_zerosp(sp[k], 0.0, 0.0);	
      sprintf(buf, "Page = %d (of %d)", k, l);
      fl_set_object_label(gui->spectrum, buf);
      step = EPR_XSTEP(sp[k]);
      for (i = 0; i < sp[k]->samples; i++)
	x[i] = (float) EPR_ITOX(sp[k],i,step);
      bzero(y, sizeof(float) * sp[k]->samples);
      fl_set_xyplot_data(gui->spectrum, x, y, sp[k]->samples,sp[k]->fname, 
			 units(sp[k]->xunits), "I");    

      fl_set_xyplot_ybounds(gui->spectrum,-100.0,100.0);
      measure_epr(sp[k], update_spectrum, interact);
      draw_spectrum();
      sleep(delay);
    }
    fl_hide_object(gui->mprog);
    sprintf(buf, "Page = %d", l-1);
    fl_set_object_label(gui->spectrum, buf);
    return;
  }

  if(!strcmp(str, "Measure ENDOR")) {
    if(!sp[page]) {
      sp[page] = epr_misc_new();
      init_params();
    }

    if(sp[page]->spek_type != 1) {
      fl_show_message("Wrong mode in spectral parameters.", "", "");
      return;
    }
    if(sp[page]->endor_b < 500.0) {
      fl_show_message("ENDOR magnetic field < 500 Gauss?","","");
      return;
    }
    if(sp[page]->scans == 0) {
      fl_show_message("Number of scans is zero?","","");
      return;
    }
    if(sp[page]->samples == 0) {
      fl_show_message("Number of data points is zero?","","");
      return;
    }
    if(sp[page]->xbeg == 0.0 || sp[page]->xend == 0.0) {
      fl_show_message("RF field begin/end are zero?","","");
      return;
    }

    delay = atof(fl_get_input(params->delay)); /* in sec */
    if(delay > 0.0 && sp[page]->xbeg != sp[page]->xend) {
      l = page + sp[page]->scans; /* scan to subsequent pages */
      sp[page]->scans = 1;
    } else l = page + 1;
   
    fl_show_object(gui->mprog);
    for (k = page; k < l; k++) {
      if(k > page) {
	if(!sp[k]) sp[k] = epr_misc_new();
	epr_misc_copy(sp[page], sp[k]);
      }
      epr_misc_zerosp(sp[k], 0.0, 0.0);	
      sprintf(buf, "Page = %d (of %d)", k, l);
      fl_set_object_label(gui->spectrum, buf);
      step = EPR_XSTEP(sp[k]);
      for (i = 0; i < sp[k]->samples; i++)
	x[i] = (float) EPR_ITOX(sp[k],i,step);
      bzero(y, sizeof(float) * sp[k]->samples);
      fl_set_xyplot_data(gui->spectrum, x, y, sp[k]->samples,sp[k]->fname, 
			 units(sp[k]->xunits), "I");    
      
      fl_set_xyplot_ybounds(gui->spectrum,-100.0,100.0);
      measure_endor(sp[k], update_spectrum, interact);
      draw_spectrum();
      sleep(delay);
    }
    fl_hide_object(gui->mprog);
    sprintf(buf, "Page = %d", l-1);
    fl_set_object_label(gui->spectrum, buf);
    return;
  }

  /* Note: No point in running TRIPLE in kinetics mode */
  if(!strcmp(str, "Measure TRIPLE general")) {
    if(!sp[page]) {
      sp[page] = epr_misc_new();
      init_params();
    }
    if(sp[page]->spek_type != 2) {
      fl_show_message("Wrong mode in spectral parameters.", "", "");
      return;
    }
    if(sp[page]->endor_b < 500.0) {
      fl_show_message("ENDOR magnetic field < 500 Gauss?","","");
      return;
    }
    if(sp[page]->scans == 0) {
      fl_show_message("Number of scans is zero?","","");
      return;
    }
    if(sp[page]->samples == 0) {
      fl_show_message("Number of data points is zero?","","");
      return;
    }
    if(sp[page]->xbeg == 0.0 || sp[page]->xend == 0.0) {
      fl_show_message("RF field begin/end are zero?","","");
      return;
    }
    if(sp[page]->endor_pump_freq <= 0.0) sp[page]->endor_pump_freq = last_triple_b;
    step = EPR_XSTEP(sp[page]);
    for (i = 0; i < sp[page]->samples; i++)
      x[i] = (float) EPR_ITOX(sp[page],i,step);
    bzero(y, sizeof(float) * sp[page]->samples);
    fl_set_xyplot_data(gui->spectrum, x, y, sp[page]->samples,sp[page]->fname, 
		       units(sp[page]->xunits), "I");    

    fl_set_xyplot_ybounds(gui->spectrum,-100.0,100.0);
    fl_show_object(gui->mprog);
    measure_tripleg(sp[page], update_spectrum, interact);
    fl_hide_object(gui->mprog);
    draw_spectrum();
    return;
  }

  /* Note: No point in running TRIPLE in kinetics mode */
  if(!strcmp(str, "Measure TRIPLE special")) {
    if(!sp[page]) {
      sp[page] = epr_misc_new();
      init_params();
    }
    if(sp[page]->spek_type != 3) {
      fl_show_message("Wrong mode in spectral parameters.", "", "");
      return;
    }
    if(sp[page]->endor_b < 500.0) {
      fl_show_message("ENDOR magnetic field < 500 Gauss?","","");
      return;
    }
    if(sp[page]->scans == 0) {
      fl_show_message("Number of scans is zero?","","");
      return;
    }
    if(sp[page]->samples == 0) {
      fl_show_message("Number of data points is zero?","","");
      return;
    }
    if(sp[page]->xbeg == 0.0 || sp[page]->xend == 0.0) {
      fl_show_message("RF field begin/end are zero?","","");
      return;
    }
    if(sp[page]->endor_b < 10.0) sp[page]->endor_b = last_endor_b;
    step = EPR_XSTEP(sp[page]);
    for (i = 0; i < sp[page]->samples; i++)
      x[i] = (float) EPR_ITOX(sp[page],i,step);
    bzero(y, sizeof(float) * sp[page]->samples);
    fl_set_xyplot_data(gui->spectrum, x, y, sp[page]->samples,sp[page]->fname, 
		       units(sp[page]->xunits), "I");    

    fl_set_xyplot_ybounds(gui->spectrum,-100.0,100.0);
    fl_show_object(gui->mprog);
    measure_triples(sp[page], update_spectrum, interact);
    fl_hide_object(gui->mprog);
    draw_spectrum();
    return;
  }

  /* NOTE: no point in running EIE in kinetics mode */
  if(!strcmp(str, "Measure EIE")) {
    if(!sp[page]) {
      sp[page] = epr_misc_new();
      init_params();
    }
    if(sp[page]->spek_type != 0) {
      fl_show_message("Wrong mode in spectral parameters.", "", "");
      return;
    }
    if(sp[page]->endor_b < 500.0) {
      fl_show_message("ENDOR magnetic field < 500 Gauss?","","");
      return;
    }
    if(sp[page]->scans == 0) {
      fl_show_message("Number of scans is zero?","","");
      return;
    }
    if(sp[page]->samples == 0) {
      fl_show_message("Number of data points is zero?","","");
      return;
    }
    if(sp[page]->xbeg == 0.0 || sp[page]->xend == 0.0) {
      fl_show_message("RF field begin/end are zero?","","");
      return;
    }
    if(sp[page]->endor_pump_freq <= 0.0) sp[page]->endor_pump_freq = last_triple_b;
    step = EPR_XSTEP(sp[page]);
    for (i = 0; i < sp[page]->samples; i++)
      x[i] = (float) EPR_ITOX(sp[page],i,step);
    bzero(y, sizeof(float) * sp[page]->samples);
    fl_set_xyplot_data(gui->spectrum, x, y, sp[page]->samples,sp[page]->fname, 
		       units(sp[page]->xunits), "I");    
    
    fl_set_xyplot_ybounds(gui->spectrum,-100.0,100.0);
    fl_show_object(gui->mprog);
    measure_eie(sp[page], update_spectrum, interact);
    fl_hide_object(gui->mprog);
    draw_spectrum();
    return;
  }
  
  if(!strcmp(str, "Spectrometer setup")) {
    spectrometer_setup();
    return;
  }

  if(!strcmp(str, "FF lock on")) {
    /* TODO */
#if 0
   double dum1, dum2;
    sp_meas_open_magnet(&dum1, &dum2);
    sp_meas_ioctl_magnet(1, (void *) 0);    
    sp_meas_close_magnet();
#endif
    return;
  }

  if(!strcmp(str, "FF lock off")) {
    /* TODO */
#if 0
    int dum3;
    double dum1, dum2;
    sp_meas_open_magnet(&dum1, &dum2);
    sp_meas_ioctl_magnet(2, (void *) 0);
    sp_meas_close_magnet();
    sp_meas_open_mwfreq();
    dum3 = 63;
    sp_meas_ioctl_mwfreq(1, (void *) (&dum3));
    sp_meas_close_mwfreq();
    sp_meas_open_nmr();
    sp_meas_ioctl_nmr(0, 1, 63);
    sp_meas_ioctl_nmr(1, 1, 63);
    sp_meas_ioctl_nmr(2, 1, 63);
    sp_meas_close_nmr();
#endif
    return;
  }

  if(!strcmp(str, "Cancel delayed")) {    
    cancel_flag = 1;
    return;
  }
  if(!strcmp(str, "Cancel now")) {
    cancel_flag = 2;
    return;
  }
}

void callback_peak(FL_OBJECT *obj, long param) {

  double peaks[2048], widths[2048];
  double x, y, z, diffs[2048];
  int npeaks, ndiffs, dfound[2048];
  int i, j, k, l;

  str = fl_get_menu_text(obj);
  if(!str) return;
  if(!sp[page]) return;
  if(!strcmp(str, "Differences")) {
    fl_set_input(differences->resolution, "0.1");
    x = fabs(sp[page]->xend - sp[page]->xbeg) / 2.0;
    sprintf(buf,"%lf", x);
    fl_set_input(differences->maxhfc, buf);
    sprintf(buf, "2.0");
    fl_set_input(differences->noiselimit,buf);
    /* dialog: differences.fd */
    fl_show_form(differences->differences, FL_FREE_SIZE,FL_TRANSIENT,"Differences");
    while(1) {
      if((tmp = fl_do_forms()) == differences->cancel) {
	fl_hide_form(differences->differences);
	return;
      }
      if(tmp == differences->ok) break;
    }
    fl_hide_form(differences->differences);
    x = atof(fl_get_input(differences->resolution));
    y = atof(fl_get_input(differences->maxhfc));
    z = atof(fl_get_input(differences->noiselimit));
    if(x < 0.0 || y < 0.0 || z < 0.0) {
      fl_show_message("Invalid input parameters.","","");
      fl_hide_form(differences->differences);
      return;
    }
    npeaks = epr_peak_find(sp[page], peaks, widths, 2048, z, fl_get_choice(differences->method)-1);
    sprintf(buf, "Total of %d peaks found.", npeaks);
    fl_show_message(buf, "", "");
    ndiffs = epr_peak_diffs(peaks, npeaks, diffs, dfound, 2048, x, y);
    sprintf(buf, "Total of %d differences found.", ndiffs);
    fl_show_message(buf, "", "");
    /* dialog: diffout.fd */
    fl_clear_chart(diffout->results);
    for (i = 0; i < ndiffs; i++) {
      sprintf(buf,"%.2lf",diffs[i]);
      fl_add_chart_value(diffout->results,(double) dfound[i], buf, i % 10);
    }
    fl_show_form(diffout->diffout, FL_PLACE_MOUSE|FL_FREE_SIZE,FL_TRANSIENT,"Differences");
    while(fl_do_forms() != diffout->ok);
    fl_hide_form(diffout->diffout);
    return;
  }
  if(!strcmp(str, "Position analysis")) {
    /* peakout.fd */
    sprintf(buf, "2.0");
    fl_set_input(position->noiselevel,buf);
    fl_show_form(position->position,FL_FREE_SIZE,FL_TRANSIENT,"Peak position");
    while(fl_do_forms() != position->ok);
    fl_hide_form(position->position);
    z = atof(fl_get_input(position->noiselevel));
    npeaks = epr_peak_find(sp[page], peaks, widths, 2048, z, fl_get_choice(position->method)-1);
    sprintf(buf, "Total of %d peaks found.", npeaks);
    fl_show_message(buf, "", "");
    fl_clear_browser(peakout->results);
    sprintf(buf, "Peak\t\tMiddle\t\tWidth");
    fl_add_browser_line(peakout->results, buf);
    x = EPR_XSTEP(sp[page]);
    y = 0.0;
    for (i = 0; i < npeaks; i++) {
      sprintf(buf, "%d\t%.2lf\t%.2lf", i+1,
	      peaks[i], widths[i]);
      fl_add_browser_line(peakout->results, buf); 
      y += peaks[i];
    }
    y /= (double) npeaks;
    sprintf(buf, "Symmetry error = %lf %", epr_peak_symm(peaks, npeaks, y) * 100.0);
    fl_add_browser_line(peakout->results, buf);
    fl_show_form(peakout->peakout, FL_FREE_SIZE,FL_TRANSIENT,"Peak analysis");
    while(fl_do_forms() != peakout->ok);
    fl_hide_form(peakout->peakout);
    return;
  }
  if(!strcmp(str, "Double integrate")) {
    if(c1 > c2 || c1 == 0.0 || c2 == 0.0) {
      fl_show_message("Select integration area with cursors first.","","");
      return;
    }
    x = epr_peak_int2(sp[page], c1, c2, 0);
    sprintf(buf, "Integration result = %lf", x);
    fl_show_message(buf, "", "");
    return;
  }
  if(!strcmp(str, "Single integrate")) {
    if(c1 > c2 || c1 == 0.0 || c2 == 0.0) {
      fl_show_message("Select integration area with cursors first.","","");
      return;
    }
    x = epr_peak_int1(sp[page], c1,c2);  
    sprintf(buf, "Integration result = %lf", x);
    fl_show_message(buf, "", "");
    return;
  }
}

void *setup_simparam(epr_sim_set *sims, int *ri, int *rj, int *e, int *t) {

  double fltmp1, fltmp2, fltmp3;
  int i, j, k, syst, fict, lb, le;
  void *prm = NULL;

  syst = fl_get_choice(simparam->system);
  if(syst > 4) {
    fict = 1;
    syst -= 4;
  } else fict = 0;

  fltmp1 = M_PI * atof(fl_get_input(simparam->theta)) / 180.0;
  fltmp2 = M_PI * atof(fl_get_input(simparam->phi)) / 180.0;
  j = fl_get_button(simparam->auto_amp);
  k = fl_get_choice(simparam->dlevel);
  if(k == 4) {
    k = 1;
    epr_leave_fft = 1;
  } else epr_leave_fft = 0;
  if(fl_get_button(simparam->active)) {
    lb = atoi(fl_get_input(simparam->begin));
    le = atoi(fl_get_input(simparam->end));
  } else {
    lb = -1;
    le = -1;
  }
  if(fl_get_button(simparam->boltzmann)) 
    fltmp3 = atof(fl_get_input(simparam->temp));
  else
    fltmp3 = -1.0;

  for (i = 0; i < nspec; i++) {
    sims[i].dlevel = k;
    sims[i].auto_amp = j;
    sims[i].theta = fltmp1;
    sims[i].phi = fltmp2;
    sims[i].begin = lb;
    sims[i].end = le;
    sims[i].spin_temp = fltmp3;
  }
  
  for (i = 0; i < nelws; i++) {
    sims[0].e_ilw[i] = elws[i];
    sims[0].e_iamp[i] = amps[i];
  }
  
  switch (fl_get_choice(simparam->lineshape)) {
  case 1:
    i = EPR_LS_STICK;
    break;
  case 2:
    i = EPR_LS_NORMAL;
    break;
  case 3:
    i = EPR_LS_ASYMMLW;
    break;
  case 4:
    i = EPR_LS_NORRIS;
    prm = (void *) &norris_k;
    break;
  case 5:
    i = EPR_LS_HEINZER_INTRA;
    prm = (void *) heinzer_k;
    break;
  case 6:
    i = EPR_LS_DDEP;
    break;
  }
  
  if(syst == 2 || syst == 4) i += EPR_LS_POWDER;
  if(syst > 2 && fl_get_choice(simparam->lineshape) > 2) {
    fl_show_message("Only stick and normal lineshapes are available for ENDOR.",
		    "","");
    return (void *) -1;
  }
  
  j = fl_get_choice(simparam->hamilton);
  
  if(j > 1) {
    switch(j) {
    case 2: /* g.B.S + A.S.I */
      *e = EPR_HM_GBS | EPR_HM_ASI;
      break;
    case 3: /* g.B.S + A.S.I + S.D.S */
      *e = EPR_HM_GBS | EPR_HM_ASI | EPR_HM_SDS;
      break;
    case 4: /* g.B.S + A.S.I + I.P.I */
      *e = EPR_HM_GBS | EPR_HM_ASI | EPR_HM_IPI;
      break;
    case 5: /* g.B.S + A.S.I + S.D.S + I.P.I */
      *e = EPR_HM_GBS | EPR_HM_ASI | EPR_HM_IPI | EPR_HM_SDS;
      break;
    case 6: /* g.B.S + gN.B.I + A.S.I */
      *e = EPR_HM_GBS | EPR_HM_ASI | EPR_HM_GBI;
      break;
    case 7: /* g.B.S + GN.B.I + A.S.I + S.D.S */
      *e = EPR_HM_GBS | EPR_HM_ASI | EPR_HM_GBI | EPR_HM_SDS;
      break;
    case 8: /* g.B.S + GN.B.I + A.S.I + I.P.I */
      *e = EPR_HM_GBS | EPR_HM_ASI | EPR_HM_GBI | EPR_HM_IPI;
      break;
    case 9: /* g.B.S + GN.B.I + A.S.I + I.P.I + S.D.S */
      *e = EPR_HM_GBS | EPR_HM_ASI | EPR_HM_GBI | EPR_HM_IPI | EPR_HM_SDS;
      break;
    }
    switch(fl_get_choice(simparam->transition)) {
    case 1: /* B1.Sx */
      *t = EPR_TR_S_90;
      break;
    case 2: /* -B1.Ix */
      *t = EPR_TR_I_90;
      break;
    case 3: /* both of above */
      *t = EPR_TR_S_90 | EPR_TR_I_90;
      break;
    case 4: /* B1.Sz */
      *t = EPR_TR_S_0;
      break;
    case 5: /* -B1.Iz */
      *t = EPR_TR_I_0;
      break;
    case 6: /* both of above */
      *t = EPR_TR_S_0 | EPR_TR_I_0;
      break;
    case 7:
      *t = EPR_TR_CONSTANT | EPR_TR_S_90;
      break;
    case 8:
      *t = EPR_TR_CONSTANT | EPR_TR_S_0;
      break;
    }
    if(syst > 2)
      j = EPR_SM_EXACT_ENDOR; /* endor SM_EXACT_ENDOR */
    else j = EPR_SM_EXACT; /* epr SM_EXACT */
  } else { /* 1st order */
    if(syst < 3) j = EPR_SM_1STORDER;
    if(syst > 2) j = EPR_SM_1STENDOR;
  }
  if(fict) *e |= EPR_HM_FICTIOUS;
  /* orbital params */
  switch(fl_get_choice(simparam->orbital)) {
  case 1:
    break;
  case 2:
    *e |= EPR_HM_BL;
    break;
  case 3:
    *e |= EPR_HM_BL | EPR_HM_CRYSTAL;
    break;
  case 4:
    *e |= EPR_HM_BL | EPR_HM_SL;
    break;
  case 5:
    *e |= EPR_HM_BL | EPR_HM_SL | EPR_HM_CRYSTAL;
    break;
  case 6:
    *e |= EPR_HM_SL;
    break;
  case 7:
    *e |= EPR_HM_SL | EPR_HM_CRYSTAL;
    break;
  case 8:
    *e |= EPR_HM_CRYSTAL;
    break;
  }
  epr_simulation_model = i;
  epr_simulation_type = j;
  *ri =i;
  *rj = j;
  epr_simulation_hamilton = *e;
  epr_simulation_transition = *t;

  return prm;
}


void callback_simulate(FL_OBJECT *obj, long param) {

  int n, npeaks, target, lw_x, lw_y, rest;
  epr_type tmp_sp;
  double z, width;
  int i, j, k, l, w, e, t, syst;
  void *prm;
  clock_t ini, final;

  str = fl_get_menu_text(obj);
  if(!str) return;

  /* Simulate->Run simulation */
  if(!strcmp(str, "Run simulation")) {
    if(!nspec) return;
    target = atoi(fl_get_input(simparam->target));
    if(target < 0 || target >= MAXSP) {
      fl_show_alert("Invalid target page number","","",1);
      return;
    }
    if(sp[target])
      if(!fl_show_question("Overwrite spectrum on target page?",0)) return;
    if(sp[target]) free(sp[target]);
    sp[target] = epr_misc_new();
    reset_yscale = reset_xscale = 1;
    syst = fl_get_choice(simparam->system);
    if(syst > 4) syst -= 4;
    /* simulation */
    if(fl_get_choice(simparam->run) == 1) {
      width = atof(fl_get_input(runsimul->width));
      strcpy(sp[target]->fname, "Simulation");
      sp[target]->frequency = atof(fl_get_input(runsimul->frequency));
      if(sp[target]->frequency < 100.0) sp[target]->frequency = DEF_FREQ;
      if(syst > 2) { /* endor ? */
	z = atof(fl_get_input(runsimul->center));
	sp[target]->xunits = 2; /* MHz */
      } else {
	z = EPR_G2M(atof(fl_get_input(runsimul->center)), sp[target]->frequency);
	sp[target]->xunits = 0; /* G */
      }
      sp[target]->xbeg = z - width/2.0;
      sp[target]->xend = z + width/2.0;
      if(sp[target]->xbeg < 0.0) sp[target]->xbeg = 0.0;
      sp[target]->samples = atoi(fl_get_input(runsimul->resolution));
      sp[target]->endor_b = atof(fl_get_input(runsimul->endor_b));
      bzero(sp[target]->y, sp[target]->samples*sizeof(double));

      if((prm = setup_simparam(sims, &i, &j, &e, &t)) == (void *) -1) return;

      ini = clock();
      if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
      npeaks = epr_misc_create_sp(sp[target], sims, nspec, j, i, prm, e, t, imode, ipoints);
      final = (clock() - ini) / CLOCKS_PER_SEC;
      if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;

      sprintf(buf, "%d peaks generated (cpu = %lu s).", npeaks, final);
      fl_show_message(buf,"","");
    } else { /* fitting */
      if(!sp[page]) {
	fl_show_alert("Experimental spectrum page empty!","","",1);
	return;
      }
      if(target == page) {
	fl_show_alert("Experimental and simulated pages cannot be the same.",
		      "","",1);
	return;
      }
      /* update variables & aliases from var_* -> vars[] */
      convert_variables();
      convert_aliases();
      if(sp[page]->frequency < 100.0) sp[page]->frequency = DEF_FREQ;

      if((prm = setup_simparam(sims, &i, &j, &e, &t)) == (void *) -1) return;
      epr_simulation_imode = imode;
      epr_simulation_npt = ipoints;
      epr_optim_debug = 1;
      if(epr_hamilton_lfollow) epr_hamilton_lfollow = 2;
      switch(fl_get_choice(simparam->run)) {
      case 2:
	/* Monte Carlo */
	str = fl_get_input(runmon->iterations);
	if(!str) return;
	epr_optim_maxit = (double) atoi(str);
	ini = clock();
	last_rms = epr_fit_sp(sp[page],sp[target],sims,nspec,vars, nvars,0,prm);
	sprintf(buf, "MC RMS error = %lf (cpu %lu s)", last_rms,
		(clock() - ini)/CLOCKS_PER_SEC);
	fl_show_message(buf, "", "");
	break;
      case 3:
	/* Simplex */
	str = fl_get_input(runsim->iterations);
	if(!str) return;
	epr_optim_maxit = (double) atoi(str);
	str = fl_get_input(runsim->restarts);
	if(!str) return;
	rest = atoi(str);
	ini = clock();
	for(i = 0; i < rest; i++) {
	  last_rms = epr_fit_sp(sp[page], sp[target], sims, nspec, vars, nvars, 3, prm);
	  for (k = 0; k < MAXOVER; k++) 
	    overlay[k] = -1;
	  overlay[0] = target;
	  draw_spectrum();
	  printf("Restart #%d finished (cumulative cpu %lu s).\n", i+1,
		 (clock()-ini)/CLOCKS_PER_SEC);
	}
	sprintf(buf, "SIMPLEX RMS error = %lf (total cpu %lu s)", last_rms,
		(clock()-ini)/CLOCKS_PER_SEC);
	fl_show_message(buf, "", "");
	break;
	/* L-M */
      case 4:
	epr_fit_mltol = atof(fl_get_input(runml->tolerance));
	epr_fit_difl = atof(fl_get_input(runml->diff));
	ini = clock();
	sprintf(buf, "M-L RMS error = %lf (cpu %lu s)", (last_rms = epr_fit_sp(sp[page], sp[target], sims, nspec,
								  vars, nvars, 4, prm)), (clock()-ini)/CLOCKS_PER_SEC);
	fl_show_message(buf, "", "");
	break;
      case 5: /* run Hooke-Jeeves */
	epr_fit_difl = atof(fl_get_input(hj->diff));
	epr_optim_maxit = atoi(fl_get_input(hj->maxiter));
	ini = clock();
	sprintf(buf, "H-J RMS error = %lf (cpu %lu s)",
		(last_rms = epr_fit_sp(sp[page], sp[target], sims, nspec,
				      vars, nvars, 5, prm)), (clock()-ini)/CLOCKS_PER_SEC);
	fl_show_message(buf, "", "");
	break;
      }
      if(epr_hamilton_lfollow) epr_hamilton_lfollow = 1;
      
      for (j = 0; j < MAXOVER; j++) 
	overlay[j] = -1;
      overlay[0] = target;
      for (i = 0; i < nelws; i++) {
	elws[i] = sims[0].e_ilw[i];
	amps[i] = sims[0].e_iamp[i];
      }
    }
    reset_yscale = reset_xscale = 1;
    draw_spectrum();
    return;
  }

  /* Simulate->Read parameters from disk */
  if(!strcmp(str, "Read parameters from disk")) {
    str = fl_show_fselector("Load simulation parameters", ".", "*.sps","");
    if(!str) return;
    strcpy(last_param_file, str);
    read_param(str);
    return;
  }

  /* Simulate->Define simulation */
  if(!strcmp(str, "Define simulation")) {
    /* dialog simpar.fd */
    fl_show_form(simparam->simparam,FL_PLACE_MOUSE,FL_TRANSIENT,"Define Simulation");
    do {
      j = fl_get_choice(simparam->system);
      if(j > 4) j -= 4; /* we dont need fictious stuff here */
      if(j == 1 || j == 3) {
	fl_show_object(simparam->phi);
	fl_show_object(simparam->theta);
      } else {
	fl_hide_object(simparam->phi);
	fl_hide_object(simparam->theta);
      }	
      if(fl_get_button(simparam->active)) {
	fl_show_object(simparam->begin);
	fl_show_object(simparam->end);
      } else {
	fl_hide_object(simparam->begin);
	fl_hide_object(simparam->end);	
      }
      if(fl_get_button(simparam->boltzmann))
	fl_show_object(simparam->temp);
      else
	fl_hide_object(simparam->temp);
      if((tmp = fl_do_forms()) == simparam->cancel) {
	fl_hide_form(simparam->simparam);
	return;
      }
      if(fl_get_choice(simparam->hamilton) == 1) {
	fl_set_choice(simparam->transition, 7);
	fl_set_input(generic->espin, "0.5");
      }
    } while(tmp != simparam->ok);
    fl_hide_form(simparam->simparam);
    str = fl_get_input(simparam->nspec);
    if(!str) nspec = 0;
    nspec = atoi(str);

    return;
  }

  /* Simulate->Edit run parameters */
  if(!strcmp(str, "Edit run parameters")) {
    switch(fl_get_choice(simparam->run)-1) {
    case 0: /* simulation */
      /* dialog runsimul.fd */
      fl_show_form(runsimul->runsimul,FL_PLACE_MOUSE,FL_TRANSIENT,"Run:Simulation");
      do {
	if((tmp = fl_do_forms()) == runsimul->cancel) {
	  fl_hide_form(runsimul->runsimul);
	  return;
	}
      } while(tmp != runsimul->ok);
      fl_hide_form(runsimul->runsimul);
      return;
    case 1: /* Fit Monte Carlo */
      /* dialog runmon.fd */
      fl_show_form(runmon->runmon,FL_PLACE_MOUSE,FL_TRANSIENT,"Run:Monte Carlo");
      do {
	if((tmp = fl_do_forms()) == runmon->cancel) {
	  fl_hide_form(runmon->runmon);
	  return;
	}
	if(tmp == runmon->variables) {
	  /* show variables.fd */
	  do_variables();
	}
	if(tmp == runmon->equivalences) {
	  /* show equivalence.fd */
	  do_equivalences();
	}
      } while (tmp != runmon->ok);
      fl_hide_form(runmon->runmon);
      return;
    case 2: /* Fit simplex */
      /* dialog runsim.fd */
      fl_show_form(runsim->runsim,FL_PLACE_MOUSE,FL_TRANSIENT,"Run:Simplex");
      do {
	if((tmp = fl_do_forms()) == runsim->cancel) {
	  fl_hide_form(runsim->runsim);
	  return;
	}
	if(tmp == runsim->variables) {
	  /* show variables.fd */
	  do_variables();
	}
	if(tmp == runsim->equivalences) {
	  /* show equivalence.fd */
	  do_equivalences();
	}
      } while (tmp != runsim->ok);
      fl_hide_form(runsim->runsim);
      return;
    case 3: /* Fit marquardt */
      /* dialog runml.fd */
      fl_show_form(runml->runml,FL_PLACE_MOUSE,FL_TRANSIENT,"Run:Marquardt");
      do {
	if((tmp = fl_do_forms()) == runml->cancel) {
	  fl_hide_form(runml->runml);
	  return;
	}
	if(tmp == runml->variables) {
	  /* show variables.fd */
	  do_variables();
	}
	if(tmp == runml->equivalences) {
	  /* show equivalence.fd */
	  do_equivalences();
	}
      } while (tmp != runml->ok);
      fl_hide_form(runml->runml);
      epr_fit_mltol = atof(fl_get_input(runml->tolerance));
      epr_fit_difl = atof(fl_get_input(runml->diff));
      return;
    case 4: /* Hooke-Jeeves */
      /* dialog hj.fd */
      fl_show_form(hj->hj,FL_PLACE_MOUSE,FL_TRANSIENT,"Run:H-J");
      do {
	if((tmp = fl_do_forms()) == hj->cancel) {
	  fl_hide_form(hj->hj);
	  return;
	}
	if(tmp == hj->variables) {
	  /* show variables.fd */
	  do_variables();
	}
	if(tmp == hj->equivalences) {
	  /* show equivalence.fd */
	  do_equivalences();
	}
      } while (tmp != hj->ok);
      fl_hide_form(hj->hj);
      epr_fit_difl = atof(fl_get_input(hj->diff));
      epr_optim_maxit = atoi(fl_get_input(hj->maxiter));
      return;
    }
  }

  /* Simulate->Edit numerical parameters */
  if(!strcmp(str, "Edit numerical parameters")) {
    /* Dialog misc.fd */
    int tmp;
    sprintf(buf, "%lf", epr_line_cutoff);
    fl_set_input(misc->linecutoff, buf);
    sprintf(buf, "%d", epr_phi);
    fl_set_input(misc->phisteps, buf);
    sprintf(buf, "%d", epr_theta);
    fl_set_input(misc->thetasteps, buf);
    sprintf(buf, "%d", epr_hamilton_divs);
    fl_set_input(misc->diags, buf);
    tmp = (int) (0.5 + 2.0 * epr_phi_begin / M_PI) + 1;
    fl_set_choice(misc->phibegin, tmp);
    tmp = (int) (0.5 + 2.0 * epr_phi_end / M_PI) + 1;
    fl_set_choice(misc->phiend, tmp);
    tmp = (int) (0.5 + 2.0 * epr_theta_begin / M_PI) + 1;
    fl_set_choice(misc->thetabegin, tmp);
    tmp = (int) (0.5 + 2.0 * epr_theta_end / M_PI) + 1;
    fl_set_choice(misc->thetaend, tmp);
    fl_set_choice(misc->method, imode);
    fl_set_choice(misc->points, ipoints-1);
    sprintf(buf, "%d", epr_powder_fm);
    fl_set_input(misc->interp, buf);
    fl_set_choice(misc->follow, epr_hamilton_lfollow+1);

    fl_show_form(misc->misc,FL_PLACE_MOUSE,FL_TRANSIENT,"Numerical parameters");
    while (fl_do_forms() != misc->ok);
    fl_hide_form(misc->misc);

    if(!(str = fl_get_input(misc->linecutoff))) return;
    epr_line_cutoff = atof(str);

    if(!(str = fl_get_input(misc->phisteps))) return;
    epr_phi = atoi(str);

    if(!(str = fl_get_input(misc->thetasteps))) return;
    epr_theta = atoi(str);

    epr_phi_begin = ((double) (fl_get_choice(misc->phibegin) - 1)) * M_PI / 2.0;
    epr_phi_end = ((double) (fl_get_choice(misc->phiend) - 1)) * M_PI / 2.0;
    epr_theta_begin = ((double) (fl_get_choice(misc->thetabegin) - 1)) * M_PI / 2.0;
    epr_theta_end = ((double) (fl_get_choice(misc->thetaend) - 1)) * M_PI / 2.0;
    imode = fl_get_choice(misc->method);
    ipoints = fl_get_choice(misc->points) + 1;
    
    if(!(str = fl_get_input(misc->diags))) return;
    epr_hamilton_divs = atoi(str);

    if(!(str = fl_get_input(misc->interp))) return;
    epr_powder_fm = atoi(str);

    epr_hamilton_lfollow = fl_get_choice(misc->follow) - 1;

    return;
  }

  /* Simulate->Edit spin paramters */
  if(!strcmp(str, "Edit spin parameters")) {
    do_generic();
    return;
  }

  /* Simulate->Edit electron g parameters */
  if(!strcmp(str, "Edit electron g parameters")) {
    do_gvalue();
    return;
  }

  /* Simulate->Edit lineshape parameters */
  if(!strcmp(str, "Edit lineshape parameters")) {
    if(fl_get_choice(simparam->system) == 3 &&
       fl_get_choice(simparam->hamilton) == 1) {
      do_elw();
      return;
    }
    switch(fl_get_choice(simparam->lineshape)) {
    case 1: /* stick */
      return; /* no parameters */
    case 2: /* Lorentz/Gauss */
    case 6: /* field dir. dep. lw */
      do_alinewidth();
      return;
    case 3: /* asymm. lw */
      /* dialog: linewidth.fd */
      do_asymmlw();
      return;
    case 4:
      /* select.fd */
      fl_show_form(selectlw->selectlw, FL_PLACE_MOUSE, FL_TRANSIENT,
		   "Which parameters?");
      while(1) {
	tmp = fl_do_forms();
	if(tmp == selectlw->rate) {
	  sprintf(buf, "%le", norris_k);
	  str = fl_show_input("Enter average rate constant:", buf);
	  if(!str) {
	    fl_hide_form(selectlw->selectlw);
	    return;
	  }
	  norris_k = atof(str);
	  fl_hide_form(selectlw->selectlw);
	  return;
	}
	if(tmp == selectlw->linewidth) {
	  fl_hide_form(selectlw->selectlw);
	  do_alinewidth();
	  return;
	}
      }
    case 5:
      /* select.fd */
      fl_show_form(selectlw->selectlw, FL_PLACE_MOUSE, FL_TRANSIENT,
		   "Which parameters?");
      while(1) {
	tmp = fl_do_forms();
	if(tmp == selectlw->rate) {
	  fl_hide_form(selectlw->selectlw);
	  /* heinzer.fd */
	  do_heinzer();
	  return;
	}
	if(tmp == selectlw->linewidth) {
	  fl_hide_form(selectlw->selectlw);
	  /* alinewidth.fd */
	  do_alinewidth();
	  return;
	}
      }
    }
  }

  if(!strcmp(str, "Edit hyperfine parameters")) {
    do_hyperfine();
    return;
  }

  if(!strcmp(str, "Edit electron-electron parameters")) {
    do_elel();
    return;
  }

  if(!strcmp(str, "Edit nuclear quadrupole parameters")) {
    do_quadrupole();
    return;
  }

  if(!strcmp(str, "Edit orbital parameters")) {
    do_orbital();
    return;
  }

  if(!strcmp(str, "Write parameters to disk")) {
    if(!nspec) return;
    str = fl_show_fselector("Save fitting parameters",".","*.sps",last_param_file);
    if(!str) return;
    strcpy(buf, str);
    str3 = buf + strlen(buf) - 4;
    if(strcmp(str3, ".sps")) strcat(buf, ".sps");
    if(access(buf, 0) != -1)
      if(!fl_show_question("File exists. Do you want to overwrite?",0))
	return;
    write_param(buf);
  }

  if(!strcmp(str, "Print parameters")) {
    if(!nspec) return;
    print_param();    
    return;
  }
  if(!strcmp(str, "Clear parameters")) {
    last_rms = 0.0;
    strcpy(last_param_file, "");
    for(i = 0; i < nspec; i++) bzero(&sims[i], sizeof(epr_sim_set));
    nspec = 0;
    for(i = 0; i < MAXVAR; i++) active[i] = 1;
    return;
  }
}

void callback_graph(FL_OBJECT *obj, long param) {

  char cmd[2*MAXPATHLEN];
  const char *str;
  FILE *fp;
  double step;
  int i, j, k, l;

  str = fl_get_menu_text(obj);
  if(!str) return;

  /* TODO: use the graphics routines in libmeas */
  if(!strcmp(str, "Execute Grace for all pages")) {
    strcpy(cmd, GRAPH);
    strcat(cmd, " ");
    for (i = j = 0; i < MAXSP; i++)
      if(sp[i]) {
	sprintf(buf, "/tmp/%d.%d", getuid(), j);
	strcat(cmd, " ");
	strcat(cmd, buf);
	if(!(fp = fopen(buf, "w"))) {
	  fl_show_alert("Can't open file","","",1);
	  return;
	}
	step = EPR_XSTEP(sp[i]);
	for(k = 0; k < sp[i]->samples; k++)
	  fprintf(fp, "%lf %lf\n", EPR_ITOX(sp[i],k,step), sp[i]->y[k]);
	fclose(fp);
	j++;
      }
    system(cmd);
    sprintf(cmd, "rm -f /tmp/%d.*", getuid());
    system(cmd);
    return;
  }
  if(!strcmp(str, "Execute Grace for current page")) {
    strcpy(cmd, GRAPH);
    strcat(cmd, " ");
    if(!sp[page]) return;
    sprintf(buf, "/tmp/%d.%d", getuid(), page);
    strcat(cmd, " ");
    strcat(cmd, buf);
    if(!(fp = fopen(buf, "w"))) {
      fl_show_alert("Can't open file","","",1);
      return;
    }
    step = EPR_XSTEP(sp[page]);
    for(k = 0; k < sp[page]->samples; k++)
      fprintf(fp, "%lf %lf\n", EPR_ITOX(sp[page],k,step), sp[page]->y[k]);
    fclose(fp);
    system(cmd);
    sprintf(cmd, "rm -f /tmp/%d.*", getuid());
    system(cmd);
    return;
  }
  if(!strcmp(str, "Stackplot with Grace")) {
    double xshift, yshift, xc, yc, z;
    if(!sp[0]) return;
    sprintf(buf, "%lf", fabs(sp[0]->xbeg - sp[0]->xend) / 200.0);
    if(!(str = fl_show_input("Enter X shift", buf))) return;
    xshift = atof(str);
    xc = 0.0;
    for (i = 0; i < sp[0]->samples; i++)
      if((z = fabs(sp[0]->y[i])) > xc) xc = z;
    sprintf(buf, "%lf", xc / 1.6);
    if(!(str = fl_show_input("Enter Y shift", buf))) return;
    yshift = atof(str);    
    strcpy(cmd, GRAPH);
    strcat(cmd, " ");
    xc = yc = 0.0;
    for (i = j = 0; i < MAXSP; i++)
      if(sp[i]) {
	xc += xshift;
	yc += yshift;
	sprintf(buf, "/tmp/%d.%d", getuid(), j);
	strcat(cmd, " ");
	strcat(cmd, buf);
	if(!(fp = fopen(buf, "w"))) {
	  fl_show_alert("Can't open file","","",1);
	  return;
	}
	step = EPR_XSTEP(sp[i]);
	for(k = 0; k < sp[i]->samples; k++)
	  fprintf(fp, "%lf %lf\n", EPR_ITOX(sp[i],k,step)+xc, sp[i]->y[k]+yc);
	fclose(fp);
	j++;
      }
    system(cmd);
    sprintf(cmd, "rm -f /tmp/%d.*", getuid());
    system(cmd);    
    return;
  }

  if(!strcmp(str, "Reversed stackplot with Grace")) {
    double xshift, yshift, xc, yc, z;
    if(!sp[0]) return;
    sprintf(buf, "%lf", fabs(sp[0]->xbeg - sp[0]->xend) / 200.0);
    if(!(str = fl_show_input("Enter X shift", buf))) return;
    xshift = atof(str);
    xc = 0.0;
    for (i = 0; i < sp[0]->samples; i++)
      if((z = fabs(sp[0]->y[i])) > xc) xc = z;
    sprintf(buf, "%lf", xc / 1.6);
    if(!(str = fl_show_input("Enter Y shift", buf))) return;
    yshift = atof(str);    
    strcpy(cmd, GRAPH);
    strcat(cmd, " ");
    xc = yc = 0.0;
    for (i = MAXSP-1, j = 0; i >= 0; i--)
      if(sp[i]) {
	xc += xshift;
	yc += yshift;
	sprintf(buf, "/tmp/%d.%d", getuid(), j);
	strcat(cmd, " ");
	strcat(cmd, buf);
	if(!(fp = fopen(buf, "w"))) {
	  fl_show_alert("Can't open file","","",1);
	  return;
	}
	step = EPR_XSTEP(sp[i]);
	for(k = 0; k < sp[i]->samples; k++)
	  fprintf(fp, "%lf %lf\n", EPR_ITOX(sp[i],k,step)+xc, sp[i]->y[k]+yc);
	fclose(fp);
	j++;
      }
    system(cmd);
    sprintf(cmd, "rm -f /tmp/%d.*", getuid());
    system(cmd);    
    return;
  }

  if(!strcmp(str, "Generate Breit-Rabi graph")) {
    FL_OBJECT *tmp;
    int e, t;
    char fisu[512];

    if(!(str = fl_show_input("From field:", "0.0"))) return;
    epr_hamilton_from = atof(str);
    if(!(str = fl_show_input("To field:", "4000.0"))) return;
    epr_hamilton_to = atof(str);
    if(!(epr_hamilton_br = fopen("/tmp/br.tmp", "w"))) {
      fl_show_alert("File write error", "", "", 1);
      return;
    }
    fl_show_form(cmhz->cmhz, FL_PLACE_MOUSE, FL_TRANSIENT, "Units");
    while(1) {
      tmp = fl_do_forms();
      if(tmp == cmhz->ghz) {
	epr_hamilton_units = 0;
	break;
      }
      if(tmp == cmhz->cm) {
	epr_hamilton_units = 1;
	break;
      }
    }
    fl_hide_form(cmhz->cmhz);

    if(setup_simparam(sims, &i, &j, &e, &t) == (void *) -1) return;

    epr_hamilton_mode = 3;
    (void) epr_misc_create_sp((epr_type *) 0, sims, nspec, j, 0, (void *) 0, e, t, 0, 0);
    epr_hamilton_mode = 0;
    fclose(epr_hamilton_br);
    epr_hamilton_br = (FILE *) NULL;
    sprintf(fisu, "%s /tmp/br.tmp", GRAPH);
    system(fisu);
    unlink("/tmp/br.tmp");
    return;
  }

  if(!strcmp(str, "Generate Breit-Rabi transition state diagram")) {
    int e, t;
    char fisu[512];

    if(!(str = fl_show_input("Enter field value:", "3000.0"))) return;
    epr_hamilton_from = atof(str);
    if(!(str = fl_show_input("Enter first level index (0,dim):", "0"))) return;
    epr_hamilton_l1 = atoi(str);
    if(!(str = fl_show_input("Enter second level index (0,dim):", "1"))) return;
    epr_hamilton_l2 = atoi(str);
    if(!(epr_hamilton_br = fopen("/tmp/br.tmp", "w"))) {
      fl_show_alert("File write error", "", "", 1);
      return;
    }

    if(setup_simparam(sims, &i, &j, &e, &t) == (void *) -1) return;

    epr_hamilton_mode = 1;
    (void) epr_misc_create_sp((epr_type *) 0, sims, nspec, j, 0, (void *) 0, e, t, 0, 0);
    epr_hamilton_mode = 0;
    fclose(epr_hamilton_br);
    epr_hamilton_br = (FILE *) NULL;
    sprintf(fisu, "%s /tmp/br.tmp", GRAPH);
    system(fisu);
    unlink("/tmp/br.tmp");
    return;
  }

  if(!strcmp(str, "Generate Breit-Rabi transition moment diagram")) {
    int e, t;
    char fisu[512];

    if(!(str = fl_show_input("Enter field value:", "0.0"))) return;
    epr_hamilton_from = atof(str);
    if(!(epr_hamilton_br = fopen("/tmp/br.tmp", "w"))) {
      fl_show_alert("File write error", "", "", 1);
      return;
    }

    if(setup_simparam(sims, &i, &j, &e, &t) == (void *) -1) return;

    epr_hamilton_mode = 2;
    (void) epr_misc_create_sp((epr_type *) 0, sims, nspec, j, 0, (void *) 0, e, t, 0, 0);
    fclose(epr_hamilton_br);
    epr_hamilton_br = (FILE *) NULL;
    sprintf(fisu, "%s /tmp/br.tmp", GRAPH);
    system(fisu);
    unlink("/tmp/br.tmp");
    return;
  }

  if(!strcmp(str, "Generate angle graph")) {
    int e, t;
    double te, tb;
    char fisu[512];

    /* FIXME: level following does not quite work for graphing!
     * It does, however, work in the real interpolation situation where
     * reference state is set close to the other points.
     * The graphing and level follow works somewhat when using large angle
     * steps.
     */

    if(!(str = fl_show_input("Enter begin field value:", "0.0"))) return;
    epr_hamilton_from = atof(str);
    if(!(str = fl_show_input("Enter end field value:", "6000.0"))) return;
    epr_hamilton_to = atof(str);
    if(!(str = fl_show_input("Enter begin theta value:", "0.0"))) return;
    tb = epr_theta_begin;
    epr_theta_begin = M_PI * atof(str) / 180.0;
    if(!(str = fl_show_input("Enter end theta value:", "90.0"))) return;
    te = epr_theta_end;
    epr_theta_end = M_PI * atof(str) / 180.0;

    if(!(epr_hamilton_br = fopen("/tmp/br.tmp", "w"))) {
      fl_show_alert("File write error", "", "", 1);
      return;
    }

    if(setup_simparam(sims, &i, &j, &e, &t) == (void *) -1) return;

    epr_hamilton_mode = 4;
    epr_hamilton_freq = atof(fl_get_input(runsimul->frequency));
    (void) epr_misc_create_sp((epr_type *) 0, sims, nspec, j, 0, (void *) 0, e, t, 0, 0);
    epr_hamilton_mode = 0;
    fclose(epr_hamilton_br);
    epr_hamilton_br = (FILE *) NULL;
    
    sprintf(fisu, "%s /tmp/br.tmp", GRAPH);
    system(fisu);
    unlink("/tmp/br.tmp");
    epr_theta_end = te;
    epr_theta_begin = tb;
    return;
  }
}

void sig_handler(int i) {

  signal(SIGINT, sig_handler);
  if(!fl_show_question("Do you wish to interrupt processing?", 0)) return;
  longjmp(env, 1);
}

int main(int argc, char **argv) {

  int i;
  Pixmap p, mask;
  unsigned int w, h;

  /* TODO: openmp stuff should be in libepr */
#ifdef _OPENMP
  omp_set_num_threads(THREADS);
#endif

  signal(SIGINT, sig_handler);
  last_param_file[0] = 0;
  if(fl_initialize(&argc, argv, "xemr", 0, 0) == NULL) {
    fprintf(stderr,"Can't open display.\n");
    exit(1);
  }
  for(i = 0; i < MAXOVER; i++)
    overlay[i] = -1;

  for(i = 0; i < MAXVAR; i++)
    active[i] = 1;

  for (i = 0; i < MAXSP; i++) {
    sp[i] = (epr_type *) 0;
    saved[i] = (char) 1;
  }
  if(!(heinzer_k = (double *) malloc(sizeof(double) * EPR_MAX_JUMPS * EPR_MAX_JUMPS))) {
    fprintf(stderr, "Out of memory!\n");
    exit(1);
  }
  bzero(heinzer_k, sizeof(double) * EPR_MAX_JUMPS * EPR_MAX_JUMPS);
  clear_variables();
  clear_equivalences();
  /* TODO */
  /* sp_meas_ioctl_nmr(0, 3, 0); /* later make a routine: init_meas() */

  create(); /* create forms and menus */
  p = fl_create_from_pixmapdata(fl_root,xemr_icon, &w, &h, &mask, 0, 0, 0);
  fl_set_form_icon(gui->gui, p, mask);
  gui->spectrum->input = 1;
  gui->spectrum->wantkey = FL_KEY_ALL;
  fl_show_form(gui->gui, FL_PLACE_MOUSE|FL_FREE_SIZE,FL_FULLBORDER,"Xemr");
  setjmp(env);
  fl_hide_object(gui->mprog);
  fl_do_forms();
}

/* dummy */
int MAIN__() {
}
