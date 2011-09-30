/*
 * Spectrometer setup functions.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "xemr.h"
#include "../config.h"
#ifdef LIBMEAS
#include <meas/meas.h>
#endif

spec_calib spectr = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0};

void spectrometer_setup() {
  
#ifndef LIBMEAS
  fprintf(stderr, "XEMR compiled without libmeas support.\n");  
#else
  int i, j, k, l, upd = 1, mii, mai, MII, MAI, m;
  FL_OBJECT *tmp;
  char *str, buf[256];
  FILE *fp;
  double mi, ma, MI, MA, step, c1, c2, x[512];

  fl_show_form(spec_setup->spec_setup, FL_PLACE_MOUSE,FL_TRANSIENT,"Spectrometer setup");
  while(1) {
    if(upd) {
      sprintf(buf, "%d", spectr.mwcounter_id);
      fl_set_input(spec_setup->mwid, buf);
      fl_set_choice(spec_setup->mwcounter, spectr.mwcounter);

      sprintf(buf, "%d", spectr.sigchan_id);
      fl_set_input(spec_setup->sigchanid, buf);
      fl_set_choice(spec_setup->sigchan, spectr.sigchan);

      sprintf(buf, "%d", spectr.gaussmeter_id);
      fl_set_input(spec_setup->gaussmeterid, buf);
      fl_set_choice(spec_setup->gaussmeter, spectr.gaussmeter);
      
      sprintf(buf, "%d", spectr.magnet_id);
      fl_set_input(spec_setup->magnetid, buf);
      fl_set_choice(spec_setup->magnet, spectr.magnet);

      sprintf(buf, "%d", spectr.endorunit_id);
      fl_set_input(spec_setup->endorid, buf);
      fl_set_choice(spec_setup->endorunit, spectr.endorunit);

      sprintf(buf, "%d", spectr.rfcounter_id);
      fl_set_input(spec_setup->rfid, buf);
      fl_set_choice(spec_setup->rfcounter, spectr.rfcounter);

      fl_set_button(spec_setup->verbose, measure_verbose);

      fl_set_input(spec_setup->comment, spectr.comment);

      sprintf(buf, "%.2lf", spectr.field_mod_freq);
      fl_set_input(spec_setup->mod_freq, buf);	
      sprintf(buf, "%.2lf", spectr.field_mod_max);
      fl_set_input(spec_setup->mod_max, buf);	
      sprintf(buf, "%.2lf", spectr.signal_phase_max);
      fl_set_input(spec_setup->signal_phase_max, buf);	
      sprintf(buf, "%.2lf", spectr.signal_phase_min);
      fl_set_input(spec_setup->signal_phase_min, buf);      

      sprintf(buf, "%.2lf", spectr.endor_field_mod_freq);
      fl_set_input(spec_setup->endor_mod_freq, buf);	
      sprintf(buf, "%.2lf", spectr.endor_field_mod_max);
      fl_set_input(spec_setup->endor_mod_max, buf);	
      sprintf(buf, "%.2lf", spectr.endor_signal_phase_max);
      fl_set_input(spec_setup->endor_signal_phase_max, buf);	
      sprintf(buf, "%.2lf", spectr.endor_signal_phase_min);
      fl_set_input(spec_setup->endor_signal_phase_min, buf);      

      upd = 0;
    }
    tmp = fl_do_forms();
    if(tmp == spec_setup->ok) break;
    if(tmp == spec_setup->cancel) {
      fl_hide_form(spec_setup->spec_setup);
      return;
    }
    if(tmp == spec_setup->save) {
      str = (char*) fl_show_fselector("Save spectrometer setup", CALIBRATION, "*.set", "");
      if(str) {
	char buf[MAXPATHLEN];
	strcpy(buf, str);
	if(strcmp(buf+strlen(buf)-4, ".set")) strcat(buf, ".set");
	if((fp = fopen(buf, "w"))) {
	  spectr.mwcounter_id = atoi(fl_get_input(spec_setup->mwid));
	  spectr.mwcounter = fl_get_choice(spec_setup->mwcounter);
	  spectr.sigchan_id = atoi(fl_get_input(spec_setup->sigchanid));
	  spectr.sigchan = fl_get_choice(spec_setup->sigchan);
	  spectr.gaussmeter_id = atoi(fl_get_input(spec_setup->gaussmeterid));
	  spectr.gaussmeter = fl_get_choice(spec_setup->gaussmeter);
	  spectr.magnet_id = atoi(fl_get_input(spec_setup->magnetid));
	  spectr.magnet = fl_get_choice(spec_setup->magnet);
	  spectr.endorunit_id = atoi(fl_get_input(spec_setup->endorid));
	  spectr.endorunit = fl_get_choice(spec_setup->endorunit);
	  spectr.rfcounter_id = atoi(fl_get_input(spec_setup->rfid));
	  spectr.rfcounter = fl_get_choice(spec_setup->rfcounter);

	  strcpy(spectr.comment, fl_get_input(spec_setup->comment));

	  spectr.field_mod_freq = atof(fl_get_input(spec_setup->mod_freq));
	  spectr.field_mod_max = atof(fl_get_input(spec_setup->mod_max));
	  spectr.signal_phase_max = atof(fl_get_input(spec_setup->signal_phase_max));
	  spectr.signal_phase_min = atof(fl_get_input(spec_setup->signal_phase_min));

	  spectr.endor_field_mod_freq = atof(fl_get_input(spec_setup->endor_mod_freq));
	  spectr.endor_field_mod_max = atof(fl_get_input(spec_setup->endor_mod_max));
	  spectr.endor_signal_phase_max = atof(fl_get_input(spec_setup->endor_signal_phase_max));
	  spectr.endor_signal_phase_min = atof(fl_get_input(spec_setup->endor_signal_phase_min));

	  fprintf(fp, "%d %d\n", spectr.mwcounter, spectr.mwcounter_id);
	  fprintf(fp, "%d %d\n", spectr.sigchan, spectr.sigchan_id);
	  fprintf(fp, "%d %d\n", spectr.gaussmeter, spectr.gaussmeter_id);
	  fprintf(fp, "%d %d\n", spectr.magnet, spectr.magnet_id);
	  fprintf(fp, "%d %d\n", spectr.endorunit, spectr.endorunit_id);
	  fprintf(fp, "%d %d\n", spectr.rfcounter, spectr.rfcounter_id);
	  fprintf(fp, "%s\n", spectr.comment);
	  fprintf(fp, "%le %le %le %le\n", spectr.field_mod_freq,
		  spectr.field_mod_max, spectr.signal_phase_max,
		  spectr.signal_phase_min);
	  fprintf(fp, "%le %le %le %le\n", spectr.endor_field_mod_freq,
		  spectr.endor_field_mod_max, spectr.endor_signal_phase_max,
		  spectr.endor_signal_phase_min);
	  fclose(fp);
	} else fl_show_alert("Unable to save file!","","",1);
      }
    }
    if(tmp == spec_setup->load) {
      str = (char *) fl_show_fselector("Load spectrometer setup", CALIBRATION, "*.set", "");
      if(str) {
	if((fp = fopen(str, "r"))) {
	  fscanf(fp, " %d %d", &spectr.mwcounter, &spectr.mwcounter_id);
	  fscanf(fp, " %d %d", &spectr.sigchan, &spectr.sigchan_id);
	  fscanf(fp, " %d %d", &spectr.gaussmeter, &spectr.gaussmeter_id);
	  fscanf(fp, " %d %d", &spectr.magnet, &spectr.magnet_id);
	  fscanf(fp, " %d %d", &spectr.endorunit, &spectr.endorunit_id);
	  fscanf(fp, " %d %d", &spectr.rfcounter, &spectr.rfcounter_id);
	  fscanf(fp, " %[^\n]", spectr.comment);
	  fscanf(fp, " %le %le %le %le", &spectr.field_mod_freq,
		 &spectr.field_mod_max, &spectr.signal_phase_max,
		 &spectr.signal_phase_min);
	  fscanf(fp, " %le %le %le %le", &spectr.endor_field_mod_freq,
		 &spectr.endor_field_mod_max, &spectr.endor_signal_phase_max,
		 &spectr.endor_signal_phase_min);
	  fclose(fp);
	  upd = 1;
	} else fl_show_alert("Unable to open file!","","",1);
      }
    }
  }
  fl_hide_form(spec_setup->spec_setup);
  
  spectr.mwcounter_id = atoi(fl_get_input(spec_setup->mwid));
  spectr.mwcounter = fl_get_choice(spec_setup->mwcounter);
  spectr.sigchan_id = atoi(fl_get_input(spec_setup->sigchanid));
  spectr.sigchan = fl_get_choice(spec_setup->sigchan);
  spectr.gaussmeter_id = atoi(fl_get_input(spec_setup->gaussmeterid));
  spectr.gaussmeter = fl_get_choice(spec_setup->gaussmeter);
  spectr.magnet_id = atoi(fl_get_input(spec_setup->magnetid));
  spectr.magnet = fl_get_choice(spec_setup->magnet);
  spectr.endorunit_id = atoi(fl_get_input(spec_setup->endorid));
  spectr.endorunit = fl_get_choice(spec_setup->endorunit);
  spectr.rfcounter_id = atoi(fl_get_input(spec_setup->rfid));
  spectr.rfcounter = fl_get_choice(spec_setup->rfcounter);
  
  measure_verbose = fl_get_button(spec_setup->verbose);
  strcpy(spectr.comment, fl_get_input(spec_setup->comment));

  spectr.field_mod_freq = atof(fl_get_input(spec_setup->mod_freq));
  spectr.field_mod_max = atof(fl_get_input(spec_setup->mod_max));
  spectr.signal_phase_max = atof(fl_get_input(spec_setup->signal_phase_max));
  spectr.signal_phase_min = atof(fl_get_input(spec_setup->signal_phase_min));
  
  spectr.endor_field_mod_freq = atof(fl_get_input(spec_setup->endor_mod_freq));
  spectr.endor_field_mod_max = atof(fl_get_input(spec_setup->endor_mod_max));
  spectr.endor_signal_phase_max = atof(fl_get_input(spec_setup->endor_signal_phase_max));
  spectr.endor_signal_phase_min = atof(fl_get_input(spec_setup->endor_signal_phase_min));

  /* initialize instruments */
  switch(spectr.sigchan) {
  case 1:
    printf("Warning: experiments cannot be run without signal channel.\n");
    break;
  case 2: /* ER 023 */
    meas_er023_init(0, 0, spectr.sigchan_id);
    meas_er023_calibrate(0, spectr.field_mod_freq, spectr.field_mod_max, spectr.signal_phase_max);
    meas_er023_resonator(0, 1);               /* EPR modulation output */
    break;
  case 3: /* SR245 - TREPR (experiment trigger from ttl port 1) */
    meas_sr245_init(0, 0, spectr.sigchan_id, NULL); /* GPIB mode */
    meas_sr245_disable_trigger(0);
    meas_sr245_mode(0, 1); /* sync mode */
    meas_sr245_ports(0, 8); /* all 8 ports as inputs */
    meas_sr245_ttl_mode(0, 1, 0); /* both ttl ports as inputs */
    meas_sr245_ttl_mode(0, 2, 0); 
  default:
    printf("Warning: no signal channel defined!\n");
  }

  switch(spectr.mwcounter) {
  case 1:
    printf("Note: no microwave frequency counter.\n");
    break;
  case 2: /* HP 5350 */
    meas_hp5350_init(0, 0, spectr.mwcounter_id);
    break;
  default:
    printf("Warning: microwave counter not defined.\n");
    break;
  }

  switch(spectr.magnet) {
  case 1:
    printf("Warning: experiments cannot be run without magnet control.\n");
    break;
  case 2: /* ER-032 */
    meas_er032_init(0, 0, spectr.magnet_id);
    break;
  default:
    printf("Warning: magnet control not defined.\n");
    break;
  }

  switch(spectr.gaussmeter) {
  case 1:
    printf("Note: no gaussmeter.\n");
    break;
  case 2: /* Varian E-500 */
    meas_e500_init(0, 0, spectr.gaussmeter_id);
    break;
  default:
    printf("Warning: no gaussmeter defined.\n");
    break;
  }

  switch(spectr.rfcounter) {
  case 1:
    printf("Note: no RF counter.\n");
    break;
  case 2: /* HP 5384 */
    meas_hp5384_init(0, 0, spectr.rfcounter_id);
    break;
  case 3: /* HP 53131 */
    meas_hp53131_init(0, 0, spectr.rfcounter_id);
    break;
  default:
    printf("Warning: RF counter not defined.\n");
    break;
  }    
  
  switch(spectr.endorunit) {
  case 1:
    printf("Note: no ENDOR unit.\n");
    break;
  case 2: /* aspect */
    meas_sl_init();
    meas_sl_pts(15.0);
    meas_sl_wtk(15.0);
    meas_sl_attn1(63);
    meas_sl_attn2(63);
    meas_sl_attn3(63);
    meas_sl_relays(7);
    meas_sl_wtk_mod(1);
    break;
  default:
    printf("Warning: no ENDOR unit defined.\n");
    break;
  }
#endif ?* LIBMEAS */
}
