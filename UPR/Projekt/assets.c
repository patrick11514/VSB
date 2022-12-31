// basic includes
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

// SDL2
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

// my includes
#include "breakout.h"

bool renderText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, int width, int height, int x, int y)
{
    SDL_Rect rect = {.x = x, .y = y, .w = width, .h = height};

    SDL_Surface *surface = TTF_RenderUTF8_Solid(font, text, color);
    if (!surface)
    {
        return false;
    }

    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (!texture)
    {
        return false;
    }

    if (SDL_RenderCopy(renderer, texture, NULL, &rect) != 0)
    {
        return false;
    }

    // free memory
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);

    return true;
}

bool renderTexture(SDL_Renderer *renderer, SDL_Texture *texture, int x, int y, int w, int h)
{
    SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};

    if (SDL_RenderCopy(renderer, texture, NULL, &rect) != 0)
    {
        return false;
    }

    return true;
}

bool renderRect(SDL_Renderer *renderer, int x, int y, int w, int h, SDL_Color color)
{
    SDL_Rect rect = {.x = x, .y = y, .w = w, .h = h};

    if (SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a) != 0)
    {
        return false;
    }

    if (SDL_RenderFillRect(renderer, &rect) != 0)
    {
        return false;
    }

    return true;
}

void strToLower(char *string)
{
    int len = (int)strlen(string);

    for (int i = 0; i < len; i++)
    {
        string[i] = tolower(string[i]);
    }
}

void loadTexture(SDL_Renderer *renderer, Texture *texture, char *path)
{
    SDL_Texture *IMGTexture = IMG_LoadTexture(renderer, path);
    int width, height;
    SDL_QueryTexture(IMGTexture, NULL, NULL, &width, &height);

    texture->texture = IMGTexture;
    texture->width = width;
    texture->height = height;
}

// https://stackoverflow.com/a/32936928/13157719
size_t count_utf8_code_points(const char *s)
{
    size_t count = 0;
    while (*s)
    {
        count += (*s++ & 0xC0) != 0x80;
    }
    return count;
}