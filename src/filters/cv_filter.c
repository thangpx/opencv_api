
#include "cv_filter.h"

float cv_filter_convComp_32F(cv_array2D_32F_t kernel, cv_array2D_32F_t arr2D, cv_point_t pos, bool padding) {
    float conv = 0.0;
    if(kernel.data == NULL || arr2D.data == NULL) {
        printf("cv_filter_convComp_32F : Error - There is no data input.\n");
        return conv;
    }
    if(!((kernel.size.height % 2) || (kernel.size.width % 2))) {
        printf("cv_filter_convComp_32F : Error - The size of kernel must be odd number.\n");
        return conv;
    }
    const int krx = kernel.size.width / 2;
    const int kry = kernel.size.height / 2;
    if(!padding) {
        if(pos.x < krx || pos.y < kry || pos.x >= (arr2D.size.width - krx) || pos.y >= (arr2D.size.height - kry)) {
            printf("cv_filter_convComp_32F : Error - Cannot implement convolution at this point.\n");
            return conv;
        }
    }
    
    for(int y = -kry; y <= kry; y++) {
        int py = (y + pos.y);
        if(py < 0 || py >= arr2D.size.height) continue;
        py *= arr2D.size.width;
        const int ky = (y + kry) * kernel.size.width;
        for(int x = -krx; x <= krx; x++) {
            const int px = x + pos.x;
            const int kx = x + krx;
            if(px < 0 || px >= arr2D.size.width) continue;
            conv += arr2D.data[py + px] * kernel.data[ky + kx];
        }
    }
    return conv;
}