/*
 * Spin Hamilton related routines based on numerical diagonalization
 * of the matrix yielding energies and transition moments.
 *
 * Last modified: Oct-13-2009 jussi.eloranta@csun.edu
 *
 * Written by:
 * Jussi Eloranta (jussi.eloranta@csun.edu)
 *
 * Department of chemistry and biochemistry
 * California State University at Northridge
 *
 */

#include "epr.h"

static double gen_w1[EPR_MAXDIM], gen_w2[EPR_MAXDIM], gen_b[EPR_MAXDIVS];
static doublecomplex *sto_h, *sto_h_org;
static doublecomplex *sto_h1;
static epr_hamilton h, h_org, h_1, h_1_n;
static doublecomplex *gen_vec1, *gen_vec2, *gen_vec_ini;
static doublecomplex ops[EPR_MAXGROUPS][100];
static doublecomplex *corr;
extern int epr_misc_preload;
static double bz1[EPR_MAXDIM], bz2[EPR_MAXDIM];

/*
 * Function: void epr_hamilton_correlate(doublecomplex *, doublecomplex *, int, int *)
 *
 * Purpose: Calculate correlation matrix for two matrices (containing
 *          eigenvectors).
 *
 * a    = 1st matrix containing eigen vectors [input; doublecomplex *]
 * b    = 2nd matrix containing eigen vectors [input; doublecomplex *]
 * n    = orders of a and b matrices [input, int]
 * c    = mapping vector between a and b [output, int *]
 *        (mapping between level i in a <-> c[i] in b)
 *
 */

void epr_hamilton_correlate(doublecomplex *a, doublecomplex *b, int n, int *c) {

  int i, j, k;
  doublecomplex alpha, beta;
  double m, t;
  static int inited = 0;
  char tra = 'c', trb = 'n';

  if(!inited) {
    if(!(corr = (doublecomplex *) malloc(sizeof(doublecomplex) * EPR_MAXDIM * EPR_MAXDIM))) {
      fprintf(stderr, "libepr: Out of memory in epr_hamilton_correlate().\n");
      exit(1);
    }
    inited = 1;
  }
  alpha.r = 1.0;
  alpha.i = beta.r = beta.i = 0.0;
  /* calculate level correlation matrix */
  zgemm_(&tra, &trb, &n, &n, &n, &alpha, a, &n, b, &n, &beta, corr, &n);

#if 0
  printf("--------------\n");
  for (i = 0; i < n; i++) {
    for (j = 0; j < n; j++) printf(" %le", corr[i*n+j].r);
    printf("\n");
  }
  printf("--------------\n");
#endif

  /* calculate the eigen level translation table */
  for (i = 0; i < n; i++) {
    m = 1.0E99;
    k = i * n;
    for (j = 0; j < n; j++) {
      t = 1.0 - fabs(corr[k+j].r) + fabs(corr[k+j].i);
      if(t < m) {
	c[i] = j;
	m = t;
      }
    }
    /*    if(m != 0.0)
	  printf("Match: %d <-> %d, with score = %le\n", i, c[i], m); */
  }
}

/*
 * Initialize spin hamilton data structures.
 *
 */

void epr_hamilton_init() {

  static int inited = 0;
  int i;

  /* to avoid allocating too much memory at the beginning */
  /* small memory machines would core dump ... */
  if(!inited) {
    if(!(sto_h = (doublecomplex *) malloc(sizeof(doublecomplex) * EPR_MAXDIM * EPR_MAXDIM))) {
      fprintf(stderr, "libepr: out of memory!\n");
      exit(1);
    }
    if(!(sto_h_org = (doublecomplex *) malloc(sizeof(doublecomplex) * EPR_MAXDIM * EPR_MAXDIM))) {
      fprintf(stderr, "libepr: out of memory!\n");
      exit(1);
    }
    if(!(sto_h1 = (doublecomplex *) malloc(sizeof(doublecomplex) * EPR_MAXDIM * EPR_MAXDIM))) {
      fprintf(stderr, "libepr: out of memory!\n");
      exit(1);
    }
    if(!(gen_vec1 = (doublecomplex *) malloc(sizeof(doublecomplex) * EPR_MAXDIM * EPR_MAXDIM))) {
      fprintf(stderr, "libepr: out of memory!\n");
      exit(1);
    }
    if(!(gen_vec2 = (doublecomplex *) malloc(sizeof(doublecomplex) * EPR_MAXDIM * EPR_MAXDIM))) {
      fprintf(stderr, "libepr: out of memory!\n");
      exit(1);
    }
    if(!(gen_vec_ini = (doublecomplex *) malloc(sizeof(doublecomplex) * EPR_MAXDIM * EPR_MAXDIM))) {
      fprintf(stderr, "libepr: out of memory!\n");
      exit(1);
    }
    inited = 1;
  }
}

