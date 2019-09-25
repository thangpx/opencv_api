#include "stdio.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "OpenCVShow.h"

#include "cv_math.h"
#include "cv_math_img.h"
#include "cv_meanshift.h"
#include "cv_filter.h"

void openCVShowRGBImage(COpenCVShow openCVTEST, cv_image8U_t* image, string windowName) {
    uint8_t *imgCV;
    imgCV = (uint8_t*)malloc(sizeof(uint8_t) * image->size.width * image->size.height * image->nChannel);
    // convert to openCV style
    for(int x = 0; x < image->size.width; x++) {
        for(int y = 0; y < image->size.height; y++) {
            imgCV[y*image->size.width*3 + 3*x + 0] = image->data[y*image->size.width*3 + 3*x + 2];
            imgCV[y*image->size.width*3 + 3*x + 1] = image->data[y*image->size.width*3 + 3*x + 1];
            imgCV[y*image->size.width*3 + 3*x + 2] = image->data[y*image->size.width*3 + 3*x + 0];
        }
    }
    openCVTEST.showImage(imgCV,image->size.width,image->size.height,1,windowName,1,CV_8UC3);

    free(imgCV);
}

void openCVShowGrayImage(COpenCVShow openCVTEST, cv_image8U_t* image, string windowName) {
    openCVTEST.showImage(image->data,image->size.width,image->size.height,1,windowName,1,CV_8UC1);
}

