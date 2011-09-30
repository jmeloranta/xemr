#ifndef EPRLIB
#define EPRLIB

/*
 * Main include file for libepr
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <math.h>
#include <ctype.h>
#include <string.h>

#if !defined(MIN)
#define MIN(x,y) (((x)<(y))?(x):(y))
#endif
#if !defined(MAX)
#define MAX(x,y) (((x)>(y))?(x):(y))
#endif

/* ================ data field types ================= */
#define EPR_NIL    0
#define EPR_STRING 1
#define EPR_FLOAT  2
#define EPR_INT    3
#define EPR_FLAG   4

struct epr_idents {
  char *id;           /* identifier string */
  unsigned char type; /* data type */
  void *ptr;          /* result during parse */
};

/* ================ constants ================== */

/*
 * Various useful constants.
 *
 * Source: J. A. Wertz & Co. 's Book.
 *
 */

#define EPR_NATOMS 49

/* General "unit independend" constants */

/* "Small number" - overridden in some places */
#define EPR_EPS 1.0E-8
/* Speed of light in vacuum (m/s) */
#define EPR_C   2.99792458E8
/* Planck's constant (Js) */
#define EPR_H   6.626075540E-34
/* Planck's constant / 2Pi (Js) */
#define EPR_HBAR (EPR_H / (2.0 * M_PI))
/* Absolute value of electron charge (C) */
#define EPR_E   1.6021773349E-19
/* Electron mass (kg) */
#define EPR_ME  9.109389754E-31
/* Proton mass (kg) */
#define EPR_MP  1.672623110E-27
/* g-value of free electron */
#define EPR_GE  2.00231930438620
/* Proton g-factor */
#define EPR_GP  5.5856947713
/* Bohr radius (m) */
#define EPR_BOHR 5.2917724924E-11
/* Boltzmann constant (J/K) */
#define EPR_BOLTZMANN 1.38065812E-23

/* SI unit (= T) section */

/* Vacuum permeability (T^2 m^3 / J) */
#define EPR_SI_VPERMEAB 12.566370614E-7
/* Vacuum permittivity (C*C/(J*m)) */
#define EPR_SI_VPERMITT (1.0 / (EPR_SI_VPERMEAB * EPR_C * EPR_C))
/* Bohr magneton (J/T) */
#define EPR_SI_BE (EPR_E * EPR_HBAR / (2.0 * EPR_ME))
/* Electron magnetic moment (J/T; S = 0.5) */
#define EPR_SI_UE (-EPR_GE * EPR_SI_BE * 0.5)
/* Free-electron magnetogyric ratio (rad/(sT)) */
#define EPR_SI_GE (-2.0 * EPR_SI_UE / EPR_HBAR)
/* Free-electron magnetogyric ratio (1/(sT)) */
#define EPR_SI_GE_NRAD (EPR_SI_GE / (2.0 * M_PI))
/* Nuclear magneton (J/T) */
#define EPR_SI_BN (EPR_E * EPR_HBAR / (2.0 * EPR_MP))
/* Proton magnetic moment (J/T; I = 0.5) */
#define EPR_SI_UP (EPR_GP * EPR_SI_BN / 2.0)
/* Proton magnetogyric ration (1/(sT)) */
#define EPR_SI_GP (EPR_SI_UP / (0.5 * EPR_HBAR))


/* "Gauss" section (watch out - I just blindly converted T to G!) */

/* Vacuum permeability (G^2 m^3 / J) */
#define EPR_GAUSS_VPERMEAB (EPR_SI_VPERMEAB * 1E8)
/* Vacuum permittivity (G^2 * m^3 / J) */
#define EPR_GAUSS_VPERMITT (EPR_SI_VPERMITT * 1E8)
/* Bohr magneton (J/G) */
#define EPR_GAUSS_BE (1E-4 * EPR_SI_BE)
/* Electron magnetic moment (J/G; S = 0.5) */
#define EPR_GAUSS_UE (EPR_SI_UE * 1E-4)
/* Free-electron magnetogyric ratio (rad/(sG)) */
#define EPR_GAUSS_GE (1E-4 * EPR_SI_GE)
/* Free-electron magnetogyric ratio (1/(sG)) */
#define EPR_GAUSS_GE_NRAD (EPR_SI_GE_NRAD * 1.0E-4)
/* Nuclear magneton (J/G) */
#define EPR_GAUSS_BN (EPR_SI_BN * 1E-4)
/* Proton magnetic moment (J/G; I = 0.5) */
#define EPR_GAUSS_UP (EPR_SI_UP * 1E-4)
/* Proton magnetogyric ration (1/(sT)) */
#define EPR_GAUSS_GP (EPR_SI_GP * 1E-4)

