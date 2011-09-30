/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "alinewidth.h"

FD_alinewidth *create_form_alinewidth(void)
{
  FL_OBJECT *obj;
  FD_alinewidth *fdui = (FD_alinewidth *) fl_calloc(1, sizeof(*fdui));

  fdui->alinewidth = fl_bgn_form(FL_NO_BOX, 440, 350);
  obj = fl_add_box(FL_UP_BOX,0,0,440,350,"");
  fdui->header = obj = fl_add_text(FL_NORMAL_TEXT,20,20,410,50,"Linewidth  parameters for spectrum 1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_ENGRAVED_STYLE);
  fdui->lwx = obj = fl_add_input(FL_NORMAL_INPUT,90,90,100,30,"EPR Lw(x)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,100,240,90,40,"OK");
  fdui->nextsp = obj = fl_add_button(FL_NORMAL_BUTTON,230,240,90,40,"Next SP");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,100,300,90,40,"Cancel");
  fdui->prevsp = obj = fl_add_button(FL_NORMAL_BUTTON,230,300,90,40,"Prev SP");
  fdui->lg_fact = obj = fl_add_input(FL_NORMAL_INPUT,330,90,100,30,"Lorentz/Gauss");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->elw = obj = fl_add_input(FL_NORMAL_INPUT,330,140,100,30,"ENDOR Lw");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->lwy = obj = fl_add_input(FL_NORMAL_INPUT,90,140,100,30,"EPR Lw(y)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->lwz = obj = fl_add_input(FL_NORMAL_INPUT,90,190,100,30,"EPR Lw(z)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fl_end_form();

  fdui->alinewidth->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

