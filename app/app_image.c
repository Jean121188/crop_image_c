#include <math.h>
#include "image.h"
#include "utils.h"

int main(void){
    Image img_sky;

    Image_load(&img_sky, "sky.jpg");
    ON_ERROR_EXIT(img_sky.data == NULL, "Erro em carregar a imagem");

    //void Image_resize(Image *img, int x1, int y1, int dx, int dy);
    // Imagem Original:
    // height ==> 1024px
    // width ===> 768px
    Image_resize(&img_sky, (1024/2), (768/2), 300, 300);
    Image_free(&img_sky);
   
    // // convert image in grayscale
    // Image img_sky_gray;
    // Image_to_gray(&img_sky, &img_sky_gray);

    // // Save Image
    // Image_save(&img_sky_gray, "sky_gray.jpg");

    // // Release image
    // Image_free(&img_sky);
    // Image_free(&img_sky_gray);
}