/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "variables.h"

FD_variables *create_form_variables(void)
{
  FL_OBJECT *obj;
  FD_variables *fdui = (FD_variables *) fl_calloc(1, sizeof(*fdui));

  fdui->variables = fl_bgn_form(FL_NO_BOX, 1070, 610);
  obj = fl_add_box(FL_UP_BOX,0,0,1070,610,"");
  fdui->lab[0] = obj = fl_add_text(FL_NORMAL_TEXT,10,70,30,30,"1.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[0] = obj = fl_add_input(FL_NORMAL_INPUT,150,70,50,30,"Set");
  fdui->arg1[0] = obj = fl_add_input(FL_NORMAL_INPUT,240,70,50,30,"Arg1");
  fdui->arg2[0] = obj = fl_add_input(FL_NORMAL_INPUT,330,70,50,30,"Arg2");
  fdui->mc_limit[0] = obj = fl_add_input(FL_NORMAL_INPUT,600,70,60,30,"MC");
  fdui->high[0] = obj = fl_add_input(FL_NORMAL_INPUT,690,70,70,30,"High");
  fdui->low[0] = obj = fl_add_input(FL_NORMAL_INPUT,790,70,70,30,"Low");
  fdui->lab[1] = obj = fl_add_text(FL_NORMAL_TEXT,10,120,30,30,"2.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[1] = obj = fl_add_input(FL_NORMAL_INPUT,150,120,50,30,"Set");
  fdui->arg1[1] = obj = fl_add_input(FL_NORMAL_INPUT,240,120,50,30,"Arg1");
  fdui->arg2[1] = obj = fl_add_input(FL_NORMAL_INPUT,330,120,50,30,"Arg2");
  fdui->mc_limit[1] = obj = fl_add_input(FL_NORMAL_INPUT,600,120,60,30,"MC");
  fdui->high[1] = obj = fl_add_input(FL_NORMAL_INPUT,690,120,70,30,"High");
  fdui->low[1] = obj = fl_add_input(FL_NORMAL_INPUT,790,120,70,30,"Low");
  fdui->lab[2] = obj = fl_add_text(FL_NORMAL_TEXT,10,170,30,30,"3.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[2] = obj = fl_add_input(FL_NORMAL_INPUT,150,170,50,30,"Set");
  fdui->arg1[2] = obj = fl_add_input(FL_NORMAL_INPUT,240,170,50,30,"Arg1");
  fdui->arg2[2] = obj = fl_add_input(FL_NORMAL_INPUT,330,170,50,30,"Arg2");
  fdui->mc_limit[2] = obj = fl_add_input(FL_NORMAL_INPUT,600,170,60,30,"MC");
  fdui->high[2] = obj = fl_add_input(FL_NORMAL_INPUT,690,170,70,30,"High");
  fdui->low[2] = obj = fl_add_input(FL_NORMAL_INPUT,790,170,70,30,"Low");
  fdui->lab[3] = obj = fl_add_text(FL_NORMAL_TEXT,10,220,30,30,"4.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[3] = obj = fl_add_input(FL_NORMAL_INPUT,150,220,50,30,"Set");
  fdui->arg1[3] = obj = fl_add_input(FL_NORMAL_INPUT,240,220,50,30,"Arg1");
  fdui->arg2[3] = obj = fl_add_input(FL_NORMAL_INPUT,330,220,50,30,"Arg2");
  fdui->mc_limit[3] = obj = fl_add_input(FL_NORMAL_INPUT,600,220,60,30,"MC");
  fdui->high[3] = obj = fl_add_input(FL_NORMAL_INPUT,690,220,70,30,"High");
  fdui->low[3] = obj = fl_add_input(FL_NORMAL_INPUT,790,220,70,30,"Low");
  fdui->lab[4] = obj = fl_add_text(FL_NORMAL_TEXT,10,270,30,30,"5.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[4] = obj = fl_add_input(FL_NORMAL_INPUT,150,270,50,30,"Set");
  fdui->arg1[4] = obj = fl_add_input(FL_NORMAL_INPUT,240,270,50,30,"Arg1");
  fdui->arg2[4] = obj = fl_add_input(FL_NORMAL_INPUT,330,270,50,30,"Arg2");
  fdui->mc_limit[4] = obj = fl_add_input(FL_NORMAL_INPUT,600,270,60,30,"MC");
  fdui->high[4] = obj = fl_add_input(FL_NORMAL_INPUT,690,270,70,30,"High");
  fdui->low[4] = obj = fl_add_input(FL_NORMAL_INPUT,790,270,70,30,"Low");
  fdui->lab[5] = obj = fl_add_text(FL_NORMAL_TEXT,10,320,30,30,"6.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[5] = obj = fl_add_input(FL_NORMAL_INPUT,150,320,50,30,"Set");
  fdui->arg1[5] = obj = fl_add_input(FL_NORMAL_INPUT,240,320,50,30,"Arg1");
  fdui->arg2[5] = obj = fl_add_input(FL_NORMAL_INPUT,330,320,50,30,"Arg2");
  fdui->mc_limit[5] = obj = fl_add_input(FL_NORMAL_INPUT,600,320,60,30,"MC");
  fdui->high[5] = obj = fl_add_input(FL_NORMAL_INPUT,690,320,70,30,"High");
  fdui->low[5] = obj = fl_add_input(FL_NORMAL_INPUT,790,320,70,30,"Low");
  fdui->lab[6] = obj = fl_add_text(FL_NORMAL_TEXT,10,370,30,30,"7.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[6] = obj = fl_add_input(FL_NORMAL_INPUT,150,370,50,30,"Set");
  fdui->arg1[6] = obj = fl_add_input(FL_NORMAL_INPUT,240,370,50,30,"Arg1");
  fdui->arg2[6] = obj = fl_add_input(FL_NORMAL_INPUT,330,370,50,30,"Arg2");
  fdui->mc_limit[6] = obj = fl_add_input(FL_NORMAL_INPUT,600,370,60,30,"MC");
  fdui->high[6] = obj = fl_add_input(FL_NORMAL_INPUT,690,370,70,30,"High");
  fdui->low[6] = obj = fl_add_input(FL_NORMAL_INPUT,790,370,70,30,"Low");
  fdui->lab[7] = obj = fl_add_text(FL_NORMAL_TEXT,10,420,30,30,"8.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[7] = obj = fl_add_input(FL_NORMAL_INPUT,150,420,50,30,"Set");
  fdui->arg1[7] = obj = fl_add_input(FL_NORMAL_INPUT,240,420,50,30,"Arg1");
  fdui->arg2[7] = obj = fl_add_input(FL_NORMAL_INPUT,330,420,50,30,"Arg2");
  fdui->mc_limit[7] = obj = fl_add_input(FL_NORMAL_INPUT,600,420,60,30,"MC");
  fdui->high[7] = obj = fl_add_input(FL_NORMAL_INPUT,690,420,70,30,"High");
  fdui->low[7] = obj = fl_add_input(FL_NORMAL_INPUT,790,420,70,30,"Low");
  fdui->lab[8] = obj = fl_add_text(FL_NORMAL_TEXT,10,470,30,30,"9.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[8] = obj = fl_add_input(FL_NORMAL_INPUT,150,470,50,30,"Set");
  fdui->arg1[8] = obj = fl_add_input(FL_NORMAL_INPUT,240,470,50,30,"Arg1");
  fdui->arg2[8] = obj = fl_add_input(FL_NORMAL_INPUT,330,470,50,30,"Arg2");
  fdui->mc_limit[8] = obj = fl_add_input(FL_NORMAL_INPUT,600,470,60,30,"MC");
  fdui->high[8] = obj = fl_add_input(FL_NORMAL_INPUT,690,470,70,30,"High");
  fdui->low[8] = obj = fl_add_input(FL_NORMAL_INPUT,790,470,70,30,"Low");
  fdui->lab[9] = obj = fl_add_text(FL_NORMAL_TEXT,10,520,30,30,"10.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->set[9] = obj = fl_add_input(FL_NORMAL_INPUT,150,520,50,30,"Set");
  fdui->arg1[9] = obj = fl_add_input(FL_NORMAL_INPUT,240,520,50,30,"Arg1");
  fdui->arg2[9] = obj = fl_add_input(FL_NORMAL_INPUT,330,520,50,30,"Arg2");
  fdui->mc_limit[9] = obj = fl_add_input(FL_NORMAL_INPUT,600,520,60,30,"MC");
  fdui->high[9] = obj = fl_add_input(FL_NORMAL_INPUT,690,520,70,30,"High");
  fdui->low[9] = obj = fl_add_input(FL_NORMAL_INPUT,790,520,70,30,"Low");
  obj = fl_add_text(FL_NORMAL_TEXT,300,10,160,40,"Define variables");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,40,570,80,30,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,170,570,80,30,"Cancel");
  fdui->dflan = obj = fl_add_button(FL_NORMAL_BUTTON,300,570,80,30,"DFL/AL");
  fdui->next = obj = fl_add_button(FL_NORMAL_BUTTON,780,570,80,30,"Next");
  fdui->prev = obj = fl_add_button(FL_NORMAL_BUTTON,900,570,80,30,"Prev");
  fdui->clear = obj = fl_add_button(FL_NORMAL_BUTTON,660,570,80,30,"Clear");
  fdui->type[0] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,70,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->type[1] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,120,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->type[2] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,170,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->type[3] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,220,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->type[4] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,270,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->type[5] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,320,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->type[6] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,370,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->type[7] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,420,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->type[8] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,470,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->type[9] = obj = fl_add_choice(FL_NORMAL_CHOICE,60,520,60,30,"Type");
    fl_set_object_boxtype(obj,FL_UP_BOX);
  fdui->arg3[0] = obj = fl_add_input(FL_NORMAL_INPUT,420,70,50,30,"Arg3");
  fdui->arg3[1] = obj = fl_add_input(FL_NORMAL_INPUT,420,120,50,30,"Arg3");
  fdui->arg3[2] = obj = fl_add_input(FL_NORMAL_INPUT,420,170,50,30,"Arg3");
  fdui->arg3[3] = obj = fl_add_input(FL_NORMAL_INPUT,420,220,50,30,"Arg3");
  fdui->arg3[4] = obj = fl_add_input(FL_NORMAL_INPUT,420,270,50,30,"Arg3");
  fdui->arg3[5] = obj = fl_add_input(FL_NORMAL_INPUT,420,320,50,30,"Arg3");
  fdui->arg3[6] = obj = fl_add_input(FL_NORMAL_INPUT,420,370,50,30,"Arg3");
  fdui->arg3[7] = obj = fl_add_input(FL_NORMAL_INPUT,420,420,50,30,"Arg3");
  fdui->arg3[8] = obj = fl_add_input(FL_NORMAL_INPUT,420,470,50,30,"Arg3");
  fdui->arg3[9] = obj = fl_add_input(FL_NORMAL_INPUT,420,520,50,30,"Arg3");
  fdui->arg4[0] = obj = fl_add_input(FL_NORMAL_INPUT,510,70,50,30,"Arg4");
  fdui->arg4[1] = obj = fl_add_input(FL_NORMAL_INPUT,510,120,50,30,"Arg4");
  fdui->arg4[2] = obj = fl_add_input(FL_NORMAL_INPUT,510,170,50,30,"Arg4");
  fdui->arg4[3] = obj = fl_add_input(FL_NORMAL_INPUT,510,220,50,30,"Arg4");
  fdui->arg4[4] = obj = fl_add_input(FL_NORMAL_INPUT,510,270,50,30,"Arg4");
  fdui->arg4[5] = obj = fl_add_input(FL_NORMAL_INPUT,510,320,50,30,"Arg4");
  fdui->arg4[6] = obj = fl_add_input(FL_NORMAL_INPUT,510,370,50,30,"Arg4");
  fdui->arg4[7] = obj = fl_add_input(FL_NORMAL_INPUT,510,420,50,30,"Arg4");
  fdui->arg4[8] = obj = fl_add_input(FL_NORMAL_INPUT,510,470,50,30,"Arg4");
  fdui->arg4[9] = obj = fl_add_input(FL_NORMAL_INPUT,510,520,50,30,"Arg4");
  fdui->dflis = obj = fl_add_button(FL_NORMAL_BUTTON,420,570,80,30,"DFL/IS");
  fdui->dflendor = obj = fl_add_button(FL_NORMAL_BUTTON,540,570,80,30,"DFL/ENDOR");
  fdui->active[0] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,70,70,30,"Active");
  fdui->active[1] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,120,70,30,"Active");
  fdui->active[2] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,170,70,30,"Active");
  fdui->active[3] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,220,70,30,"Active");
  fdui->active[4] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,270,70,30,"Active");
  fdui->active[5] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,320,70,30,"Active");
  fdui->active[6] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,370,70,30,"Active");
  fdui->active[7] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,420,70,30,"Active");
  fdui->active[8] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,470,70,30,"Active");
  fdui->active[9] = obj = fl_add_checkbutton(FL_PUSH_BUTTON,860,520,70,30,"Active");
  fdui->value[0] = obj = fl_add_text(FL_NORMAL_TEXT,940,70,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value[1] = obj = fl_add_text(FL_NORMAL_TEXT,940,120,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value[2] = obj = fl_add_text(FL_NORMAL_TEXT,940,170,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value[3] = obj = fl_add_text(FL_NORMAL_TEXT,940,220,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value[4] = obj = fl_add_text(FL_NORMAL_TEXT,940,270,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value[5] = obj = fl_add_text(FL_NORMAL_TEXT,940,320,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value[6] = obj = fl_add_text(FL_NORMAL_TEXT,940,370,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value[7] = obj = fl_add_text(FL_NORMAL_TEXT,940,420,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value[8] = obj = fl_add_text(FL_NORMAL_TEXT,940,470,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->value[9] = obj = fl_add_text(FL_NORMAL_TEXT,940,520,110,30,"");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fl_end_form();

  fdui->variables->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

