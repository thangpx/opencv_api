#include "stdio.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "OpenCVShow.h"

#include "cv_math.h"

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

    uint8_t* img = stbi_load("test_images/portrait_01.jpg",&size.width,&size.height,&nChannel,3);

    printf("(w,h,c) = (%d,%d,%d)\n", size.width, size.height, nChannel);

    cv_image8U_t img8U;
    img8U.size = size;
    img8U.nChannel = nChannel;
    img8U.data = img;

    cv_image32F_t img32F;
    img32F.data = NULL;
    cv_cvtImage8Uto32F(&img8U, &img32F);

    cv_image32F_t imgHSV_32F;
    imgHSV_32F.data = NULL;
    cv_cvtImageRGBtoHSV(&img32F, &imgHSV_32F);

    cv_array2D_8U_t mask;
    mask.data = NULL;

    float lRange[3] = {230,0.5,0.5};
    float uRange[3] = {250,1,1};

    cv_array32F_t lowerb, upperb;
    lowerb.len = 3;
    upperb.len = 3;
    lowerb.data = lRange;
    upperb.data = uRange;

    cv_imageMaskInRange(&imgHSV_32F,lowerb,upperb, &mask, false);
    // lRange[0] = 350;
    // uRange[0] = 360;
    // cv_imageMaskInRange(&imgHSV_32F,lowerb,upperb, &mask, true);

    cv_array32F_t hist;
    hist.data = NULL;
    const float range[2] = {0, 360};
    cv_imageHist(&hist, &imgHSV_32F, &mask, range, 360, 0);

    cv_normalize_minmax(&hist,0,1.0);

    cv_image32F_t probImage;
    probImage.data = NULL;

    cv_createProbabilityImage(&imgHSV_32F, &probImage, &mask, &hist, 1, 0);

    cv_array2D_32F_t probArr;
    probArr.size = probImage.size;
    probArr.data = probImage.data;
    float m00 = cv_moments(&probArr,0,0);
    float m10 = cv_moments(&probArr,1,0);
    float m01 = cv_moments(&probArr,0,1);
    float m11 = cv_moments(&probArr,1,1);
    float m20 = cv_moments(&probArr,2,0);
    float m02 = cv_moments(&probArr,0,2);
    float xc = m10/m00;
    float yc = m01/m00;
    float a = (m20/m00) - xc*xc;
    float b = 2*(m11/m00 - xc*yc);
    float c = m02/m00 - yc*yc;
    float l = sqrt((a + c + sqrt(b*b + (a-c)*(a-c)))/2);
    float w = sqrt((a + c - sqrt(b*b + (a-c)*(a-c)))/2);
    printf("(x,y) = (%lf,%lf)\n",xc,yc);
    printf("(l,w) = (%lf,%lf)\n",l,w);

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
    waitKey(0);
    free(img32F.data);
    free(imgHSV_32F.data);
    free(hist.data);
    return 0;
}