/*
 * Evaluate matrix element for given angular momentum operator.
 *
 * The variable d specifies the operator:
 *
 * d = 0:        Lx
 * d = 1:        Ly
 * d = 2:        Lz
 * d = 3:        L+
 * d = 4:        L-
 * d = 5:        I  (identity operator)
 *
 * Variable s specifies the spin value and i and j refer to the
 * matrix element requested.
 *
 * The result is stored in data (complex number).
 *
 */

static void epr_hamilton_opeval(int d, double s, int i, int j, doublecomplex *data) {

  double m, m2;

  /* all the above operators have band matrix structure (width 3) */
  if(abs(j - i) > 1) {
    data->i = data->r = 0.0;
    return;
  }

  /* identity operator corresponds to identity matrix */
  if(d == 5) {
    if(j != i)
      data->r = data->i = 0.0;
    else {
      data->r = 1.0;
      data->i = 0.0;
    }
    return;
  }

  /* the rest requre some more thought */
  m = s - ((double) j);
  m2 = 2.0 * s;
  switch(j-i) {
  case 0: /* diagonal element */
    if(d == 2) { /* only Lz contributes */
      data->r = m;
      data->i = 0.0;
    } else { /* others are zero */
      data->r = data->i = 0.0;
    }
    return;
  case 1: /* one above diagonal */
    if(d == 2) { /* Lz is identically zero */
      data->r = data->i = 0.0;
      return;
    }
    if(d == 0) { /* Lx */
      /* <m+1|Lx|m> */
      data->r = 0.5*sqrt(s * (s + 1.0) - m * (m + 1.0));
      data->i = 0.0;
      return;
    }
    if(d == 1) { /* Ly */
      /* <m+1|Ly|m> */
      data->r = 0.0;
      data->i = -0.5*sqrt(s * (s + 1.0) - m * (m + 1.0));
      return;
    }
    if(d == 3) { /* L+ */
      /* <m+1|L+|m> */
      data->r = sqrt(s * (s + 1.0) - m * (m + 1.0));
      data->i = 0.0;
      return;
    }
    data->r = data->i = 0.0;
    return;
  case -1: /* one below diagonal */
    if(d == 2) { /* Lz is identically zero */
      data->r = data->i = 0.0;
      return;
    }
    if(d == 0) { /* Lx */
      /* <m-1|Lx|m> */
      data->r = 0.5*sqrt(s * (s + 1.0) - m * (m - 1.0));
      data->i = 0.0;      
      return;
    }
    if(d == 1) { /* Ly */
      /* <m-1|Ly|m> */
      data->r = 0.0;
      data->i = 0.5*sqrt(s * (s + 1.0) - m * (m - 1.0));
      return;
    }
    if(d == 4) { /* L- */
      /* <m-1|L-|m> */
      data->r = sqrt(s * (s + 1.0) - m * (m - 1.0));
      data->i = 0.0;
      return;
    }
    data->r = data->i = 0.0;
    return;
  }
}

/*
 * Prepare operator matrices.
 * 
 * o speciefies the group of operators.
 * spins the spin quantum number vector and nspins the number of spins.
 *
 * The corresponding matrix forms are stored in ops[][].
 *
 */

static void epr_hamilton_opprep(epr_oper *o, double *spins, int nspins, int *ul) {

  int i, j, k, cur_spin, u;
  doublecomplex tmp[EPR_MAXDIM];

  /* first initialize everything to identity matrix */
  for (i = 0; i < nspins; i++) {
    bzero(ops[i], sizeof(doublecomplex) * EPR_MAXDIM);
    for (j = 0; j < ul[i]; j++)
      ops[i][EPR_INDEX(ul[i],j,j)].r = 1.0;
  }

  /* form operator product matrices by successive matrix multiplication */
  for (i = 0; i < o->nbr; i++) {
    cur_spin = o->n[i];
    u = ul[cur_spin];
    bzero(tmp, sizeof(doublecomplex) * EPR_MAXDIM);
    if(o->e[i] == 0) o->d[i] = 5; /* op^0 = identity */
    for (j = 0; j < u; j++)
      for (k = 0; k < u; k++)
	epr_hamilton_opeval(o->d[i], spins[cur_spin], j, k,
			   &tmp[EPR_INDEX(u,j,k)]);
    for (j = 0; j < o->e[i]; j++)
      epr_imag_matr_mult(ops[cur_spin], tmp, u);
  }
#if 0
#define OPN 0
  printf("Dim = %d\n", ul[o->n[OPN]]);
  printf("Matrix:\n");
  for (i = 0; i < ul[o->n[OPN]]; i++) {
    for (j = 0; j < ul[o->n[OPN]]; j++)
      printf(" %le", ops[OPN][EPR_INDEX(ul[o->n[OPN]],i,j)].r);
    printf("\n");
  }
#endif
}

/*
 * Function: void epr_hamilton_prepare(epr_sim_set *, epr_hamilton *)
 *
 * ss   = Simulation set used for building spin vector [input; epr_sim_set *]
 * h    = Hamilton to be prepared [input/output; epr_hamilton *]
 *
 * This initializes the spin processing.
 *
 */

