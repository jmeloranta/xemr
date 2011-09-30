#ifndef FD_generic_h_
#define FD_generic_h_
/* Header file generated with fdesign. */

/** Callback routines and free object handlers **/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *generic;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *label;
	FL_OBJECT *espin;
	FL_OBJECT *theta_offset;
	FL_OBJECT *phi_offset;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *nextsp;
	FL_OBJECT *prevsp;
	FL_OBJECT *amplitude;
	FL_OBJECT *info[12];
	FL_OBJECT *nbr[12];
	FL_OBJECT *atom[12];
} FD_generic;

extern FD_generic * create_form_generic(void);

#endif /* FD_generic_h_ */
