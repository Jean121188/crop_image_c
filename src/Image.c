#include <math.h>
#include "image.h"
#include "utils.h"

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_RESIZE_IMPLEMENTATION

#include "stb_image_resize.h"
#include "stb_image_write.h"
#include "stb_image.h"

// filtros
// #define STBIR_DEFAULT_FILTER_UPSAMPLE
// #define STBIR_DEFAULT_FILTER_DOWNSAMPLE

void Image_load(Image *img, const char* fname){
    if((img->data = stbi_load(fname, &img->width, &img->height, &img->channels, 0)) != NULL){
        img->size = img->width * img->height * img->channels;
        img->allocation = STB_ALLOCATED;
    }
}

void Image_creator(Image *img, int width, int height, int channels, bool zerod){
    size_t size = width * height * channels;
    if(zerod){
        img->data = calloc(size, 1);
    }else{
        img->data = malloc(size);
    }

    if(img->data !=NULL){
        img->width      = width;
        img->height     = height;
        img->size       = size;
        img->channels   = channels;
        img->allocation = SELF_ALLOCATED;
    }
}

void Image_save(const Image *img, const char* fname){
    if(str_ends_in(fname, ".jpg") || str_ends_in(fname, ".JPG") || str_ends_in(fname, ".jpeg") || str_ends_in(fname, ".JPEG")){
        stbi_write_jpg(fname, img->width, img->height, img->channels, img->data, 100);         
    }else if(str_ends_in(fname, ".png") || str_ends_in(fname, ".PNG")){
        stbi_write_png(fname, img->width, img->height, img->channels, img->data, img->width * img->channels);
    }else{
        ON_ERROR_EXIT(false, "");
    }
}

void Image_free(Image *img){
    if(img->allocation != NO_ALLOCATION && img->data != NULL){
        if(img->allocation == STB_ALLOCATED){
            stbi_image_free(img->data);
        }else{
            free(img->data);
        }
        img->data       = NULL;
        img->width      = 0;
        img->height     = 0;
        img->size       = 0;
        img->allocation = NO_ALLOCATION;
    }
}

void Image_to_gray(const Image *orig, Image *gray){
    ON_ERROR_EXIT(!(orig->allocation != NO_ALLOCATION && orig->channels >= 3), "A imagem de entrada deve ter ao menos 3 canais");
    int channels = orig->channels == 4 ? 2 : 1;
    Image_creator(gray, orig->width, orig->height, channels, false);
    ON_ERROR_EXIT(gray->data == NULL, "Erro em criar a imagem em grayscale");

    for(unsigned char *p = orig->data, *pg = gray->data; p != orig->data + orig->size; p += orig->channels, pg += gray->channels){
        *pg = (uint8_t)((*p + *(p + 1) + *(p + 2))/3.0);
        if(orig->channels == 4){
            *(pg + 1) = *(p + 3);
        }
    }
}

Image Image_resize(Image *img, uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch){
    img->size = cw * ch * img->channels;
    uint8_t cropped[img->size];
    memset(cropped, 0, img->size);

    for(uint16_t y = 0; y < ch; ++y){       // created y
        if(y + cy >= img->height){break;}
         for(uint16_t x = 0; x < cw; ++x){  // created x
            if(x + cx >= img->width){break;}
            // void *memcpy(void *dest, const void * src, size_t n)
            memcpy(&cropped[(x + y * cw) * img->channels], &img->data[(x + cx + (y + cy) * img->width) * img->channels], img->channels);
         }
    }
    /*
    typedef struct{
        int width;
        int height;
        int channels;
        size_t size;
        uint8_t *data;
        enum alocation_type allocation;
    }Image;
    */
    img->width  = cw;
    img->height = ch;
    memset(&img->data, 0, img->size);
    img->data   = cropped;
    memset(cropped, 0, 0);
    img->allocation = STB_ALLOCATED;

    // saved Image
    Image_save(&img, "cropped.jpg");

}

// void Image_to_sepia(const Image *orig, Image *sepia){
//     ON_ERROR_EXIT(!(orig->allocation != NO_ALLOCATION && orig->channels >= 3), "A imagem de entrada deve ter ao menos 3 canais");
//     Image_creator(sepia, orig->width, orig->height, orig->channels, false);
//     ON_ERROR_EXIT(sepia->data == NULL, "Erro em criar a imagem em sepia");
//     for(unsigned char *p = orig->data, *pg = sepia->data; p != orig->data + orig->size; p += orig->channels, pg += sepia->channels){
//         *pg       = (uint8_t)fmin(0.393 * *p + 0.769 * *(p + 1) + 0.189 * *(p + 2), 255.0);
//         *(pg + 1) = (uint8_t)fmin(0.349 * *p + 0.686 * *(p + 1) + 0.168 * *(p + 2), 255.0);
//         *(pg + 2) = (uint8_t)fmin(0.272 * *p + 0.534 * *(p + 1) + 0.131 * *(p + 2), 255.0);
//         if(orig->channels == 4){
//             *(pg + 3) = *(p + 3);
//         }
//     }
// }
