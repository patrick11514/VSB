#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

typedef struct
{
    uint8_t idLength;
    uint8_t clourMapType;
    uint8_t dataTypeCode;
    uint16_t colourMapOrigin;
    uint16_t colourMapLength;
    uint8_t colourMapDepth;
    uint16_t xOrigin;
    uint16_t yOrigin;
    uint16_t width;
    uint16_t height;
    uint8_t bitsPerPixel;
    uint8_t imageDescriptor;
} TGA_Header;

typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
} Pixel;

int main()
{
    TGA_Header header = {0, 0, 2, 0, 0, 0, 0, 0, 1920, 1080, 24, 0};

    Pixel *image = (Pixel *)calloc(header.width * header.height, sizeof(Pixel));

    if (image != NULL)
    {
        for (int i = 0; i < header.height; i++)
        {
            for (int l = 0; l < header.width; l++)
            {
                int index = i * header.width + l;
                image[index].R = 255;
                image[index].G = 255;
                image[index].B = 255;
            }
        }

        for (int i = 0; i < header.height; i += 2)
        {
            for (int l = 0; l < header.width; l++)
            {
                int index = i * header.width + l;
                image[index].R = 0;
                image[index].G = 255;
                image[index].B = 255;
            }
        }
    }

    FILE *tgaImage = fopen("image.tga", "wb");

    fwrite(&header.idLength, sizeof(header.idLength), 1, tgaImage);
    fwrite(&header.clourMapType, sizeof(header.clourMapType), 1, tgaImage);
    fwrite(&header.dataTypeCode, sizeof(header.dataTypeCode), 1, tgaImage);
    fwrite(&header.colourMapOrigin, sizeof(header.colourMapOrigin), 1, tgaImage);
    fwrite(&header.colourMapLength, sizeof(header.colourMapLength), 1, tgaImage);
    fwrite(&header.colourMapDepth, sizeof(header.colourMapDepth), 1, tgaImage);
    fwrite(&header.xOrigin, sizeof(header.xOrigin), 1, tgaImage);
    fwrite(&header.yOrigin, sizeof(header.yOrigin), 1, tgaImage);
    fwrite(&header.width, sizeof(header.width), 1, tgaImage);
    fwrite(&header.height, sizeof(header.height), 1, tgaImage);
    fwrite(&header.bitsPerPixel, sizeof(header.bitsPerPixel), 1, tgaImage);
    fwrite(&header.imageDescriptor, sizeof(header.imageDescriptor), 1, tgaImage);

    fwrite(image, sizeof(Pixel), header.width * header.height, tgaImage);

    fclose(tgaImage);

    free(image);
    image = NULL;

    return 0;
}
