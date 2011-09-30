/*
 * Measurement routines (i.e. interface to libmeas). Initialization in setup.c
 *
 * To add new devices:
 *
 * 1) Edit this file (measure.c) and add the device functions
 * 2) Edit setup.c and add device initialization
 * 3) Edit create.c to add the device to setup menus.
 *
 * ENDOR specific:
 *
 * The modulation from port #2 for the signal channel (ER023) is used
 * for ENDOR modulation. Port #1 is used for EPR.
 *
 * TREPR specific:
 * 
 * External timing etc. required. The signal channel (SR245) will be
 * read using the synchronous mode (requires triggering of the unit).
 * The trigger signal must be provided at the digial I/O port #1 and
 * and the signal to port #1 (from boxcar).
 *
 */

#include <time.h>
#include "xemr.h"
#include "../config.h"
#ifdef LIBMEAS
#include <meas/meas.h>
#endif

int measure_verbose = 0;

void measure_epr(epr_type *sp, int (*upd)(epr_type *, int, int), double (*ply)(int, char *)) {

#ifndef LIBMEAS
  fprintf(stderr, "XEMR compiled without libmeas support.\n");
#else
  int i, j, k, stop, delay, normal_scan;
  double step, cfield, ofield, offset;
  time_t start;

  sp->spek_type = 0; /* EPR */
  sp->modfreq = spectr.field_mod_freq;
  sp->resonator = 1;
  normal_scan = sp->scans;

  if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
  switch(spectr.sigchan) {
  case 2: /* ER 023 */
    meas_er023_calibrate(0, spectr.field_mod_freq, spectr.field_mod_max, spectr.signal_phase_max);
    meas_er023_resonator(0, 1);     /* resonator 1 modulation is for EPR (2 for ENDOR) */
    meas_er023_modulation_amp(0, sp->modulampl);
    if(sp->harmonic == 0) sp->harmonic = 1;
    meas_er023_harmonic(0, sp->harmonic);
    meas_er023_gain(0, sp->gain);
    meas_er023_timeconstant(0, sp->timec);
    meas_er023_conversiontime(0, sp->ctime);
    meas_er023_phase(0, sp->sphase);    
    break;
  case 3: /* SR245 - TREPR */
    /* all settings are manual (boxcar etc.) */
    /* this setup requires scanning to be / point */
    meas_sr245_enable_trigger(0);
    normal_scan = 1;
    break;
  default:
    fl_show_message("Cannot proceed without signal channel","","");
    return;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2:
    /* make sure ENDOR is off */
    meas_sl_attn1(63);
    meas_sl_attn2(63);
    meas_sl_attn3(63);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to MW counter...");
  switch(spectr.mwcounter) {
  case 2:
    sp->frequency = meas_hp5350_read(0);
    break;
  default:
    if(sp->frequency == 0.0) sp->frequency = 9.4E9;  /* just a guess... */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(sp->xbeg != sp->xend)
    sp->xunits = EPR_UNIT_G; /* normal EPR spectrum */
  else
    sp->xunits = EPR_UNIT_S; /* kinetics at fixed mag. field */

  start = time(0);
  strcpy(sp->date, ctime(&start));
  step = fabs(sp->xend - sp->xbeg) / (double) (sp->samples - 1);

  if(spectr.magnet == 0 || spectr.magnet == 1) {
    fl_show_message("Cannot proceed without magnet control","","");
    return;
  }

  ofield = sp->xbeg;

  if(measure_verbose) fprintf(stderr, "Talking to magnet control...");
  switch(spectr.magnet) {
  case 2:
    meas_er032_write(0, ofield);
    sleep(5);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
  switch(spectr.sigchan) {
  case 2: /* er023 */
    offset = meas_er023_read(0);
    break;
  case 3: /* SR245 - TREPR */
    offset = meas_sr245_read(0, 1); /* signal at port #1 */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(sp->xunits == EPR_UNIT_G) {
    if(measure_verbose) fprintf(stderr, "Talking to Gaussmeter...");
    switch(spectr.gaussmeter) {
    case 2:
      sp->xbeg = meas_e500_read(0);
      break;
    }
    if(measure_verbose) fprintf(stderr, "...done.\n");
  } else {
    sp->xbeg = sp->xend = 0.0; /* sec */
    sp->samples = sp->scans;
    sp->scans = 1;
    delay = atof(fl_get_input(params->delay)); /* in sec */
    epr_misc_zerosp(sp, 0.0, 0.0);
  }
  
  for (i = 0; i < sp->scans; i++) {
    cfield = ofield;
    if(!i && sp->xunits == EPR_UNIT_G) { /* make sure that the magnet keeps up */
      if(measure_verbose) fprintf(stderr, "Talking to magnet control...");
      switch(spectr.magnet) {
      case 2:
	meas_er032_write(0, cfield);
	sleep(5);
	break;
      }
      if(measure_verbose) fprintf(stderr, "...done.\n");
    }
    for (j = 0; j < sp->samples; j++) {
      if(sp->xunits == EPR_UNIT_G) {
	if(measure_verbose) fprintf(stderr, "Talking to magnet control...");	
	switch(spectr.magnet) {
	case 2:
	  meas_er032_write(0, cfield);
	  break;
	}
	if(measure_verbose) fprintf(stderr, "...done.\n");
      }

      if(normal_scan != sp->scans) { /* TREPR cans */
	for(k = 0; k < sp->scans; k++) {
	  if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
	  switch(spectr.sigchan) {
	  case 2:
	    sp->y[j] += meas_er023_read(0) - offset; /* not really needed */
	    break;
	  case 3:
	    sp->y[i] += meas_sr245_read(0, 1) - offset; /* signal at #1 */
	  }
	  if(measure_verbose) fprintf(stderr, "...done.\n");
	}
      } else { /* normal scan (CW) */
	if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
	switch(spectr.sigchan) {
	case 2:
	  sp->y[j] += meas_er023_read(0) - offset;
	  break;
	case 3:
	  sp->y[i] += meas_sr245_read(0, 1) - offset;  /* not really needed */
	}
	if(measure_verbose) fprintf(stderr, "...done.\n");
      }

      if(upd != NULL) stop = (*upd)(sp, j, i);
      if(stop == 2) { stop = 1; break; }

      if(sp->xunits == EPR_UNIT_S) {
	sp->xend = time(0) - start;
	sleep(delay);
      } else cfield += step;

    }
    if(stop == 1) {
      i++;
      break;
    }
  }
  
  if(sp->xunits == EPR_UNIT_G) {
    if(measure_verbose) fprintf(stderr, "Talking to Gaussmeter...");
    switch(spectr.gaussmeter) {
    case 2:
      sp->xend = meas_e500_read(0);
      break;
    }
    if(measure_verbose) fprintf(stderr, "...done.\n");
    if(i) {
      for (j = 0; j < sp->samples; j++)
	sp->y[j] /= (double) i;
    }
  }

  switch(spectr.sigchan) {
  case 2: /* er023 */
    break;
  case 3: /* SR245 - TREPR */
    meas_sr245_disable_trigger(0);
    break;
  }
#endif /* LIBMEAS */
}

void measure_endor(epr_type *sp, int (*upd)(epr_type *, int, int), double (*ply)(int, char *)) {

#ifndef LIBMEAS
  fprintf(stderr, "XEMR compiled without libmeas support.\n");
#else
  int i, j, stop, delay;
  double step, cfield, ofield;
  time_t start;

  sp->spek_type = 1; /* ENDOR */
  sp->resonator = 2; /* ENDOR through the 2nd modulation output */

  if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
  switch(spectr.sigchan) {
  case 2:
    meas_er023_calibrate(0, spectr.endor_field_mod_freq, spectr.endor_field_mod_max, spectr.endor_signal_phase_max);
    meas_er023_resonator(0, 2);     /* modulation to the endor unit */
    meas_er023_modulation_amp(0, 25.0); /* this gives 10 V p-p for wavetek */
    if(sp->harmonic == 0) sp->harmonic = 1;
    meas_er023_harmonic(0, sp->harmonic);
    meas_er023_gain(0, sp->gain);
    meas_er023_timeconstant(0, sp->timec);
    meas_er023_conversiontime(0, sp->ctime);
    meas_er023_phase(0, sp->sphase + 180); /* fix phase */
    break;
  default:
    fl_show_message("Cannot proceed without a proper signal channel","","");
    return;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  switch(spectr.endorunit) {
  case 2: /* aspect */
    meas_sl_attn1((unsigned int) sp->endor_power); /* WTK on */
    meas_sl_attn2(63); /* PTS off */
    meas_sl_attn3(63);  /* common attn on for now */
    meas_sl_relays(7); /* ENDOR mode */
    meas_sl_wtk_mod((unsigned int) sp->endor_mod);
    break;
  default:
    fl_show_message("Cannot proceed without ENDOR unit","","");
    return;
  }

  if(measure_verbose) fprintf(stderr, "Talking to MW counter...");
  switch(spectr.mwcounter) {
  case 2:
    sp->frequency = meas_hp5350_read(0);
    break;
  default:
    if(sp->frequency == 0.0) sp->frequency = 9.4E9;  /* just a guess... */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(sp->xbeg != sp->xend)
    sp->xunits = EPR_UNIT_MHZ;
  else
    sp->xunits = EPR_UNIT_S;

  if(spectr.magnet == 0 || spectr.magnet == 1) {
    fl_show_message("Cannot proceed without magnet control","","");
    return;
  }
  
  if(measure_verbose) fprintf(stderr, "Talking to magnet...");
  switch(spectr.magnet) {
  case 2:
    meas_er032_write(0, sp->endor_b);
    sleep(5);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  
  if(measure_verbose) fprintf(stderr, "Talking to gaussmeter...");
  switch(spectr.gaussmeter) {
  case 2:
    sp->endor_b = meas_e500_read(0);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  start = time(0);
  strcpy(sp->date, ctime(&start));
  step = fabs(sp->xend - sp->xbeg) / (double) (sp->samples - 1);

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2:
    meas_sl_attn3(0);  /* common attn off for measurement */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  ofield = sp->xbeg;

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2:
    meas_sl_wtk(ofield);
    sleep(1);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(sp->xunits == EPR_UNIT_MHZ) {
    if(measure_verbose) fprintf(stderr, "Talking to RF counter...");
    switch(spectr.rfcounter) {
    case 2:
      sp->xbeg = meas_hp5384_read(0, HP5384_CHA) / 1.0E6; /* CH1 = ENDOR field */
      break;
    case 3:
      sp->xbeg = meas_hp53131_read(0,1) / 1.0E6;
      break;
    }
    if(measure_verbose) fprintf(stderr, "...done.\n");
  } else {
    sp->xbeg = sp->xend = 0.0; /* sec */
    sp->samples = sp->scans;
    sp->scans = 1;
    delay = atof(fl_get_input(params->delay)); /* in sec */
    epr_misc_zerosp(sp, 0.0, 0.0);
  }

  for (i = 0; i < sp->scans; i++) {
    cfield = ofield; /* avoid freq walk off due to xbeg update above */
    if(!i && sp->xunits == EPR_UNIT_MHZ) {
      if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
      switch(spectr.endorunit) {
      case 2:
	meas_sl_wtk(ofield);
	sleep(1);
	break;
      }
    }
    if(measure_verbose) fprintf(stderr, "...done.\n");
    for (j = 0; j < sp->samples; j++) {
      if(sp->xunits == EPR_UNIT_MHZ) {
	if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
	switch(spectr.endorunit) {
	case 2:
	  meas_sl_wtk(cfield);
	  break;
	}
	if(measure_verbose) fprintf(stderr, "...done.\n");
      }

      if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
      switch(spectr.sigchan) {
      case 2:
	sp->y[j] += meas_er023_read(0);
	break;
      }
      if(measure_verbose) fprintf(stderr, "...done.\n");

      if(upd != NULL) stop = (*upd)(sp, j, i);
      if(stop == 2) { stop = 1; break; }

      if(sp->xunits == EPR_UNIT_S) {
	sp->xend = time(0) - start;
	sleep(delay);
      } else cfield += step;
    }

    if(stop == 1) {
      i++;
      break;
    }
  }

  if(sp->xunits == EPR_UNIT_MHZ) {
    if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
    switch(spectr.endorunit) {
    case 2:
      meas_sl_wtk(sp->xend);
      break;
    }
    if(measure_verbose) fprintf(stderr, "...done.\n");
    if(measure_verbose) fprintf(stderr, "Talking to RF counter...");
    switch(spectr.rfcounter) {
    case 2:
      sp->xend = meas_hp5384_read(0, HP5384_CHA) / 1.0E6; /* get the actual freq */
      break;
    case 3:
      sp->xend = meas_hp53131_read(0,1) / 1.0E6;
      break;
    }
    if(measure_verbose) fprintf(stderr, "...done.\n");    
    if(i) {
      for (j = 0; j < sp->samples; j++)
	sp->y[j] /= (double) i;
    }
  }

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2: /* aspect */
    meas_sl_attn3(63);  /* common attn on after measurement */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
#endif /* LIBMEAS */
}

void measure_tripleg(epr_type *sp, int (*upd)(epr_type *, int, int), double (*ply)(int, char *)) {

#ifndef LIBMEAS
  fprintf(stderr, "XEMR compiled without libmeas support.\n");
#else
  int i, j, stop;
  double step, cfield, ofield;
  time_t start;

  sp->spek_type = 2; /* TRIPLE gen. */
  sp->resonator = 2;

  if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
  switch(spectr.sigchan) {
  case 2:
    meas_er023_calibrate(0, spectr.endor_field_mod_freq, spectr.endor_field_mod_max, spectr.endor_signal_phase_max);
    meas_er023_resonator(0, 2);     /* modulation to the endor unit */
    meas_er023_modulation_amp(0, 25.0); /* this gives 10 V p-p for wavetek */
    if(sp->harmonic == 0) sp->harmonic = 1;
    meas_er023_harmonic(0, sp->harmonic);
    meas_er023_gain(0, sp->gain);
    meas_er023_timeconstant(0, sp->timec);
    meas_er023_conversiontime(0, sp->ctime);
    meas_er023_phase(0, sp->sphase + 180); /* fix phase */
    break;
  default:
    fl_show_message("Cannot proceed without a proper signal channel","","");
    return;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  
  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2: /* aspect */
    meas_sl_attn1((unsigned int) sp->endor_power); /* WTK on */
    meas_sl_attn2((unsigned int) sp->endor_pump_power); /* PTS on */
    meas_sl_attn3(63);  /* common attn on for now */
    meas_sl_relays(7); /* ENDOR/TRIPLEG mode */
    meas_sl_wtk_mod((unsigned int) sp->endor_mod);
    break;
  default:
    fl_show_message("Cannot proceed without ENDOR unit","","");
    return;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to MW counter...");
  switch(spectr.mwcounter) {
  case 2:
    sp->frequency = meas_hp5350_read(0);
    break;
  default:
    sp->frequency = 9.4E9;  /* just a guess... */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  sp->xunits = EPR_UNIT_MHZ;

  if(spectr.magnet == 0 || spectr.magnet == 1) {
    fl_show_message("Cannot proceed without magnet control","","");
    return;
  }
  
  if(measure_verbose) fprintf(stderr, "Talking to magnet control...");
  switch(spectr.magnet) {
  case 2:
    meas_er032_write(0,sp->endor_b);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to Gaussmeter...");
  switch(spectr.gaussmeter) {
  case 2:
    sp->endor_b = meas_e500_read(0);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  
  start = time(0);
  strcpy(sp->date, ctime(&start));
  step = fabs(sp->xend - sp->xbeg) / (double) (sp->samples - 1);  
  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2:
    meas_sl_attn3(0);  /* common attn off for measurement */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  ofield = sp->xbeg;
  for (i = 0; i < sp->scans; i++) {
    cfield = ofield;
    for (j = 0; j < sp->samples; j++) {
      if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
      switch(spectr.endorunit) {
      case 2:
	meas_sl_wtk(cfield);
	break;
      }
      if(measure_verbose) fprintf(stderr, "...done.\n");
      if(!j) {
	sleep(1);
	if(!i) {
	  if(measure_verbose) fprintf(stderr, "Talking to RF counter...");
	  switch(spectr.rfcounter) {
	  case 2:
	    sp->xbeg = meas_hp5384_read(0, HP5384_CHA);
	    break;
	  case 3:
	    sp->xbeg = meas_hp53131_read(0,1);
	    break;
	  }
	  if(measure_verbose) fprintf(stderr, "...done.\n");
	}
      }
      if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
      switch(spectr.sigchan) {
      case 2:
	sp->y[j] += meas_er023_read(0);
	break;
      }
      if(measure_verbose) fprintf(stderr, "...done.\n");
      if(upd != NULL) stop = (*upd)(sp, j, i);
      if(stop == 2) { stop = 1; break; }
      cfield += step;
    }
    if(stop == 1) {
      i++;
      break;
    }
  }

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2:
    meas_sl_wtk(sp->xend);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  
  if(measure_verbose) fprintf(stderr, "Talking to RF counter...");
  switch(spectr.rfcounter) {
  case 2:
    sp->xend = meas_hp5384_read(0, HP5384_CHA); /* get the actual freq */
    break;
  case 3:
    sp->xend = meas_hp53131_read(0,1);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2: /* aspect */
    meas_sl_attn3(63);  /* common attn on after measurement */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(i) {
    for (j = 0; j < sp->samples; j++)
      sp->y[j] /= (double) i;
  }
#endif /* LIBMEAS */
}

void measure_triples(epr_type *sp, int (*upd)(epr_type *, int, int), double (*ply)(int, char *)) {

#ifndef LIBMEAS
  fprintf(stderr, "XEMR compiled without libmeas support.\n");
#else
  int i, j, stop;
  double step, cfield, ofield;
  time_t start;

  sp->spek_type = 3; /* TRIPLE spec. */
  sp->resonator = 2;

  if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
  switch(spectr.sigchan) {
  case 2:
    meas_er023_calibrate(0, spectr.endor_field_mod_freq, spectr.endor_field_mod_max, spectr.endor_signal_phase_max);
    meas_er023_resonator(0, 2);     /* modulation to the endor unit */
    meas_er023_modulation_amp(0, 25.0); /* this gives 10 V p-p for wavetek */
    if(sp->harmonic == 0) sp->harmonic = 1;
    meas_er023_harmonic(0, sp->harmonic);
    meas_er023_gain(0, sp->gain);
    meas_er023_timeconstant(0, sp->timec);
    meas_er023_conversiontime(0, sp->ctime);
    meas_er023_phase(0, sp->sphase + 180); /* fix phase */
    break;
  default:
    fl_show_message("Cannot proceed without a proper signal channel","","");
    return;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  
  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2: /* aspect */
    /* TODO: we may want to balance the two sources... */
    meas_sl_attn1((unsigned int) 0); /* WTK on */
    meas_sl_attn2((unsigned int) 0); /* PTS on */
    meas_sl_attn3(63);  /* common attn on for now */
    meas_sl_relays(0); /* ENDOR/TRIPLES mode */
    meas_sl_wtk_mod((unsigned int) sp->endor_mod);
    break;
  default:
    fl_show_message("Cannot proceed without ENDOR unit.","","");
    return;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to MW counter...");
  switch(spectr.mwcounter) {
  case 2:
    sp->frequency = meas_hp5350_read(0);
    break;
  default:
    if(sp->frequency = 0.0) sp->frequency = 9.4E9;  /* just a guess... */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  sp->xunits = EPR_UNIT_MHZ;

  if(spectr.magnet == 0 || spectr.magnet == 1) {
    fl_show_message("Cannot proceed without magnet control","","");
    return;
  }
  
  if(measure_verbose) fprintf(stderr, "Talking to magnet control...");
  switch(spectr.magnet) {
  case 2:
    meas_er032_write(0, sp->endor_b);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  if(measure_verbose) fprintf(stderr, "Talking to Gaussmeter...");
  switch(spectr.gaussmeter) {
  case 2:
    sp->endor_b = meas_e500_read(0);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  
  start = time(0);
  strcpy(sp->date, ctime(&start));
  step = fabs(sp->xend - sp->xbeg) / (double) (sp->samples - 1);  
  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2:
    /* common for both */
    meas_sl_attn3(sp->endor_power);  /* common attn off for measurement */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  ofield = sp->xbeg;
  for (i = 0; i < sp->scans; i++) {
    cfield = ofield;
    for (j = 0; j < sp->samples; j++) {
      if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
      switch(spectr.endorunit) {
      case 2:
	meas_sl_wtk(cfield);
	break;
      }
      if(measure_verbose) fprintf(stderr, "...done.\n");
      if(!j) {
	sleep(1);
	if(!i) {
	  if(measure_verbose) fprintf(stderr, "Talking to RF counter...");
	  switch(spectr.rfcounter) {
	  case 2:
	    sp->xbeg = meas_hp5384_read(0, HP5384_CHA);
	    break;
	  case 3:
	    sp->xbeg = meas_hp53131_read(0,1);
	    break;
	  }
	  if(measure_verbose) fprintf(stderr, "...done.\n");
	}
      }
      if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
      switch(spectr.sigchan) {
      case 2:
	sp->y[j] += meas_er023_read(0);
	break;
      }
      if(measure_verbose) fprintf(stderr, "...done.\n");
      if(upd != NULL) stop = (*upd)(sp, j, i);
      if(stop == 2) { stop = 1; break; }
      cfield += step;
    }
    if(stop == 1) {
      i++;
      break;
    }
  }

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2:
    meas_sl_wtk(sp->xend);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to RF counter...");
  switch(spectr.rfcounter) {
  case 2:
    sp->xend = meas_hp5384_read(0, HP5384_CHA); /* get the actual freq */
    break;
  case 3:
    sp->xend = meas_hp53131_read(0,1);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2: /* aspect */
    meas_sl_attn3(63);  /* common attn on after measurement */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  
  if(i) {
    for (j = 0; j < sp->samples; j++)
      sp->y[j] /= (double) i;
  }
#endif /* LIBMEAS */
}

void measure_eie(epr_type *sp, int (*upd)(epr_type *, int, int), double (*ply)(int, char *)) {

#ifndef LIBMEAS
  fprintf(stderr, "XEMR compiled without libmeas support.\n");  
#else
  int i, j, stop;
  double step, cfield, ofield;
  time_t start;

  sp->spek_type = 0; /* EPR */
  sp->resonator = 2;

  if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
  switch(spectr.sigchan) {
  case 2:
    meas_er023_calibrate(0, spectr.endor_field_mod_freq, spectr.endor_field_mod_max, spectr.endor_signal_phase_max);
    meas_er023_resonator(0, 2);     /* modulation to the endor unit */
    meas_er023_modulation_amp(0, 25.0); /* this gives 10 V p-p for wavetek */
    if(sp->harmonic == 0) sp->harmonic = 1;
    meas_er023_harmonic(0, sp->harmonic);
    meas_er023_gain(0, sp->gain);
    meas_er023_timeconstant(0, sp->timec);
    meas_er023_conversiontime(0, sp->ctime);
    meas_er023_phase(0, sp->sphase + 180); /* fix phase */
    break;
  default:
    fl_show_message("Cannot proceed without a proper signal channel","","");
    return;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  
  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2: /* aspect */
    meas_sl_attn1((unsigned int) sp->endor_power); /* WTK on */
    meas_sl_attn2(63); /* PTS off */
    meas_sl_attn3(63);  /* common attn on for now */
    meas_sl_relays(7); /* ENDOR mode */
    meas_sl_wtk_mod((unsigned int) sp->endor_mod);
    break;
  default:
    fl_show_message("Cannot proceed without ENDOR unit","","");
    return;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to MW counter...");
  switch(spectr.mwcounter) {
  case 2:
    sp->frequency = meas_hp5350_read(0);
    break;
  default:
    sp->frequency = 9.4E9;  /* just a guess... */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");
  sp->xunits = EPR_UNIT_G;

  if(spectr.magnet == 0 || spectr.magnet == 1) {
    fl_show_message("Cannot proceed without magnet control.","","");
    return;
  }
  
  if(measure_verbose) fprintf(stderr, "Talking to magnet control...");
  switch(spectr.magnet) {
  case 2:
    meas_er032_write(0, sp->xbeg);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2:
    meas_sl_wtk(sp->endor_pump_freq);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to RF counter...");
  switch(spectr.rfcounter) {
  case 2:
    sp->endor_pump_freq = meas_hp5384_read(0,1); /* CH1 = ENDOR field */
    break;
  case 3:
    sp->endor_pump_freq = meas_hp53131_read(0,1);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  start = time(0);
  strcpy(sp->date, ctime(&start));

  step = fabs(sp->xend - sp->xbeg) / (double) (sp->samples - 1);
  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2:
    meas_sl_attn3(0);  /* common attn off for measurement */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  ofield = sp->xbeg;
  for (i = 0; i < sp->scans; i++) {
    cfield = ofield;
    for (j = 0; j < sp->samples; j++) {
      if(measure_verbose) fprintf(stderr, "Talking to magnet control...");      
      switch(spectr.magnet) {
      case 2:
	meas_er032_write(0, cfield);
	break;
      }
      if(measure_verbose) fprintf(stderr, "...done.\n");
      if(!j) {
	sleep(5);
	if(!i) {
	  if(measure_verbose) fprintf(stderr, "Talking to Gaussmeter...");
	  switch(spectr.gaussmeter) {
	  case 2:
	    sp->xbeg = meas_e500_read(0);
	    break;
	  }
	  if(measure_verbose) fprintf(stderr, "...done.\n");
	}
      }
      if(measure_verbose) fprintf(stderr, "Talking to signal channel...");
      switch(spectr.sigchan) {
      case 2:
	sp->y[j] += meas_er023_read(0);
	break;
      }
      if(measure_verbose) fprintf(stderr, "...done.\n");
      if(upd != NULL) stop = (*upd)(sp, j, i);
      if(stop == 2) { stop = 1; break; }
      cfield += step;
    }
    if(stop == 1) {
      i++;
      break;
    }
  }
  
  if(measure_verbose) fprintf(stderr, "Talking to magnet control...");      
  switch(spectr.magnet) {
  case 2:
    meas_er032_write(0, sp->xend);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to ENDOR unit...");
  switch(spectr.endorunit) {
  case 2: /* aspect */
    meas_sl_attn3(63);  /* common attn on after measurement */
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(measure_verbose) fprintf(stderr, "Talking to Gaussmeter...");
  switch(spectr.gaussmeter) {
  case 2:
    sp->xend = meas_e500_read(0);
    break;
  }
  if(measure_verbose) fprintf(stderr, "...done.\n");

  if(i) {
    for (j = 0; j < sp->samples; j++)
      sp->y[j] /= (double) i;
  }
#endif /* LIBMEAS */
}
