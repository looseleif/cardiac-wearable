/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: _coder_fingerFFT_mex.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 04-Apr-2023 15:29:16
 */

/* Include Files */
#include "_coder_fingerFFT_mex.h"
#include "_coder_fingerFFT_api.h"

/* Function Definitions */
/*
 * Arguments    : int32_T nlhs
 *                mxArray *plhs[]
 *                int32_T nrhs
 *                const mxArray *prhs[]
 * Return Type  : void
 */
void mexFunction(int32_T nlhs, mxArray *plhs[], int32_T nrhs,
                 const mxArray *prhs[])
{
  (void)plhs;
  (void)prhs;
  mexAtExit(&fingerFFT_atexit);
  /* Module initialization. */
  fingerFFT_initialize();
  /* Dispatch the entry-point. */
  unsafe_fingerFFT_mexFunction(nlhs, nrhs);
  /* Module termination. */
  fingerFFT_terminate();
}

/*
 * Arguments    : void
 * Return Type  : emlrtCTX
 */
emlrtCTX mexFunctionCreateRootTLS(void)
{
  emlrtCreateRootTLSR2021a(&emlrtRootTLSGlobal, &emlrtContextGlobal, NULL, 1,
                           NULL);
  return emlrtRootTLSGlobal;
}

/*
 * Arguments    : int32_T nlhs
 *                int32_T nrhs
 * Return Type  : void
 */
void unsafe_fingerFFT_mexFunction(int32_T nlhs, int32_T nrhs)
{
  emlrtStack st = {
      NULL, /* site */
      NULL, /* tls */
      NULL  /* prev */
  };
  st.tls = emlrtRootTLSGlobal;
  /* Check for proper number of arguments. */
  if (nrhs != 0) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:WrongNumberOfInputs", 5, 12, 0, 4,
                        9, "fingerFFT");
  }
  if (nlhs > 0) {
    emlrtErrMsgIdAndTxt(&st, "EMLRT:runTime:TooManyOutputArguments", 3, 4, 9,
                        "fingerFFT");
  }
  /* Call the function. */
  fingerFFT_api();
}

/*
 * File trailer for _coder_fingerFFT_mex.c
 *
 * [EOF]
 */
