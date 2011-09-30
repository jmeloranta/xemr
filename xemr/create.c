/*
 * Create forms and menus
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include "xemr.h"

void create() {

  gui = create_form_gui();
  create_menu_gui();
  params = create_form_params();
  create_menu_params();
  mark = create_form_mark();
  polyn = create_form_polyn();
  symmetry = create_form_symmetry();
  calibsig = create_form_calibsig();
  convol = create_form_convol();
  create_menu_convol();
  differences = create_form_differences();
  create_menu_differences();
  diffout = create_form_diffout();
  position = create_form_position();
  create_menu_position();
  peakout = create_form_peakout();
  convert = create_form_convert();
  create_menu_convert();
  linewidth = create_form_linewidth();
  equivalence = create_form_equivalence();
  create_menu_equivalence();
  create_menu_resol();
  simparam = create_form_simparam();
  variables = create_form_variables();
  create_menu_variables();
  runsimul = create_form_runsimul();
  runmon = create_form_runmon();
  runsim = create_form_runsim();
  runml = create_form_runml();
  misc = create_form_misc();
  create_menu_misc();
  window = create_form_window();
  orbital = create_form_orbital();
  cmhz = create_form_cmhz();
  hj = create_form_hj();
  heinzer = create_form_heinzer();
  hyperfine = create_form_hyperfine();
  generic = create_form_generic();
  alinewidth = create_form_alinewidth();
  elel = create_form_elel();
  quadrupole = create_form_quadrupole();
  selectlw = create_form_selectlw();
  gvalue = create_form_gvalue();
  create_menu_dlevel();
  shift = create_form_shift();
  summary = create_form_summary();
  elw = create_form_elw();
  create_menu_simpar();
  create_menu_atoms();
  spec_setup = create_form_spec_setup();
  create_menu_spec_setup();
  create_menu_window();
}

void create_menu_window() {

  fl_addto_choice(window->type, "Exponential");
  fl_addto_choice(window->type, "Triangular");
  fl_addto_choice(window->type, "Hanning");
  fl_addto_choice(window->type, "Welch");  
  fl_addto_choice(window->type, "Hamming");
  fl_addto_choice(window->type, "Blackman");
  fl_addto_choice(window->type, "Parzen");
}

void create_menu_spec_setup() {

  /* HW support by libmeas */

  /* MW counters */
  fl_addto_choice(spec_setup->mwcounter, "None");
  fl_addto_choice(spec_setup->mwcounter, "HP-5350B");

  /* Signal channels */
  fl_addto_choice(spec_setup->sigchan, "None");
  fl_addto_choice(spec_setup->sigchan, "Bruker ER-023");
  fl_addto_choice(spec_setup->sigchan, "Stanford Research SR245");

  /* Gaussmeters */
  fl_addto_choice(spec_setup->gaussmeter, "None");
  fl_addto_choice(spec_setup->gaussmeter, "Varian E-500");

  /* Magnet control */
  fl_addto_choice(spec_setup->magnet, "None");
  fl_addto_choice(spec_setup->magnet, "Bruker ER-032");
  
  /* ENDOR unit */
  fl_addto_choice(spec_setup->endorunit, "None");
  fl_addto_choice(spec_setup->endorunit, "Bruker Aspect");

  /* RF counter */
  fl_addto_choice(spec_setup->rfcounter, "None");
  fl_addto_choice(spec_setup->rfcounter, "HP-5384A/HP-5385A");
  fl_addto_choice(spec_setup->rfcounter, "HP-53131A");
}

