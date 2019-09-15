
#include "cv_math.h"
#include "stdio.h"
#include "stdlib.h"

float cv_euclideanDistance_2d_sqr(cv_point32F_t p1, cv_point32F_t p2) {
    const float dx = p1.x - p2.x;
    const float dy = p1.y - p2.y;
    return (dx*dx + dy*dy);
}

float cv_moments(cv_array2D_32F_t arrIn, int j, int i, float bw) {
    float mji = 0.0;
    if(arrIn.data == NULL) {
        printf("cv_moments : Error - There is no input data.\n");
        return mji;
    }
    if(!((arrIn.size.width % 2) || (arrIn.size.height % 2))) {
        printf("cv_moments : Error - The size of frame input have to be odd number.\n");
        return mji;
    }
    // normalize the coordination
    int xmax = arrIn.size.width / 2;
    int ymax = arrIn.size.height / 2;
    float xstep = bw / (float)xmax;
    float ystep = bw / (float)ymax;
    
    for(int y = 0; y < arrIn.size.height; y++) {
        float py = (float)pow((double)((y - ymax) * ystep), (double)i);
        for(int x = 0; x < arrIn.size.width; x++) {
            float px = (float)pow((double)((x - xmax) * xstep), (double)j);
            mji += arrIn.data[y * arrIn.size.width + x] * px * py;
        }
    }
    return mji;
}

float cv_moments_window(cv_array2D_32F_t arrIn, cv_window_2d_t window, int j, int i, float bw) {
    float mji = 0.0;
    if(arrIn.data == NULL) {
        printf("cv_moments_window : Error - There is no input data.\n");
        return mji;
    }
    if(!((window.size.width % 2) || (window.size.height % 2))) {
        printf("cv_moments_window : Error - The size of window input have to be odd number.\n");
        return mji;
    }
    const int w_xmax = window.size.width / 2;
    const int w_ymax = window.size.height / 2;
    if(window.pos.x < w_xmax || window.pos.y < w_ymax || window.pos.x >= (arrIn.size.width - w_xmax) || window.pos.y >= (arrIn.size.height - w_ymax)) {
        printf("cv_moments_window : Error - Cannot get the window input because of the position.\n");
        return mji;
    }

    // normalize the coordination
    float xstep = bw / (float)w_xmax;
    float ystep = bw / (float)w_ymax;
    for(int y = -w_ymax; y <= w_ymax; y++) {
        int py = (window.pos.y + y) * arrIn.size.width;
        float sy = (float)pow((float)y * ystep,(float)i);
        for(int x = -w_xmax; x <= w_xmax; x++) {
            int px = (window.pos.x + x);
            float sx = (float)pow((float)x * xstep,(float)j);
            mji += arrIn.data[py + px] * sx * sy;
        }
    }

    return mji;
}

int cv_normalize_minmax(cv_array32F_t *arrIn, const float min, const float max) {
    if(arrIn->data == NULL) {
        printf("cv_normalize_minmax : Error - There is no input data.\n");
        return 1;
    }

    if(min >= max) {
        printf("cv_normalize_minmax : Error - The input value min have to be less than max.\n");
        return 1;
    }

    float minV, maxV;
    minV = arrIn->data[0];
    maxV = arrIn->data[0];

    for(int i = 1; i < arrIn->len; i++) {
        if(minV > arrIn->data[i]) minV = arrIn->data[i];
        if(maxV < arrIn->data[i]) maxV = arrIn->data[i];
    }

    const float inDelta = maxV - minV;
    const float outDelta = max - min;
    const float ratio = outDelta / inDelta;
    printf("ratio: %lf\n",ratio);
    for(int i = 0; i < arrIn->len; i++) {
        arrIn->data[i] = (arrIn->data[i] - minV) * ratio + min;
    }

    return 0;
}

