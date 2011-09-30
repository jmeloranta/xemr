/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "calibsig.h"

FD_calibsig *create_form_calibsig(void)
{
  FL_OBJECT *obj;
  FD_calibsig *fdui = (FD_calibsig *) fl_calloc(1, sizeof(*fdui));

  fdui->calibsig = fl_bgn_form(FL_NO_BOX, 330, 180);
  obj = fl_add_box(FL_UP_BOX,0,0,330,180,"");
  fdui->intensity = obj = fl_add_text(FL_NORMAL_TEXT,60,30,220,40,"Signal intensity = ");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->exit = obj = fl_add_button(FL_NORMAL_BUTTON,130,110,90,30,"Exit");
  fdui->timer = obj = fl_add_timer(FL_HIDDEN_TIMER,30,90,60,40,"Timer");
  fl_end_form();

  return fdui;
}
/*---------------------------------------*/

