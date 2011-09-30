/* global data not defined anywhere else */

#include "epr.h"

epr_atom epr_atoms[EPR_NATOMS] = {
  /* Z Isotope Name Abun. spin gn isotr.hfc. uniax. */
  {1, 1, "H", 99.9850, 0.5, 5.5856948, 506.850, 0.0 },
  {1, 2, "D", 0.0148, 1.0, 0.8574388, 77.8027, 0.0},
  {2, 3, "He", 0.000138, 0.5, -4.255280, 226.83, 0.0},
  {3, 6, "Li", 7.5, 1.0, 0.8220575, 5.43, 0.0},
  {3, 7, "Li", 92.5, 1.5, 2.170977, 14.34, 0.0},
  {4, 9, "Be", 100, 1.5, -0.7850, -16.11, 0.0},
  {5, 10, "B", 19.8, 3.0, 0.600220, 30.43, 0.760},
  {5, 11, "B", 80.2, 1.5, 1.792437, 90.88, 2.271},
  {6, 13, "C", 1.11, 0.5, 1.40483, 134.77, 3.832},
  {7, 14, "N", 99.63, 1.0, 0.4037637, 64.62, 1.981},
  {7, 15, "N", 0.366, 0.5, -0.5663826, -90.65, -2.779},
  {8, 17, "O", 0.038, 2.5, -0.757522, -187.80, -6.009},
  {9, 19, "F", 100.0, 0.5, 5.257771, 1886.53, 62.80},
  {10, 21, "Ne", 0.27, 1.5, -0.441200, -221.02, -7.536},
  {11, 23, "Na", 100.0, 1.5, 1.478402, 31.61, 0.0},
  {12, 25, "Mg", 10.0, 2.5, -0.34218, 17.338, 0.0 },
  {13, 27, "Al", 100.0, 2.5, 1.456612, 139.55, 2.965},
  {14, 29, "Si", 4.67, 0.5, -1.1106, -163.93, -4.075},
  {15, 31, "P", 100.0, 0.5, 2.26322, 474.79, 13.088},
  {16, 33, "S", 0.75, 1.5, 0.42911, 123.57, 3.587},
  {17, 35, "Cl", 75.77, 1.5, 0.5479198, 204.21, 6.266},
  {17, 37, "Cl", 24.23, 1.5, 0.4560854, 169.98, 5.216},
  {19, 39, "K", 93.26, 1.5, 0.2609928, 8.238, 0.0},
  {19, 41, "K", 6.73, 1.5, 0.1432553, 4.525, 0.0},
  {20, 43, "Ca", 0.135, 3.5, -0.376417, -22.862, 0.0},
  {21, 45, "Sc", 100.0, 3.5, 1.35962, 100.73, 3.430},
  {22, 47, "Ti", 7.4, 2.5, -0.31539, -27.904, -1.051},
  {22, 49, "Ti", 5.4, 3.5, -0.315468, -27.910, -1.051},
  {23, 50, "V", 0.250, 6.0, 0.556597, 56.335, 2.368},
  {23, 51, "V", 99.750, 3.5, 1.46837, 148.62, 6.246},
  {24, 53, "Cr", 9.50, 1.5, -0.3147, -26.698, -1.470},
  {25, 55, "Mn", 100.0, 2.5, 1.3819, 179.70, -8.879},
  {26, 57, "Fe", 2.15, 0.5, 0.1806, 26.662, 1.395},
  {27, 59, "Co", 100.0, 3.5, 1.318, 212.20, 12.065},
  {28, 61, "Ni", 1.13, 1.5, -0.50001, -89.171, -5.360},
  {29, 63, "Cu", 69.2, 1.5, 1.484, 213.92, 17.085},
  {29, 65, "Cu", 30.8, 1.5, 1.588, 228.92, 18.283},
  {30, 67, "Zn", 4.10, 2.5, 0.350315, 74.470, 5.021},
  {31, 69, "Ga", 60.1, 1.5, 1.34440, 435.68, 7.274},
  {31, 71, "Ga", 39.9, 1.5, 1.70819, 553.58, 9.242},
  {32, 73, "Ge", 7.8, 4.5, -0.1954371, -84.32, -1.716},
  {33, 75, "As", 100.0, 1.5, 0.959654, 523.11, 11.905},
  {34, 77, "Se", 7.6, 0.5, 1.0693, 717.93, 17.542},
  {35, 79, "Br", 50.69, 1.5, 1.404276, 1144.34, 29.174},
  {35, 81, "Br", 49.31, 1.5, 1.513717, 1233.52, 31.448},
  {36, 83, "Kr", 11.5, 4.5, -0.215706, -211.85, -5.515},
  {79, 197, "Au", 100.0, 1.5, 0.097969, 102.62, 1.884},
  /* ... add more here */
  {54, 129, "Xe", 26.4, 0.5, -1.55595, -2418.92, -47.815},
  {54, 131, "Xe", 21.2, 1.5, 0.461243, 717.06, 14.143}
};
