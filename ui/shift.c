/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "shift.h"

FD_shift *create_form_shift(void)
{
  FL_OBJECT *obj;
  FD_shift *fdui = (FD_shift *) fl_calloc(1, sizeof(*fdui));

  fdui->shift = fl_bgn_form(FL_NO_BOX, 410, 280);
  obj = fl_add_box(FL_UP_BOX,0,0,410,280,"");
  fdui->dxpos = obj = fl_add_button(FL_NORMAL_BUTTON,60,110,60,30,"X pos");
  fdui->ixpos = obj = fl_add_button(FL_NORMAL_BUTTON,320,110,60,30,"X pos");
  fdui->iint = obj = fl_add_button(FL_NORMAL_BUTTON,180,20,60,30,"Int");
  fdui->iypos = obj = fl_add_button(FL_NORMAL_BUTTON,180,70,60,30,"Y Pos");
  fdui->dypos = obj = fl_add_button(FL_NORMAL_BUTTON,180,160,60,30,"Y Pos");
  fdui->dint = obj = fl_add_button(FL_NORMAL_BUTTON,180,210,60,30,"Int");
  fdui->exit = obj = fl_add_button(FL_NORMAL_BUTTON,50,180,70,30,"Exit");
  fdui->xstep = obj = fl_add_input(FL_NORMAL_INPUT,320,160,70,30,"X step");
  fdui->ystep = obj = fl_add_input(FL_NORMAL_INPUT,320,200,70,30,"Y step");
  fdui->istep = obj = fl_add_input(FL_NORMAL_INPUT,320,240,70,30,"I step");
  fdui->gtxt = obj = fl_add_text(FL_NORMAL_TEXT,20,20,140,30,"Center g = ");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->btxt = obj = fl_add_text(FL_NORMAL_TEXT,20,60,130,30,"Center B = ");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->itxt = obj = fl_add_text(FL_NORMAL_TEXT,270,20,110,30,"Intensity = ");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->ytxt = obj = fl_add_text(FL_NORMAL_TEXT,270,70,110,30,"Y shift = ");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->reset = obj = fl_add_button(FL_NORMAL_BUTTON,190,120,40,20,"Reset");
  fl_end_form();

  fdui->shift->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

