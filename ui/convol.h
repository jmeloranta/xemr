#ifndef FD_convol_h_
#define FD_convol_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *convol;
	FL_OBJECT *convolution;
	FL_OBJECT *type;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	void *vdata;
	long ldata;
} FD_convol;

extern FD_convol * create_form_convol(void);

#endif /* FD_convol_h_ */
