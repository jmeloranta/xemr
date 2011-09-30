/* Form definition file generated by fdesign */

#include <stdlib.h>
#include "gui.h"


/***************************************
 ***************************************/

FD_gui *
create_form_gui( void )
{
    FL_OBJECT *obj;
    FD_gui *fdui = fl_malloc( sizeof *fdui );

    fdui->vdata = fdui->cdata = NULL;
    fdui->ldata = 0;

    fdui->gui = fl_bgn_form( FL_NO_BOX, 900, 490 );

    obj = fl_add_box( FL_UP_BOX, 0, 0, 900, 490, "" );

    fdui->file = obj = fl_add_menu( FL_PULLDOWN_MENU, 75, 20, 40, 30, "File" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE + FL_EMBOSSED_STYLE );
    fl_set_object_callback( obj, callback_file, 0 );

    fdui->edit = obj = fl_add_menu( FL_PULLDOWN_MENU, 135, 20, 40, 30, "Edit" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE + FL_EMBOSSED_STYLE );
    fl_set_object_callback( obj, callback_edit, 0 );

    fdui->spectrum = obj = fl_add_xyplot( FL_NORMAL_XYPLOT, 20, 70, 830, 350, "Page = 0" );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );

    fdui->view = obj = fl_add_menu( FL_PULLDOWN_MENU, 195, 20, 50, 30, "View" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE + FL_EMBOSSED_STYLE );
    fl_set_object_callback( obj, callback_view, 0 );

    fdui->calc = obj = fl_add_menu( FL_PULLDOWN_MENU, 265, 20, 50, 30, "Calc" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE + FL_EMBOSSED_STYLE );
    fl_set_object_callback( obj, callback_calc, 0 );

    fdui->baseline = obj = fl_add_menu( FL_PULLDOWN_MENU, 345, 20, 80, 30, "Baseline" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE + FL_EMBOSSED_STYLE );
    fl_set_object_callback( obj, callback_baseline, 0 );

    fdui->measure = obj = fl_add_menu( FL_PULLDOWN_MENU, 445, 20, 80, 30, "Measure" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE + FL_EMBOSSED_STYLE );
    fl_set_object_callback( obj, callback_measure, 0 );

    fdui->peak = obj = fl_add_menu( FL_PULLDOWN_MENU, 535, 20, 80, 30, "Peak" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE + FL_EMBOSSED_STYLE );
    fl_set_object_callback( obj, callback_peak, 0 );

    fdui->simulate = obj = fl_add_menu( FL_PULLDOWN_MENU, 625, 20, 80, 30, "Simulate" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE + FL_EMBOSSED_STYLE );
    fl_set_object_callback( obj, callback_simulate, 0 );

    fdui->speed = obj = fl_add_text( FL_NORMAL_TEXT, 770, 420, 70, 20, "Speed = 0" );

    fdui->endor_field = obj = fl_add_text( FL_NORMAL_TEXT, 60, 420, 150, 20, "ENDOR field = 0" );

    fdui->triple_freq = obj = fl_add_text( FL_NORMAL_TEXT, 60, 450, 230, 20, "TRIPLE freq = 0" );

    fdui->scans = obj = fl_add_text( FL_NORMAL_TEXT, 405, 450, 130, 20, "Scans 0 / 0" );

    fdui->mprog = obj = fl_add_text( FL_NORMAL_TEXT, 670, 450, 180, 20, "Measurement in progress...." );
    fl_set_object_lsize( obj, FL_NORMAL_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE );

    fdui->graph = obj = fl_add_menu( FL_PULLDOWN_MENU, 725, 20, 80, 30, "Graph" );
    fl_set_object_lsize( obj, FL_MEDIUM_SIZE );
    fl_set_object_lstyle( obj, FL_BOLD_STYLE + FL_EMBOSSED_STYLE );
    fl_set_object_callback( obj, callback_graph, 0 );

    fl_end_form( );

    fdui->gui->fdui = fdui;

    return fdui;
}