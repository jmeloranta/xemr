/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "window.h"

FD_window *create_form_window(void)
{
  FL_OBJECT *obj;
  FD_window *fdui = (FD_window *) fl_calloc(1, sizeof(*fdui));

  fdui->window = fl_bgn_form(FL_NO_BOX, 230, 280);
  obj = fl_add_box(FL_UP_BOX,0,0,230,280,"");
  obj = fl_add_text(FL_NORMAL_TEXT,20,20,250,30,"Windowing");
    fl_set_object_lsize(obj,FL_MEDIUM_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->parametera = obj = fl_add_input(FL_NORMAL_INPUT,70,80,100,30,"Parameter A");
  fdui->parameterb = obj = fl_add_input(FL_NORMAL_INPUT,70,120,100,30,"Parameter B");
  fdui->type = obj = fl_add_choice(FL_NORMAL_CHOICE2,70,180,100,30,"Type");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,20,230,80,30,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,130,230,80,30,"Cancel");
  fl_end_form();

  fdui->window->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

