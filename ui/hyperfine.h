/** Header file generated with fdesign on Sun Jun 20 15:34:02 1999.**/

#ifndef FD_hyperfine_h_
#define FD_hyperfine_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *hyperfine;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *header;
	FL_OBJECT *ok;
	FL_OBJECT *nextnucl;
	FL_OBJECT *nextsp;
	FL_OBJECT *cancel;
	FL_OBJECT *prevnucl;
	FL_OBJECT *prevsp;
	FL_OBJECT *ince;
	FL_OBJECT *dece;
	FL_OBJECT *a1;
	FL_OBJECT *a2;
	FL_OBJECT *a3;
	FL_OBJECT *a[9];
} FD_hyperfine;

extern FD_hyperfine * create_form_hyperfine(void);

#endif /* FD_hyperfine_h_ */
