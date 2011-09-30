#ifndef FD_runmon_h_
#define FD_runmon_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *runmon;
	void *vdata;
	long ldata;
	FL_OBJECT *iterations;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *variables;
	FL_OBJECT *equivalences;
} FD_runmon;

extern FD_runmon * create_form_runmon(void);

#endif /* FD_runmon_h_ */