int main(int argc, char const *argv[])
{

    COpenCVShow openCVTEST;
    cv_size_int_t size;
    int nChannel;

    uint8_t* img = stbi_load("test_images/girl_01.jpg",&size.width,&size.height,&nChannel,3);

    printf("(w,h,c) = (%d,%d,%d)\n", size.width, size.height, nChannel);

    cv_image8U_t img8U;
    img8U.size = size;
    img8U.nChannel = nChannel;
    img8U.data = img;

    cv_image32F_t img32F;
    img32F.data = NULL;

    float sample[40] = {1,  2,  3,  4,  5,
                        6,  7,  8,  9,  10,
                        11, 12, 13, 14, 15,
                        16, 17, 18, 19, 20,
                        21, 22, 23, 24, 25,
                        26, 27, 28, 29, 30,
                        31, 32, 33, 34, 35,
                        36, 37, 38, 39, 40};
    cv_size_int_t sizeSample = {.width = 5, .height = 8};
    img32F.size = sizeSample;
    img32F.nChannel = 1;
    img32F.data = sample;

    cv_image32F_t integralImage;
    integralImage.data = NULL;
    cv_imgIntegral_create(img32F,&integralImage,0,true);

    for(int y = 0; y < 8; y++) {
        for(int x = 0; x < 5; x++) {
            printf("%lf ",integralImage.data[y*5 + x]);
        }
        printf("\n");
    }

    double recSum = cv_rectangleSum(integralImage,2,1,2,2,true);
    printf("recSum = %lf\n",recSum);

 /*   cv_cvtImage8Uto32F(&img8U, &img32F);

    // for meanShift filter testing
    cv_image32F_t filed_img;
    filed_img.data = NULL;
    cv_meanShift_filter(&img32F, &img32F, 5,30,100,20,5);

    printf("finish...\n");

    cv_image8U_t filImg_8U;
    filImg_8U.data = NULL;
    cv_cvtImage32Fto8U(&img32F,&filImg_8U);
    
    openCVShowRGBImage(openCVTEST,&filImg_8U,"filed Image");

    // free data
    FREE_IMAGE_T(filed_img);
    FREE_IMAGE_T(filImg_8U);
    FREE_IMAGE_T(img8U);
    FREE_IMAGE_T(img32F); */

/*  // For mean-shift testing
    cv_image32F_t imgHSV_32F;
    imgHSV_32F.data = NULL;
    cv_cvtImageRGBtoHSV(&img32F, &imgHSV_32F);

    cv_array2D_8U_t mask;
    mask.data = NULL;

    float lRange[3] = {0,0.5,0.5};
    float uRange[3] = {10,1,1};

    cv_array32F_t lowerb, upperb;
    lowerb.len = 3;
    upperb.len = 3;
    lowerb.data = lRange;
    upperb.data = uRange;

    cv_imageMaskInRange(&imgHSV_32F,lowerb,upperb, &mask, false);
    lRange[0] = 350;
    uRange[0] = 360;
    cv_imageMaskInRange(&imgHSV_32F,lowerb,upperb, &mask, true);

    cv_array32F_t hist;
    hist.data = NULL;
    const float range[2] = {0, 360};
    cv_imageHist(&hist, &imgHSV_32F, &mask, range, 360, 0);

    cv_normalize_minmax(&hist,0,1.0);

    cv_image32F_t probImage;
    probImage.data = NULL;

    cv_createProbabilityImage(&imgHSV_32F, &probImage, &mask, &hist, 1, 0);

    cv_array2D_32F_t probArr;
    cv_window_2d_t window = {
        .pos = {
            .x = 65,
            .y = 73
        },
        .size = {
            .width = 91,
            .height = 91
        }
    };
    probArr.size = probImage.size;
    probArr.data = probImage.data;

    float bw = (float)(window.size.width / 2);

    float m00 = cv_moments_window(probArr,window,0,0,bw);
    float m10 = cv_moments_window(probArr,window,1,0,bw);
    float m01 = cv_moments_window(probArr,window,0,1,bw);
    float m11 = cv_moments_window(probArr,window,1,1,bw);
    float m20 = cv_moments_window(probArr,window,2,0,bw);
    float m02 = cv_moments_window(probArr,window,0,2,bw);

    float xc = m10/m00;
    float yc = m01/m00;
    float a = (m20/m00) - xc*xc;
    float b = 2*((m11/m00) - xc*yc);
    float c = m02/m00 - yc*yc;
    float l = sqrt((a + c + sqrt(b*b + (a-c)*(a-c)))/2);
    float w = sqrt((a + c - sqrt(b*b + (a-c)*(a-c)))/2);
    float orient = 0.5*atan2f(b,a-c);
    printf("(x,y) = (%lf,%lf)\n",xc,yc);
    printf("(l,w) = (%lf,%lf)\n",l,w);
    printf("orient : %lf\n",orient);

    // check meanshift
    cv_array2D_32F_t kernel;
    kernel.data = NULL;
    float totalKernel = cv_createKernel_Epanechnikov(&kernel, 91, 91, 0.5);
    cv_point_t pos = {
        .x = 70,
        .y = 90
    };
    cv_point32F_t mx;
    
    cv_meanShift_trace(kernel,probArr, &pos, 0.1, 50);
    printf("m00 = %lf\nNew Pos : (x,y) = (%d,%d).\n",m00,pos.x,pos.y);
    // cv_meanShift(kernel,probArr,pos,&mx);

    // printf("totalKernel : %lf \nmeanshift = (%lf,%lf)\n", totalKernel, mx.x,mx.y);


    cv_image8U_t probImage8U;
    probImage8U.data = NULL;
    // probImage8U.size = probImage.size;
    // probImage8U.nChannel = 1;
    // probImage8U.data = mask.data;
    cv_imageNormalization_minmax(&probImage,0,255,0);
    cv_cvtImage32Fto8U(&probImage,&probImage8U);
    // cv_image32F_t wImg;
    // wImg.data = NULL;

    // cv_window_2d_t W = {.pos = {.x = 100,.y = 100},.size = {.width = 100,.height = 100}};

    // cv_getImageArea_32F(&img32F, &wImg, &W);
    // cv_image8U_t wImg8U;
    // cv_cvtImage32Fto8U(&wImg, &wImg8U);

    openCVShowGrayImage(openCVTEST,&probImage8U,"probability image");
    openCVShowRGBImage(openCVTEST,&img8U,"original Image");
    // openCVShowRGBImage(openCVTEST,&wImg8U,"window sample Image");
    FREE_IMAGE_T(img32F);
    FREE_IMAGE_T(imgHSV_32F);
    free(hist.data);
    */
    // waitKey(0);
    return 0;
}
