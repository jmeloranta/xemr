/*
 * Generic include file for xemr.
 *
 */

#ifndef XEMRDEF
#define XEMRDEF

/*
 * Tunable parameters 
 *
 */

/* Maximum number of spectra (number of pages) */
#define MAXSP 128
/* Maximum number of overlay spectra */
#define MAXOVER 32
/* Maximum number of marks on spectrum */
#define MAXMARKS 128
/* Maximum number of simulation sets */
#define MAXSIMSETS 32
/* Maximum number of variables */
#define MAXVAR 512
/* Maximum number of equivalences (aliases) */
#define NEQUIV 90

/* default microwave frequency */
#define DEF_FREQ 9.42E9

/* Max endor peaks */
#define MAXENDOR 14

/* template spectrum directory (defined by Makefile.am) */
/* #define TEMPLATES "/usr/lib/xemr/templates" */

/* calibration file directory (define by Makefile.am) */
/* #define CALIBRATION "/usr/lib/xemr/calibration" */

/* graphics program (qtiplot or xmgrace) */
#define GRAPH "xmgrace"
/* #define GRAPH "qtiplot" */

#include "epr.h"

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
#include "equivalence.h"
#include "variables.h"
#include "simpar.h"
#include "runsimul.h"
#include "runmon.h"
#include "runsim.h"
#include "misc.h"
#include "heinzer.h"
#include "alinewidth.h"
#include "hyperfine.h"
#include "generic.h"
#include "elel.h"
#include "quadrupole.h"
#include "selectlw.h"
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

extern int var_type[], var_set[], var_arg1[], var_arg2[], var_arg3[], 
  var_arg4[], nvars;
extern double var_mclimit[], var_high[], var_low[];
extern epr_variable vars[];
extern int alias_type[],alias_from_set[], alias_from_arg1[], alias_from_arg2[],
  alias_from_arg3[], alias_from_arg4[], alias_to_set[], alias_to_arg1[],
  alias_to_arg2[], alias_to_arg3[], alias_to_arg4[], nalias, active[];
extern epr_type *sp[];
extern int nspec;
extern epr_sim_set sims[];
extern double norris_k, *heinzer_k;
extern int nvars;
extern int nequiv;
extern double last_rms;
extern char last_param_file[];

extern FD_gui *gui;        /* GUI pointer */
extern FD_params *params;  /* parameter page pointer */
extern FD_mark *mark;      /* mark window */
extern FD_polyn *polyn;    /* polyn degree window */
extern FD_symmetry *symmetry; /* symmetry dialog */
extern FD_calibsig *calibsig; /* signal calibration window */
extern FD_convol *convol;     /* convolution dialog */
extern FD_differences *differences; /* differences dialog */
extern FD_diffout *diffout; /* differences output window */
extern FD_position *position; /* peak position analysis window */
extern FD_peakout *peakout; /* peak output window */
extern FD_convert *convert;   /* convert format dialog */
extern FD_linewidth *linewidth; /* linewidth editor */
extern FD_equivalence *equivalence; /* equivalence editor */
extern FD_simparam *simparam;   /* simulation parameters dialog */
extern FD_variables *variables; /* variables dialog */
extern FD_runsimul *runsimul;   /* simulation parameter dialog */
extern FD_runmon *runmon;       /* monte carlo fitting parameter dialog */
extern FD_runsim *runsim;       /* simplex fitting parameter dialog */
extern FD_misc *misc;           /* Numerical params */
extern FD_heinzer *heinzer;     /* Rate constants for Heinzer simulation */
extern FD_hyperfine *hyperfine; /* hyperfine parameters */
extern FD_generic *generic;
extern FD_elel *elel;
extern FD_quadrupole *quadrupole;
extern FD_selectlw *selectlw;
extern FD_shift *shift;         /* spectrum shifting dialog */
extern FD_summary *summary;     /* page summary dialog */
extern FD_alinewidth *alinewidth;
extern FD_gvalue *gvalue;
extern FD_elw *elw;             /* ENDOR lw & amp */
extern FD_runml *runml;
extern FD_spec_setup *spec_setup;
extern FD_window *window;
extern FD_orbital *orbital;
extern FD_cmhz *cmhz;
extern FD_hj *hj;

