/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "symmetry.h"

FD_symmetry *create_form_symmetry(void)
{
  FL_OBJECT *obj;
  FD_symmetry *fdui = (FD_symmetry *) fl_calloc(1, sizeof(*fdui));

  fdui->symmetry = fl_bgn_form(FL_NO_BOX, 280, 170);
  obj = fl_add_box(FL_UP_BOX,0,0,280,170,"");
  obj = fl_add_text(FL_NORMAL_TEXT,30,30,180,30,"Enter center for symmetry smooth.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  obj = fl_add_text(FL_NORMAL_TEXT,30,50,180,30,"Select using the mouse middle button.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,50,100,70,30,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,180,100,70,30,"Cancel");
  fl_end_form();

  return fdui;
}
/*---------------------------------------*/

