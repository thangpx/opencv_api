#include "stdio.h"
#include "stdlib.h"

#include "cv_meanshift.h"
#include "cv_math.h"

float cv_createKernel_Epanechnikov(cv_array2D_32F_t *kernel, int w, int h, float c){
    float total = 0.0;
    if(!((w % 2) || (h % 2))) {
        printf("cv_createKernel_Epanechnikov : Error - The size of window have to be odd number.\n");
        return total;
    }
    if(kernel->data != NULL) {
        free(kernel->data);
        kernel->data = NULL;
    }
    kernel->size.width = w;
    kernel->size.height = h;
    kernel->data = (float*)calloc(w*h,sizeof(float));

    int xmax = w / 2;
    int ymax = h / 2;
    float xstep = 1.0 / (float)xmax;
    float ystep = 1.0 / (float)ymax;

    cv_point32F_t pos;
    for(int x = 0; x < w; x++) {
        pos.x = (float)(x - xmax) * xstep;
        for(int y = 0; y < h; y++) {
            pos.y = (float)(y - ymax) * ystep;
            float dsqr = pos.x*pos.x + pos.y*pos.y;
            float d = (float)sqrt((double)(dsqr));
            if(d <= 1.0) {
                kernel->data[y*w + x] = (1.0 - dsqr) * c;
            } else
            {
                kernel->data[y*w + x] = 0.0;
            }
            total += kernel->data[y*w + x];
        }
    }
    return total;
}

float cv_createKernel_Gaussian(cv_array2D_32F_t *kernel, int w, int h, float c) {
    float total = 0.0;
    if(!((w % 2) || (h % 2))) {
        printf("cv_createKernel_Epanechnikov : Error - The size of window have to be odd number.\n");
        return total;
    }
    if(kernel->data != NULL) {
        free(kernel->data);
        kernel->data = NULL;
    }
    kernel->size.width = w;
    kernel->size.height = h;
    kernel->data = (float*)calloc(w*h,sizeof(float));

    int xmax = w / 2;
    int ymax = h / 2;
    float xstep = 1.0 / (float)xmax;
    float ystep = 1.0 / (float)ymax;

    cv_point32F_t pos;
    for(int x = 0; x < w; x++) {
        pos.x = (float)(x - xmax) * xstep;
        for(int y = 0; y < h; y++) {
            pos.y = (float)(y - ymax) * ystep;
            float dsqr = pos.x*pos.x + pos.y*pos.y;
            kernel->data[y*w + x] = (float)exp((double)(-dsqr)) * c;
            total += kernel->data[y*w + x];
        }
    }
    return total;
}

int cv_meanShift(cv_array2D_32F_t kernel, cv_array2D_32F_t arr2D, cv_point_t point, cv_point32F_t *mx) {
    if(kernel.data == NULL || arr2D.data == NULL) {
        printf("cv_meanShift : Error - There is no data input.\n");
        return 1;
    }
    if(!((kernel.size.height % 2) || (kernel.size.width % 2))) {
        printf("cv_meanShift : Error - The size of kernel must be odd number.\n");
        return 1;
    }
    const int krx = kernel.size.width / 2;
    const int kry = kernel.size.height / 2;
    if(point.x < krx || point.y < kry || point.x >= (arr2D.size.width - krx) || point.y >= (arr2D.size.height - kry)) {
        printf("cv_meanShift : Error - Cannot implement convolution at this point.\n");
        return 1;
    }
    const float xStep = 1.0 / (float)krx;
    const float yStep = 1.0 / (float)kry;
    float totalKernel = 0.0;
    mx->x = 0.0;
    mx->y = 0.0;
    for(int y = -kry; y <= kry; y++) {
        int py = (y + point.y) * arr2D.size.width;
        int pky = (y + kry)*kernel.size.width;
        for(int x = -krx; x <= krx; x++) {
            int px = x + point.x;
            int pkx = (x + krx);
            float pv = arr2D.data[py + px] * kernel.data[pky + pkx];
            totalKernel += kernel.data[pky + pkx];
            mx->x += pv * (float)x;
            mx->y += pv * (float)y;
        }
    }

    mx->x /= totalKernel;
    mx->y /= totalKernel;
    return 0;
}

