#ifndef FD_polyn_h_
#define FD_polyn_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *polyn;
	FL_OBJECT *degree;
	FL_OBJECT *cont;
	FL_OBJECT *cancel;
	void *vdata;
	long ldata;
} FD_polyn;

extern FD_polyn * create_form_polyn(void);

#endif /* FD_polyn_h_ */
