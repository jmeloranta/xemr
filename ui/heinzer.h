/** Header file generated with fdesign on Wed Apr  1 09:14:41 1998.**/

#ifndef FD_heinzer_h_
#define FD_heinzer_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *heinzer;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *header;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *zero;
	FL_OBJECT *k[64];
} FD_heinzer;

extern FD_heinzer * create_form_heinzer(void);

#endif /* FD_heinzer_h_ */
