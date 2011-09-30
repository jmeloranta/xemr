#ifndef FD_summary_h_
#define FD_summary_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *summary;
	void *vdata;
	long ldata;
	FL_OBJECT *txt;
	FL_OBJECT *ok;
} FD_summary;

extern FD_summary * create_form_summary(void);

#endif /* FD_summary_h_ */
