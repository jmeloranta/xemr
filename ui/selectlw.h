#ifndef FD_selectlw_h_
#define FD_selectlw_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *selectlw;
	void *vdata;
	long ldata;
	FL_OBJECT *rate;
	FL_OBJECT *linewidth;
} FD_selectlw;

extern FD_selectlw * create_form_selectlw(void);

#endif /* FD_selectlw_h_ */
