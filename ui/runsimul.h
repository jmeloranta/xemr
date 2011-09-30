/** Header file generated with fdesign on Sun Mar 22 10:15:56 1998.**/

#ifndef FD_runsimul_h_
#define FD_runsimul_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *runsimul;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *width;
	FL_OBJECT *resolution;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *frequency;
	FL_OBJECT *center;
	FL_OBJECT *endor_b;
} FD_runsimul;

extern FD_runsimul * create_form_runsimul(void);

#endif /* FD_runsimul_h_ */
