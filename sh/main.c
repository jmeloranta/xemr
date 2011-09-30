#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <math.h>
#include <epr.h>

void MAIN__() { }

epr_hamilton h;

doublecomplex vecs[256*256];
double eigen[256];

main() {

  int i, j;
  char buf[256], *ptr;
  double c;
  epr_oper op;
  double cs[EPR_MAXGROUPS];
  char labels[EPR_MAXGROUPS][20];

  printf("*** Spin definitions ***\n");
  printf("Number of spins: ");
  scanf(" %d", &(h.nspins));
  h.dim = 1;
  epr_hamilton_init();
  for (i = 0; i < h.nspins; i++) {
    printf("Spin #%d: ", i+1);
    scanf(" %lf", &(h.spins[i]));
    h.dim *= (int) (2.0 * h.spins[i] + 1.0);
    cs[i] = -h.spins[i];
  }
  h.h = (doublecomplex *) malloc(sizeof(doublecomplex) * h.dim * h.dim);
  bzero(h.h, sizeof(doublecomplex) * h.dim * h.dim);

  j = 0;
  while (1) {
    strcpy(labels[j], "");
    for (i = 0; i < h.nspins; i++) {
      sprintf(buf,"%.1lf ", -cs[i]);
      strcat(labels[j], buf);
    }
    j++;
    for (i = 0; i < h.nspins; i++)
      if(cs[i] != h.spins[i]) {
	cs[i] += 1.0;
	break;
      } else cs[i] = -h.spins[i];
    if(i == h.nspins) break;
  }

  while (1) {
    printf("cmd: ");
    bzero(buf, 256);
    /* scanf(" %[^\n\r]", buf); */
    gets(buf);
    if(feof(stdin)) exit(0);
    if(buf[0] == '#') continue;
    if((ptr = strchr(buf, '\n'))) *ptr = (char) 0;
    if(!buf[0]) continue;
    if(!strncmp(buf, "print", 5)) {
      printf("\n");
      printf("%27s", "");
      for (i = 0; i < h.dim; i++)
	printf("%27s", labels[i]);
      printf("\n");
      for (i = 0; i < h.dim; i++) {
	printf("%27s", labels[i]);
	for (j = 0; j < h.dim; j++)
	  printf("(%le,%le) ", h.h[EPR_INDEX(h.dim,i,j)].r, h.h[EPR_INDEX(h.dim,i,j)].i);
	printf("\n");
      }
      continue;
    }
    if(!strncmp(buf, "add", 3)) {
      char *ptr;
      int ii, jj;
      doublecomplex tm;
      ptr = buf + 4;
      sscanf(ptr, " %d %d %le %le", &ii, &jj, &tm.r, &tm.i);
      h.h[EPR_INDEX(h.dim, ii, jj)].r += tm.r;
      h.h[EPR_INDEX(h.dim, ii, jj)].i += tm.i;
      continue;
    }
    if(!strncmp(buf, "int", 3)) {
      printf("\nIntensity matrix:\n");
      for (i = 0; i < h.dim; i++) {
	for (j = 0; j < h.dim; j++)
	  printf("%le ", epr_hamilton_int(&h, vecs, i, j));
	puts("\n");
      }
      continue;
    }
    if(!strncmp(buf, "diag", 4)) {
      printf("\n");
      epr_misc_heigen(h.h, h.dim, eigen, vecs);
      for (i = 0; i < h.dim; i++) {
	printf("Eigen value: %le\n", eigen[i]);
	printf("Eigen vector (real): ");
	for (j = 0; j < h.dim; j++)
	  printf("%le ", vecs[EPR_INDEX(h.dim,j,i)].r);
	puts("\n");
	printf("Eigen vector (imag): ");
	for (j = 0; j < h.dim; j++)
	  printf("%le ", vecs[EPR_INDEX(h.dim,j,i)].i);
	puts("\n");
      }
      continue;
    }
    if(!strncmp(buf, "expec", 5)) {
      doublecomplex tmp;
      epr_imag_matr_mult(h.h, vecs, h.dim);
      for (i = 0; i < h.dim; i++) {
	tmp.r = tmp.i = 0.0;
	for (j = 0; j < h.dim; j++) {
	  tmp.r += vecs[EPR_INDEX(h.dim, j, i)].r * h.h[EPR_INDEX(h.dim, j, i)].r
	    - vecs[EPR_INDEX(h.dim, j, i)].i * h.h[EPR_INDEX(h.dim, j, i)].i;
	  tmp.i += vecs[EPR_INDEX(h.dim, j, i)].i * h.h[EPR_INDEX(h.dim, j, i)].r
	    + vecs[EPR_INDEX(h.dim, j, i)].r * h.h[EPR_INDEX(h.dim, j, i)].i;
	}
	printf("%d. Expectation value = (%le, %le)\n", i, tmp.r, tmp.i);
      }
    }
    if(!strncmp(buf, "zero", 4)) {
      bzero(h.h, sizeof(doublecomplex) * h.dim * h.dim);
      continue;
    }
    if(!strncmp(buf, "exit", 4)) exit(0);
    sscanf(buf, " %le", &c);
    op.m = c;
    ptr = buf;
    op.nbr = 0;
    for(i = 0; *ptr; i++) {
      for ( ; *ptr && *ptr != ' '; ptr++);
      for ( ; *ptr && *ptr == ' '; ptr++);
      if(!*ptr) break;
      if(*ptr == '+') op.d[i] = 3;
      else if(*ptr == '-') op.d[i] = 4;
      else if(*ptr == 'i') op.d[i] = 5;
      else op.d[i] = (int) (*ptr - 'x');
      op.e[i] = 1;
      ptr += 2;
      op.n[i] = atoi(ptr)-1;
      op.bdeg = 0;
      op.b = 0.0;
      op.nbr++;
    }
    epr_hamilton_add(&h, &op, -1);
  }
}
