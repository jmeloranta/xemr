/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "quadrupole.h"

FD_quadrupole *create_form_quadrupole(void)
{
  FL_OBJECT *obj;
  FD_quadrupole *fdui = (FD_quadrupole *) fl_calloc(1, sizeof(*fdui));

  fdui->quadrupole = fl_bgn_form(FL_NO_BOX, 520, 440);
  obj = fl_add_box(FL_UP_BOX,0,0,520,440,"");
  fdui->header = obj = fl_add_text(FL_NORMAL_TEXT,20,20,460,50,"Nuclear quadrupole  parameters for spectrum 1 / nucleus 1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_ENGRAVED_STYLE);
  fdui->p[0] = obj = fl_add_input(FL_NORMAL_INPUT,60,150,100,30,"Px");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p[3] = obj = fl_add_input(FL_NORMAL_INPUT,230,150,100,30,"Px");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p[6] = obj = fl_add_input(FL_NORMAL_INPUT,390,150,100,30,"Px");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p[1] = obj = fl_add_input(FL_NORMAL_INPUT,60,200,100,30,"Py");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p[4] = obj = fl_add_input(FL_NORMAL_INPUT,230,200,100,30,"Py");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p[7] = obj = fl_add_input(FL_NORMAL_INPUT,390,200,100,30,"Py");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p[2] = obj = fl_add_input(FL_NORMAL_INPUT,60,250,100,30,"Pz");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p[5] = obj = fl_add_input(FL_NORMAL_INPUT,230,250,100,30,"Pz");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p[8] = obj = fl_add_input(FL_NORMAL_INPUT,390,250,100,30,"Pz");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,90,320,90,40,"OK");
  fdui->nextnucl = obj = fl_add_button(FL_NORMAL_BUTTON,220,320,90,40,"Next nucl.");
  fdui->nextsp = obj = fl_add_button(FL_NORMAL_BUTTON,350,320,90,40,"Next SP");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,90,380,90,40,"Cancel");
  fdui->prevnucl = obj = fl_add_button(FL_NORMAL_BUTTON,220,380,90,40,"Prev nucl.");
  fdui->prevsp = obj = fl_add_button(FL_NORMAL_BUTTON,350,380,90,40,"Prev SP");
  fdui->p1 = obj = fl_add_input(FL_NORMAL_INPUT,60,80,100,30,"P1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p2 = obj = fl_add_input(FL_NORMAL_INPUT,230,80,100,30,"P2");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->p3 = obj = fl_add_input(FL_NORMAL_INPUT,390,80,100,30,"P3");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fl_end_form();

  fdui->quadrupole->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