/* unitless constants (for others see /usr/include/math.h) */

/* sqrt(3) */
#define EPR_SQRT3 1.7320508
/* 2Pi */
#define EPR_PI2 6.2831853
/* 4*pi^2 */
#define EPR_PI42 (4.0*M_PI*M_PI)
/* sqrt(3/2) */
#define EPR_SQRT32 1.2247449
/* sqrt(5/2) */
#define EOR_SQRT52 1.5811388
/* FFT line generation Epsilon */
#define EPR_EXP_EPS -60.0
/* Constant for Gaussian integration formula (sqrt(3)/6) */
#define EPR_GAUSS_QUAD 0.28867513
/* Large number */
#define EPR_LARGE 1E50

/* cm-1 to Hz conversion constant */
#define EPR_CM2HZ 2.99792458E10

/* Hz to J conversion */
#define EPR_HZ2J 6.6260738E-34

/* tunable simulation parameters */
#define EPR_MAX_SAMPLES (256 * 1024)
#if !defined(MAXPATHLEN)
#define MAXPATHLEN 1024
#endif
#define EPR_MAXGROUPS 50
#define EPR_MAXPEAKS 80000
#define EPR_MAXELS 8
#define EPR_MAXNS    30
#define EPR_MAXDIVS 512
#define EPR_MAX_EQUIV 12
#define EPR_MAX_JUMPS 25
#define EPR_MAXOPER 512
#define EPR_MAXDIM 2048
#define EPR_IBUFLEN 2048
#define EPR_MAXVARS 256
#define EPR_MAXFILES 2048

/* Fortran data types for LAPACK */
typedef struct { double r, i; } doublecomplex;
typedef long int logical;

struct epr_struct {
  char fname[MAXPATHLEN];     /* filename  */
  char name[80];              /* compound name */
  char operator[80];          /* operator name */
  char date[80];              /* date of creation */
  char time[80];              /* time of creation */
  int resonator;              /* resonator selection */
  char spek_type;             /* Type of resonance: 0=EPR, 1=ENDOR, 2=Triple */
                              /* gen., 3 = Triple spec., 4 = kinetic */
  char samp_type;             /* 0=mesaured, 1=simulated, 2=modified  */
  char solvent[80];           /* solvent or matrix */
  double temperature;         /* sample temperature */
  int scans;                  /* number of scans */
  double gain;                /* Receiver gain */
  double timec;               /* time constant */
  double ctime;               /* A/D sample conversion time */
  double modulampl;           /* Modulation amplitude (G) */
  double modfreq;             /* Modulation frequency (Hz) */
  double frequency;           /* Microwave frequency (Hz) */
  double power;               /* microwave power (mW) */
  double endor_b;             /* ENDOR field */
  double endor_mod;           /* ENDOR modulation */
  double endor_pump_freq;     /* Pump frequency (ie. TRIPLE) */
  double endor_power;         /* ENDOR RF power */
  double endor_pump_power;    /* TRIPLE RF power */
  char info[320];             /* General information field */
  double xbeg, xend;          /* x-value at the beginning & end of spectrum */
  char xunits;                /* X-axis units (0 = G, 1 = mT, 2 = MHz, */
                              /* 3 = GHz, 4 = seconds, 5 = arbitary) */
  int samples;                /* number of samples (ie. (x,y) pairs) */
  double y[EPR_MAX_SAMPLES];      /* intensity array */
  double theta, phi;          /* sample orientation */
  double sphase;              /* signal channel phase */
  int harmonic;               /* first or second harmonic */
};

