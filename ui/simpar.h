/** Header file generated with fdesign on Thu Mar 18 17:57:34 1999.**/

#ifndef FD_simparam_h_
#define FD_simparam_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *simparam;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *lineshape;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *system;
	FL_OBJECT *run;
	FL_OBJECT *nspec;
	FL_OBJECT *target;
	FL_OBJECT *auto_amp;
	FL_OBJECT *phi;
	FL_OBJECT *theta;
	FL_OBJECT *hamilton;
	FL_OBJECT *transition;
	FL_OBJECT *dlevel;
	FL_OBJECT *iunits;
	FL_OBJECT *orbital;
	FL_OBJECT *active;
	FL_OBJECT *begin;
	FL_OBJECT *end;
	FL_OBJECT *boltzmann;
	FL_OBJECT *temp;
} FD_simparam;

extern FD_simparam * create_form_simparam(void);

#endif /* FD_simparam_h_ */