struct spec_calib {
  int mwcounter;           /* MW counter type (1 = none, ...) */
  int mwcounter_id;        /* GPIB id */
  int sigchan;             /* signal channel type */
  int sigchan_id;          /* GPIB id */
  int gaussmeter;          /* gaussmeter type */
  int gaussmeter_id;       /* GPIB id */
  int magnet;              /* magnet controller type */
  int magnet_id;           /* GPIB id */
  int endorunit;           /* ENDOR unit type */
  int endorunit_id;        /* GPIB id */
  int rfcounter;           /* RF counter type */
  int rfcounter_id;        /* GPIB id */
  char comment[256];       /* calibration comment */
  double field_mod_freq;   /* available mag. field mod. frequencies */
  double field_mod_max;    /* maximum magnetic field modulation depth */
  double signal_phase_max; /* signal phase setting (0 degrees) */
  double signal_phase_min; /* signal phase setting (90 degrees) */
  double endor_field_mod_freq;   /* available RF field mod. frequencies */
  double endor_field_mod_max;    /* maximum RF field modulation depth */
  double endor_signal_phase_max; /* signal phase setting (0 degrees) */
  double endor_signal_phase_min; /* signal phase setting (90 degrees) */
};

typedef struct spec_calib spec_calib;
extern spec_calib spectr;
extern int measure_verbose;

#define TYPE_NONE  0
#define TYPE_LG    1
#define TYPE_LW    2
#define TYPE_POSN  3
#define TYPE_AMP   4
#define TYPE_HFC   5
#define TYPE_B     6
#define TYPE_C     7
#define TYPE_D     8
#define TYPE_T     9
#define TYPE_K     10
#define TYPE_AD    11
#define TYPE_AP    12
#define TYPE_ELW   13
#define TYPE_EAMP  14
#define TYPE_EOFF  15
#define TYPE_SO    16
#define TYPE_CRA1  17
#define TYPE_CRA2  18
#define TYPE_CRA3  19
#define TYPE_CRB   20

/* Prototypes */

int first_free_page();
char *units(int);
void do_asymmlw(), update_equivalences(int), update_variables(int),
  do_equivalences(), do_variables(), read_param(const char *), do_elw(),
  write_param(char *), print_param(), create(), create_menu_gui(),
  create_menu_params(), create_menu_params(), create_menu_resol(),
  create_menu_setup(), create_menu_convol(), create_menu_differences(),
  create_menu_position(), create_menu_convert(), create_menu_equivalence(),
  create_menu_simpar(), update_asymmlw(int, int), do_heinzer(), create_menu_window(),
  do_isotropic(), do_anisotropic(), create_menu_variables(), spectrometer_setup(),
  keyboard(FL_OBJECT *, long), create_menu_atoms(), create_menu_s(),
  create_menu_dlevel(), create_menu_setup(), create_menu_misc(), create_menu_spec_setup();
int first_free_page(), isalias();
char *units(int);
int find_var(int, int, int, int, int, int);
double *find_param(int, int, int, int, int, int);
void clear_variables();
void update_variables(int);
void measure_epr(epr_type *, int (*)(epr_type *, int, int), double (*)(int, char *));
void measure_endor(epr_type *, int (*)(epr_type *, int, int), double (*)(int, char *));
void measure_tripleg(epr_type *, int (*)(epr_type *, int, int), double (*)(int, char *));
void measure_triples(epr_type *, int (*)(epr_type *, int, int), double (*)(int, char *));
void measure_eie(epr_type *, int (*)(epr_type *, int, int), double (*)(int, char *));
int action_spectrum(FL_OBJECT *, int, FL_Coord, FL_Coord, int, void *);
#endif /* XEMRDEF */