typedef struct epr_struct epr_type;

struct epr_variable {
  char name[30];         /* comment for variable */
  double *value;         /* variable value + equivalences */
  int nequiv;            /* number of equivalnces for this variable */
  double *equiv[EPR_MAX_EQUIV];/* equivalent variables */
  double ovalue;         /* "original" value */
  double deriv;          /* first derivative with respect to this variable */
  double oderiv;         /* storage old derivative (used in conjugate grad) */
  double high;           /* upper limit */
  double low;            /* lower limit */
  double mc_limit;       /* max change in one MC optimization iteration */
};

typedef struct epr_variable epr_variable;

struct epr_sim_set {
  int ngr;               /* # of groups (length of hfc) */
  int units;             /* units of couplings 0 = G, 1 = mT, 2 = MHz, 3 GHz */
                         /* this is obsolete? */
  double hfc[EPR_MAXELS][EPR_MAXGROUPS][9];/* array of anisotropic hyperfine couplings */
  double spin[EPR_MAXGROUPS];/* nucl. spin */
  int   nbr[EPR_MAXGROUPS];  /* number of equiv. nuclei */
  int   atoms[EPR_MAXGROUPS];/* atom types */
  double espin;          /* total electron spin */
  int begin;             /* begin at level */
  int end;               /* end at level */
  int orbit;             /* orbital ang. mom. for electrons */
  double cra1,cra2,cra3,crb;/* crystal field parameters */
  double spin_orbit; /* spin-orbit coupling */
  double spin_temp;      /* (Boltzmann) temperature - -1 = not in use */
  double posn[EPR_MAXELS][9];/* anisotropic position */
  double lw;             /* lw (x) */
  double lwy, lwz;       /* (y) and (z) components for lw */
  double d[EPR_MAXELS][EPR_MAXELS][9];   /* S.D.S (electron dipole interaction) */
  double p[EPR_MAXGROUPS][9];/* Anisotropic P (I.P.I) */
  double amp;            /* amplitude */
  int auto_amp;          /* automatic amplitude adjustment during fitting */
  double lg_fact;        /* Lorentz - Gauss factor ( 1.0 = Lorentz, */
                         /* 0.0 = Gauss) */
  int dlevel;            /* derivative level */
  double b[EPR_MAXGROUPS];   /* linewidths depending on M_I */
  double c[EPR_MAXGROUPS][EPR_MAXGROUPS];
  /* linewidths depending on products of M_I and M_I^2 */
  double phi, theta;     /* sample orientation */
  double o_phi, o_theta; /* (phi,theta) offset from (phi, theta) - site split */
  double elw;            /* endor lw in MHz */
  double e_ilw[EPR_MAXPEAKS]; /* Individual endor lws in MHz */
  double e_iamp[EPR_MAXPEAKS];/* Individual endor amps */
  double e_offset;       /* ENDOR field offset */
};

typedef struct epr_sim_set epr_sim_set;

struct epr_hamilton {
  doublecomplex *h;       /* spin Hamilton matrix */
  int dim;                /* matrix dimension */
  int nspins;             /* number of spins in spin vector */
  double spins[EPR_MAXGROUPS];/* spin vector (incl. electron spins) */
};

typedef struct epr_hamilton epr_hamilton;

struct epr_peak {
  double pos;              /* peak position */
  double intensity;        /* peak intensity */
  int mi[EPR_MAXGROUPS];       /* nucl quantum numbers for transition */
  int dummy;               /* used in some special cases... */
  double lw;
};

typedef struct epr_peak epr_peak;

struct epr_operator {
  int n[EPR_MAXGROUPS];        /* Spin number operator acts on */
  double m;                /* Constant multiplier for this term */
  int d[EPR_MAXGROUPS];        /* Angular momentum operator type: */
                           /* 0 = X, 1 = Y, 2 = Z */
  int e[EPR_MAXGROUPS];        /* exponents for operators */
  int bdeg;                /* has a B^bdeg multiplier */
  int nbr;                 /* number of multiplied operators in this term */
  double b;                /* field value */
};

