/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "hj.h"

FD_hj *create_form_hj(void)
{
  FL_OBJECT *obj;
  FD_hj *fdui = (FD_hj *) fl_calloc(1, sizeof(*fdui));

  fdui->hj = fl_bgn_form(FL_NO_BOX, 350, 330);
  obj = fl_add_box(FL_UP_BOX,0,0,350,330,"");
  obj = fl_add_text(FL_NORMAL_TEXT,80,20,190,40,"Run: Hooke-Jeeves");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->maxiter = obj = fl_add_input(FL_NORMAL_INPUT,130,70,100,40,"Maxiter");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,40,260,90,40,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,210,260,90,40,"Cancel");
  fdui->diff = obj = fl_add_input(FL_NORMAL_INPUT,130,130,100,40,"Difference length");
  fdui->variables = obj = fl_add_button(FL_NORMAL_BUTTON,40,190,90,40,"Variables");
  fdui->equivalences = obj = fl_add_button(FL_NORMAL_BUTTON,210,190,90,40,"Equivalences");
  fl_end_form();

  fdui->hj->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

