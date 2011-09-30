#ifndef FD_differences_h_
#define FD_differences_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *differences;
	FL_OBJECT *resolution;
	FL_OBJECT *maxhfc;
	FL_OBJECT *noiselimit;
	FL_OBJECT *method;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	void *vdata;
	long ldata;
} FD_differences;

extern FD_differences * create_form_differences(void);

#endif /* FD_differences_h_ */
