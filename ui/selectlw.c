/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "selectlw.h"

FD_selectlw *create_form_selectlw(void)
{
  FL_OBJECT *obj;
  FD_selectlw *fdui = (FD_selectlw *) fl_calloc(1, sizeof(*fdui));

  fdui->selectlw = fl_bgn_form(FL_NO_BOX, 320, 190);
  obj = fl_add_box(FL_UP_BOX,0,0,320,190,"");
  obj = fl_add_text(FL_NORMAL_TEXT,30,20,260,50,"Edit rate constants or linewidth?");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->rate = obj = fl_add_button(FL_NORMAL_BUTTON,20,110,90,40,"Rate");
  fdui->linewidth = obj = fl_add_button(FL_NORMAL_BUTTON,180,110,90,40,"linewidth");
  fl_end_form();

  fdui->selectlw->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

