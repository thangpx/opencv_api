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

// other maths
int cv_imageNormalization_minmax(cv_image32F_t *img, const float lower, const float upper, int channel);
int cv_imageHist(cv_array32F_t* hist, cv_image32F_t *img, cv_array2D_8U_t *mask, const float range[2], const int histSize, int channel);
int cv_imageMaskInRange(cv_image32F_t *img, cv_array32F_t lowerb, cv_array32F_t upperb, cv_array2D_8U_t *mask, bool addmore);

// image
cv_hsv_t cv_cvtRGB2HSV(cv_rgb_t rgb);

int cv_cvtImage8Uto32F(cv_image8U_t *img8U, cv_image32F_t *img32F);
int cv_cvtImage32Fto8U(cv_image32F_t *img32F, cv_image8U_t *img8U);
int cv_cvtImageRGBtoHSV(cv_image32F_t *rgbImage, cv_image32F_t *hsvImage);
int cv_createProbabilityImage(cv_image32F_t *hsvImage, cv_image32F_t *probImage, cv_array2D_8U_t *mask, cv_array32F_t *hist, const float step, const int channel);

int cv_getImageArea_32F(cv_image32F_t *img, cv_image32F_t *imgArea, cv_window_2d_t *window);

#ifdef __cplusplus
}
#endif

#endif