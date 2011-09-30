/** Header file generated with fdesign on Wed May 13 11:12:19 1998.**/

#ifndef FD_equivalence_h_
#define FD_equivalence_h_

/** Callbacks, globals and object handlers **/


/**** Forms and Objects ****/
typedef struct {
	FL_FORM *equivalence;
	void *vdata;
	char *cdata;
	long  ldata;
	FL_OBJECT *ok;
	FL_OBJECT *cancel;
	FL_OBJECT *next;
	FL_OBJECT *prev;
	FL_OBJECT *clear;
	FL_OBJECT *dflis;
	FL_OBJECT *dflendor;
	FL_OBJECT *property[7];
	FL_OBJECT *from_set[7];
	FL_OBJECT *from_index1[7];
	FL_OBJECT *from_index2[7];
	FL_OBJECT *to_set[7];
	FL_OBJECT *to_index1[7];
	FL_OBJECT *to_index2[7];
	FL_OBJECT *lab[7];
	FL_OBJECT *from_index3[7];
	FL_OBJECT *to_index3[7];
	FL_OBJECT *from_index4[7];
	FL_OBJECT *to_index4[7];
} FD_equivalence;

extern FD_equivalence * create_form_equivalence(void);

#endif /* FD_equivalence_h_ */
