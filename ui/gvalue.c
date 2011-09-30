/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "gvalue.h"

FD_gvalue *create_form_gvalue(void)
{
  FL_OBJECT *obj;
  FD_gvalue *fdui = (FD_gvalue *) fl_calloc(1, sizeof(*fdui));

  fdui->gvalue = fl_bgn_form(FL_NO_BOX, 520, 490);
  obj = fl_add_box(FL_UP_BOX,0,0,520,490,"");
  fdui->header = obj = fl_add_text(FL_NORMAL_TEXT,20,20,460,50,"Electron g  parameters for spectrum 1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_ENGRAVED_STYLE);
  fdui->posn[0] = obj = fl_add_input(FL_NORMAL_INPUT,60,170,100,30,"gx");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->posn[3] = obj = fl_add_input(FL_NORMAL_INPUT,230,170,100,30,"gx");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->posn[6] = obj = fl_add_input(FL_NORMAL_INPUT,390,170,100,30,"gx");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->posn[1] = obj = fl_add_input(FL_NORMAL_INPUT,60,220,100,30,"gy");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->posn[4] = obj = fl_add_input(FL_NORMAL_INPUT,230,220,100,30,"gy");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->posn[7] = obj = fl_add_input(FL_NORMAL_INPUT,390,220,100,30,"gy");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->posn[2] = obj = fl_add_input(FL_NORMAL_INPUT,60,270,100,30,"gz");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->posn[5] = obj = fl_add_input(FL_NORMAL_INPUT,230,270,100,30,"gz");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->posn[8] = obj = fl_add_input(FL_NORMAL_INPUT,390,270,100,30,"gz");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,80,350,90,40,"OK");
  fdui->nextsp = obj = fl_add_button(FL_NORMAL_BUTTON,200,350,90,40,"Next SP");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,80,410,90,40,"Cancel");
  fdui->prevsp = obj = fl_add_button(FL_NORMAL_BUTTON,200,410,90,40,"Prev SP");
  fdui->nextel = obj = fl_add_button(FL_NORMAL_BUTTON,320,350,90,40,"Next el");
  fdui->prevel = obj = fl_add_button(FL_NORMAL_BUTTON,320,410,90,40,"Prev el");
  fdui->g1 = obj = fl_add_input(FL_NORMAL_INPUT,60,90,100,30,"g1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->g2 = obj = fl_add_input(FL_NORMAL_INPUT,230,90,100,30,"g2");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fdui->g3 = obj = fl_add_input(FL_NORMAL_INPUT,390,90,100,30,"g3");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fl_end_form();

  fdui->gvalue->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

