/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "cmhz.h"

FD_cmhz *create_form_cmhz(void)
{
  FL_OBJECT *obj;
  FD_cmhz *fdui = (FD_cmhz *) fl_calloc(1, sizeof(*fdui));

  fdui->cmhz = fl_bgn_form(FL_NO_BOX, 290, 90);
  obj = fl_add_box(FL_UP_BOX,0,0,290,90,"");
  fdui->ghz = obj = fl_add_button(FL_NORMAL_BUTTON,20,50,80,30,"GHz");
  fdui->cm = obj = fl_add_button(FL_NORMAL_BUTTON,180,50,80,30,"cm-1");
  fdui->units = obj = fl_add_text(FL_NORMAL_TEXT,70,10,130,30,"Units");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fl_end_form();

  fdui->cmhz->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

