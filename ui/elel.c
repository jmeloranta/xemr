/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "elel.h"

FD_elel *create_form_elel(void)
{
  FL_OBJECT *obj;
  FD_elel *fdui = (FD_elel *) fl_calloc(1, sizeof(*fdui));

  fdui->elel = fl_bgn_form(FL_NO_BOX, 550, 460);
  obj = fl_add_box(FL_UP_BOX,0,0,550,460,"");
  fdui->header = obj = fl_add_text(FL_NORMAL_TEXT,20,20,460,50,"Electron-electron dipolar  parameters for spectrum 1 (1,1)");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_ENGRAVED_STYLE);
  fdui->d[0] = obj = fl_add_input(FL_NORMAL_INPUT,40,160,100,30,"Dx");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d[3] = obj = fl_add_input(FL_NORMAL_INPUT,210,160,100,30,"Dx");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d[6] = obj = fl_add_input(FL_NORMAL_INPUT,370,160,100,30,"Dx");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d[1] = obj = fl_add_input(FL_NORMAL_INPUT,40,210,100,30,"Dy");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d[4] = obj = fl_add_input(FL_NORMAL_INPUT,210,210,100,30,"Dy");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d[7] = obj = fl_add_input(FL_NORMAL_INPUT,370,210,100,30,"Dy");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d[2] = obj = fl_add_input(FL_NORMAL_INPUT,40,260,100,30,"Dz");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d[5] = obj = fl_add_input(FL_NORMAL_INPUT,210,260,100,30,"Dz");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d[8] = obj = fl_add_input(FL_NORMAL_INPUT,370,260,100,30,"Dz");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,30,330,90,40,"OK");
  fdui->nextsp = obj = fl_add_button(FL_NORMAL_BUTTON,150,330,90,40,"Next SP");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,30,390,90,40,"Cancel");
  fdui->prevsp = obj = fl_add_button(FL_NORMAL_BUTTON,150,390,90,40,"Prev SP");
  fdui->ince1 = obj = fl_add_button(FL_NORMAL_BUTTON,270,330,90,40,"Inc e1");
  fdui->dece1 = obj = fl_add_button(FL_NORMAL_BUTTON,270,390,90,40,"Dec e1");
  fdui->ince2 = obj = fl_add_button(FL_NORMAL_BUTTON,390,330,90,40,"Inc e2");
  fdui->dece2 = obj = fl_add_button(FL_NORMAL_BUTTON,390,390,90,40,"Dec e2");
  fdui->quick = obj = fl_add_button(FL_NORMAL_BUTTON,490,30,50,40,"DEJ");
  fdui->d1 = obj = fl_add_input(FL_NORMAL_INPUT,40,90,100,30,"D1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d2 = obj = fl_add_input(FL_NORMAL_INPUT,210,90,100,30,"D2");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->d3 = obj = fl_add_input(FL_NORMAL_INPUT,370,90,100,30,"D3");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fl_end_form();

  fdui->elel->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