typedef struct epr_operator epr_oper;

struct epr_atom {
  int z;
  int isotope;
  char *symbol;
  double abundance;
  double spin;
  double gn;
  double istropic;
  double uniaxial;
};

typedef struct epr_atom epr_atom;

/* mag. field -> g-value */
#define EPR_M2G(m,f) ((EPR_H / EPR_GAUSS_BE) * (f) / (m))
/* mag. field -> freq. */
#define EPR_M2F(m,f,mb) (M2G(m,f) * (mb) * EPR_GAUSS_BE / EPR_H)
/* coupling (G) -> coupling (MHz) NOTE: g is assumed to be g_e ! */
/* #define GTOMHZ(x) (2.802494 * (x)) */
#define EPR_G2M(g,f) (EPR_H * (f) / ((g) * EPR_GAUSS_BE))
#define EPR_GTOMHZ(x) (EPR_GE * EPR_GAUSS_BE * (x) / (EPR_H*1E3))
#define EPR_MHZTOG(x) (EPR_H * (x) / (EPR_GE * EPR_GAUSS_BE*1E3))
#define EPR_GTOHZ(x) (EPR_GE * EPR_GAUSS_BE * (x) / EPR_H)
#define EPR_HZTOG(x) (EPR_H * (x) / (EPR_GE * EPR_GAUSS_BE))
/* line width (G) to 1/T2 */
#define EPR_LWTOIT2(x) ((x) * EPR_GAUSS_GE_NRAD)

/* index to x-value */
#define EPR_ITOX(sp,i,step) ((sp)->xbeg + ((double) (i)) * (step))
/* x-value to index */
#define EPR_XTOI(sp,b,step) ((int) ((((b) - (sp)->xbeg) / (step))))
/* x-value step size */
#define EPR_XSTEP(sp) (((sp)->xend - (sp)->xbeg) / ((double) (sp)->samples - 1))

/* sign of a double */
#define EPR_SIGN(x) (((x)<0.0)?-1.0:1.0)
/* Fortran style array indexing (first row then column) */
#define EPR_INDEX(n,i,j) ((n)*(j) + (i))

/* Peak types */
#define EPR_ABSORPTION 1
#define EPR_FIRSTDERIV 2
#define EPR_SECONDERIV 3

#define EPR_ABS(x) ((x < 0)?-(x):(x))

#define EPR_UNIT_G   0
#define EPR_UNIT_MT  1
#define EPR_UNIT_MHZ 2
#define EPR_UNIT_GHZ 3
#define EPR_UNIT_GV  4
#define EPR_UNIT_S   5
#define EPR_UNIT_ARB 6

/* "minor" lineshape modes */
#define EPR_LS_NORMAL        0      /* normal (fft) */
#define EPR_LS_ASYMMLW       1      /* nucl. spin dep. (traditional) */
#define EPR_LS_NORRIS        2      /* Norris exchange */
#define EPR_LS_HEINZER_INTRA 3      /* Heinzer intra exchange */
#define EPR_LS_HEINZER_INTER 4      /* Heinzer inter exchange */
#define EPR_LS_STICK         5      /* stick spectrum */
#define EPR_LS_GENERIC       6      /* ??? */
#define EPR_LS_DDEP          7      /* field direction dependend lw */

/* Hamiltonian terms for generic */
#define EPR_HM_GBS           1    /* Electron zeeman */
#define EPR_HM_ASI           2    /* hyperfine coupling */
#define EPR_HM_SDS           4    /* electron dipolar spin-spin coupling */
#define EPR_HM_IPI           8    /* nuclear quadrupole coupling */
#define EPR_HM_GBI           16   /* nuclear Zeeman */
#define EPR_HM_FICTIOUS      32   /* use fictious spin describing the electrons*/
#define EPR_HM_BL            64   /* magnetic field - orbital ang. mom coupling */
#define EPR_HM_SL           128   /* spin - orbit coupling */
#define EPR_HM_CRYSTAL      256   /* crystal field */

