/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "orbital.h"

FD_orbital *create_form_orbital(void)
{
  FL_OBJECT *obj;
  FD_orbital *fdui = (FD_orbital *) fl_calloc(1, sizeof(*fdui));

  fdui->orbital = fl_bgn_form(FL_NO_BOX, 360, 500);
  obj = fl_add_box(FL_UP_BOX,0,0,360,500,"");
  fdui->so = obj = fl_add_input(FL_NORMAL_INPUT,200,50,100,40,"Spin orbit coupling (cm-1)");
  fdui->parama1 = obj = fl_add_input(FL_NORMAL_INPUT,200,100,100,40,"Tetrahedral CF parameter - x (cm-1)");
  fdui->paramb = obj = fl_add_input(FL_NORMAL_INPUT,200,250,100,40,"Octahedral CF parameter (cm-1)");
  fdui->angmom = obj = fl_add_input(FL_NORMAL_INPUT,200,300,100,40,"Orbital ang. mom. (p = 1, ...)");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,80,420,90,40,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,220,420,90,40,"Cancel");
  fdui->next = obj = fl_add_button(FL_NORMAL_BUTTON,80,360,90,40,"Next SP");
  fdui->prev = obj = fl_add_button(FL_NORMAL_BUTTON,220,360,90,40,"Prev SP");
  fdui->number = obj = fl_add_text(FL_NORMAL_TEXT,20,10,230,30,"Current page = ");
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->parama2 = obj = fl_add_input(FL_NORMAL_INPUT,200,150,100,40,"Tetrahedral CF parameter - y (cm-1)");
  fdui->parama3 = obj = fl_add_input(FL_NORMAL_INPUT,200,200,100,40,"Tetrahedral CF parameter - z (cm-1)");
  fl_end_form();

  fdui->orbital->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

