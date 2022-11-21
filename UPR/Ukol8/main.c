#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

//======================== [ TYPES ] ========================
typedef unsigned char byte;

typedef struct
{
    byte id_length;
    byte color_map_type;
    byte image_type;
    byte color_map[5];
    byte x_origin[2];
    byte y_origin[2];
    byte width[2];
    byte height[2];
    byte depth;
    byte descriptor;
} TGA_Header;

typedef struct
{
    uint8_t B;
    uint8_t G;
    uint8_t R;
} Pixel;

typedef struct
{
    TGA_Header header;
    Pixel *image;
    int width;
    int height;
} TGA_Image;

typedef struct
{
    int count;
    TGA_Image **images;
} Alphabet;

typedef struct
{
    int count;
    char **lines;
} Lines;

// ==========================================================

//==================== [ MAIN FUNCTIONS ] ====================

// load number of top and bottom lines separated by space
void loadLinesCount(int *top, int *bottom)
{

    char row[100];

    if (!fgets(row, 20, stdin))
    {
        printf("Could not read input\n");
        exit(1);
    }

    if (sscanf(row, "%d %d", top, bottom) != 2)
    {
        printf("Could not read input\n");
        exit(1);
    }
}

// char to upper
char toUpper(char c)
{
    if (c >= 'a' && c <= 'z')
        return 'A' + (c - 'a');
    else
        return c;
}

// string to upper
void strToUpper(char *string)
{
    int len = strlen(string);
    for (int i = 0; i < len; i++)
    {
        string[i] = toUpper(string[i]);
    }
}

// load uppercase lines from stdin
void loadLines(Lines *topLines, Lines *bottomLines)
{
    char row[100];

    for (int i = 0; i < topLines->count; i++)
    {
        if (!fgets(row, 100, stdin))
        {
            printf("Could not read input\n");
            exit(1);
        }

        topLines->lines[i] = (char *)malloc(strlen(row) * sizeof(char) + 1);
        strcpy(topLines->lines[i], row);
        strToUpper(topLines->lines[i]);
    }

    for (int i = 0; i < bottomLines->count; i++)
    {
        if (!fgets(row, 100, stdin))
        {
            printf("Could not read input\n");
            exit(1);
        }

        bottomLines->lines[i] = (char *)malloc(strlen(row) * sizeof(char) + 1);
        strcpy(bottomLines->lines[i], row);
        strToUpper(bottomLines->lines[i]);
    }
}

// get start position for text in line, to center it
int getStart(char *string, Alphabet *alphabet, int width)
{
    int len = strlen(string);

    int lenOfImage = 0;
    for (int i = 0; i < len; i++)
    {
        char ch = string[i];

        if (ch == ' ')
        {
            // space is 6 pixels;
            lenOfImage += 6;
            continue;
        }
        else if (ch < 'A' || ch > 'Z')
            continue;

        TGA_Image *image = alphabet->images[ch - 'A'];

        lenOfImage += image->width + 1;
    }

    return (width - lenOfImage - 1) / 2;
}

// ============================================================

//================== [ IMAGE FUNCTIONS ] ====================

// load image from path and return pointer on TGA_Image struct
TGA_Image *loadImage(char *path)
{
    FILE *file = fopen(path, "rb");
    if (!file)
    {
        printf("Could not load image\n");
        exit(1);
    }

    TGA_Image *image = (TGA_Image *)calloc(1, sizeof(TGA_Image));

    if (!image)
    {
        printf("Could not allocate memory for image\n");
        exit(1);
    }

    if (!fread(&image->header, sizeof(image->header), 1, file))
    {
        printf("Could not read image header\n");
        exit(1);
    }

    memcpy(&image->width, image->header.width, sizeof(image->header.width));
    memcpy(&image->height, image->header.height, sizeof(image->header.height));

    Pixel *pixels = (Pixel *)malloc(image->width * image->height * sizeof(Pixel));

    if (!pixels)
    {
        printf("Could not allocate memory for pixels\n");
        exit(1);
    }

    if (!fread(pixels, sizeof(Pixel), image->width * image->height, file))
    {
        printf("Could not read image pixels\n");
        exit(1);
    }

    image->image = pixels;

    fclose(file);

    return image;
}

// save image to file
void saveImage(TGA_Image *image, char *path)
{
    FILE *file = fopen(path, "wb");
    if (!file)
    {
        printf("Unable to write image\n");
        exit(1);
    }

    fwrite(&image->header, sizeof(image->header), 1, file);
    fwrite(image->image, sizeof(Pixel), image->width * image->height, file);

    fclose(file);

    free(image->image);
    free(image);
}

