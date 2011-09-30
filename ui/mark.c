/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "mark.h"

FD_mark *create_form_mark(void)
{
  FL_OBJECT *obj;
  FD_mark *fdui = (FD_mark *) fl_calloc(1, sizeof(*fdui));

  fdui->mark = fl_bgn_form(FL_NO_BOX, 310, 170);
  obj = fl_add_box(FL_UP_BOX,0,0,310,170,"");
  obj = fl_add_text(FL_NORMAL_TEXT,20,30,270,30,"Set background marks with the middle mouse button");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,20,60,270,30,"and press OK when done.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,120,120,70,30,"OK");
  fl_end_form();

  return fdui;
}
/*---------------------------------------*/

