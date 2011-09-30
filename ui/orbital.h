/** Header file generated with fdesign on Wed Mar  3 15:49:46 1999.**/

#ifndef FD_orbital_h_
#define FD_orbital_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *orbital;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *so;
	FL_OBJECT *parama1;
	FL_OBJECT *paramb;
	FL_OBJECT *angmom;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *next;
	FL_OBJECT *prev;
	FL_OBJECT *number;
	FL_OBJECT *parama2;
	FL_OBJECT *parama3;
} FD_orbital;

extern FD_orbital * create_form_orbital(void);

#endif /* FD_orbital_h_ */
