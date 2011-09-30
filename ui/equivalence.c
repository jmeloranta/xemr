/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "equivalence.h"

FD_equivalence *create_form_equivalence(void)
{
  FL_OBJECT *obj;
  FD_equivalence *fdui = (FD_equivalence *) fl_calloc(1, sizeof(*fdui));

  fdui->equivalence = fl_bgn_form(FL_NO_BOX, 1060, 430);
  obj = fl_add_box(FL_UP_BOX,0,0,1060,430,"");
  fdui->property[0] = obj = fl_add_choice(FL_NORMAL_CHOICE,110,60,50,30,"Property");
  fdui->from_set[0] = obj = fl_add_input(FL_NORMAL_INPUT,200,60,40,30,"Set #");
  fdui->from_index1[0] = obj = fl_add_input(FL_NORMAL_INPUT,280,60,40,30,"Index1");
  fdui->from_index2[0] = obj = fl_add_input(FL_NORMAL_INPUT,360,60,40,30,"Index 2");
  obj = fl_add_text(FL_NORMAL_TEXT,570,60,50,30,"--->");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->to_set[0] = obj = fl_add_input(FL_NORMAL_INPUT,660,60,40,30,"Set #");
  fdui->to_index1[0] = obj = fl_add_input(FL_NORMAL_INPUT,740,60,40,30,"Index1");
  fdui->to_index2[0] = obj = fl_add_input(FL_NORMAL_INPUT,820,60,40,30,"Index 2");
  fdui->property[1] = obj = fl_add_choice(FL_NORMAL_CHOICE,110,100,50,30,"Property");
  fdui->from_set[1] = obj = fl_add_input(FL_NORMAL_INPUT,200,100,40,30,"Set #");
  fdui->from_index1[1] = obj = fl_add_input(FL_NORMAL_INPUT,280,100,40,30,"Index1");
  fdui->from_index2[1] = obj = fl_add_input(FL_NORMAL_INPUT,360,100,40,30,"Index 2");
  obj = fl_add_text(FL_NORMAL_TEXT,570,100,50,30,"--->");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->to_set[1] = obj = fl_add_input(FL_NORMAL_INPUT,660,100,40,30,"Set #");
  fdui->to_index1[1] = obj = fl_add_input(FL_NORMAL_INPUT,740,100,40,30,"Index1");
  fdui->to_index2[1] = obj = fl_add_input(FL_NORMAL_INPUT,820,100,40,30,"Index 2");
  fdui->property[2] = obj = fl_add_choice(FL_NORMAL_CHOICE,110,140,50,30,"Property");
  fdui->from_set[2] = obj = fl_add_input(FL_NORMAL_INPUT,200,140,40,30,"Set #");
  fdui->from_index1[2] = obj = fl_add_input(FL_NORMAL_INPUT,280,140,40,30,"Index1");
  fdui->from_index2[2] = obj = fl_add_input(FL_NORMAL_INPUT,360,140,40,30,"Index 2");
  obj = fl_add_text(FL_NORMAL_TEXT,570,140,50,30,"--->");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->to_set[2] = obj = fl_add_input(FL_NORMAL_INPUT,660,140,40,30,"Set #");
  fdui->to_index1[2] = obj = fl_add_input(FL_NORMAL_INPUT,740,140,40,30,"Index1");
  fdui->to_index2[2] = obj = fl_add_input(FL_NORMAL_INPUT,820,140,40,30,"Index 2");
  fdui->property[3] = obj = fl_add_choice(FL_NORMAL_CHOICE,110,180,50,30,"Property");
  fdui->from_set[3] = obj = fl_add_input(FL_NORMAL_INPUT,200,180,40,30,"Set #");
  fdui->from_index1[3] = obj = fl_add_input(FL_NORMAL_INPUT,280,180,40,30,"Index1");
  fdui->from_index2[3] = obj = fl_add_input(FL_NORMAL_INPUT,360,180,40,30,"Index 2");
  obj = fl_add_text(FL_NORMAL_TEXT,570,180,50,30,"--->");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->to_set[3] = obj = fl_add_input(FL_NORMAL_INPUT,660,180,40,30,"Set #");
  fdui->to_index1[3] = obj = fl_add_input(FL_NORMAL_INPUT,740,180,40,30,"Index1");
  fdui->to_index2[3] = obj = fl_add_input(FL_NORMAL_INPUT,820,180,40,30,"Index 2");
  fdui->property[4] = obj = fl_add_choice(FL_NORMAL_CHOICE,110,220,50,30,"Property");
  fdui->from_set[4] = obj = fl_add_input(FL_NORMAL_INPUT,200,220,40,30,"Set #");
  fdui->from_index1[4] = obj = fl_add_input(FL_NORMAL_INPUT,280,220,40,30,"Index1");
  fdui->from_index2[4] = obj = fl_add_input(FL_NORMAL_INPUT,360,220,40,30,"Index 2");
  obj = fl_add_text(FL_NORMAL_TEXT,570,220,50,30,"--->");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->to_set[4] = obj = fl_add_input(FL_NORMAL_INPUT,660,220,40,30,"Set #");
  fdui->to_index1[4] = obj = fl_add_input(FL_NORMAL_INPUT,740,220,40,30,"Index1");
  fdui->to_index2[4] = obj = fl_add_input(FL_NORMAL_INPUT,820,220,40,30,"Index 2");
  fdui->property[5] = obj = fl_add_choice(FL_NORMAL_CHOICE,110,260,50,30,"Property");
  fdui->from_set[5] = obj = fl_add_input(FL_NORMAL_INPUT,200,260,40,30,"Set #");
  fdui->from_index1[5] = obj = fl_add_input(FL_NORMAL_INPUT,280,260,40,30,"Index1");
  fdui->from_index2[5] = obj = fl_add_input(FL_NORMAL_INPUT,360,260,40,30,"Index 2");
  obj = fl_add_text(FL_NORMAL_TEXT,570,260,50,30,"--->");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->to_set[5] = obj = fl_add_input(FL_NORMAL_INPUT,660,260,40,30,"Set #");
  fdui->to_index1[5] = obj = fl_add_input(FL_NORMAL_INPUT,740,260,40,30,"Index1");
  fdui->to_index2[5] = obj = fl_add_input(FL_NORMAL_INPUT,820,260,40,30,"Index 2");
  fdui->property[6] = obj = fl_add_choice(FL_NORMAL_CHOICE,110,300,50,30,"Property");
  fdui->from_set[6] = obj = fl_add_input(FL_NORMAL_INPUT,200,300,40,30,"Set #");
  fdui->from_index1[6] = obj = fl_add_input(FL_NORMAL_INPUT,280,300,40,30,"Index1");
  fdui->from_index2[6] = obj = fl_add_input(FL_NORMAL_INPUT,360,300,40,30,"Index 2");
  obj = fl_add_text(FL_NORMAL_TEXT,570,300,50,30,"--->");
    fl_set_object_lsize(obj,FL_NORMAL_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->to_set[6] = obj = fl_add_input(FL_NORMAL_INPUT,660,300,40,30,"Set #");
  fdui->to_index1[6] = obj = fl_add_input(FL_NORMAL_INPUT,740,300,40,30,"Index1");
  fdui->to_index2[6] = obj = fl_add_input(FL_NORMAL_INPUT,820,300,40,30,"Index 2");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,180,370,80,30,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,300,370,80,30,"Cancel");
  obj = fl_add_text(FL_NORMAL_TEXT,270,10,140,20,"Equivalence editor");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->next = obj = fl_add_button(FL_NORMAL_BUTTON,670,370,80,30,"Next");
  fdui->prev = obj = fl_add_button(FL_NORMAL_BUTTON,780,370,80,30,"Prev");
  fdui->clear = obj = fl_add_button(FL_NORMAL_BUTTON,920,370,80,30,"Clear");
  fdui->lab[0] = obj = fl_add_text(FL_NORMAL_TEXT,20,60,30,30,"1.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->lab[1] = obj = fl_add_text(FL_NORMAL_TEXT,20,100,30,30,"2.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->lab[2] = obj = fl_add_text(FL_NORMAL_TEXT,20,140,30,30,"3.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->lab[3] = obj = fl_add_text(FL_NORMAL_TEXT,20,180,30,30,"4.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->lab[4] = obj = fl_add_text(FL_NORMAL_TEXT,20,220,30,30,"5.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->lab[5] = obj = fl_add_text(FL_NORMAL_TEXT,20,260,30,30,"6.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->lab[6] = obj = fl_add_text(FL_NORMAL_TEXT,20,300,30,30,"7.");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->from_index3[0] = obj = fl_add_input(FL_NORMAL_INPUT,440,60,40,30,"Index 3");
  fdui->from_index3[1] = obj = fl_add_input(FL_NORMAL_INPUT,440,100,40,30,"Index 3");
  fdui->from_index3[2] = obj = fl_add_input(FL_NORMAL_INPUT,440,140,40,30,"Index 3");
  fdui->from_index3[3] = obj = fl_add_input(FL_NORMAL_INPUT,440,180,40,30,"Index 3");
  fdui->from_index3[4] = obj = fl_add_input(FL_NORMAL_INPUT,440,220,40,30,"Index 3");
  fdui->from_index3[5] = obj = fl_add_input(FL_NORMAL_INPUT,440,260,40,30,"Index 3");
  fdui->from_index3[6] = obj = fl_add_input(FL_NORMAL_INPUT,440,300,40,30,"Index 3");
  fdui->to_index3[0] = obj = fl_add_input(FL_NORMAL_INPUT,900,60,40,30,"Index 3");
  fdui->to_index3[1] = obj = fl_add_input(FL_NORMAL_INPUT,900,100,40,30,"Index 3");
  fdui->to_index3[2] = obj = fl_add_input(FL_NORMAL_INPUT,900,140,40,30,"Index 3");
  fdui->to_index3[3] = obj = fl_add_input(FL_NORMAL_INPUT,900,180,40,30,"Index 3");
  fdui->to_index3[4] = obj = fl_add_input(FL_NORMAL_INPUT,900,220,40,30,"Index 3");
  fdui->to_index3[5] = obj = fl_add_input(FL_NORMAL_INPUT,900,260,40,30,"Index 3");
  fdui->to_index3[6] = obj = fl_add_input(FL_NORMAL_INPUT,900,300,40,30,"Index 3");
  fdui->from_index4[0] = obj = fl_add_input(FL_NORMAL_INPUT,520,60,40,30,"Index 4");
  fdui->from_index4[1] = obj = fl_add_input(FL_NORMAL_INPUT,520,100,40,30,"Index 4");
  fdui->from_index4[2] = obj = fl_add_input(FL_NORMAL_INPUT,520,140,40,30,"Index 4");
  fdui->from_index4[3] = obj = fl_add_input(FL_NORMAL_INPUT,520,180,40,30,"Index 4");
  fdui->from_index4[4] = obj = fl_add_input(FL_NORMAL_INPUT,520,220,40,30,"Index 4");
  fdui->from_index4[5] = obj = fl_add_input(FL_NORMAL_INPUT,520,260,40,30,"Index 4");
  fdui->from_index4[6] = obj = fl_add_input(FL_NORMAL_INPUT,520,300,40,30,"Index 4");
  fdui->to_index4[0] = obj = fl_add_input(FL_NORMAL_INPUT,980,60,40,30,"Index 4");
  fdui->to_index4[1] = obj = fl_add_input(FL_NORMAL_INPUT,980,100,40,30,"Index 4");
  fdui->to_index4[2] = obj = fl_add_input(FL_NORMAL_INPUT,980,140,40,30,"Index 4");
  fdui->to_index4[3] = obj = fl_add_input(FL_NORMAL_INPUT,980,180,40,30,"Index 4");
  fdui->to_index4[4] = obj = fl_add_input(FL_NORMAL_INPUT,980,220,40,30,"Index 4");
  fdui->to_index4[5] = obj = fl_add_input(FL_NORMAL_INPUT,980,260,40,30,"Index 4");
  fdui->to_index4[6] = obj = fl_add_input(FL_NORMAL_INPUT,980,300,40,30,"Index 4");
  fdui->dflis = obj = fl_add_button(FL_NORMAL_BUTTON,420,370,80,30,"DFL/IS");
  fdui->dflendor = obj = fl_add_button(FL_NORMAL_BUTTON,540,370,80,30,"DFL/ENDOR");
  fl_end_form();

  fdui->equivalence->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