void create_menu_gui() {

  fl_set_menu(gui->file, "New spectrum|Clear spectrum|New page|Load template spectrum|Load spectrum|Load many spectra|Save spectrum|Search spectrum|Convert|Export ASCII file|About Xemr|Exit Xemr");

  fl_set_menu(gui->edit, "Set center field|Set scan width|Parameters|Show page summary|Zero between cursors|Shrink spectrum|Extend spectrum|Copy current page|Shift spectrum|Field calibration");

  fl_set_menu(gui->view, "ZoomX|UnzoomX|ZoomY|UnzoomY|Add overlay|Delete overlay");

  fl_set_menu(gui->calc, "Add spectra|Add spectra direct|Multiply by constant|Add constant|Differentiate|Integrate|9 point smooth|Symmetry smooth|Forward FFT|Power spectrum|Reverse FFT|Windowing|Convolution|Cross/Autocorrelation|Cepstral analysis|Auto scale|Calc LW B|Calc LW C|RMS difference");

  fl_set_menu(gui->baseline, "Polynomial correct|Spline correct|Mean value subtract|Calculate noise level");

  fl_set_menu(gui->measure, "Measure EPR|Measure ENDOR|Measure TRIPLE general|Measure TRIPLE special|Measure EIE|Spectrometer setup|FF lock on|FF lock off|Cancel delayed|Cancel now");

  fl_set_menu(gui->peak, "Differences|Position analysis|Double integrate|Single integrate");

  fl_set_menu(gui->simulate, "Run simulation|Read parameters from disk|Define simulation|Edit run parameters|Edit numerical parameters|Edit spin parameters|Edit electron g parameters|Edit hyperfine parameters|Edit lineshape parameters|Edit electron-electron parameters|Edit nuclear quadrupole parameters|Edit orbital parameters|Write parameters to disk|Print parameters|Clear parameters");
  
  fl_set_menu(gui->graph, "Execute Grace for all pages|Execute Grace for current page|Stackplot with Grace|Reversed stackplot with Grace|Generate Breit-Rabi graph|Generate Breit-Rabi transition state diagram|Generate Breit-Rabi transition moment diagram|Generate angle graph");

}

void create_menu_atoms() {

  int i;
  char buffer[2048], tmp[50];

  buffer[0] = 0;
  for (i = 0; i < EPR_NATOMS; i++) {
    sprintf(tmp, "%s(%d)", epr_atoms[i].symbol, epr_atoms[i].isotope);
    strcat(buffer, tmp);
    strcat(buffer, "|");
  }

  for (i = 0; i < 12; i++)
    fl_addto_choice(generic->atom[i], buffer);
  fl_setpup_fontsize(FL_NORMAL_SIZE);
}

void create_menu_dlevel() {

  fl_addto_choice(simparam->dlevel, "Absorption|1st deriv|2nd deriv|Freq. space");
}

void create_menu_params() {

  fl_addto_choice(params->type, "EPR|ENDOR|TRIPLE gen.|TRIPLE spec.");
  fl_addto_choice(params->units, "G|mT|MHz|GHz|s|Arbitary");
}

void create_menu_resol() {

  fl_addto_choice(params->resolution, "64|128|256|512|1K|2K|4K|8K|16K|32K|64K");
}

void create_menu_convol() {

  fl_addto_choice(convol->type, "with Lorentz lineshape");
  fl_addto_choice(convol->type, "with Gaussian lineshape");
  fl_addto_choice(convol->type, "with another spectrum");
}

void create_menu_differences() {

  fl_addto_choice(differences->method, "Baseline");
  fl_addto_choice(differences->method, "Tangent");
}

void create_menu_position() {

  fl_addto_choice(position->method, "Baseline");
  fl_addto_choice(position->method, "Tangent");
}

void create_menu_convert() {

  fl_addto_choice(convert->format, "Bruker ESP");
  fl_addto_choice(convert->format, "Bruker Aspect");
  fl_addto_choice(convert->format, "Varian epr-in");
  fl_addto_choice(convert->format, "ASCII XY-data");
  fl_addto_choice(convert->format, "ASCII Y-data");
}

void create_menu_equivalence() {

  register int i;

  /* the numbering scheme is fixed in varutil.h! */
  for (i = 0; i < 7; i++) {
    fl_addto_choice(equivalence->property[i], "None");
    fl_addto_choice(equivalence->property[i], "LG-factor");
    fl_addto_choice(equivalence->property[i], "Linewidth");
    fl_addto_choice(equivalence->property[i], "Position");
    fl_addto_choice(equivalence->property[i], "Amplitude");
    fl_addto_choice(equivalence->property[i], "HFC");
    fl_addto_choice(equivalence->property[i], "Vector B");
    fl_addto_choice(equivalence->property[i], "Vector C");
    fl_addto_choice(equivalence->property[i], "Matrix D");
    fl_addto_choice(equivalence->property[i], "------------");
    fl_addto_choice(equivalence->property[i], "Heinzer Rate");
    fl_addto_choice(equivalence->property[i], "Electron-electron D");
    fl_addto_choice(equivalence->property[i], "Nuclear quadrupole P");
    fl_addto_choice(equivalence->property[i], "ENDOR lw");
    fl_addto_choice(equivalence->property[i], "ENDOR amp");
    fl_addto_choice(equivalence->property[i], "------------");    
    fl_addto_choice(equivalence->property[i], "Spin-orbit constant");
    fl_addto_choice(equivalence->property[i], "Tetragonal CF-x");
    fl_addto_choice(equivalence->property[i], "Tetragonal CF-y");
    fl_addto_choice(equivalence->property[i], "Tetragonal CF-z");
    fl_addto_choice(equivalence->property[i], "Octahedral CF");    
  }
}

