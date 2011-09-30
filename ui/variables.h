/** Header file generated with fdesign on Sat Oct  2 16:18:13 1999.**/

#ifndef FD_variables_h_
#define FD_variables_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *variables;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *dflan;
	FL_OBJECT *next;
	FL_OBJECT *prev;
	FL_OBJECT *clear;
	FL_OBJECT *dflis;
	FL_OBJECT *dflendor;
	FL_OBJECT *lab[10];
	FL_OBJECT *set[10];
	FL_OBJECT *arg1[10];
	FL_OBJECT *arg2[10];
	FL_OBJECT *mc_limit[10];
	FL_OBJECT *high[10];
	FL_OBJECT *low[10];
	FL_OBJECT *type[10];
	FL_OBJECT *arg3[10];
	FL_OBJECT *arg4[10];
	FL_OBJECT *active[10];
	FL_OBJECT *value[10];
} FD_variables;

extern FD_variables * create_form_variables(void);

#endif /* FD_variables_h_ */
