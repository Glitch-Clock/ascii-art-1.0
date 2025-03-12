#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include <stdio.h>
#include <stdlib.h>

const char *ascii_chars = "@#%*+=-:. ";

void save_ascii_as_image(unsigned char *image, int width, int height, int channels) {
    int scale = 8;
    int out_width = width * scale;
    int out_height = height * scale;

    unsigned char *output_image = (unsigned char *)malloc(out_width * out_height);
    if (!output_image) {
        printf("Failed to allocate memory for output image.\n");
        return;
    }

    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * channels;
            int gray = (image[index] + image[index + 1] + image[index + 2]) / 3;

            int char_index = gray * (sizeof(ascii_chars) - 2) / 255;
            unsigned char pixel_value = 255 - (char_index * 255 / (sizeof(ascii_chars) - 2));

            for (int dy = 0; dy < scale; dy++) {
                for (int dx = 0; dx < scale; dx++) {
                    int out_index = ((y * scale + dy) * out_width) + (x * scale + dx);
                    output_image[out_index] = pixel_value;
                }
            }
        }
    }

    stbi_write_png("ascii_image.png", out_width, out_height, 1, output_image, out_width);
    free(output_image);
    printf("\nASCII Art saved as ascii_image.png\n");
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Usage: %s <image file>\n", argv[0]);
        return 1;
    }

    int width, height, channels;
    unsigned char *image = stbi_load(argv[1], &width, &height, &channels, 0);

    if (!image) {
        printf("Failed to load image!\n");
        return 1;
    }

    printf("Loaded image: %s (%d x %d, %d channels)\n", argv[1], width, height, channels);
    save_ascii_as_image(image, width, height, channels);

    stbi_image_free(image);
    return 0;
}
