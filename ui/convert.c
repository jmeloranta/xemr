/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "convert.h"

FD_convert *create_form_convert(void)
{
  FL_OBJECT *obj;
  FD_convert *fdui = (FD_convert *) fl_calloc(1, sizeof(*fdui));

  fdui->convert = fl_bgn_form(FL_NO_BOX, 210, 200);
  obj = fl_add_box(FL_UP_BOX,0,0,210,200,"");
  fdui->format = obj = fl_add_choice(FL_NORMAL_CHOICE2,50,30,120,40,"Format");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,60,110,100,40,"OK");
  fl_end_form();

  fdui->convert->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

