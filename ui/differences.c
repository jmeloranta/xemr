/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "differences.h"

FD_differences *create_form_differences(void)
{
  FL_OBJECT *obj;
  FD_differences *fdui = (FD_differences *) fl_calloc(1, sizeof(*fdui));

  fdui->differences = fl_bgn_form(FL_NO_BOX, 250, 270);
  obj = fl_add_box(FL_UP_BOX,0,0,250,270,"");
  obj = fl_add_text(FL_NORMAL_TEXT,40,10,170,30,"Calculate peak differences");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->resolution = obj = fl_add_input(FL_NORMAL_INPUT,110,50,70,30,"Difference resolution");
  fdui->maxhfc = obj = fl_add_input(FL_NORMAL_INPUT,110,90,70,30,"Maximum HFC");
  fdui->noiselimit = obj = fl_add_input(FL_NORMAL_INPUT,110,130,70,30,"Noise limit");
  fdui->method = obj = fl_add_choice(FL_NORMAL_CHOICE,110,170,70,30,"Peak finding method");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,30,220,70,30,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,150,220,70,30,"Cancel");
  fl_end_form();

  return fdui;
}
/*---------------------------------------*/

