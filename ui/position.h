#ifndef FD_position_h_
#define FD_position_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *position;
	FL_OBJECT *noiselevel;
	FL_OBJECT *method;
	FL_OBJECT *ok;
	void *vdata;
	long ldata;
} FD_position;

extern FD_position * create_form_position(void);

#endif /* FD_position_h_ */