/* Transition moment operator types */
#define EPR_TR_S_90           1    /* B1.S (perpendicular) */
#define EPR_TR_I_90           2    /* -B1.I (perpendicular) */
#define EPR_TR_S_0            4    /* B1.S (parallel) */
#define EPR_TR_I_0            8    /* B1.I (parallel) */
#define EPR_TR_CONSTANT       16   /* constant transition moment */

/* "major" lineshape modes */
#define EPR_LS_ORDERED     0
#define EPR_LS_POWDER     10

/* integration modes */
#define EPR_I_GAUSS_LEGENDRE       1
#define EPR_I_GAUSS_LEGENDRE_IGLOO 2
#define EPR_I_GAUSS_CHEBYSEV       3
#define EPR_I_GAUSS_CHEBYSEV_IGLOO 4
#define EPR_I_SIMPSON              5
#define EPR_I_TRASIM               6
#define EPR_I_TRAPEZ               7
#define EPR_I_INTERP               8
#define EPR_I_INTERP_IGLOO         9
#define EPR_I_INTERP_SPL           10
#define EPR_I_INTERP_SPL_IGLOO     11
#define EPR_I_AXIAL                12
#define EPR_I_INTERP_AXIAL         13
#define EPR_I_INTERP_AXIAL_SPL     14

/* Simulation models */
#define EPR_SM_1STORDER      0
#define EPR_SM_EXACT         1
#define EPR_SM_EXACT_ENDOR   2
#define EPR_SM_1STENDOR      3

/* Windowing types */
#define EPR_WN_EXPONENTIAL   0
#define EPR_WN_TRIANGULAR    1
#define EPR_WN_HANNING       2
#define EPR_WN_WELCH         3
#define EPR_WN_HAMMING       4
#define EPR_WN_BLACKMAN      5
#define EPR_WN_PARZEN        6

/* sp_hamilton_mode settings (graphing) */
#define EPR_SH_NORMAL        0
#define EPR_SH_SRULE         1
#define EPR_SH_TRMOM         2
#define EPR_SH_BRGRAPH       3
#define EPR_SH_ANGLE         4

/* prototypes */

