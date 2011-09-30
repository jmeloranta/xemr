/** Header file generated with fdesign on Tue May 19 16:53:09 1998.**/

#ifndef FD_runml_h_
#define FD_runml_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *runml;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *tolerance;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *diff;
	FL_OBJECT *variables;
	FL_OBJECT *equivalences;
} FD_runml;

extern FD_runml * create_form_runml(void);

#endif /* FD_runml_h_ */
