/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_fingerFFT_mex.h
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 04-Apr-2023 15:29:16
 */

#ifndef _CODER_FINGERFFT_MEX_H
#define _CODER_FINGERFFT_MEX_H

/* Include Files */
#include "emlrt.h"
#include "mex.h"
#include "tmwtypes.h"

#ifdef __cplusplus
extern "C" {
#endif

/* Function Declarations */
MEXFUNCTION_LINKAGE void mexFunction(int32_T nlhs, mxArray *plhs[],
                                     int32_T nrhs, const mxArray *prhs[]);

emlrtCTX mexFunctionCreateRootTLS(void);

void unsafe_fingerFFT_mexFunction(int32_T nlhs, int32_T nrhs);

#ifdef __cplusplus
}
#endif

#endif
/*
 * File trailer for _coder_fingerFFT_mex.h
 *
 * [EOF]
 */
