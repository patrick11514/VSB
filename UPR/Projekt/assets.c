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

bool renderText(SDL_Renderer *renderer, char *text, TTF_Font *font, SDL_Color color, int width, int height, int x, int y)
{
    SDL_Rect rect = {.x = x, .y = y, .w = width, .h = height};

    SDL_Surface *surface = TTF_RenderText_Solid(font, text, color);
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
    if (SDL_QueryTexture(texture, NULL, NULL, &w, &h) != 0)
    {
        return false;
    }

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