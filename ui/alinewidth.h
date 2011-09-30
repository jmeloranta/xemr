/** Header file generated with fdesign on Sat Oct 30 08:26:55 1999.**/

#ifndef FD_alinewidth_h_
#define FD_alinewidth_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *alinewidth;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *header;
	FL_OBJECT *lwx;
	FL_OBJECT *ok;
	FL_OBJECT *nextsp;
	FL_OBJECT *cancel;
	FL_OBJECT *prevsp;
	FL_OBJECT *lg_fact;
	FL_OBJECT *elw;
	FL_OBJECT *lwy;
	FL_OBJECT *lwz;
} FD_alinewidth;

extern FD_alinewidth * create_form_alinewidth(void);

#endif /* FD_alinewidth_h_ */