extern int epr_misc_fictious;
extern int epr_misc_orbit;

/* first electron spins, electron ang. momenta, nuclear spins */

void epr_hamilton_prepare(epr_sim_set *ss, epr_hamilton *h) {

  int i, j, dim, dim2, nel, spin_pos = 0, tmp;

  epr_hamilton_init();

  /* electron spins */
  nel = (int) (ss->espin / 0.5);
  if(epr_misc_fictious) {
    h->spins[spin_pos++] = ss->espin;
    dim = nel + 1;
    if(epr_misc_orbit) {
      h->spins[spin_pos++] = (double) ss->orbit;
      dim *= 2 * ss->orbit + 1;
    }
  } else {
    for (i = 0; i < nel; i++)
      h->spins[spin_pos++] = 0.5;
    dim = pow(2.0, (double) nel);
    if(epr_misc_orbit) {
      tmp = 2 * ss->orbit + 1;
      for (i = 0; i < nel; i++) {
	h->spins[spin_pos++] = (double) ss->orbit;
	dim *= tmp;
      }
    }
  }

  /* nuclear spins */
  for (i = 0; i < ss->ngr; i++)
    for (j = 0; j < ss->nbr[i]; j++) {
      dim *= (int) (2.0 * ss->spin[i] + 1.0);
      h->spins[spin_pos++] = ss->spin[i];
    }

  h->dim = dim;
  dim2 = dim * dim;
  h->nspins = spin_pos;
  bzero(h->h, sizeof(doublecomplex) * dim2);
}

/*
 * Function: void epr_hamilton_add(epr_hamilton *, epr_oper *, int)
 *
 * Purpose: Perform a series of operations on given spin Hamilton matrix.
 *
 * h    = Spin Hamilton matrix [input/output; doublecomplex *]
 * o    = Operator acting on spin Hamilton matrix [input; epr_oper *]
 * flag = 0 = do not include field dependent terms,
 *        1 = include field dependent terms only [input; int]
 *       -1 = include all terms [input; int];
 *
 */

void epr_hamilton_add(epr_hamilton *h, epr_oper *o, int flag) {

  int i, j, k, l, m, ij, ji;
  int dim, nspins, nbr;
  double *spins, mult;
  doublecomplex total, *data;
  int a1[EPR_MAXGROUPS], a2[EPR_MAXGROUPS], ind[EPR_MAXGROUPS], ul[EPR_MAXGROUPS];
  
  /* initializaton */
  /* epr_hamilton_init(); - should be done elsewhere ! */
  data = h->h;
  spins = h->spins;
  dim = h->dim;
  nspins = h->nspins;
  nbr = o->nbr;
  if(nbr <= 0) return;
  if(flag != -1)
    if((flag == 0 && o->bdeg > 0) || (flag == 1 && o->bdeg == 0)) return;   
  mult = o->m;
  if(o->bdeg != 0) {
    if(o->bdeg == 1) mult *= o->b;
    else mult *= pow(o->b, (double) o->bdeg);
  }

  /* arrays for basis set counting */
  for (i = 0, j = 1; i < nspins; i++) {
    ul[i] = (int) (2.0 * spins[i] + 1.0);
    ind[i] = j;
    j *= ul[i];
    a1[i] = a2[i] = 0;
  }

  /* form operator matrices (this could be avoided if the same 
   * operator stream is used again) */
  epr_hamilton_opprep(o, spins, nspins, ul);

  /* for each matrix row */
  for (i = 0; i < dim; i++) {

    /* basis set index counting */
    for (j = 0; j < nspins; j++)
      if(!(i % ind[j])) {
	if(a1[j] < ul[j]) a1[j]++; else a1[j] = 1;
	a2[j] = 0;
      }

    /* for each matrix column (Hermitian) */
    for (j = 0; j <= i; j++) {

      /* basis set index counting */
      for (k = 0; k < nspins; k++)
	if(!(j % ind[k])) {
	  if(a2[k] < ul[k]) a2[k]++; else a2[k] = 1;
	}

      /* calculate matrix element */
      total.r = mult; total.i = 0.0;
      for (k = 0; k < nspins; k++) {
	epr_imag_mult(&total, &(ops[k][EPR_INDEX(ul[k], a1[k]-1, a2[k]-1)]));
	if(total.r == 0.0 && total.i == 0.0) break;
      }
      ij = EPR_INDEX(dim,i,j);
      ji = EPR_INDEX(dim,j,i);
      data[ij].r += total.r;
      if(ij != ji) data[ji].r += total.r;
      data[ij].i += total.i;
      if(ij != ji) data[ji].i -= total.i;
    }
    bzero(a2, sizeof(int) * nspins);
  }
}

