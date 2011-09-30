/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "misc.h"

FD_misc *create_form_misc(void)
{
  FL_OBJECT *obj;
  FD_misc *fdui = (FD_misc *) fl_calloc(1, sizeof(*fdui));

  fdui->misc = fl_bgn_form(FL_NO_BOX, 720, 770);
  obj = fl_add_box(FL_UP_BOX,0,0,720,770,"");
  obj = fl_add_text(FL_NORMAL_TEXT,70,40,210,40,"Powder integrator parameters");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_ENGRAVED_STYLE);
  obj = fl_add_frame(FL_ROUNDED_FRAME,40,30,660,270,"");
  fdui->phibegin = obj = fl_add_choice(FL_NORMAL_CHOICE2,120,100,120,40,"Phi begin");
  fdui->phiend = obj = fl_add_choice(FL_NORMAL_CHOICE2,330,100,120,40,"Phi end");
  fdui->thetabegin = obj = fl_add_choice(FL_NORMAL_CHOICE2,120,170,120,40,"Theta begin");
  fdui->thetaend = obj = fl_add_choice(FL_NORMAL_CHOICE2,330,170,120,40,"Theta end");
  fdui->phisteps = obj = fl_add_input(FL_NORMAL_INPUT,530,100,110,40,"Steps");
  fdui->thetasteps = obj = fl_add_input(FL_NORMAL_INPUT,530,170,110,40,"Steps");
  fdui->method = obj = fl_add_choice(FL_NORMAL_CHOICE2,120,240,120,40,"Method");
  fdui->points = obj = fl_add_choice(FL_NORMAL_CHOICE2,330,240,120,40,"Points");
  obj = fl_add_frame(FL_ROUNDED_FRAME,50,490,650,180,"");
  obj = fl_add_text(FL_NORMAL_TEXT,90,500,170,40,"Misc. parameters");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_ENGRAVED_STYLE);
  fdui->diags = obj = fl_add_input(FL_NORMAL_INPUT,140,400,110,40,"# Hamilton diags");
  fdui->linecutoff = obj = fl_add_input(FL_NORMAL_INPUT,150,580,110,40,"Line cutoff");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,300,700,130,40,"OK");
  fdui->debug = obj = fl_add_input(FL_NORMAL_INPUT,350,580,110,40,"Debug level");
  fdui->interp = obj = fl_add_input(FL_NORMAL_INPUT,530,240,110,40,"El. step");
  obj = fl_add_frame(FL_ROUNDED_FRAME,40,320,660,160,"");
  obj = fl_add_text(FL_NORMAL_TEXT,80,330,200,40,"Diagonalization parameters");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_ENGRAVED_STYLE);
  fdui->follow = obj = fl_add_choice(FL_NORMAL_CHOICE2,330,400,120,40,"Level follow");
  fl_end_form();

  fdui->misc->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

