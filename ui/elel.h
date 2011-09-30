/** Header file generated with fdesign on Sun Jun 20 15:38:10 1999.**/

#ifndef FD_elel_h_
#define FD_elel_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *elel;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *header;
	FL_OBJECT *ok;
	FL_OBJECT *nextsp;
	FL_OBJECT *cancel;
	FL_OBJECT *prevsp;
	FL_OBJECT *ince1;
	FL_OBJECT *dece1;
	FL_OBJECT *ince2;
	FL_OBJECT *dece2;
	FL_OBJECT *quick;
	FL_OBJECT *d1;
	FL_OBJECT *d2;
	FL_OBJECT *d3;
	FL_OBJECT *d[9];
} FD_elel;

extern FD_elel * create_form_elel(void);

#endif /* FD_elel_h_ */
