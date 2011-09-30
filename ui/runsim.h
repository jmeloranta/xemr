#ifndef FD_runsim_h_
#define FD_runsim_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *runsim;
	void *vdata;
	long ldata;
	FL_OBJECT *iterations;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *restarts;
	FL_OBJECT *variables;
	FL_OBJECT *equivalences;
} FD_runsim;

extern FD_runsim * create_form_runsim(void);

#endif /* FD_runsim_h_ */
