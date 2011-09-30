#ifndef FD_mark_h_
#define FD_mark_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *mark;
	FL_OBJECT *ok;
	void *vdata;
	long ldata;
} FD_mark;

extern FD_mark * create_form_mark(void);

#endif /* FD_mark_h_ */
