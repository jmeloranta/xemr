/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "generic.h"

FD_generic *create_form_generic(void)
{
  FL_OBJECT *obj;
  FD_generic *fdui = (FD_generic *) fl_calloc(1, sizeof(*fdui));

  fdui->generic = fl_bgn_form(FL_NO_BOX, 720, 750);
  obj = fl_add_box(FL_UP_BOX,0,0,720,750,"");
  fdui->label = obj = fl_add_text(FL_NORMAL_TEXT,20,0,240,50,"Generic parameters for spectrum 1");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    fl_set_object_lstyle(obj,FL_NORMAL_STYLE+FL_ENGRAVED_STYLE);
  fdui->espin = obj = fl_add_input(FL_NORMAL_INPUT,110,40,90,40,"Total electron spin");
  fdui->theta_offset = obj = fl_add_input(FL_NORMAL_INPUT,440,40,90,40,"Theta offset");
  fdui->phi_offset = obj = fl_add_input(FL_NORMAL_INPUT,600,40,90,40,"Phi offset");
  obj = fl_add_text(FL_NORMAL_TEXT,20,90,20,40,"1.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[0] = obj = fl_add_text(FL_NORMAL_TEXT,70,90,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[0] = obj = fl_add_input(FL_NORMAL_INPUT,440,90,90,40,"Number of nuclei");
  fdui->atom[0] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,90,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,140,20,40,"2.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[1] = obj = fl_add_text(FL_NORMAL_TEXT,70,140,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[1] = obj = fl_add_input(FL_NORMAL_INPUT,440,140,90,40,"Number of nuclei");
  fdui->atom[1] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,140,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,190,20,40,"3.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[2] = obj = fl_add_text(FL_NORMAL_TEXT,70,190,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[2] = obj = fl_add_input(FL_NORMAL_INPUT,440,190,90,40,"Number of nuclei");
  fdui->atom[2] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,190,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,240,20,40,"4.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[3] = obj = fl_add_text(FL_NORMAL_TEXT,70,240,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[3] = obj = fl_add_input(FL_NORMAL_INPUT,440,240,90,40,"Number of nuclei");
  fdui->atom[3] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,240,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,290,20,40,"5.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[4] = obj = fl_add_text(FL_NORMAL_TEXT,70,290,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[4] = obj = fl_add_input(FL_NORMAL_INPUT,440,290,90,40,"Number of nuclei");
  fdui->atom[4] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,290,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,340,20,40,"6.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[5] = obj = fl_add_text(FL_NORMAL_TEXT,70,340,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[5] = obj = fl_add_input(FL_NORMAL_INPUT,440,340,90,40,"Number of nuclei");
  fdui->atom[5] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,340,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,390,20,40,"7.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[6] = obj = fl_add_text(FL_NORMAL_TEXT,70,390,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[6] = obj = fl_add_input(FL_NORMAL_INPUT,440,390,90,40,"Number of nuclei");
  fdui->atom[6] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,390,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,440,20,40,"8.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[7] = obj = fl_add_text(FL_NORMAL_TEXT,70,440,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[7] = obj = fl_add_input(FL_NORMAL_INPUT,440,440,90,40,"Number of nuclei");
  fdui->atom[7] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,440,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,490,20,40,"9.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[8] = obj = fl_add_text(FL_NORMAL_TEXT,70,490,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[8] = obj = fl_add_input(FL_NORMAL_INPUT,440,490,90,40,"Number of nuclei");
  fdui->atom[8] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,490,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,540,20,40,"10.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[9] = obj = fl_add_text(FL_NORMAL_TEXT,70,540,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[9] = obj = fl_add_input(FL_NORMAL_INPUT,440,540,90,40,"Number of nuclei");
  fdui->atom[9] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,540,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,590,20,40,"11.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[10] = obj = fl_add_text(FL_NORMAL_TEXT,70,590,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[10] = obj = fl_add_input(FL_NORMAL_INPUT,440,590,90,40,"Number of nuclei");
  fdui->atom[10] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,590,90,40,"Atom");
  obj = fl_add_text(FL_NORMAL_TEXT,20,640,20,40,"12.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->info[11] = obj = fl_add_text(FL_NORMAL_TEXT,70,640,270,40,"Nuclear spin = 0.0, abundance = 0.0");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->nbr[11] = obj = fl_add_input(FL_NORMAL_INPUT,440,640,90,40,"Number of nuclei");
  fdui->atom[11] = obj = fl_add_choice(FL_NORMAL_CHOICE2,600,640,90,40,"Atom");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,90,690,100,40,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,230,690,100,40,"Cancel");
  fdui->nextsp = obj = fl_add_button(FL_NORMAL_BUTTON,370,690,100,40,"Next SP");
  fdui->prevsp = obj = fl_add_button(FL_NORMAL_BUTTON,510,690,100,40,"Prev SP");
  fdui->amplitude = obj = fl_add_input(FL_NORMAL_INPUT,260,40,90,40,"AMP");
  fl_end_form();

  fdui->generic->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

