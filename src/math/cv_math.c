
#include "cv_math.h"
#include "stdio.h"
#include "stdlib.h"

double cv_euclideanDistance_2d_sqr(cv_point_t p1, cv_point_t p2) {
    const double dx = p1.x - p2.x;
    const double dy = p1.y - p2.y;
    return (dx*dx + dy*dy);
}

float cv_moments(cv_array2D_32F_t *arrIn, int j, int i) {
    float mji = 0.0;
    if(arrIn->data == NULL) {
        printf("cv_moments_m00 : Error - There is no input data.\n");
        return mji;
    }
    if(!((arrIn->size.width % 2) || (arrIn->size.height % 2))) {
        printf("cv_moments_m00 : Error - The size of frame input have to be odd number.\n");
        return mji;
    }
    // normalize the coordination
    int xmax = arrIn->size.width / 2;
    int ymax = arrIn->size.height / 2;
    float xstep = 1.0 / (float)xmax;
    float ystep = 1.0 / (float)ymax;
    
    for(int y = 0; y < arrIn->size.height; y++) {
        float py = (float)pow((double)((y - ymax) * ystep), (double)i);
        for(int x = 0; x < arrIn->size.width; x++) {
            float px = (float)pow((double)((x - xmax) * xstep), (double)j);
            mji += arrIn->data[y * arrIn->size.width + x] * px * py;
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

int cv_imageMaskInRange(cv_image32F_t *img, cv_array32F_t lowerb, cv_array32F_t upperb, cv_array2D_8U_t *mask, bool addmore) {

    if(img->data == NULL) {
        printf("cv_imageMaskInRange : Error - There is no input image.\n");
        return 1;
    }

    if(lowerb.len != upperb.len) {
        printf("cv_imageMaskInRange : Error - The size of threshold inputs are not same.\n");
        return 1;
    }

    if(lowerb.len > img->nChannel) {
        printf("cv_imageMaskInRange : Error - The number of threshold input over the number of image channel.\n");
        return 1;
    }
    const int totalPixel = img->size.height * img->size.width;
    if(addmore) {
        if(mask->data == NULL) {
            printf("cv_imageMaskInRange : Error : Don't exist the mask data.\n");
            return 1;
        }
    } else {
        if(mask->data != NULL) {
            free(mask->data);
        }
        mask->size = img->size;
        mask->data = (uint8_t*)calloc(totalPixel,sizeof(uint8_t));
    }

    for(int i = 0; i < totalPixel; i++) {
        uint8_t res = 0xFF;
        for(int j = 0; j < lowerb.len; j++) {
            const int pos = i*img->nChannel + j;
            if((img->data[pos] >= lowerb.data[j]) && (img->data[pos] <= upperb.data[j])) continue;
            else {
                res = 0x00;
                break;
            }
        }
        mask->data[i] |= res;
    }

    return 0;
}

int cv_imageNormalization_minmax(cv_image32F_t *img, const float lower, const float upper, int channel) {
    if(img->data == NULL) {
        printf("cv_imageNormalization_minmax : Error - There is no input image.\n");
        return 1;
    }

    if(channel >= img->nChannel) {
        printf("cv_imageNormalization_minmax : Error - The input channel is not exist. It must be less than %d.\n",img->nChannel);
        return 1;
    }

    if(upper <= lower) {
        printf("cv_imageNormalization_minmax : Error - upper have to be greater than lower value.\n");
        return 1;
    }

    const int totalPixel = img->size.width*img->size.height;
    float min, max;
    min = img->data[channel];
    max = img->data[channel];

    for(int i = 1; i < totalPixel; i++) {
        float val = img->data[i*img->nChannel + channel];
        if(min > val) min = val;
        if(max < val) max = val;
    }

    const float inDelta = max - min;
    const float outDelta = upper - lower;
    const float ratio = outDelta / inDelta;

    for (int i = 0; i < totalPixel; i++) {
        int pos = i*img->nChannel + channel;
        img->data[pos] = (img->data[pos] - min) * ratio + lower;
    }

    return 0;
}

int cv_imageHist(cv_array32F_t* hist, cv_image32F_t *img, cv_array2D_8U_t *mask, const float range[2], const int histSize, int channel) {

    if(img->data == NULL) {
        printf("cv_imageHist : Error - There is no input image.\n");
        return 1;
    }
    if(mask->data == NULL) {
        printf("cv_imageHist : Error - There is no input mask.\n");
        return 1;
    }
    if((mask->size.width != img->size.width) || (mask->size.height != img->size.height)) {
        printf("cv_imageHist : Error - The mask size doesn't match with the input image.\n");
        return 1;
    }
    if(range[0] >= range[1]) {
        printf("cv_imageHist : Error - The input range.\n");
        return 1;
    }
    if(channel >= img->nChannel) {
        printf("cv_imageHist : Error - The channel cannot greater than or equal %d.\n", img->nChannel);
        return 1;
    }

    if(hist->data != NULL) {
        free(hist->data);
        hist->len = 0;
    }

    hist->len = histSize;
    hist->data = (float*)calloc(hist->len,sizeof(float));

    const float step = (range[1] - range[0]) / histSize;
    const int totalPixel = img->size.width * img->size.height;

    for(int i = 0; i < totalPixel; i++) {
        if(mask->data[i]) {
            int pos = (int)(img->data[i*img->nChannel + channel] / step + 0.5);
            if(pos >= histSize) pos = histSize - 1;
            hist->data[pos] ++;
        } else {
            continue;
        }
    }

    return 0;
}

int cv_cvtImage8Uto32F(cv_image8U_t *img8U, cv_image32F_t *img32F) {
    if(img8U->data == NULL) {
        printf("cv_cvtImage8Uto32F : Error - there is no input image.\n");
        return 1;
    }

    if(img32F->data != NULL) {
        free(img32F->data);
    }

    img32F->size = img8U->size;
    img32F->nChannel = img8U->nChannel;

    int imageSize = img32F->size.width*img32F->size.height*img32F->nChannel;
    img32F->data = (float*)malloc(sizeof(float)*imageSize);

    if(img32F->data == NULL) {
        printf("cv_cvtImage8Uto32F : Error - Cannot allocate the memory for output image.\n");
        return 1;
    }

    for(int i = 0; i < imageSize; i++) {
        img32F->data[i] = (float)img8U->data[i];
    }

    return 0;
}

int cv_cvtImage32Fto8U(cv_image32F_t *img32F, cv_image8U_t *img8U) {
    if (img32F->data == NULL) {
        printf("cv_cvtImage32Fto8U : Error - There is no input data.\n");
        return 1;
    }

    if(img8U->data != NULL) {
        free(img8U->data);
    }

    img8U->size = img32F->size;
    img8U->nChannel = img32F->nChannel;
    
    const int totalPixel = img8U->size.height * img8U->size.width * img8U->nChannel;
    img8U->data = (uint8_t*)malloc(sizeof(uint8_t)*totalPixel);
    if(img8U->data == NULL) {
        printf("cv_cvtImage32Fto8U : Error - Cannot allocate the memory for saving the output data.\n");
        return 1;
    }

    for(int i = 0; i < totalPixel; i++) {
        float val = img32F->data[i] + 0.5;
        if(val > 255) val = 255;
        img8U->data[i] = (uint8_t)(val);
    }

    return 0;
}

cv_hsv_t cv_cvtRGB2HSV(cv_rgb_t rgb) {
    cv_hsv_t hsv;

    const float R = rgb.r;
    const float G = rgb.g;
    const float B = rgb.b;

    float cmax = R > G ? R : G;
    cmax = cmax > B ? cmax : B;
    float cmin = R > G ? G : R;
    cmin = cmin > B ? B : cmin;

    float delta = cmax - cmin;

    // Hue calculation
    if(cmax == 0.0) {
        hsv.s = 0.0;
    } else {
        hsv.s = delta / cmax;
    }
    if (hsv.s == 0) {
        hsv.h = 0.0;
    } else {
        hsv.s = delta / cmax;
        if (cmax == R) {
            hsv.h = (G - B) / delta;
        } else if (cmax == G) {
            hsv.h = 2 + (B - R) / delta;
        } else if (cmax == B) {
            hsv.h = 4 + (R - G) / delta;
        }

        hsv.h *= 60.0;

        if (hsv.h < 0) hsv.h += 360.0;
    }

    hsv.v = cmax / 255;

    return hsv;
}

int cv_cvtImageRGBtoHSV(cv_image32F_t *rgbImage, cv_image32F_t *hsvImage) {
    if(rgbImage->nChannel != 3) {
        printf("cv_cvtImageRGB2HSV : Error - the input image have to be rgb image with 3 channels.\n");
        return 1;
    }

    if(rgbImage->data == NULL) {
        printf("cv_cvtImageRGB2HSV : Error - There is no input image.\n");
        return 1;
    }

    if(hsvImage->data != NULL) {
        free(hsvImage->data);
        hsvImage->data = NULL;
    }

    int totalPixel = rgbImage->size.height*rgbImage->size.width;

    hsvImage->size = rgbImage->size;
    hsvImage->nChannel = rgbImage->nChannel;
    hsvImage->data = (float*)malloc(sizeof(float)*totalPixel*hsvImage->nChannel);

    if(hsvImage->data == NULL) {
        printf("cv_cvtImageRGB2HSV : Error - Cannot allocate the memory.\n");
        return 1;
    }

    cv_rgb_t rgb;
    cv_hsv_t hsv;

    for(int i = 0; i < totalPixel; i++) {
        rgb.r = rgbImage->data[i*rgbImage->nChannel + 0];
        rgb.g = rgbImage->data[i*rgbImage->nChannel + 1];
        rgb.b = rgbImage->data[i*rgbImage->nChannel + 2];

        hsv = cv_cvtRGB2HSV(rgb);

        hsvImage->data[i*hsvImage->nChannel + 0] = hsv.h;
        hsvImage->data[i*hsvImage->nChannel + 1] = hsv.s;
        hsvImage->data[i*hsvImage->nChannel + 2] = hsv.v;
    }

    return 0;
}

int cv_createProbabilityImage(cv_image32F_t *hsvImage, cv_image32F_t *probImage, cv_array2D_8U_t *mask, cv_array32F_t *hist, const float step, const int channel) {
    if(hsvImage->data == NULL) {
        printf("cv_createProbabilityImage : Error - There is no input image.\n");
        return 1;
    }
    if(hist->data == NULL) {
        printf("cv_createProbabilityImage : Error - There is no input histogram.\n");
        return 1;
    }
    if(mask->data == NULL) {
        printf("cv_createProbabilityImage : Error - There is no input mask.\n");
        return 1;
    }
    if((mask->size.width != hsvImage->size.width) || (mask->size.height != hsvImage->size.height)) {
        printf("cv_createProbabilityImage : Error - The mask size doesn't match with the input image.\n");
        return 1;
    }
    if(probImage->data != NULL) {
        free(probImage->data);
        probImage->data = NULL;
    }
    probImage->size = hsvImage->size;
    probImage->nChannel = 1;
    probImage->data = (float*)malloc(sizeof(float)*probImage->size.width*probImage->size.height);

    const int totalPixel = probImage->size.width * probImage->size.height;
    for(int i = 0; i < totalPixel; i++) {
        if(mask->data[i]) {
            int pos = (int)(hsvImage->data[i*hsvImage->nChannel + channel] / step + 0.5);
            if(pos < 0) pos = 0;
            if(pos >= hist->len) pos = hist->len;
            probImage->data[i] = hist->data[pos];
        } else {
            probImage->data[i] = 0;
        }
    }

    return 0;
}

int cv_getImageArea_32F(cv_image32F_t *img, cv_image32F_t *imgArea, cv_window_2d_t *window) {
    if(img->data == NULL) {
        printf("cv_getImageArea_32F : Error - There is no input image.\n");
        return 1;
    }
    if(((window->size.width / 2) >= window->pos.x) || ((window->size.height / 2) >= window->pos.y)) {
        printf("cv_getImageArea_32F : Error - Cannot get the window because of the window size.\n");
        return 1;
    }
    if(imgArea->data != NULL) {
        free(imgArea->data);
    }

    if(!(window->size.height % 2)) window->size.height ++;
    if(!(window->size.width % 2)) window->size.width ++;
    
    imgArea->size = window->size;
    imgArea->nChannel = img->nChannel;
    imgArea->data = (float*)malloc(sizeof(float)*imgArea->size.height*imgArea->size.width*imgArea->nChannel);

    const int startx = window->pos.x - (window->size.width / 2);
    const int starty = window->pos.y - (window->size.height / 2);

    for(int x = 0; x < imgArea->size.width; x++) {
        for(int y = 0; y < imgArea->size.height; y++) {
            int orgPos = (y + starty) * img->size.width * img->nChannel + (x + startx) * img->nChannel;
            int wPos = y * imgArea->size.width * img->nChannel + x * img->nChannel;
            for(int i = 0; i < imgArea->nChannel; i++) {
                imgArea->data[wPos + i] = img->data[orgPos + i];
            }
        }
    }
    return 0;
}