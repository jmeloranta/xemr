/** Header file generated with fdesign on Sat Jun 12 11:45:23 1999.**/

#ifndef FD_hj_h_
#define FD_hj_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *hj;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *maxiter;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *diff;
	FL_OBJECT *variables;
	FL_OBJECT *equivalences;
} FD_hj;

extern FD_hj * create_form_hj(void);

#endif /* FD_hj_h_ */
