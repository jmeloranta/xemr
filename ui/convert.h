#ifndef FD_convert_h_
#define FD_convert_h_
/* Header file generated with fdesign. */

/**** Callback routines ****/



/**** Forms and Objects ****/

typedef struct {
	FL_FORM *convert;
	void *vdata;
	long ldata;
	FL_OBJECT *format;
	FL_OBJECT *ok;
} FD_convert;

extern FD_convert * create_form_convert(void);

#endif /* FD_convert_h_ */