int epr_powder_interp_spl(epr_type *, epr_sim_set *, int, int, void *, int, int, int, int, int);
int epr_powder_interp(epr_type *, epr_sim_set *, int, int, void *, int, int, int);
int epr_powder_gauss(epr_type *, epr_sim_set *, int, int, void *, int, int, int, int, int);
int epr_powder_simpson(epr_type *, epr_sim_set *, int, int, void *, int, int);
int epr_powder_trasin(epr_type *, epr_sim_set *, int, int, void *, int, int);
int epr_powder_axial(epr_type *, epr_sim_set *, int, int, void *, int, int);
int epr_powder_axial_interp(epr_type *, epr_sim_set *, int, int, void *, int, int);
int epr_powder_axial_spl(epr_type *, epr_sim_set *, int, int, void *, int, int, int);
int epr_powder_trapez(epr_type *, epr_sim_set *, int, int, void *, int, int);
double epr_calc_add(epr_type *, epr_type *, double, int);
double epr_calc_addc(epr_type *, double);
double epr_calc_mult(epr_type *, double);
epr_type *epr_calc_diff(epr_type *);
epr_type *epr_calc_integ(epr_type *);
double epr_calc_max(epr_type *);
double epr_calc_rot_c(epr_type *, double, double, double);
double epr_calc_rot_b(epr_type *, double, double, double, double);
char *epr_file_fgets(char *, int, FILE *);
epr_type *epr_file_write(epr_type *);
epr_type *epr_file_read(char *);
void epr_file_export(char *, epr_type *);
void epr_misc_gls(epr_type *, epr_type *, epr_sim_set *, int, int, int, void *);
int epr_misc_create_sp(epr_type *, epr_sim_set *, int, int, int, void *, int, int, int, int);
epr_type *epr_misc_change_resolution(epr_type *, int);
epr_type *epr_misc_new();
epr_type *epr_misc_copy(epr_type *, epr_type *);
epr_type *epr_misc_zero(epr_type *);
epr_type *epr_misc_zerosp(epr_type *, double, double);
double epr_misc_drms(epr_type *, epr_type *);
epr_type *epr_misc_rz(epr_type *, double);
double epr_misc_normalize(epr_type *, epr_type *);
epr_type *epr_misc_scale(epr_type *, double);
void epr_misc_eigen(doublecomplex *, doublecomplex *, doublecomplex *, doublecomplex *, int);
void epr_misc_linear(double *, int, double *);
void epr_misc_heigen(doublecomplex *, int, double *, doublecomplex *);
int epr_database_load_many(epr_type **, int, char *, char *);
int epr_database_lookup(epr_type **, int, char *, char *);
epr_type *epr_fft(epr_type *);
epr_type *epr_fft_inv(epr_type *);
int epr_fft_fill(epr_type *, double);
epr_type *epr_fft_zfill(epr_type *, int, int);
epr_type *epr_fft_power(epr_type *);
epr_type *epr_fft_window(epr_type *, int, double, double);
inline double epr_fftw_win1(double, double, double, double);
inline double epr_fftw_win2(double, double, double, double);
inline double epr_fftw_win3(double, double, double, double);
inline double epr_fftw_win4(double, double, double, double);
inline double epr_fftw_win5(double, double, double, double);
inline double epr_fftw_win6(double, double, double, double);
inline double epr_fftw_win7(double, double, double, double);
double epr_peak_baseline(epr_type *);
double epr_peak_nlevel(epr_type *);
int epr_peak_diffs(double *, int, double *, int *, int, double, double);
int epr_peak_find(epr_type *, double *, double *, int, double, char);
double epr_peak_int1(epr_type *, double, double);
double epr_peak_int2(epr_type *, double, double, char);
double epr_peak_symm(double *, int, double);
int epr_view(int, epr_type **, char);
int epr_view_xgraph(int, epr_type **, char);
epr_type *epr_cepstr(epr_type *);
epr_type *epr_conv_scale(epr_type *, int);
epr_type *epr_conv_f(epr_type *, double (*) (double, epr_type *));
epr_type *epr_conv(epr_type *, epr_type *);
epr_type *epr_cross(epr_type *, epr_type *);
epr_type *epr_cross_f(epr_type *, double (*) (double, epr_type *));

epr_type *epr_base_line(epr_type *, int, int, int);
epr_type *epr_base_polyn(epr_type *, int, int *);
epr_type *epr_base_spline(epr_type *, int, int *);
void epr_base_splin1(double *, double *, int, double, double, double *);
double epr_base_splie1(double *, double *, double *, int, double);
void epr_base_splie2(double *, double *, double **, int, int, double **);
void epr_base_splin2(double *, double *, double **, double **, int, int, double, double, double *);

epr_type *epr_smooth_9pt(epr_type *);
epr_type *epr_smooth_symmetry(epr_type *, double);
double epr_line_gauss(double, double, double, double);
double epr_line_lorentz(double, double, double, double);
double epr_line_dgauss(double, double, double, double);
double epr_line_dlorentz(double, double, double, double);
epr_type *epr_line_create_conv(epr_type *, epr_sim_set *);
epr_type *epr_line_create_add(epr_type *, double *, double *, double *, double *, int, int);
epr_type *epr_line_create_norris(epr_type *, epr_sim_set *, double *, double, double, int);
epr_type *epr_line_create_heinzer_intra(epr_type *, epr_sim_set *, double *, double, int, double *);
epr_type *epr_line_asymmlw(epr_type *, epr_sim_set *, epr_peak *, int);
epr_type *epr_line_norris(epr_type *, epr_sim_set *, epr_peak *[], int, int, double);
epr_type *epr_line_heinzer_intra(epr_type *, epr_sim_set *, epr_peak *[], int, int, double *);
double epr_optim_hj(double (*)(int, epr_variable *), double, int, epr_variable *);
double epr_optim_cgrad(double (*)(int, epr_variable *), void (*)(int, epr_variable *),
                    int, epr_variable *);
double epr_optim_grad(double (*)(int, epr_variable *), void (*)(int, epr_variable *),
                    int, epr_variable *);
