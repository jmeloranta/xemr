/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "hyperfine.h"

FD_hyperfine *create_form_hyperfine(void)
{
  FL_OBJECT *obj;
  FD_hyperfine *fdui = (FD_hyperfine *) fl_calloc(1, sizeof(*fdui));

  fdui->hyperfine = fl_bgn_form(FL_NO_BOX, 520, 430);
  obj = fl_add_box(FL_UP_BOX,0,0,520,430,"");
  fdui->header = obj = fl_add_text(FL_NORMAL_TEXT,20,20,470,50,"Hyperfine parameters for spectrum 1 / nucleus 1 / electron 1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_ENGRAVED_STYLE);
  fdui->a[0] = obj = fl_add_input(FL_NORMAL_INPUT,60,140,100,40,"Ax");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a[3] = obj = fl_add_input(FL_NORMAL_INPUT,230,140,100,40,"Ax");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a[6] = obj = fl_add_input(FL_NORMAL_INPUT,390,140,100,40,"Ax");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a[1] = obj = fl_add_input(FL_NORMAL_INPUT,60,190,100,40,"Ay");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a[4] = obj = fl_add_input(FL_NORMAL_INPUT,230,190,100,40,"Ay");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a[7] = obj = fl_add_input(FL_NORMAL_INPUT,390,190,100,40,"Ay");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a[2] = obj = fl_add_input(FL_NORMAL_INPUT,60,240,100,40,"Az");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a[5] = obj = fl_add_input(FL_NORMAL_INPUT,230,240,100,40,"Az");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a[8] = obj = fl_add_input(FL_NORMAL_INPUT,390,240,100,40,"Az");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,20,310,90,40,"OK");
  fdui->nextnucl = obj = fl_add_button(FL_NORMAL_BUTTON,150,310,90,40,"Next nucl.");
  fdui->nextsp = obj = fl_add_button(FL_NORMAL_BUTTON,280,310,90,40,"Next SP");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,20,370,90,40,"Cancel");
  fdui->prevnucl = obj = fl_add_button(FL_NORMAL_BUTTON,150,370,90,40,"Prev nucl.");
  fdui->prevsp = obj = fl_add_button(FL_NORMAL_BUTTON,280,370,90,40,"Prev SP");
  fdui->ince = obj = fl_add_button(FL_NORMAL_BUTTON,410,310,90,40,"Inc e");
  fdui->dece = obj = fl_add_button(FL_NORMAL_BUTTON,410,370,90,40,"Dec e");
  fdui->a1 = obj = fl_add_input(FL_NORMAL_INPUT,60,80,100,40,"A1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a2 = obj = fl_add_input(FL_NORMAL_INPUT,230,80,100,40,"A2");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->a3 = obj = fl_add_input(FL_NORMAL_INPUT,390,80,100,40,"A3");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fl_end_form();

  fdui->hyperfine->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

