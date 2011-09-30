/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "runml.h"

FD_runml *create_form_runml(void)
{
  FL_OBJECT *obj;
  FD_runml *fdui = (FD_runml *) fl_calloc(1, sizeof(*fdui));

  fdui->runml = fl_bgn_form(FL_NO_BOX, 350, 330);
  obj = fl_add_box(FL_UP_BOX,0,0,350,330,"");
  obj = fl_add_text(FL_NORMAL_TEXT,80,20,170,40,"Run: Marquardt");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->tolerance = obj = fl_add_input(FL_NORMAL_INPUT,130,70,100,40,"Tolerance");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,40,260,90,40,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,210,260,90,40,"Cancel");
  fdui->diff = obj = fl_add_input(FL_NORMAL_INPUT,130,130,100,40,"Difference length");
  fdui->variables = obj = fl_add_button(FL_NORMAL_BUTTON,40,190,90,40,"Variables");
  fdui->equivalences = obj = fl_add_button(FL_NORMAL_BUTTON,210,190,90,40,"Equivalences");
  fl_end_form();

  fdui->runml->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

