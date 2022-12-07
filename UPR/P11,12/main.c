#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>
#include <stdbool.h>
#include <assert.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600

typedef struct
{
    SDL_Point pos;
    SDL_Point vel;
} Missle;

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("SDL experiments", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        fprintf(stderr, "SDL_CreateWindow Error: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!renderer)
    {
        SDL_DestroyWindow(window);
        fprintf(stderr, "SDL_CreateRenderer Error: %s", SDL_GetError());
        SDL_Quit();
        return 1;
    }

    TTF_Init();

    SDL_Event e;
    bool quit = false;

    SDL_Point shield_point = {.x = 0, .y = 0};

    int shield_time = 0;

    bool bomb_active = true;

    int score = 0;
    char scoreText[80];
    Missle missle = {.pos.x = 0, .pos.y = 40, .vel.x = 1, .vel.y = 1};

    SDL_Color white = {255, 255, 255};

    TTF_Font *sans = TTF_OpenFont("Roboto-Bold.ttf", 22);
    assert(sans);

    SDL_Rect scoreRect = {.x = WINDOW_WIDTH / 2 - 100 / 2, .y = 50, .w = 100, .h = 20};

    SDL_Texture *missleTexture = IMG_LoadTexture(renderer, "atom.svg");

    while (!quit)
    {
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                quit = true;
            }
            else if (e.type == SDL_MOUSEBUTTONDOWN)
            {
                // printf("[%d, %d]\n", e.button.x, e.button.y);
                shield_point.x = e.button.x;
                shield_point.y = e.button.y;

                shield_time = 200;
            }
        }
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255); // Nastavení barvy na černou
        SDL_RenderClear(renderer);                      // Vykreslení pozadí

        // int bomb_x1 = pos, bomb_y1 = pos, bomb_x2 = pos + 100, bomb_y2 = pos + 100;

        if (bomb_active)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255); // Nastavení barvy na červenou
            SDL_Rect missleRect = {.h = 24, .w = 24, .x = missle.pos.x - 12, .y = missle.pos.y - 12};
            SDL_RenderDrawRect(renderer, &missleRect);
            // SDL_RenderDrawLine(renderer, bomb_x1, bomb_x1, bomb_x2, bomb_y2); // Vykreslení čáry
        }

        // Building
        SDL_Rect r = {.x = 500, .y = 500, .w = 50, .h = 75};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &r);

        if (shield_time)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            int shield_width = (shield_time / 200.0) * 80;
            SDL_Rect shield_rect = {.w = shield_width, .h = shield_width, .x = shield_point.x - shield_width / 2, .y = shield_point.y - shield_width / 2};

            SDL_RenderDrawRect(renderer, &shield_rect);
            SDL_RenderCopy(renderer, missleTexture, NULL, &shield_rect);

            SDL_Rect missleRect = {.h = 24, .w = 24, .x = missle.pos.x - 12, .y = missle.pos.y - 12};

            if (SDL_HasIntersection(&shield_rect, &missleRect))
            {
                shield_time = 0;
                bomb_active = false;
                score += 10;
            }
        }

        if (bomb_active)
        {
            missle.pos.x += missle.vel.x;
            missle.pos.y += missle.vel.y;
        }
        if (shield_time > 0)
        {
            shield_time--;
        }

        sprintf(scoreText, "Score: %d", score);

        SDL_Surface *scoreSurface = TTF_RenderText_Solid(sans, scoreText, white);
        SDL_Texture *scoreTexture = SDL_CreateTextureFromSurface(renderer, scoreSurface);

        SDL_RenderCopy(renderer, scoreTexture, NULL, &scoreRect);

        SDL_RenderPresent(renderer); // Prezentace kreslítka

        SDL_DestroyTexture(scoreTexture);
        SDL_FreeSurface(scoreSurface);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}