void create_menu_variables() {

  register int i;

  for (i = 0; i < 10; i++) {
    fl_addto_choice(variables->type[i], "None");
    fl_addto_choice(variables->type[i], "LG-factor");
    fl_addto_choice(variables->type[i], "Linewidth");
    fl_addto_choice(variables->type[i], "Position");
    fl_addto_choice(variables->type[i], "Amplitude");
    fl_addto_choice(variables->type[i], "HFC");
    fl_addto_choice(variables->type[i], "Vector B");
    fl_addto_choice(variables->type[i], "Vector C");
    fl_addto_choice(variables->type[i], "Matrix D");
    fl_addto_choice(variables->type[i], "Norris Rate");
    fl_addto_choice(variables->type[i], "Heinzer Rate");
    fl_addto_choice(variables->type[i], "Electron-electron D");
    fl_addto_choice(variables->type[i], "Nuclear quadrupole P");
    fl_addto_choice(variables->type[i], "ENDOR lw");
    fl_addto_choice(variables->type[i], "ENDOR amp");
    fl_addto_choice(variables->type[i], "ENDOR field offset");
    fl_addto_choice(variables->type[i], "Spin-orbit constant");
    fl_addto_choice(variables->type[i], "Tetragonal CF-x");
    fl_addto_choice(variables->type[i], "Tetragonal CF-y");
    fl_addto_choice(variables->type[i], "Tetragonal CF-z");
    fl_addto_choice(variables->type[i], "Octahedral CF");    
  }
}

