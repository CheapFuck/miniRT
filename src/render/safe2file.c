#include "../../includes/minirt.h"

void save_image_to_file(const unsigned char* raw_image_data, unsigned width, unsigned height, const char* filename) {
    unsigned char* png_data = NULL; // Buffer for PNG data
    size_t png_size = 0;           // Size of PNG data

    // Encode the image to PNG format
    unsigned error = lodepng_encode32(&png_data, &png_size, raw_image_data, width, height);
    if (error) {
        printf("Error %u: %s\n", error, lodepng_error_text(error));
        return;
    }
      // Save the PNG data to a file
    error = lodepng_save_file(png_data, png_size, filename);
    if (error) {
        printf("Error %u: %s\n", error, lodepng_error_text(error));
    } else {
        printf("Image saved to %s\n", filename);
    }
    free(png_data);
}
