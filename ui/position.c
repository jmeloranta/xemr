/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "position.h"

FD_position *create_form_position(void)
{
  FL_OBJECT *obj;
  FD_position *fdui = (FD_position *) fl_calloc(1, sizeof(*fdui));

  fdui->position = fl_bgn_form(FL_NO_BOX, 230, 250);
  obj = fl_add_box(FL_UP_BOX,0,0,230,250,"");
  obj = fl_add_text(FL_NORMAL_TEXT,50,20,160,30,"Peak position analysis");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->noiselevel = obj = fl_add_input(FL_NORMAL_INPUT,60,80,120,30,"Noise level");
  fdui->method = obj = fl_add_choice(FL_NORMAL_CHOICE,60,140,80,30,"Method");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,80,200,70,30,"OK");
  fl_end_form();

  return fdui;
}
/*---------------------------------------*/

