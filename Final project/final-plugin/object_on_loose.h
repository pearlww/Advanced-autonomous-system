//
// Student License - for use by students to meet course requirements and
// perform academic research at degree granting institutions only.  Not
// for government, commercial, or other organizational use.
// File: object_on_loose.h
//
// MATLAB Coder version            : 4.3
// C/C++ source code generated on  : 29-May-2020 15:35:37
//
#ifndef OBJECT_ON_LOOSE_H
#define OBJECT_ON_LOOSE_H

// Include Files
#include <cstddef>
#include <cstdlib>
#include "rtwtypes.h"
#include "object_on_loose_types.h"

// Function Declarations
extern void object_on_loose(const double x[10000], const double y[10000], double
  *guess_num, double *guess_x, double *guess_y, double *guess_q, double
  *guess_flipped);
extern void object_on_loose_initialize();
extern void object_on_loose_terminate();

#endif

//
// File trailer for object_on_loose.h
//
// [EOF]
//
