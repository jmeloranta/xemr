/** Header file generated with fdesign on Wed Jun 10 14:41:30 1998.**/

#ifndef FD_elw_h_
#define FD_elw_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *elw;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *offset;
	FL_OBJECT *amp[14];
	FL_OBJECT *lw[14];
} FD_elw;

extern FD_elw * create_form_elw(void);

#endif /* FD_elw_h_ */
