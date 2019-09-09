#ifndef __CV_FILTER_H__
#define __CV_FILTER_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "cv_types.h"

float cv_filter_convComp_32F(cv_array2D_32F_t kernel, cv_array2D_32F_t arr2D, cv_point_t pos, bool padding);

#ifdef __cplusplus
}
#endif

#endif