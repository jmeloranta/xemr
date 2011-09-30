/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "simpar.h"

FD_simparam *create_form_simparam(void)
{
  FL_OBJECT *obj;
  FD_simparam *fdui = (FD_simparam *) fl_calloc(1, sizeof(*fdui));

  fdui->simparam = fl_bgn_form(FL_NO_BOX, 720, 330);
  obj = fl_add_box(FL_UP_BOX,0,0,720,330,"");
  fdui->lineshape = obj = fl_add_choice(FL_NORMAL_CHOICE2,500,70,210,40,"Lineshape");
  obj = fl_add_text(FL_NORMAL_TEXT,10,10,160,30,"Simulation parameters");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_EMBOSSED_STYLE);
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,190,280,80,30,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,400,280,90,30,"Cancel");
  fdui->system = obj = fl_add_choice(FL_NORMAL_CHOICE2,200,70,90,40,"System");
  fdui->run = obj = fl_add_choice(FL_NORMAL_CHOICE2,50,70,90,40,"Run");
  fdui->nspec = obj = fl_add_input(FL_NORMAL_INPUT,120,130,80,30,"Number of spectra");
  fdui->target = obj = fl_add_input(FL_NORMAL_INPUT,310,130,80,30,"Target page");
  fdui->auto_amp = obj = fl_add_checkbutton(FL_PUSH_BUTTON,500,180,110,40,"Auto amplitude");
  fdui->phi = obj = fl_add_input(FL_NORMAL_INPUT,190,180,80,30,"Phi");
  fdui->theta = obj = fl_add_input(FL_NORMAL_INPUT,310,180,80,30,"Theta");
  obj = fl_add_text(FL_NORMAL_TEXT,30,180,120,30,"Sample orientation");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->hamilton = obj = fl_add_choice(FL_NORMAL_CHOICE2,350,70,90,40,"Hamilton");
  fdui->transition = obj = fl_add_choice(FL_NORMAL_CHOICE2,350,10,90,40,"Transition");
  fdui->dlevel = obj = fl_add_choice(FL_NORMAL_CHOICE2,500,20,210,40,"Dlevel");
  fdui->iunits = obj = fl_add_choice(FL_NORMAL_CHOICE2,500,130,210,40,"IHFC units");
  fdui->orbital = obj = fl_add_choice(FL_NORMAL_CHOICE2,200,10,90,40,"Orbital");
  fdui->active = obj = fl_add_checkbutton(FL_PUSH_BUTTON,30,230,110,30,"Active levels");
  fdui->begin = obj = fl_add_input(FL_NORMAL_INPUT,190,230,80,30,"Begin");
  fdui->end = obj = fl_add_input(FL_NORMAL_INPUT,310,230,80,30,"End");
  fdui->boltzmann = obj = fl_add_checkbutton(FL_PUSH_BUTTON,440,230,90,30,"Boltzmann");
  fdui->temp = obj = fl_add_input(FL_NORMAL_INPUT,570,230,80,30,"T (K)");
  fl_end_form();

  fdui->simparam->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

