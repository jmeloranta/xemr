/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "peakout.h"

FD_peakout *create_form_peakout(void)
{
  FL_OBJECT *obj;
  FD_peakout *fdui = (FD_peakout *) fl_calloc(1, sizeof(*fdui));

  fdui->peakout = fl_bgn_form(FL_NO_BOX, 320, 510);
  obj = fl_add_box(FL_UP_BOX,0,0,320,510,"");
  obj = fl_add_text(FL_NORMAL_TEXT,20,20,200,30,"Results from the peak analysis");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->results = obj = fl_add_browser(FL_NORMAL_BROWSER,20,70,280,380,"");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,110,470,90,30,"OK");
  fl_end_form();

  return fdui;
}
/*---------------------------------------*/

