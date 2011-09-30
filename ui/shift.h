#ifndef FD_shift_h_
#define FD_shift_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *shift;
	void *vdata;
	long ldata;
	FL_OBJECT *dxpos;
	FL_OBJECT *ixpos;
	FL_OBJECT *iint;
	FL_OBJECT *iypos;
	FL_OBJECT *dypos;
	FL_OBJECT *dint;
	FL_OBJECT *exit;
	FL_OBJECT *xstep;
	FL_OBJECT *ystep;
	FL_OBJECT *istep;
	FL_OBJECT *gtxt;
	FL_OBJECT *btxt;
	FL_OBJECT *itxt;
	FL_OBJECT *ytxt;
	FL_OBJECT *reset;
} FD_shift;

extern FD_shift * create_form_shift(void);

#endif /* FD_shift_h_ */
