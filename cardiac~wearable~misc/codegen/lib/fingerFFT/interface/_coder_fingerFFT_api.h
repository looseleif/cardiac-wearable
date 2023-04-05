/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_fingerFFT_api.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 04-Apr-2023 15:29:16
 */

#ifndef _CODER_FINGERFFT_API_H
#define _CODER_FINGERFFT_API_H

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
void fingerFFT(void);

void fingerFFT_api(void);

void fingerFFT_atexit(void);

void fingerFFT_initialize(void);

void fingerFFT_terminate(void);

void fingerFFT_xil_shutdown(void);

void fingerFFT_xil_terminate(void);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_fingerFFT_api.h
 *
 * [EOF]
 */
