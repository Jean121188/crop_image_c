#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

enum alocation_type {
    NO_ALLOCATION,
    SELF_ALLOCATED,
    STB_ALLOCATED
};

typedef struct{
    int width;
    int height;
    int channels;
    size_t size;
    uint8_t *data;
    enum alocation_type allocation;
}Image;

void Image_load(Image *img, const char* fname);
void Image_creator(Image *img, int width, int height, int channels, bool zerod);
void Image_save(const Image *img, const char* fname);
void Image_free(Image *img);
void Image_to_gray(const Image *orig, Image *gray);
Image Image_resize(Image *img, uint16_t cx, uint16_t cy, uint16_t cw, uint16_t ch);
// void Image_to_sepia(const Image *orig, Image *sepia);