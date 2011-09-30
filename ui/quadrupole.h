/** Header file generated with fdesign on Sun Jun 20 15:36:12 1999.**/

#ifndef FD_quadrupole_h_
#define FD_quadrupole_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *quadrupole;
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
	FL_OBJECT *p1;
	FL_OBJECT *p2;
	FL_OBJECT *p3;
	FL_OBJECT *p[9];
} FD_quadrupole;

extern FD_quadrupole * create_form_quadrupole(void);

#endif /* FD_quadrupole_h_ */