/*
 * Function: double epr_hamilton_int(epr_hamilton *, doublecomplex *vecs, int, int)
 *
 * Purpose: Calculate transition moment matrix for given operator.
 *
 * h    = Spin Hamilton matrix for transition operator [input;
 *        doublecomplex *]. On output it contains the non-squared
 *        transition matrix elements (i.e. to get transition moments
 *        square the elements).
 * vecs = Array of eigen vectors for original Hamiltonian [input;
 *        doublecomplex *]
 * i    = Transition from state i [input; int].
 * j    = to state j [input; int].
 *
 * Returns: The relative transition moment for transition from i to j.
 *
 */

inline double epr_hamilton_int(epr_hamilton *h, doublecomplex *vecs, int i, int j) {

  static doublecomplex *tmp = NULL;
  doublecomplex alpha = {1.0, 0.0}, beta = {0.0, 0.0}, tmp2;
  char trans = 'n';
  int dim = h->dim, one = 1, k, m = dim * i, n = dim * j;
  extern doublecomplex zdotu_();

  if(tmp == NULL) tmp = (doublecomplex *) malloc(sizeof(doublecomplex) * EPR_MAXDIM);
  zgemv_(&trans, &dim, &dim, &alpha, h->h, &dim, &vecs[n], &one,
     &beta, tmp, &one);
  /* we could use zdotu BLAS function but there are problems with returning
     complex type data from ftn to C. */
  tmp2.r = tmp2.i = 0.0;
  for (k = 0; k < dim; k++) {
    tmp2.r += vecs[m+k].r * tmp[k].r + vecs[m+k].i * tmp[k].i;
    tmp2.i += vecs[m+k].r * tmp[k].i - vecs[m+k].i * tmp[k].r;
  }
  /* does not free tmp */
  return (tmp2.r * tmp2.r + tmp2.i * tmp2.i);
}      

/*
 * Function: void epr_hamilton_generate(epr_sim_set *, epr_type *, epr_oper *, int,
 *                                     epr_oper *, int, epr_peak *)
 *
 * Purpose: Calculate stick EPR (field swept) spectrum using given Hamiltonian.
 *
 * sims   = Sim set [input; epr_sim_set *].
 * sp     = Target spectrum [input/output; epr_type *].
 * e      = Energy operator for spin Hamiltonian [input; epr_oper *].
 * ne     = Number of sum terms in spin Hamiltonian energy operator[input; int]
 * t      = Transition operator for spin Hamiltonian [input; epr_oper *]
 * nt     = Number of sum terms in transition operator [input; int]
 * peaks  = Peak array for lines [output; epr_peak *]
 * 
 * Returns: Number of peaks generated.
 */

int epr_hamilton_lfollow = 0; /* 1 = follow current inital order,
				2 = set initial order */
int epr_hamilton_divs = 10;
int epr_hamilton_constant = 0;
FILE *epr_hamilton_br = NULL;
int epr_hamilton_units = 0; /* 0 = GHz, 1 = cm-1 */
double epr_hamilton_from, epr_hamilton_to;
int epr_hamilton_l1, epr_hamilton_l2;
extern int epr_misc_hverbose;
int epr_hamilton_mode = EPR_SH_NORMAL;
double epr_hamilton_freq;
int epr_hamilton_strict = 0; /* strict mode: 0 = no, 1 = yes, peaks are ordered for interpolation */

/*
 * mode = 0: normal simulation (or angle graph - see misc.c).
 * mode = 1: selection rule visualization.
 * mode = 2: transition moment visualization.
 * mode = 3: Breit-Rabi graph.
 * mode = 4: angle graph (see misc.c).
 *
 */