// ============================================================

//================== [ ALPHABET FUNCTIONS ] ===================

// load alphabet from path and return pointer on Alphabet struct with all images
Alphabet *loadAlphabet(char *path)
{
    Alphabet *alphabet = (Alphabet *)malloc(sizeof(Alphabet));

    if (!alphabet)
    {
        printf("Could not allocate memory for alphabet\n");
        exit(1);
    }

    alphabet->count = 26;
    alphabet->images = (TGA_Image **)malloc(alphabet->count * sizeof(TGA_Image *));

    if (!alphabet->images)
    {
        printf("Could not allocate memory for alphabet images\n");
        exit(1);
    }

    for (int i = 0; i < alphabet->count; i++)
    {
        char *imagePath = (char *)malloc(100 * sizeof(char));
        sprintf(imagePath, "%s/%c.tga", path, 'A' + i);
        alphabet->images[i] = loadImage(imagePath);
        free(imagePath);
    }

    return alphabet;
}

// add text to image
void addText(TGA_Image *image, Lines *texts, Alphabet *alphabet, bool bottom)
{
    // spacing 5 px
    int currentRow = 5;
    if (bottom)
    {
        currentRow = image->height - ((38 + 2) * texts->count);
    }

    for (int i = 0; i < texts->count; i++)
    {
        char *text = texts->lines[i];
        int start = getStart(text, alphabet, image->width);

        int len = strlen(text);

        for (int l = 0; l < len; l++)
        {
            char ch = text[l];

            if (ch == ' ')
            {
                // space is 6 pixels;
                start += 6;
                continue;
            }
            else if (ch < 'A' || ch > 'Z')
                continue;

            TGA_Image *character = alphabet->images[ch - 'A'];

            for (int w = 0; w < character->width; w++)
            {
                for (int h = 0; h < character->height; h++)
                {
                    int charIndex = h * character->width + w;
                    // ((38 - character->height) / 2) is moving smaler characters to center
                    int imageIndex = (currentRow + ((38 - character->height) / 2) + h) * image->width + start + w;

                    if (
                        character->image[charIndex].R != 0 ||
                        character->image[charIndex].G != 0 ||
                        character->image[charIndex].B != 0)
                    {
                        image->image[imageIndex] = character->image[charIndex];
                    }
                }
            }

            start += character->width + 1;
        }

        // 38 - height of Q and 2px space
        currentRow += 38 + 2;
    }
}

// ===========================================================

//==================== [ MAIN FUNCTION ] ======================

int main(int argc, char **argv)
{
    if (argc < 3)
    {
        printf("Wrong parameters\n");
        return 1;
    }

    // load image
    TGA_Image *image = loadImage(argv[1]);

    // load alphabet
    Alphabet *alphabet = loadAlphabet(argv[3]);

    // allocate memory for top and bottom lines count
    int *top = (int *)malloc(sizeof(int));
    int *bottom = (int *)malloc(sizeof(int));

    // load lines count to top and bottom
    loadLinesCount(top, bottom);

    // Allocate memory for lines
    Lines *topLines = (Lines *)malloc(sizeof(Lines));
    topLines->count = *top;
    topLines->lines = (char **)malloc(topLines->count * sizeof(char *));
    free(top);

    Lines *bottomLines = (Lines *)malloc(sizeof(Lines));
    bottomLines->count = *bottom;
    bottomLines->lines = (char **)malloc(bottomLines->count * sizeof(char *));
    free(bottom);

    // Load lines
    loadLines(topLines, bottomLines);

    addText(image, topLines, alphabet, false);
    addText(image, bottomLines, alphabet, true);

    // save and free image
    saveImage(image, argv[2]);

    // free alphabet
    for (int i = 0; i < alphabet->count; i++)
    {
        free(alphabet->images[i]->image);
        free(alphabet->images[i]);
    }
    free(alphabet->images);
    free(alphabet);

    // free lines
    for (int i = 0; i < topLines->count; i++)
    {
        free(topLines->lines[i]);
    }
    free(topLines->lines);
    free(topLines);

    for (int i = 0; i < bottomLines->count; i++)
    {
        free(bottomLines->lines[i]);
    }
    free(bottomLines->lines);
    free(bottomLines);

    return 0;
}

// ============================================================