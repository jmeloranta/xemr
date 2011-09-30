/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "elw.h"

FD_elw *create_form_elw(void)
{
  FL_OBJECT *obj;
  FD_elw *fdui = (FD_elw *) fl_calloc(1, sizeof(*fdui));

  fdui->elw = fl_bgn_form(FL_NO_BOX, 680, 570);
  obj = fl_add_box(FL_UP_BOX,0,0,680,570,"");
  obj = fl_add_text(FL_NORMAL_TEXT,130,10,170,50,"ENDOR lw/amp editor");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->amp[0] = obj = fl_add_input(FL_NORMAL_INPUT,80,90,90,40,"Amp (HF)");
  fdui->lw[0] = obj = fl_add_input(FL_NORMAL_INPUT,220,90,90,40,"LW (HF)");
  fdui->amp[1] = obj = fl_add_input(FL_NORMAL_INPUT,400,90,90,40,"Amp (LF)");
  fdui->lw[1] = obj = fl_add_input(FL_NORMAL_INPUT,540,90,90,40,"LW (LF)");
  obj = fl_add_text(FL_NORMAL_TEXT,10,95,20,30,"1.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->amp[2] = obj = fl_add_input(FL_NORMAL_INPUT,80,150,90,40,"Amp (HF)");
  fdui->lw[2] = obj = fl_add_input(FL_NORMAL_INPUT,220,150,90,40,"LW (HF)");
  fdui->amp[3] = obj = fl_add_input(FL_NORMAL_INPUT,400,150,90,40,"Amp (LF)");
  fdui->lw[3] = obj = fl_add_input(FL_NORMAL_INPUT,540,150,90,40,"LW (LF)");
  obj = fl_add_text(FL_NORMAL_TEXT,10,155,20,30,"2.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->amp[4] = obj = fl_add_input(FL_NORMAL_INPUT,80,210,90,40,"Amp (HF)");
  fdui->lw[4] = obj = fl_add_input(FL_NORMAL_INPUT,220,210,90,40,"LW (HF)");
  fdui->amp[5] = obj = fl_add_input(FL_NORMAL_INPUT,400,210,90,40,"Amp (LF)");
  fdui->lw[5] = obj = fl_add_input(FL_NORMAL_INPUT,540,210,90,40,"LW (LF)");
  obj = fl_add_text(FL_NORMAL_TEXT,10,215,20,30,"3.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->amp[6] = obj = fl_add_input(FL_NORMAL_INPUT,80,270,90,40,"Amp (HF)");
  fdui->lw[6] = obj = fl_add_input(FL_NORMAL_INPUT,220,270,90,40,"LW (HF)");
  fdui->amp[7] = obj = fl_add_input(FL_NORMAL_INPUT,400,270,90,40,"Amp (LF)");
  fdui->lw[7] = obj = fl_add_input(FL_NORMAL_INPUT,540,270,90,40,"LW (LF)");
  obj = fl_add_text(FL_NORMAL_TEXT,10,275,20,30,"4.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->amp[8] = obj = fl_add_input(FL_NORMAL_INPUT,80,330,90,40,"Amp (HF)");
  fdui->lw[8] = obj = fl_add_input(FL_NORMAL_INPUT,220,330,90,40,"LW (HF)");
  fdui->amp[9] = obj = fl_add_input(FL_NORMAL_INPUT,400,330,90,40,"Amp (LF)");
  fdui->lw[9] = obj = fl_add_input(FL_NORMAL_INPUT,540,330,90,40,"LW (LF)");
  obj = fl_add_text(FL_NORMAL_TEXT,10,335,20,30,"5.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->amp[10] = obj = fl_add_input(FL_NORMAL_INPUT,80,390,90,40,"Amp (HF)");
  fdui->lw[10] = obj = fl_add_input(FL_NORMAL_INPUT,220,390,90,40,"LW (HF)");
  fdui->amp[11] = obj = fl_add_input(FL_NORMAL_INPUT,400,390,90,40,"Amp (LF)");
  fdui->lw[11] = obj = fl_add_input(FL_NORMAL_INPUT,540,390,90,40,"LW (LF)");
  obj = fl_add_text(FL_NORMAL_TEXT,10,395,20,30,"6.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->amp[12] = obj = fl_add_input(FL_NORMAL_INPUT,80,450,90,40,"Amp (HF)");
  fdui->lw[12] = obj = fl_add_input(FL_NORMAL_INPUT,220,450,90,40,"LW (HF)");
  fdui->amp[13] = obj = fl_add_input(FL_NORMAL_INPUT,400,450,90,40,"Amp (LF)");
  fdui->lw[13] = obj = fl_add_input(FL_NORMAL_INPUT,540,450,90,40,"LW (LF)");
  obj = fl_add_text(FL_NORMAL_TEXT,10,455,20,30,"7.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,170,510,90,40,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,340,510,90,40,"Cancel");
  fdui->offset = obj = fl_add_input(FL_NORMAL_INPUT,540,20,90,40,"ENDOR field offset");
  fl_end_form();

  fdui->elw->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

