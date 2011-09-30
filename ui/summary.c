/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "summary.h"

FD_summary *create_form_summary(void)
{
  FL_OBJECT *obj;
  FD_summary *fdui = (FD_summary *) fl_calloc(1, sizeof(*fdui));

  fdui->summary = fl_bgn_form(FL_NO_BOX, 470, 420);
  obj = fl_add_box(FL_UP_BOX,0,0,470,420,"");
  obj = fl_add_text(FL_NORMAL_TEXT,20,20,200,30,"Page summary");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->txt = obj = fl_add_browser(FL_NORMAL_BROWSER,20,70,420,260,"");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,190,360,90,30,"OK");
  fl_end_form();

  fdui->summary->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

