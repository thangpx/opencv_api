#include "cv_math_color.h"

#include "math.h"
#include "stdio.h"
#include "stdlib.h"

cv_hsv_t cv_cvtRGB2HSV(cv_rgb_t rgb) {
    /*
        0 <= h <= 360
        0 <= s <= 1
        0 <= v <= 1
    */
    cv_hsv_t hsv;

    const double R = rgb.r;
    const double G = rgb.g;
    const double B = rgb.b;

    double cmax = R > G ? R : G;
    cmax = cmax > B ? cmax : B;
    double cmin = R > G ? G : R;
    cmin = cmin > B ? B : cmin;

    double delta = cmax - cmin;

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

cv_lab_t cv_cvtRGB2LAB(cv_rgb_t rgb) {
    cv_lab_t lab;
    double R = rgb.r / 255.0;
    double G = rgb.g / 255.0;
    double B = rgb.b / 255.0;

    double x,y,z;

    // convert to sRGB
    R = (R > 0.04045) ? pow((R + 0.055) / 1.055, 2.4) : (R / 12.92);
    G = (G > 0.04045) ? pow((G + 0.055) / 1.055, 2.4) : (G / 12.92);
    B = (B > 0.04045) ? pow((B + 0.055) / 1.055, 2.4) : (B / 12.92);
    // convert to CIE-xyz
    x = (R * 0.4124564 + G * 0.3575761 + B * 0.1804375);
    y = (R * 0.2126729 + G * 0.7151522 + B * 0.0721750);
    z = (R * 0.0193339 + G * 0.1191920 + B * 0.9503041);
    // convert to CIE-Lab
    x = (x > 0.008856) ? pow(x,0.333333) : ((7.787 * x) + 0.137931);
    y = (y > 0.008856) ? pow(y,0.333333) : ((7.787 * y) + 0.137931);
    z = (z > 0.008856) ? pow(z,0.333333) : ((7.787 * z) + 0.137931);
    
    lab.l = 116.0 * y - 16.0;
    lab.a = 500.0 * (x - y);
    lab.b = 200.0 * (y - z);

    return lab;
}

cv_xyz_t cv_cvtRGB2XYZ(cv_rgb_t rgb) {
    cv_xyz_t xyz;
    double R = rgb.r / 255.0;
    double G = rgb.g / 255.0;
    double B = rgb.b / 255.0;

    R = (R > 0.04045) ? pow((R + 0.055) / 1.055, 2.4) : (R / 12.92);
    G = (G > 0.04045) ? pow((G + 0.055) / 1.055, 2.4) : (G / 12.92);
    B = (B > 0.04045) ? pow((B + 0.055) / 1.055, 2.4) : (B / 12.92);

    xyz.x = (R * 0.4124564 + G * 0.3575761 + B * 0.1804375);
    xyz.y = (R * 0.2126729 + G * 0.7151522 + B * 0.0721750);
    xyz.z = (R * 0.0193339 + G * 0.1191920 + B * 0.9503041);

    return xyz;
}

cv_rgb_t cv_cvtHSV2RGB(cv_hsv_t hsv) {
    /*
        Inputs:
            - 0 <= H <= 360
            - 0 <= S,V <= 1
    */
    cv_rgb_t rgb;

    const double C = hsv.v * hsv.s;
    const double X = C*(1.0 - fabs(fmod(hsv.h/60.0,2) - 1.0));
    const double m = hsv.v - C;

    const int p = (int)(hsv.h) / 60;
    switch (p)
    {
    case 0: // 0 - 60
        rgb.r = C;
        rgb.g = X;
        rgb.b = 0;
        break;
    case 1: // 60 - 120
        rgb.r = X;
        rgb.g = C;
        rgb.b = 0;
        break;
    case 2: // 120 - 180
        rgb.r = 0;
        rgb.g = C;
        rgb.b = X;
        break;
    case 3: // 180 - 240
        rgb.r = 0;
        rgb.g = X;
        rgb.b = C;
        break;
    case 4: // 240 - 300
        rgb.r = X;
        rgb.g = 0;
        rgb.b = C;
        break;
    case 5: // 300 - 360
        rgb.r = C;
        rgb.g = 0;
        rgb.b = X;
        break;
    default:
        break;
    }

    rgb.r = ((double)rgb.r + m) * 255.0;
    rgb.g = ((double)rgb.g + m) * 255.0;
    rgb.b = ((double)rgb.b + m) * 255.0;

    return rgb;
}

cv_rgb_t cv_cvtLAB2RGB(cv_lab_t lab) {
    cv_rgb_t rgb;
    double L = lab.l;
    double A = lab.a;
    double B = lab.b;
    // convert to CIE-xyz
    double x,y,z;
    y = (L + 16.0)/116.0;
    x = y + A/500.0;
    z = y - B/200.0;

    x = (x > 0.206897) ? pow(x,3) : 0.12842*(x - 0.137931);
    y = (y > 0.206897) ? pow(y,3) : (0.12842*(y - 0.137931));
    z = (z > 0.206897) ? pow(z,3) : (0.12842*(z - 0.137931));

    // CIE-xyz to sRGB
    double r = 3.2404542*x - 1.5371385*y - 0.4985314*z;
    double g = -0.9692660*x + 1.8760108*y + 0.0415560*z;
    double b = 0.0556434*x - 0.2040259*y + 1.0572252*z;

    // sRGB to RGB
    rgb.r = ((r > 0.0031308) ? (1.055*pow(r,0.4166667) - 0.055) : (r * 12.92)) * 255.0;
    rgb.g = ((g > 0.0031308) ? (1.055*pow(g,0.4166667) - 0.055) : (g * 12.92)) * 255.0;
    rgb.b = ((b > 0.0031308) ? (1.055*pow(b,0.4166667) - 0.055) : (b * 12.92)) * 255.0;

    return rgb;
}

cv_rgb_t cv_cvtXYZ2RGB(cv_xyz_t xyz) {
    cv_rgb_t rgb;
    double x = xyz.x;
    double y = xyz.y;
    double z = xyz.z;

    // CIE-xyz to sRGB
    double r = 3.2404542*x - 1.5371385*y - 0.4985314*z;
    double g = -0.9692660*x + 1.8760108*y + 0.0415560*z;
    double b = 0.0556434*x - 0.2040259*y + 1.0572252*z;

    // sRGB to RGB
    rgb.r = ((r > 0.0031308) ? (1.055*pow(r,0.4166667) - 0.055) : (r * 12.92)) * 255.0;
    rgb.g = ((g > 0.0031308) ? (1.055*pow(g,0.4166667) - 0.055) : (g * 12.92)) * 255.0;
    rgb.b = ((b > 0.0031308) ? (1.055*pow(b,0.4166667) - 0.055) : (b * 12.92)) * 255.0;

    return rgb;
}

float cv_cvtRGB2GREY(cv_rgb_t rgb) {
    float grey;
    grey = 0.2126*rgb.r + 0.7152*rgb.g + 0.0722*rgb.b;
    return grey;
}

int cv_cvtImageRGBto(cv_imageFormat_t type, cv_image32F_t *rgbImage, cv_image32F_t *imgOut) {
    if(rgbImage->nChannel != 3) {
        printf("cv_cvtImageRGBto : Error - the input image have to be rgb image with 3 channels.\n");
        return 1;
    }

    if(rgbImage->data == NULL) {
        printf("cv_cvtImageRGBto : Error - There is no input image.\n");
        return 1;
    }

    if(imgOut->data != NULL) {
        if(!((imgOut->size.width == rgbImage->size.width) && (imgOut->size.height == rgbImage->size.height))) {
            switch (type)
            {
                case CV_GREY: {
                    if(imgOut->nChannel != 1) {
                        free(imgOut->data);
                        imgOut->data = NULL;
                    }
                    break;
                }
                
                default: {
                    if(imgOut->nChannel != rgbImage->nChannel) {
                        free(imgOut->data);
                        imgOut->data = NULL;
                    }
                    break;
                }
            }
        } else {
            free(imgOut->data);
            imgOut->data = NULL;
        }
    }

    int totalPixel = rgbImage->size.height*rgbImage->size.width;

    imgOut->size = rgbImage->size;
    imgOut->nChannel = (type == CV_GREY) ? 1 : rgbImage->nChannel;
    if(imgOut->data == NULL) imgOut->data = (float*)malloc(sizeof(float)*totalPixel*imgOut->nChannel);

    if(imgOut->data == NULL) {
        printf("cv_cvtImageRGBto : Error - Cannot allocate the memory.\n");
        return 1;
    }

    for(int i = 0; i < totalPixel; i++) {
        cv_rgb_t rgb;
        rgb.r = rgbImage->data[i*rgbImage->nChannel + 0];
        rgb.g = rgbImage->data[i*rgbImage->nChannel + 1];
        rgb.b = rgbImage->data[i*rgbImage->nChannel + 2];

        switch (type)
        {
            case CV_GREY: {
                float grey = cv_cvtRGB2GREY(rgb);
                imgOut->data[i*imgOut->nChannel + 0] = grey;
                break;
            }
            case CV_HSV: {
                cv_hsv_t hsv = cv_cvtRGB2HSV(rgb);
                imgOut->data[i*imgOut->nChannel + 0] = hsv.h;
                imgOut->data[i*imgOut->nChannel + 1] = hsv.s;
                imgOut->data[i*imgOut->nChannel + 2] = hsv.v;
                break;
            }
            case CV_LAB: {
                cv_lab_t lab = cv_cvtRGB2LAB(rgb);
                imgOut->data[i*imgOut->nChannel + 0] = lab.l;
                imgOut->data[i*imgOut->nChannel + 1] = lab.a;
                imgOut->data[i*imgOut->nChannel + 2] = lab.b;
                break;
            }
            case CV_XYZ: {
                cv_xyz_t xyz = cv_cvtRGB2XYZ(rgb);
                imgOut->data[i*imgOut->nChannel + 0] = xyz.x;
                imgOut->data[i*imgOut->nChannel + 1] = xyz.y;
                imgOut->data[i*imgOut->nChannel + 2] = xyz.z;
                break;
            }
            default: {
                break;
            }
        }
    }

    return 0;
}