int epr_hamilton_generate(epr_sim_set *sims, epr_type *sp, epr_oper *e, int ne, epr_oper *t, int nt, epr_peak *peaks) {

  int i, j, k, dim, dim2, npeaks = 0, ld, ud, te;
  double step, a1, a2, b1, b2, de1, de2, tmp, temp;
  double energy = sp->frequency, alpha, aeff, beff;
  doublecomplex *ptr_vec1, *ptr_vec2, *ptr_vtmp;
  double *ptr_w1, *ptr_w2, *ptr_wtmp, *ptr_bz1, *ptr_bz2, from_pop, to_pop;
  int transa[EPR_MAXDIM], transb[EPR_MAXDIM], tmptrans[EPR_MAXDIM];
  int *tra, *trb, *trt;

  /* initialize */
  if(!epr_misc_preload) epr_hamilton_init();

  /* Breit-Rabi graphs */
  if(epr_hamilton_mode > EPR_SH_NORMAL && epr_hamilton_mode < EPR_SH_ANGLE) {
    if(epr_hamilton_mode == EPR_SH_SRULE || epr_hamilton_mode == EPR_SH_TRMOM) {
      gen_b[0] = epr_hamilton_from;
    } else {
      step = fabs(epr_hamilton_from - epr_hamilton_to) / (double) (epr_hamilton_divs-1);
      for (i = 0; i < epr_hamilton_divs; i++)
	gen_b[i] = epr_hamilton_from + step * (double) i;
    }
  } else {
    if(!epr_misc_preload) {
      /* normal simulation */
      step = fabs(sp->xbeg - sp->xend) / (double) (epr_hamilton_divs-1);
      for (i = 0; i < epr_hamilton_divs; i++)
	gen_b[i] = sp->xbeg + step * (double) i;
    }
  }

  /* initialize spin hamiltonian (original) without magn. field terms */
  if(!epr_misc_preload) {
    h_org.h = sto_h_org;
    epr_hamilton_prepare(sims, &h_org);
    if(h_org.h == NULL) return -1;
    /* initialize transition moment operator */
    bcopy(&h_org, &h_1, sizeof(epr_hamilton));
    h_1.h = sto_h1;
    
    /* calculate spin hamilton matrix elements (field independent terms only) */
    for (i = 0; i < ne; i++)
      epr_hamilton_add(&h_org, &e[i], 0);
  
    /* temporary spin hamiltonian in which magn. field terms have been added */
    bcopy(&h_org, &h, sizeof(epr_hamilton));
    h.h = sto_h;
  }

  dim = h_org.dim;
  dim2 = dim * dim;
  if(dim >= EPR_MAXDIM) {
    fprintf(stderr,"libepr: spin hamilton matrix too large.\n");
    return -1; /* too large matrix (EPRMAXDIM) */
  }

  /* the tansition moment operator must be recalculated each time */
  /* because the field direction may change and the transition operator */
  /* must be 90o or 0o with respect to that */
  /* calculate transition moment operator matrix elements */
  bzero(h_1.h, sizeof(doublecomplex) * dim2);
  for (i = 0; i < nt; i++)
    epr_hamilton_add(&h_1, &t[i], -1);

  /*
   * Graphing modes.
   */

  if(epr_hamilton_mode == EPR_SH_SRULE) { /* selection rule vis. */
    int l1, l2;
    char tb[128];
    /* epr_hamilton_from gives the field value for visualization: */
    /* and l1, l2 levels which are visualized */
    bcopy(h_org.h, h.h, sizeof(doublecomplex) * dim2);
    for (k = 0; k < ne; k++) {
      if(e[k].bdeg != 0) e[k].b = epr_hamilton_from;
      epr_hamilton_add(&h, &e[k], 1);
    }
    epr_misc_heigen(h.h, dim, gen_w1, gen_vec1);
    /* square coefficients */
    for (j = 0; j < dim; j++) { /* eigen vector index */
      int tidx;
      tidx = EPR_INDEX(dim, j, epr_hamilton_l1);
      gen_vec1[tidx].r = gen_vec1[tidx].r * gen_vec1[tidx].r +
	gen_vec1[tidx].i * gen_vec1[tidx].i;
      gen_vec1[tidx].i = 0.0;
      tidx = EPR_INDEX(dim, j, epr_hamilton_l2);
      gen_vec1[tidx].r = gen_vec1[tidx].r * gen_vec1[tidx].r +
	gen_vec1[tidx].i * gen_vec1[tidx].i;
      gen_vec1[tidx].i = 0.0;
    }
    for (j = 0; j < dim; j++) /* eigen vector index */
      fprintf(epr_hamilton_br, "%d %le\n", j,
	      gen_vec1[EPR_INDEX(dim, j, epr_hamilton_l1)].r - 
	      gen_vec1[EPR_INDEX(dim, j, epr_hamilton_l2)].r);    
    return 0;
  }

  if(epr_hamilton_mode == EPR_SH_TRMOM) { /* trmom */
    /* Solve spin hamiltonian at epr_hamilton_from (ie. gen_b[0]) */
    /* the eigen vectors are required for transition moments */
    bcopy(h_org.h, h.h, sizeof(doublecomplex) * dim2);
    for (j = 0; j < ne; j++) {
      if(e[j].bdeg != 0) e[j].b = gen_b[0];
      epr_hamilton_add(&h, &e[j], 1);
    }
    epr_misc_heigen(h.h, dim, gen_w1, gen_vec1);
    /* calculate transition moments */
    for (j = 0; j < dim; j++)
      for (k = 0; k < dim; k++) {
	fprintf(epr_hamilton_br, "%d %d %le\n", j, k, epr_hamilton_int(&h_1, gen_vec1, j, k));
      }
    return 0;
  }

  if(epr_hamilton_mode == EPR_SH_BRGRAPH) { /* BR graph */
    tra = transa;
    trb = transb;
    /* loop over states and division points */
    for (i = 0; i < dim; i++) {
      /* strict level follow (translate table) */
      for(j = 0; j < dim; j++)
	tra[j] = trb[j] = j;
      for (j = 0; j < epr_hamilton_divs; j++) {
	bcopy(h_org.h, h.h, sizeof(doublecomplex) * dim2);
	for (k = 0; k < ne; k++) {
	  if(e[k].bdeg != 0) e[k].b = gen_b[j];
	  epr_hamilton_add(&h, &e[k], 1);
	}

	epr_misc_heigen(h.h, dim, gen_w1, gen_vec1);

	if(epr_hamilton_lfollow) {
	  if(!j)
	    bcopy(gen_vec1, gen_vec_ini, sizeof(doublecomplex) * dim2);
	  else {
	    epr_hamilton_correlate(gen_vec_ini, gen_vec1, dim, tmptrans);
	    for (k = 0; k < dim; k++)
	      tra[k] = tmptrans[tra[k]];
	    bcopy(gen_vec1, gen_vec_ini, sizeof(doublecomplex) * dim2);
	  }
	}
	if(epr_hamilton_units == 0)
	  fprintf(epr_hamilton_br, "%le %le\n", gen_b[j], gen_w1[tra[i]] * 1.0E-9);
	else
	  fprintf(epr_hamilton_br, "%le %le\n", gen_b[j], gen_w1[tra[i]] / EPR_CM2HZ);
      }
      fprintf(epr_hamilton_br, "\n");
    }
    return 0;
  }

  /*
   * End graphing modes.
   *
   */


  /*
   * Spectrum simulation (epr_hamilton_mode == EPR_SH_NORMAL || EPR_SH_ANGLE)
   * 
   * There is a general problem: If the energy levels cross within
   * the division points then the transition moment calculation can
   * calculate incorrect transition moments (ie. it confuses the states)
   *
   * The transition energies and moments are obtained by linear interpolation.
   *
   * See eigen state following below.
   *
   */

  /* user specified active space ? */
  if(sims->begin == -1 && sims->end) {
    ld = 0;
    ud = dim;
  } else {
    ld = sims->begin;
    ud = sims->end;
    if(ld < 0) ld = 0;
    if(ud > dim) ud = dim;
  }

  /* calculation at the first division point */
  ptr_w1 = gen_w1;
  ptr_vec1 = gen_vec1;
  ptr_bz1 = bz1;
  bcopy(h_org.h, h.h, sizeof(doublecomplex) * dim2);
  for (j = 0; j < ne; j++)
    if(e[j].bdeg != 0) {
      e[j].b = gen_b[0];
      epr_hamilton_add(&h, &e[j], 1); /* include magn. field dep. terms */
    }

   epr_misc_heigen(h.h, dim, ptr_w1, ptr_vec1);

  /* strict level follow (translate table) */
  tra = transa;
  trb = transb;
  for(i = 0; i < dim; i++)
    tra[i] = trb[i] = i;

  /*
   * epr_hamilton_lfollow:   0 = no eigen follow
   *                        1 = following based on ptr_vec_ini
   *                        2 = set ptr_vec_ini and follow
   *                        3 = follow on ptr_vec_ini and update afterwards
   *
   * mode 3 is used in lin. interpolation (see powder.c).
   *
   */

  if(epr_hamilton_lfollow) {
    if(epr_hamilton_lfollow == 2)
      bcopy(ptr_vec1, gen_vec_ini, sizeof(doublecomplex) * dim2);
    else
      epr_hamilton_correlate(gen_vec_ini, ptr_vec1, dim, tra);
    if(epr_hamilton_lfollow == 3)
      bcopy(ptr_vec1, gen_vec_ini, sizeof(doublecomplex) * dim2);
  }

  temp = sims->spin_temp;
  if(temp > 0.0) {
    tmp = 0.0;
    for (j = ld; j < ud; j++) {
      ptr_bz1[j] = exp(EPR_HZ2J * (ptr_w1[ld] - ptr_w1[j]) / (EPR_BOLTZMANN * temp));
      tmp += ptr_bz1[j];
    }
    for (j = ld; j < ud; j++)
      ptr_bz1[j] /= tmp;
  }

  ptr_w2 = gen_w2;
  ptr_vec2 = gen_vec2;
  ptr_bz2 = bz2;

  /* calculation at the rest of the division points */

  /* Transition order remains the same during powder interpolation */
  npeaks = 0;
  bzero(peaks, sizeof(epr_peak) * EPR_MAXPEAKS);

  for (i = 1; i < epr_hamilton_divs; i++) {

    /* build spin hamiltonian */
    bcopy(h_org.h, h.h, sizeof(doublecomplex) * dim2);
    for (j = 0; j < ne; j++)
      if(e[j].bdeg != 0) {
	e[j].b = gen_b[i];
	epr_hamilton_add(&h, &e[j], 1); /* include magn. field dep. terms */
      }
    /* diagonalize */
    epr_misc_heigen(h.h, dim, ptr_w2, ptr_vec2);

    /* if strict level following is requested then build translate tables */
    if(epr_hamilton_lfollow) {
      epr_hamilton_correlate(ptr_vec1, ptr_vec2, dim, tmptrans);
      for (j = 0; j < dim; j++)
	trb[j] = tmptrans[tra[j]];
    }

    /* Boltzmann weighting */
    if(temp > 0.0) {
      tmp = 0.0;
      for (j = ld; j < ud; j++) {
	ptr_bz2[trb[j]] = exp(EPR_HZ2J * (ptr_w2[trb[ld]] - ptr_w2[trb[j]])
			      / (EPR_BOLTZMANN * temp));
	tmp += ptr_bz2[trb[j]];
      }
      for (j = ld; j < ud; j++)
	ptr_bz2[trb[j]] /= tmp;
    }

    /* loop over transitions */
    for (k = ld; k < ud; k++)
      for (j = k + 1; j < ud; j++) {
	int zp;
	de1 = fabs(ptr_w1[tra[k]] - ptr_w1[tra[j]]);
	de2 = fabs(ptr_w2[trb[k]] - ptr_w2[trb[j]]);
	if((energy > de1 && energy <= de2) ||
	   (energy > de2 && energy <= de1)) {
	  /* resonance and transition moment (linear) interpolation */
	  alpha = (energy - de2) / (de1 - de2);
	  if(!epr_hamilton_constant) {
	    a1 = epr_hamilton_int(&h_1, ptr_vec1, tra[k], tra[j]);
	    a2 = epr_hamilton_int(&h_1, ptr_vec2, trb[k], trb[j]);
	    aeff = alpha * a1 + (1.0 - alpha) * a2;
	  } else aeff = 1.0;
	  b1 = gen_b[i-1];
	  b2 = gen_b[i];
	  beff = alpha * b1 + (1.0 - alpha) * b2;
	  if(temp > 0.0) {
	    from_pop = (ptr_bz1[tra[k]] + ptr_bz2[trb[k]]) / 2.0;
	    to_pop = (ptr_bz1[tra[j]] + ptr_bz2[trb[j]]) / 2.0;
	    aeff *= from_pop * fabs(from_pop - to_pop);
	  }
	  if(epr_misc_hverbose)
	    fprintf(stderr,"libepr: Peak at %le with intensity %le\n", beff, aeff);
	  if(!epr_hamilton_strict) {
	    if(fabs(aeff) > EPR_EPS) peaks[npeaks].intensity = aeff*sims->amp;
	    else peaks[npeaks].intensity = 0.0;
	    peaks[npeaks++].pos = beff;
	  } else {
	    if(fabs(aeff) > EPR_EPS) {
	      zp = EPR_INDEX(dim,k,j);
	      /* If index >= current number of peaks then grow # of peaks */
	      while(peaks[zp].dummy != 0 && zp < EPR_MAXPEAKS) zp += dim2;
	      te = zp + 1;
	      if(te > npeaks) npeaks = te;
	      if(npeaks >= EPR_MAXPEAKS) {
		fprintf(stderr,"libepr: out of peak storage! (increase MAXPEAKS)\n");
		return -1;
	      }
	      peaks[zp].dummy = 1; /* position used */
	      peaks[zp].pos = beff;
	      peaks[zp].intensity = aeff*sims->amp;
	    }
	  }
	}
      }

    /* swap 1 and 2 */
    ptr_wtmp = ptr_w2;
    ptr_vtmp = ptr_vec2;
    ptr_w2 = ptr_w1;
    ptr_vec2 = ptr_vec1;
    ptr_w1 = ptr_wtmp;
    ptr_vec1 = ptr_vtmp;

    ptr_wtmp = ptr_bz1;
    ptr_bz1 = ptr_bz2;
    ptr_bz2 = ptr_wtmp;

    trt = trb;
    trb = tra;
    tra = trt;
  }
  
  return npeaks;
}