double epr_optim_monte_carlo(double (*)(int, epr_variable *), int, epr_variable *);
double epr_optim_simplex(double (*)(int, epr_variable *), int, epr_variable *);
double epr_optim_line(double (*)(int, epr_variable *), int, epr_variable *);
void epr_optim_diff(double (*)(int, epr_variable *), int, epr_variable *);
int epr_fit_fcn(int *, int *, double *, double *, int *);
double epr_fit_marq(int, epr_variable *);
double epr_fit_sp(epr_type *, epr_type *, epr_sim_set *, int, epr_variable*, int, int, void *);
epr_type *epr_fit_line(epr_type *, double *, double *, double *, double *, double *, int, int);
void epr_view_gron();
void epr_view_groff();
void epr_view_plot(int, int, int);
int epr_view_print(char *, int, epr_type **, char);
int epr_kbhit();
int epr_simul_stick(epr_type *, epr_sim_set *, epr_peak *);
epr_type *epr_line_create_fft(epr_type *, epr_sim_set *, int);
int epr_dmat_lsolve(doublecomplex *, doublecomplex *, int);
epr_type *epr_dmat_jump(epr_type *, double *, double *, double *, double *, int);
inline void epr_imag_matr_mult(doublecomplex *, doublecomplex *, int);
inline void epr_imag_addto(doublecomplex *, doublecomplex *);
inline void epr_imag_mult(doublecomplex *, doublecomplex *);
inline void epr_imag_div(doublecomplex *, doublecomplex *);
void epr_hamilton_correlate(doublecomplex *, doublecomplex *, int, int *);
void epr_hamilton_prepare(epr_sim_set *, epr_hamilton *);
void epr_hamilton_add(epr_hamilton *, epr_oper *, int);
inline double epr_hamilton_int(epr_hamilton *, doublecomplex *, int, int);
int epr_hamilton_generate(epr_sim_set *, epr_type *, epr_oper *, int, epr_oper *, int, epr_peak *);
int epr_hamilton_endor(epr_sim_set *, epr_type *, epr_oper *, int, epr_oper *, int, epr_oper *, int, epr_peak *);
inline void epr_tensor_mult(double *, double *, double *);
inline void epr_tensor_dir(double *, double, double);
inline double epr_tensor_project(double *, double *);
inline double epr_tensor_project2(double *, double *, double *);
inline void epr_tensor_diag(double *, double *, double *);
inline void epr_tensor_udiag(double *, double *, double *);

/* Global variables */

extern double epr_line_cutoff;
extern int epr_hamilton_lfollow;
extern int epr_hamilton_divs;
extern int epr_hamilton_constant;
extern FILE *epr_hamilton_br;
extern int epr_hamilton_units;
extern double epr_hamilton_from, epr_hamilton_to;
extern int epr_hamilton_l1, epr_hamilton_l2;
extern int epr_misc_hverbose;
extern int epr_hamilton_mode;
extern double epr_hamilton_freq;
extern int epr_hamilton_strict;
extern int epr_phi, epr_theta, epr_leave_fft;
extern double epr_phi_begin, epr_phi_end, epr_theta_begin, epr_theta_end;
extern epr_peak *epr_misc_peaks[];
extern int epr_misc_npeaks[];
extern int epr_misc_fictious, epr_misc_orbit;
extern int epr_misc_preload, epr_misc_noupd;

extern double epr_optim_end;
extern double epr_optim_start;
extern double epr_optim_width;
extern double epr_optim_ltol;
extern double epr_optim_tol;
extern double epr_optim_ctol;
extern double epr_optim_maxit;
extern double epr_optim_dstep;
extern int epr_optim_debug;
extern int epr_optim_debug_line;
extern int epr_optim_line_maxretry;
extern int epr_powder_evals;
extern int epr_powder_fm;
extern struct epr_idents idtbl[];
extern epr_atom epr_atoms[];
extern int epr_simulation_imode, epr_simulation_npt;
extern double sp_fit_mltol, sp_fit_difl;
extern int epr_simulation_model, epr_simulation_type;
extern int epr_simulation_hamilton, epr_simulation_transition;
extern double epr_fit_mltol, epr_fit_difl;

#endif /* EPRLIB */
