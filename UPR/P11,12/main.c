#include <SDL2/SDL.h>
#include <stdbool.h>

int main()
{
    if (SDL_Init(SDL_INIT_VIDEO))
    {
        fprintf(stderr, "SDL_Init Error: %s\n", SDL_GetError());
        return 1;
    }
    SDL_Window *window = SDL_CreateWindow("SDL experiments", 100, 100, 800, 600, SDL_WINDOW_SHOWN);
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

    SDL_Event e;
    bool quit = false;
    int pos = 100;

    SDL_Point shield_point = {.x = 0, .y = 0};

    int shield_time = 0;

    bool bomb_active = true;

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

        int bomb_x1 = pos, bomb_y1 = pos, bomb_x2 = pos + 100, bomb_y2 = pos + 100;

        if (bomb_active)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);                 // Nastavení barvy na červenou
            SDL_RenderDrawLine(renderer, bomb_x1, bomb_x1, bomb_x2, bomb_y2); // Vykreslení čáry
        }

        // Building
        SDL_Rect r = {.x = 500, .y = 500, .w = 50, .h = 75};
        SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
        SDL_RenderDrawRect(renderer, &r);

        if (shield_time)
        {
            SDL_SetRenderDrawColor(renderer, 255, 0, 255, 255);
            int shield_width = 80;
            SDL_Rect shield_rect = {.w = shield_width, .h = shield_width, .x = shield_point.x - shield_width / 2, .y = shield_point.y - shield_width / 2};
            SDL_RenderDrawRect(renderer, &shield_rect);

            if (SDL_IntersectRectAndLine(&shield_rect, &bomb_x1, &bomb_y1, &bomb_x2, &bomb_y2))
            {
                shield_time = 0;
                bomb_active = false;
            }
        }

        if (bomb_active)
        {
            pos++;
        }
        if (shield_time > 0)
        {
            shield_time--;
        }

        SDL_RenderPresent(renderer); // Prezentace kreslítka
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}