/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "convol.h"

FD_convol *create_form_convol(void)
{
  FL_OBJECT *obj;
  FD_convol *fdui = (FD_convol *) fl_calloc(1, sizeof(*fdui));

  fdui->convol = fl_bgn_form(FL_NO_BOX, 320, 180);
  obj = fl_add_box(FL_UP_BOX,0,0,320,180,"");
  fdui->convolution = obj = fl_add_text(FL_NORMAL_TEXT,110,20,100,40,"Convolution");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->type = obj = fl_add_choice(FL_NORMAL_CHOICE,60,70,180,30,"Type");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,40,120,60,30,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,210,120,60,30,"Cancel");
  fl_end_form();

  return fdui;
}
/*---------------------------------------*/

