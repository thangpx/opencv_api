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
    float x;
    float y;
} cv_point32F_t;

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

#define COMPARE_SIZE_T(X,Y) ((X.width == Y.width) && (X.height == Y.height))

typedef struct
{
    cv_point_t pos;
    cv_size_int_t size;
} cv_window_2d_t;

// 1D array
typedef struct
{
    int len;
    float* data;
} cv_array32F_t;

typedef struct
{
    int len;
    int* data;
} cv_arrayInt_t;

#define FREE_ARRAY_1D_T(X) {free(X.data); X.data = NULL; X.len = 0;}

// 2D array
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

#define FREE_ARRAY_2D_T(X) {free(X.data); X.data = NULL; X.size.width = 0; X.size.height = 0;}

// for presentation of images
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
    float l;
    float a;
    float b;
} cv_lab_t;

typedef struct
{
    float x;
    float y;
    float z;
} cv_xyz_t;

// Image types
typedef enum cv_imageFormat_t
{
    CV_RGB, CV_HSV, CV_XYZ, CV_GREY, CV_LAB
} cv_imageFormat_t;

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

#define FREE_IMAGE_T(X) {free(X.data); X.data = NULL; X.size.width = 0; X.size.height = 0; X.nChannel = 0;}

#ifdef __cplusplus
}
#endif

#endif