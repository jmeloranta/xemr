/** Header file generated with fdesign on Sun Jun 20 16:24:24 1999.**/

#ifndef FD_gvalue_h_
#define FD_gvalue_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *gvalue;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *header;
	FL_OBJECT *ok;
	FL_OBJECT *nextsp;
	FL_OBJECT *cancel;
	FL_OBJECT *prevsp;
	FL_OBJECT *nextel;
	FL_OBJECT *prevel;
	FL_OBJECT *g1;
	FL_OBJECT *g2;
	FL_OBJECT *g3;
	FL_OBJECT *posn[9];
} FD_gvalue;

extern FD_gvalue * create_form_gvalue(void);

#endif /* FD_gvalue_h_ */
