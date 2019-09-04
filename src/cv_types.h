#ifndef __CV_TYPES_H__
#define __CV_TYPES_H__

#ifdef __cplusplus
extern "C" {
#endif

#include "stdio.h"
#include "stdbool.h"
#include "stdint.h"

typedef struct
{
    int x;
    int y;
} cv_point_t;

typedef struct
{
    double width;
    double height;
} cv_size_double_t;

typedef struct
{
    int width;
    int height;
} cv_size_int_t;

typedef struct
{
    cv_point_t pos;
    cv_size_int_t size;
} cv_window_2d_t;

typedef struct
{
    int len;
    float* data;
} cv_array32F_t;

#define INIT_CV_ARRAY32F_T(X) cv_array32F_t X = {.len = 0, .data = NULL}

typedef struct
{
    int len;
    int* data;
} cv_arrayInt_t;

typedef struct
{
    cv_size_int_t size;
    uint8_t* data;
} cv_array2D_8U_t;

typedef struct
{
    cv_size_int_t size;
    float* data;
} cv_array2D_32F_t;

#define INIT_CV_ARRAY2D_8U_T(X) cv_array2D_8U_t X = {.size = {.width = 0, .height = 0}, .data = NULL}

typedef struct
{
    float r;
    float g;
    float b;
} cv_rgb_t;

typedef struct
{
    float h;
    float s;
    float v;
} cv_hsv_t;

typedef struct
{
    cv_size_int_t   size;
    int         nChannel;
    uint8_t*    data;
} cv_image8U_t;

typedef struct
{
    cv_size_int_t   size;
    int         nChannel;
    uint16_t*    data;
} cv_image16U_t;

typedef struct
{
    cv_size_int_t   size;
    int         nChannel;
    float*      data;
} cv_image32F_t;

#ifdef __cplusplus
}
#endif

#endif