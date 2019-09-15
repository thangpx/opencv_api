#ifndef __CV_MATH_H__
#define __CV_MATH_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "math.h"
#include "cv_types.h"

// euclidean space
float cv_euclideanDistance_2d_sqr(cv_point32F_t p1, cv_point32F_t p2);

// Some functions with array
// ----- moments
float cv_moments(cv_array2D_32F_t arrIn, int j, int i, float bw);
float cv_moments_window(cv_array2D_32F_t arrIn, cv_window_2d_t window, int j, int i, float bw);
int cv_normalize_minmax(cv_array32F_t *arrIn, const float min, const float max);

#ifdef __cplusplus
}
#endif

#endif