#ifndef __CV_MATH_IMG__
#define __CV_MATH_IMG__

#ifdef __cplusplus
extern "C" {
#endif

#include "math.h"
#include "cv_types.h"

int cv_cvtImage8Uto32F(cv_image8U_t *img8U, cv_image32F_t *img32F);
int cv_cvtImage32Fto8U(cv_image32F_t *img32F, cv_image8U_t *img8U);

int cv_imageNormalization_minmax(cv_image32F_t *img, const float lower, const float upper, int channel);
int cv_imageHist(cv_array32F_t* hist, cv_image32F_t *img, cv_array2D_8U_t *mask, const float range[2], const int histSize, int channel);
int cv_imageMaskInRange(cv_image32F_t *img, cv_array32F_t lowerb, cv_array32F_t upperb, cv_array2D_8U_t *mask, bool addmore);

int cv_createProbabilityImage(cv_image32F_t *hsvImage, cv_image32F_t *probImage, cv_array2D_8U_t *mask, cv_array32F_t *hist, const float step, const int channel);

int cv_getImageArea_32F(cv_image32F_t *img, cv_image32F_t *imgArea, cv_window_2d_t *window);

int cv_imgIntegral_create(cv_image32F_t imgIn, cv_image32F_t *integralImage, int channel, bool rotate);
double cv_rectangleSum(cv_image32F_t integralImage, int x, int y, int w, int h, bool rotate);
#ifdef __cplusplus
}
#endif

#endif