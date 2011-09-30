/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "runmon.h"

FD_runmon *create_form_runmon(void)
{
  FL_OBJECT *obj;
  FD_runmon *fdui = (FD_runmon *) fl_calloc(1, sizeof(*fdui));

  fdui->runmon = fl_bgn_form(FL_NO_BOX, 350, 300);
  obj = fl_add_box(FL_UP_BOX,0,0,350,300,"");
  obj = fl_add_text(FL_NORMAL_TEXT,80,20,170,40,"Run: Monte Carlo");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->iterations = obj = fl_add_input(FL_NORMAL_INPUT,130,60,100,40,"# of MC iterations");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,40,220,90,40,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,210,220,90,40,"Cancel");
  fdui->variables = obj = fl_add_button(FL_NORMAL_BUTTON,40,140,90,40,"Variables");
  fdui->equivalences = obj = fl_add_button(FL_NORMAL_BUTTON,210,140,90,40,"Equivalences");
  fl_end_form();

  fdui->runmon->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

