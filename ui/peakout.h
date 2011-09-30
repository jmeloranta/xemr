#ifndef FD_peakout_h_
#define FD_peakout_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *peakout;
	FL_OBJECT *results;
	FL_OBJECT *ok;
	void *vdata;
	long ldata;
} FD_peakout;

extern FD_peakout * create_form_peakout(void);

#endif /* FD_peakout_h_ */