/*
 * Function: void epr_hamilton_endor(epr_sim_set *, epr_type *, epr_oper *, int,
 *                                  epr_oper *, int, epr_oper *, int, epr_peak *)
 *
 * Purpose: Calculate stick ENDOR (frequency swept) spectrum using given
 *          Hamiltonian.
 *
 * sims   = Sim set [input; epr_sim_set *].
 * sp     = Target spectrum [input/output; epr_type *].
 * e      = Energy operator for spin Hamiltonian [input; epr_oper *].
 * ne     = Number of sum terms in spin Hamiltonian energy operator[input; int]
 * t      = Transition operator for (electron) spin Hamiltonian [input; epr_oper *]
 * nt     = Number of sum terms in transition operator [input; int]
 * tn     = Transition operator for (nuclear) spin Hamiltonian [input; epr_oper *]
 * ntn    = Number of sum terms in transition operator [input; int]
 * peaks  = Stick spectrum [output; epr_peak *]
 * 
 * Returns: Number of peaks generated.
 *
 * NOTE: Boltzmann stuff is not yet implemented here.
 *
 * TODO: implement lfollow for angles!
 *
 */

int epr_hamilton_endor(epr_sim_set *sims, epr_type *sp, epr_oper *e, int ne, epr_oper *t, int nt, epr_oper *tn, int ntn, epr_peak *peaks) {

  int i, j, k, dim, dim2, npeaks = 0, ld, ud;
  double tmp, tmp2, tmp3, tmp4, tmp5, thr;

  /* initialize */
  if(!epr_misc_preload) {
    epr_hamilton_init();
    h.h = sto_h;
    epr_hamilton_prepare(sims, &h);
    if(h.h == NULL) return -1;
    /* initialize transition operators h_1 and h_1_n */
    bcopy(&h, &h_1, sizeof(epr_hamilton));
    bcopy(&h, &h_1_n, sizeof(epr_hamilton));
    h_1.h = sto_h1;
    h_1_n.h = sto_h_org;
  }
  dim = h.dim;
  dim2 = dim*dim;
  if(dim >= EPR_MAXDIM) return -1;
  thr = sims->amp / 1000.0;
	       
  /* prepare transition moment operators (EPR & ENDOR) - at every cycle! */
  bzero(h_1.h, sizeof(doublecomplex) * dim2);
  bzero(h_1_n.h, sizeof(doublecomplex) * dim2);
  /* Form electron spin transition operator */
  for (i = 0; i < nt; i++)
    epr_hamilton_add(&h_1, &t[i], -1);    
  /* Form nuclear spin transition operator */
  for (i = 0; i < ntn; i++)
    epr_hamilton_add(&h_1_n, &tn[i], -1);
  
  /* Form spin Hamiltonian (this must be done every cycle) */
  bzero(h.h, sizeof(doublecomplex) * dim2);
  for (i = 0; i < ne; i++) {
    e[i].b = sp->endor_b;
    epr_hamilton_add(&h, &e[i], -1); /* all terms included */
  }
  
  /* Diagonalize spin hamiltonian */
  epr_misc_heigen(h.h, dim, gen_w1, gen_vec1);

  /* user specified active space ? */
  if(sims->begin == -1 && sims->end) {
    ld = 0;
    ud = dim;
  } else {
    ld = sims->begin;
    ud = sims->end;
    if(ld < 0) ld = 0;
    if(ud > dim) ud = dim;
  }

  /* Generate spectrum */
  /* Loop over all transitions and select wanted EPR ones */
  /* constant trmom = real for EPR, constant for ENDOR */
  for (i = ld; i < ud; i++) {
    for (j = i+1; j < ud; j++) {
      /* intensity contribution caused by the EPR line */
      tmp3 = 1.0 / EPR_LWTOIT2(sims->lw);
      tmp4 = fabs(gen_w1[i] - gen_w1[j]);
      tmp5 = (tmp4 - sp->frequency) * tmp3;
      /* Purely Lorentzian LS - TODO: should respect LG-ratio ! */
      tmp = tmp4 * tmp3 / (1.0 + tmp5*tmp5);
      /*      printf("%le %le\n", tmp, tmp3); */
      tmp *= epr_hamilton_int(&h_1, gen_vec1, i, j);
      if(tmp < thr) continue;
      /* Loop over connected ENDOR transitions */
      for (k = ld; k < ud; k++) {
	/* i level */
	if(k != i) {
	  tmp2 = fabs(gen_w1[k] - gen_w1[i])*1.0E-6; /* to MHz */
	  /*	  printf("A. %le\n", tmp2); */
	  /* transition in scan range ? */
	  if(tmp2 >= sp->xbeg && tmp2 <= sp->xend) {
	    /* intensity contribution caused by the ENDOR transition */
	    if(!epr_hamilton_constant)   
	      tmp3 = tmp * epr_hamilton_int(&h_1_n, gen_vec1, k, i) * 1.0E6;
	    else
	      tmp3 = tmp * 1.0E6;
	    /* enough intensity? */
	    if(fabs(tmp3) > EPR_EPS) {
	      peaks[npeaks].pos = tmp2;
	      peaks[npeaks].lw = sims->elw;
	      peaks[npeaks++].intensity = sims->amp * tmp3;
	    }
	  }
	}
	if(k != j) {
	  /* j level */
	  tmp2 = fabs(gen_w1[k] - gen_w1[j])*1.0E-6; /* to MHz */
	  /*	  printf("B. %le\n", tmp2); */
	  /* transition in scan range ? */
	  if(tmp2 >= sp->xbeg && tmp2 <= sp->xend) {
	    /* intensity contribution caused by the ENDOR line */
	    if(!epr_hamilton_constant)
	      tmp3 = tmp * epr_hamilton_int(&h_1_n, gen_vec1, k, j)*1.0E6;
	    else
	      tmp3 = tmp * 1.0E6;
	    /* enough intensity? */
	    if(fabs(tmp3) > EPR_EPS) {
	      peaks[npeaks].pos = tmp2;
	      peaks[npeaks].lw = sims->elw;
	      peaks[npeaks++].intensity = sims->amp * tmp3;
	    }
	  }
	}
      }
    }
  }

  return npeaks;
}

