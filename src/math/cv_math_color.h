#ifndef __CV_COLOR_H__
#define __CV_COLOR_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cv_types.h"

cv_hsv_t cv_cvtRGB2HSV(cv_rgb_t rgb);
cv_lab_t cv_cvtRGB2LAB(cv_rgb_t rgb);
cv_xyz_t cv_cvtRGB2XYZ(cv_rgb_t rgb);

cv_rgb_t cv_cvtHSV2RGB(cv_hsv_t hsv);
cv_rgb_t cv_cvtLAB2RGB(cv_lab_t lab);
cv_rgb_t cv_cvtXYZ2RGB(cv_xyz_t xyz);

float cv_cvtRGB2GREY(cv_rgb_t rgb);

int cv_cvtImageRGBto(cv_imageFormat_t type, cv_image32F_t *rgbImage, cv_image32F_t *imgOut);

#ifdef __cplusplus
}
#endif

#endif