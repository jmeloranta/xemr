#ifndef FD_calibsig_h_
#define FD_calibsig_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *calibsig;
	FL_OBJECT *intensity;
	FL_OBJECT *exit;
	FL_OBJECT *timer;
	void *vdata;
	long ldata;
} FD_calibsig;

extern FD_calibsig * create_form_calibsig(void);

#endif /* FD_calibsig_h_ */
