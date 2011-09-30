/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "heinzer.h"

FD_heinzer *create_form_heinzer(void)
{
  FL_OBJECT *obj;
  FD_heinzer *fdui = (FD_heinzer *) fl_calloc(1, sizeof(*fdui));

  fdui->heinzer = fl_bgn_form(FL_NO_BOX, 630, 390);
  fdui->k[15] = obj = fl_add_box(FL_UP_BOX,0,0,630,390,"");
  fdui->header = obj = fl_add_text(FL_NORMAL_TEXT,240,20,150,40,"Rate constants");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,20,330,80,30,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,260,330,80,30,"Cancel");
  fdui->k[1] = obj = fl_add_input(FL_NORMAL_INPUT,210,80,70,30,"k(1,2)");
  fdui->k[2] = obj = fl_add_input(FL_NORMAL_INPUT,330,80,70,30,"k(1,3)");
  fdui->k[3] = obj = fl_add_input(FL_NORMAL_INPUT,450,80,70,30,"k(1,4)");
  fdui->k[4] = obj = fl_add_input(FL_NORMAL_INPUT,80,140,70,30,"k(2,1)");
  fdui->k[6] = obj = fl_add_input(FL_NORMAL_INPUT,330,140,70,30,"k(2,3)");
  fdui->k[7] = obj = fl_add_input(FL_NORMAL_INPUT,450,140,70,30,"k(2,4)");
  fdui->k[8] = obj = fl_add_input(FL_NORMAL_INPUT,80,200,70,30,"k(3,1)");
  fdui->k[9] = obj = fl_add_input(FL_NORMAL_INPUT,210,200,70,30,"k(3,2)");
  fdui->k[11] = obj = fl_add_input(FL_NORMAL_INPUT,450,200,70,30,"k(3,4)");
  fdui->k[12] = obj = fl_add_input(FL_NORMAL_INPUT,80,260,70,30,"k(4,1)");
  fdui->k[13] = obj = fl_add_input(FL_NORMAL_INPUT,210,260,70,30,"k(4,2)");
  fdui->k[14] = obj = fl_add_input(FL_NORMAL_INPUT,330,260,70,30,"k(4,3)");
  fdui->zero = obj = fl_add_button(FL_NORMAL_BUTTON,480,330,80,30,"Zero");
  fdui->k[63] = obj = fl_add_input(FL_NORMAL_INPUT,930,500,70,30,"k(8,8)");
    fl_set_object_boxtype(obj,FL_NO_BOX);
    fl_set_object_color(obj,FL_MCOL,FL_MCOL);
    fl_set_object_lcolor(obj,FL_MCOL);
  fl_end_form();

  fdui->heinzer->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

