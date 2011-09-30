/** Header file generated with fdesign on Wed Mar  3 18:55:18 1999.**/

#ifndef FD_cmhz_h_
#define FD_cmhz_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *cmhz;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *ghz;
	FL_OBJECT *cm;
	FL_OBJECT *units;
} FD_cmhz;

extern FD_cmhz * create_form_cmhz(void);

#endif /* FD_cmhz_h_ */
