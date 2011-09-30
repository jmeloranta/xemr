#ifndef FD_symmetry_h_
#define FD_symmetry_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *symmetry;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	void *vdata;
	long ldata;
} FD_symmetry;

extern FD_symmetry * create_form_symmetry(void);

#endif /* FD_symmetry_h_ */