int cv_meanShift_trace(cv_array2D_32F_t kernel, cv_array2D_32F_t arr2D, cv_point_t *point, float EPSILON, int maxIte) {
    cv_point32F_t mx;
    float EPSILON_sqr = EPSILON*EPSILON;
    int cnt = 0;
    while ( cnt <= maxIte)
    {
        if(cv_meanShift(kernel,arr2D,*point,&mx)) {
            return 1;
        }
        float distance_sqr = mx.x*mx.x + mx.y*mx.y;
        if(distance_sqr <= EPSILON_sqr) {
            break;
        }
        point->x += (int)(mx.x > 0 ? ceil(mx.x) : -ceil(-mx.x));
        point->y += (int)(mx.y > 0 ? ceil(mx.y) : -ceil(-mx.y));
        cnt++;
    }
    return 0;
}

int cv_meanShift_filter(cv_image32F_t *image, cv_image32F_t *img_out, float hs, float hr, float EPS_color, float EPS_spatial, int maxIte) {

    if(image->data == NULL) {
        printf("cv_meanShift_filter : Error - There is no input image.\n");
        return 1;
    }
    if(img_out->data == NULL) {
        img_out->size = image->size;
        img_out->nChannel = image->nChannel;
        img_out->data = (float*)malloc(sizeof(float)*img_out->size.width*img_out->size.height*img_out->nChannel);
    } else
    {
        if(!((img_out->size.height == image->size.height) && (img_out->size.width == image->size.width) && (img_out->nChannel == image->nChannel))) {
            img_out->size = image->size;
            img_out->nChannel = image->nChannel;
            free(img_out->data);
            img_out->data = NULL;
            img_out->data = (float*)malloc(sizeof(float)*img_out->size.width*img_out->size.height*img_out->nChannel);
        }
    }
    
    const int W = image->size.width;
    const int H = image->size.height;
    const int C = image->nChannel;
    const float EPS_sqr = hr*hr;
    const float EPS_color_sqr = EPS_color*EPS_color;
    const float EPS_spatial_sqr = EPS_spatial*EPS_spatial;

    float *currPt = (float*)calloc(C + 2,sizeof(float));
    float *prevPt = (float*)calloc(C + 2,sizeof(float));
    float *sumPt = (float*)calloc(C + 2,sizeof(float));
    
    for(int y = 0; y < H; y++) {
        for(int x = 0; x < W; x++) {

            currPt[0] = x;
            currPt[1] = y;
            for(int i = 0; i < C; i++) {
                currPt[i + 2] = image->data[y * W * C + x*C + i];
            }

            const int top = (y < hs) ? 0 : y - hs;
            const int bottom = (y < (H - hs)) ? (y + hs) : (H - 1);
            const int left = (x < hs) ? 0 : x - hs;
            const int right = (x < (W - hs)) ? (x + hs) : (W - 1);

            float distance_s, distance_c;
            int cnt = 0;
            do {
                // copy point
                for(int i = 0; i < (C+2); i++) {
                    prevPt[i] = currPt[i];
                    sumPt[i] = 0;
                }
                int numPt = 0;
                for(int wy = top; wy < bottom; wy++) {
                    const int py = wy * W * C;
                    for(int wx = left; wx < right; wx++) {
                        float distance = 0;
                        for(int i = 0; i < C; i++) {
                            distance += (image->data[py + wx*C + i] - currPt[i + 2])*(image->data[py + wx*C + i] - currPt[i + 2]);
                        }
                        if(distance <= EPS_sqr) {
                            sumPt[0] += wx;
                            sumPt[1] += wy;
                            for(int i = 0; i < C; i++) {
                                sumPt[i + 2] += image->data[py + wx*C + i];
                            }
                            numPt ++;
                        }
                    }
                }
                distance_s = 0;
                distance_c = 0;
                for(int i = 0; i < C + 2; i++) {
                    sumPt[i] /= numPt;
                    currPt[i] = sumPt[i];
                    if(i < 2) {
                        distance_s += (currPt[i] - prevPt[i])*(currPt[i] - prevPt[i]);
                    } else {
                        distance_c += (currPt[i] - prevPt[i])*(currPt[i] - prevPt[i]);
                    }
                }
                cnt++;
            } while((cnt <= maxIte) && (distance_s > EPS_spatial_sqr) && (distance_c > EPS_color_sqr));
            
            for(int i = 0; i < C; i++) {
                img_out->data[y * W * C + x*C + i] = currPt[i + 2];
            }
        }
    }

    free(currPt);
    free(prevPt);
    free(sumPt);
    
    return 0;
}