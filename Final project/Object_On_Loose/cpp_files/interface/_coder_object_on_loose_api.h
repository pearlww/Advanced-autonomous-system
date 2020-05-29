/*
 * Student License - for use by students to meet course requirements and
 * perform academic research at degree granting institutions only.  Not
 * for government, commercial, or other organizational use.
 * File: _coder_object_on_loose_api.h
 *
 * MATLAB Coder version            : 4.3
 * C/C++ source code generated on  : 29-May-2020 15:35:37
 */

#ifndef _CODER_OBJECT_ON_LOOSE_API_H
#define _CODER_OBJECT_ON_LOOSE_API_H

/* Include Files */
#include <stddef.h>
#include <stdlib.h>
#include "tmwtypes.h"
#include "mex.h"
#include "emlrt.h"

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

/* Function Declarations */
extern void object_on_loose(real_T x[10000], real_T y[10000], real_T *guess_num,
  real_T *guess_x, real_T *guess_y, real_T *guess_q, real_T *guess_flipped);
extern void object_on_loose_api(const mxArray * const prhs[2], int32_T nlhs,
  const mxArray *plhs[5]);
extern void object_on_loose_atexit(void);
extern void object_on_loose_initialize(void);
extern void object_on_loose_terminate(void);
extern void object_on_loose_xil_shutdown(void);
extern void object_on_loose_xil_terminate(void);

#endif

/*
 * File trailer for _coder_object_on_loose_api.h
 *
 * [EOF]
 */