void create_menu_simpar() {

  /* associated with run parameters */
  fl_addto_choice(simparam->run, "Simulation");
  fl_addto_choice(simparam->run, "Fit Monte Carlo");
  fl_addto_choice(simparam->run, "Fit Simplex");
  fl_addto_choice(simparam->run, "Fit L-M");
  fl_addto_choice(simparam->run, "Fit Hooke-Jeeves");
  
  /* associated with system parameters */
  fl_addto_choice(simparam->system, "Oriented EPR");
  fl_addto_choice(simparam->system, "Non-oriented EPR");
  fl_addto_choice(simparam->system, "Oriented ENDOR");
  fl_addto_choice(simparam->system, "Non-oriented ENDOR");
  fl_addto_choice(simparam->system, "Oriented EPR (fictious spin)");
  fl_addto_choice(simparam->system, "Non-oriented EPR (fictious spin)");
  fl_addto_choice(simparam->system, "Oriented ENDOR (fictious spin)");
  fl_addto_choice(simparam->system, "Non-oriented ENDOR (fictious spin)");

  /* associated with Hamilton parameters */
  fl_addto_choice(simparam->hamilton, "S = 1/2 - 1st order");
  fl_addto_choice(simparam->hamilton, "g.B.S + A.S.I");
  fl_addto_choice(simparam->hamilton, "g.B.S + A.S.I + S.D.S");
  fl_addto_choice(simparam->hamilton, "g.B.S + A.S.I + I.P.I");
  fl_addto_choice(simparam->hamilton, "g.B.S + A.S.I + S.D.S + I.P.I");
  fl_addto_choice(simparam->hamilton, "g.B.S + gN.B.I + A.S.I");
  fl_addto_choice(simparam->hamilton, "g.B.S + gN.B.I + A.S.I + S.D.S");
  fl_addto_choice(simparam->hamilton, "g.B.S + gN.B.I + A.S.I + I.P.I");
  fl_addto_choice(simparam->hamilton, "g.B.S + gN.B.I + A.S.I + S.D.S + I.P.I");

  /* transition moment parameters */
  fl_addto_choice(simparam->transition, "S (90o)");
  fl_addto_choice(simparam->transition, "-I (90o)");
  fl_addto_choice(simparam->transition, "S - I (90o)");
  fl_addto_choice(simparam->transition, "S (0o)");
  fl_addto_choice(simparam->transition, "-I (0o)");
  fl_addto_choice(simparam->transition, "S - I (0o)");
  fl_addto_choice(simparam->transition, "Constant (90o)");
  fl_addto_choice(simparam->transition, "Constant (0o)");
  /* might add L here ? */

  /* Orbital angular momentum related parameters */
  fl_addto_choice(simparam->orbital, "none");
  fl_addto_choice(simparam->orbital, "B.L");
  fl_addto_choice(simparam->orbital, "B.L + crystal");
  fl_addto_choice(simparam->orbital, "B.L + S.L");
  fl_addto_choice(simparam->orbital, "B.L + S.L + crystal");
  fl_addto_choice(simparam->orbital, "S.L");
  fl_addto_choice(simparam->orbital, "S.L + crystal");
  fl_addto_choice(simparam->orbital, "crystal");

  /* associated with lineshape parameters */
  fl_addto_choice(simparam->lineshape, "Stick");
  fl_addto_choice(simparam->lineshape, "Normal");
  fl_addto_choice(simparam->lineshape, "Asymm. Lorentz");
  fl_addto_choice(simparam->lineshape, "Norris (intra)");
  fl_addto_choice(simparam->lineshape, "Heinzer (intra)");
  fl_addto_choice(simparam->lineshape, "Field direction dep.");

  fl_addto_choice(simparam->iunits, "G");
  fl_addto_choice(simparam->iunits, "mT");
  fl_addto_choice(simparam->iunits, "MHz");
  fl_addto_choice(simparam->iunits, "cm-1");
}

void create_menu_misc() {

  fl_addto_choice(misc->phibegin, "0o");
  fl_addto_choice(misc->phibegin, "90o");
  fl_addto_choice(misc->phibegin, "180o");
  fl_addto_choice(misc->phibegin, "270o");

  fl_addto_choice(misc->phiend, "0o");
  fl_addto_choice(misc->phiend, "90o");
  fl_addto_choice(misc->phiend, "180o");
  fl_addto_choice(misc->phiend, "270o");
  fl_addto_choice(misc->phiend, "360o");

  fl_addto_choice(misc->thetabegin, "0o");
  fl_addto_choice(misc->thetabegin, "90o");

  fl_addto_choice(misc->thetaend, "0o");
  fl_addto_choice(misc->thetaend, "90o");
  fl_addto_choice(misc->thetaend, "180o");

  fl_addto_choice(misc->method, "Gauss-Legendre (regular)");
  fl_addto_choice(misc->method, "Gauss-Legendre (IGLOO)");
  fl_addto_choice(misc->method, "Gauss-Chebysev (regular)");
  fl_addto_choice(misc->method, "Gauss-Chebysev (IGLOO)");
  fl_addto_choice(misc->method, "Simpson");
  fl_addto_choice(misc->method, "Trapez&Simpson");
  fl_addto_choice(misc->method, "Trapez");
  fl_addto_choice(misc->method, "Interpolate (regular)");
  fl_addto_choice(misc->method, "Interpolate (IGLOO)");
  fl_addto_choice(misc->method, "Spline interpolate (regular)");
  fl_addto_choice(misc->method, "Spline interpolate (IGLOO)");
  fl_addto_choice(misc->method, "Axial symmetry");
  fl_addto_choice(misc->method, "Axial interpolate (linear)");
  fl_addto_choice(misc->method, "Axial interpolate (spline)");

  fl_addto_choice(misc->points, "2x2");
  fl_addto_choice(misc->points, "3x3");
  fl_addto_choice(misc->points, "4x4");
  fl_addto_choice(misc->points, "5x5");
  fl_addto_choice(misc->points, "6x6");

  fl_addto_choice(misc->follow, "off");
  fl_addto_choice(misc->follow, "on");

}
