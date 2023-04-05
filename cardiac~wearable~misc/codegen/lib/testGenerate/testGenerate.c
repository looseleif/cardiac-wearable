/*
 * Academic License - for use in teaching, academic research, and meeting
 * course requirements at degree granting institutions only.  Not for
 * government, commercial, or other organizational use.
 * File: testGenerate.c
 *
 * MATLAB Coder version            : 5.2
 * C/C++ source code generated on  : 04-Apr-2023 15:34:11
 */

/* Include Files */
#include "testGenerate.h"
#include <math.h>

/* Function Definitions */
/*
 * Calculate the mean and standard deviation of the RR intervals
 *
 * Arguments    : const double rrIntervals[10]
 *                boolean_T *afDetected
 *                double *meanRR
 *                double *stdRR
 *                double *cv
 * Return Type  : void
 */
void testGenerate(const double rrIntervals[10], boolean_T *afDetected,
                  double *meanRR, double *stdRR, double *cv)
{
  double scale;
  double t;
  double xbar;
  double y_tmp;
  int k;
  scale = rrIntervals[0];
  xbar = rrIntervals[0];
  for (k = 0; k < 9; k++) {
    y_tmp = rrIntervals[k + 1];
    scale += y_tmp;
    xbar += y_tmp;
  }
  *meanRR = scale / 10.0;
  xbar /= 10.0;
  *stdRR = 0.0;
  scale = 3.3121686421112381E-170;
  for (k = 0; k < 10; k++) {
    y_tmp = fabs(rrIntervals[k] - xbar);
    if (y_tmp > scale) {
      t = scale / y_tmp;
      *stdRR = *stdRR * t * t + 1.0;
      scale = y_tmp;
    } else {
      t = y_tmp / scale;
      *stdRR += t * t;
    }
  }
  *stdRR = scale * sqrt(*stdRR);
  *stdRR /= 3.0;
  /*  Calculate the coefficient of variation */
  *cv = *stdRR / *meanRR;
  /*  Detect atrial fibrillation based on the coefficient of variation */
  *afDetected = (*cv >= 0.156);
}

/*
 * File trailer for testGenerate.c
 *
 * [EOF]
 */
