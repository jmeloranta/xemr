#ifndef FD_linewidth_h_
#define FD_linewidth_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *linewidth;
	void *vdata;
	long ldata;
	FL_OBJECT *header;
	FL_OBJECT *a;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *nextsp;
	FL_OBJECT *prevsp;
	FL_OBJECT *nextpg;
	FL_OBJECT *prevpg;
	FL_OBJECT *zero;
	FL_OBJECT *editd;
	FL_OBJECT *lab1[5];
	FL_OBJECT *b[5];
	FL_OBJECT *lab2[5];
	FL_OBJECT *c[5];
} FD_linewidth;

extern FD_linewidth * create_form_linewidth(void);

#endif /* FD_linewidth_h_ */
