/** Header file generated with fdesign on Wed Jan 17 21:22:05 2007.**/

#ifndef FD_misc_h_
#define FD_misc_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *misc;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *phibegin;
	FL_OBJECT *phiend;
	FL_OBJECT *thetabegin;
	FL_OBJECT *thetaend;
	FL_OBJECT *phisteps;
	FL_OBJECT *thetasteps;
	FL_OBJECT *method;
	FL_OBJECT *points;
	FL_OBJECT *diags;
	FL_OBJECT *linecutoff;
	FL_OBJECT *ok;
	FL_OBJECT *debug;
	FL_OBJECT *interp;
	FL_OBJECT *follow;
} FD_misc;

extern FD_misc * create_form_misc(void);

#endif /* FD_misc_h_ */
