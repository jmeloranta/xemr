/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "linewidth.h"

FD_linewidth *create_form_linewidth(void)
{
  FL_OBJECT *obj;
  FD_linewidth *fdui = (FD_linewidth *) fl_calloc(1, sizeof(*fdui));

  fdui->linewidth = fl_bgn_form(FL_NO_BOX, 820, 430);
  obj = fl_add_box(FL_UP_BOX,0,0,820,430,"");
  fdui->header = obj = fl_add_text(FL_NORMAL_TEXT,170,10,300,50,"Linewidth parameters for spectrum #0");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->a = obj = fl_add_input(FL_NORMAL_INPUT,60,190,80,40,"A");
  fdui->lab1[0] = obj = fl_add_text(FL_NORMAL_TEXT,170,75,30,30,"1.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->b[0] = obj = fl_add_input(FL_NORMAL_INPUT,210,70,80,40,"B");
  fdui->lab1[1] = obj = fl_add_text(FL_NORMAL_TEXT,170,135,30,30,"2.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->b[1] = obj = fl_add_input(FL_NORMAL_INPUT,210,130,80,40,"B");
  fdui->lab1[2] = obj = fl_add_text(FL_NORMAL_TEXT,170,195,30,30,"3.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->b[2] = obj = fl_add_input(FL_NORMAL_INPUT,210,190,80,40,"B");
  fdui->lab1[3] = obj = fl_add_text(FL_NORMAL_TEXT,170,255,30,30,"4.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->b[3] = obj = fl_add_input(FL_NORMAL_INPUT,210,250,80,40,"B");
  fdui->lab1[4] = obj = fl_add_text(FL_NORMAL_TEXT,170,315,30,30,"5.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->b[4] = obj = fl_add_input(FL_NORMAL_INPUT,210,310,80,40,"B");
  fdui->lab2[0] = obj = fl_add_text(FL_NORMAL_TEXT,320,75,30,30,"1.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->c[0] = obj = fl_add_input(FL_NORMAL_INPUT,360,70,80,40,"C");
  fdui->lab2[1] = obj = fl_add_text(FL_NORMAL_TEXT,320,135,30,30,"2.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->c[1] = obj = fl_add_input(FL_NORMAL_INPUT,360,130,80,40,"C");
  fdui->lab2[2] = obj = fl_add_text(FL_NORMAL_TEXT,320,195,30,30,"3.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->c[2] = obj = fl_add_input(FL_NORMAL_INPUT,360,190,80,40,"C");
  fdui->lab2[3] = obj = fl_add_text(FL_NORMAL_TEXT,320,255,30,30,"4.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->c[3] = obj = fl_add_input(FL_NORMAL_INPUT,360,250,80,40,"C");
  fdui->lab2[4] = obj = fl_add_text(FL_NORMAL_TEXT,320,315,30,30,"5.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->c[4] = obj = fl_add_input(FL_NORMAL_INPUT,360,310,80,40,"C");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,60,310,80,40,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,60,370,80,40,"Cancel");
  fdui->nextsp = obj = fl_add_button(FL_NORMAL_BUTTON,310,370,80,40,"Next SP");
  fdui->prevsp = obj = fl_add_button(FL_NORMAL_BUTTON,440,370,80,40,"Prev SP");
  fdui->nextpg = obj = fl_add_button(FL_NORMAL_BUTTON,570,370,80,40,"Next PG");
  fdui->prevpg = obj = fl_add_button(FL_NORMAL_BUTTON,700,370,80,40,"Prev PG");
  fdui->zero = obj = fl_add_button(FL_NORMAL_BUTTON,190,370,80,40,"Zero");
  fdui->editd = obj = fl_add_button(FL_NORMAL_BUTTON,530,180,140,60,"Edit matrix D");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
  fl_end_form();

  fdui->linewidth->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

