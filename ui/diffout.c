/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "diffout.h"

FD_diffout *create_form_diffout(void)
{
  FL_OBJECT *obj;
  FD_diffout *fdui = (FD_diffout *) fl_calloc(1, sizeof(*fdui));

  fdui->diffout = fl_bgn_form(FL_NO_BOX, 890, 600);
  obj = fl_add_box(FL_UP_BOX,0,0,890,600,"");
  obj = fl_add_text(FL_NORMAL_TEXT,20,20,170,30,"Difference distribution");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->results = obj = fl_add_chart(FL_BAR_CHART,20,80,820,430,"Results");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,400,540,80,30,"OK");
  fl_end_form();

  fdui->diffout->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

