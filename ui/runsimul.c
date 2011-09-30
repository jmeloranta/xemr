/* Form definition file generated with fdesign. */

#include "forms.h"
#include <stdlib.h>
#include "runsimul.h"

FD_runsimul *create_form_runsimul(void)
{
  FL_OBJECT *obj;
  FD_runsimul *fdui = (FD_runsimul *) fl_calloc(1, sizeof(*fdui));

  fdui->runsimul = fl_bgn_form(FL_NO_BOX, 260, 450);
  obj = fl_add_box(FL_UP_BOX,0,0,260,450,"");
  obj = fl_add_text(FL_NORMAL_TEXT,40,20,210,50,"Run: Simulation");
    fl_set_object_lsize(obj,FL_LARGE_SIZE);
    fl_set_object_lalign(obj,FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
  fdui->width = obj = fl_add_input(FL_NORMAL_INPUT,120,80,100,40,"Spectrum width");
  fdui->resolution = obj = fl_add_input(FL_NORMAL_INPUT,120,200,100,40,"Spectrum resolution");
  fdui->ok = obj = fl_add_button(FL_NORMAL_BUTTON,20,380,90,40,"OK");
  fdui->cancel = obj = fl_add_button(FL_NORMAL_BUTTON,150,380,90,40,"Cancel");
  fdui->frequency = obj = fl_add_input(FL_NORMAL_INPUT,120,260,100,40,"Frequency (Hz)");
  fdui->center = obj = fl_add_input(FL_NORMAL_INPUT,120,140,100,40,"Center (g) / Freq");
  fdui->endor_b = obj = fl_add_input(FL_NORMAL_INPUT,120,320,100,40,"ENDOR B (G)");
  fl_end_form();

  fdui->runsimul->fdui = fdui;

  return fdui;
}
/*---------------------------------------*/

