/** Header file generated with fdesign on Sat Feb 13 16:09:09 1999.**/

#ifndef FD_window_h_
#define FD_window_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *window;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *parametera;
	FL_OBJECT *parameterb;
	FL_OBJECT *type;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
} FD_window;

extern FD_window * create_form_window(void);

#endif /* FD_window_h_ */
