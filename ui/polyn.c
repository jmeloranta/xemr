/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "polyn.h"

FD_polyn *create_form_polyn(void)
{
  FL_OBJECT *obj;
  FD_polyn *fdui = (FD_polyn *) fl_calloc(1, sizeof(*fdui));

  fdui->polyn = fl_bgn_form(FL_NO_BOX, 320, 170);
  obj = fl_add_box(FL_UP_BOX,0,0,320,170,"");
  fdui->degree = obj = fl_add_input(FL_NORMAL_INPUT,140,40,130,40,"Enter degree for polynomial");
  fdui->cont = obj = fl_add_button(FL_NORMAL_BUTTON,30,100,90,30,"Continue");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,200,100,90,30,"Cancel");
  fl_end_form();

  return fdui;
}
/*---------------------------------------*/

