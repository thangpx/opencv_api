#ifndef __CV_MEANSHIFT_H__
#define __CV_MEANSHIFT_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cv_types.h"

// functions to generate kernels
float cv_createKernel_Epanechnikov(cv_array2D_32F_t *kernel, int w, int h, float c);
float cv_createKernel_Gaussian(cv_array2D_32F_t *kernel, int w, int h, float c);
// ---------------------------
int cv_meanShift(cv_array2D_32F_t kernel, cv_array2D_32F_t arr2D, cv_point_t point, cv_point32F_t *mx);
int cv_meanShift_trace(cv_array2D_32F_t kernel, cv_array2D_32F_t arr2D, cv_point_t *point, float EPSILON, int maxIte);
int cv_meanShift_filter(cv_image32F_t *image, cv_image32F_t *img_out, float hs, float hr, float EPS_color, float EPS_spatial, int maxIte);

#ifdef __cplusplus
}
#endif
#endif