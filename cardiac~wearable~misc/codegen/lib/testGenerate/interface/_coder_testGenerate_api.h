/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_testGenerate_api.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 04-Apr-2023 15:34:11
 */

#ifndef _CODER_TESTGENERATE_API_H
#define _CODER_TESTGENERATE_API_H

/* Include Files */
#include "emlrt.h"
#include "tmwtypes.h"
#include <string.h>

/* Variable Declarations */
extern emlrtCTX emlrtRootTLSGlobal;
extern emlrtContext emlrtContextGlobal;

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
void testGenerate(real_T rrIntervals[10], boolean_T *afDetected, real_T *meanRR,
                  real_T *stdRR, real_T *cv);

void testGenerate_api(const mxArray *prhs, int32_T nlhs,
                      const mxArray *plhs[4]);

void testGenerate_atexit(void);

void testGenerate_initialize(void);

void testGenerate_terminate(void);

void testGenerate_xil_shutdown(void);

void testGenerate_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_testGenerate_api.h
 *
 * [EOF]
 */
