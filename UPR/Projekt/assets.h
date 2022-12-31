#ifndef ASSETS_H
#define ASSETS_H 1

// default includes
#include <stdbool.h>
#include <sys/cdefs.h>

// sdl
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// my includes
#include "dynamicarray.h"

// Position structure for X Y coordinates
typedef struct Position
{
    float x;
    float y;
} Position;

// Texture structure for SDL_Texture and width and height
typedef struct Texture
{
    SDL_Texture *texture;
    int width;
    int height;
} Texture;

// Main Variables from main.c file for easy access in other functions
typedef struct MainVariables
{
    // SDL WINDOW
    SDL_Window *window;

    // paddle under title position
    Position paddlePosition;
    // paddle under title start position
    int paddleStartPosition;
    // paddle under title go reverse if true
    bool paddleReverse;

    // current fps
    int FPS;

    // MAIN MENU BUTTONS
    //  main menu play button corners LeftTop + RightBottom
    Position mainMenuPlayLT;
    Position mainMenuPlayRB;
    // main menu play button if hover
    bool mainMenuPlayHover;

    // main menu settings button corners LeftTop + RightBottom
    Position mainMenuSettingsLT;
    Position mainMenuSettingsRB;
    // main menu settings button if hover
    bool mainMenuSettingsHover;

    // main menu highscores button corners LeftTop + RightBottom
    Position mainMenuHighscoresLT;
    Position mainMenuHighscoresRB;
    // main menu highscores button if hover
    bool mainMenuHighscoresHover;

    // main menu exit button corners LeftTop + RightBottom
    Position mainMenuExitLT;
    Position mainMenuExitRB;
    // main menu exit button if hover
    bool mainMenuExitHover;

    // SETTING BUTTONS
    // scale button corners LeftTop + RightBottom
    Position settingsScaleLT;
    Position settingsScaleRB;
    // scale button if hover
    bool settingsScaleHover;

    // back button corners LeftTop + RightBottom
    Position settingsBackLT;
    Position settingsBackRB;
    // back button if hover
    bool settingsBackHover;

    // HIGHSCORES BUTTONS
    // next button corners LeftTop + RightBottom
    Position highscoresNextLT;
    Position highscoresNextRB;
    // next button if hover
    bool highscoresNextHover;

    // prev button corners LeftTop + RightBottom
    Position highscoresPrevLT;
    Position highscoresPrevRB;
    // prev button if hover
    bool highscoresPrevHover;

    //  back button corners LeftTop + RightBottom
    Position highscoresBackLT;
    Position highscoresBackRB;
    // back button if hover
    bool highscoresBackHover;

    // HIGHSCORE SCREEN
    //  current offset for highscores
    int highscoresOffset;
    // buttons
    bool highscoresUpButton;
    bool highscoresDownButton;

    // LEVEL SELECT BUTTONS
    // next button corners LeftTop + RightBottom
    Position levelSelectNextLT;
    Position levelSelectNextRB;
    // next button if hover
    bool levelSelectNextHover;

    // prev button corners LeftTop + RightBottom
    Position levelSelectPrevLT;
    Position levelSelectPrevRB;
    // prev button if hover
    bool levelSelectPrevHover;

    //  back button corners LeftTop + RightBottom
    Position levelSelectBackLT;
    Position levelSelectBackRB;
    // back button if hover
    bool levelSelectBackHover;

    // LEVEL SELECT SCREEN
    //   current offset for levels
    int levelSelectOffset;
    // buttons
    bool levelSelectUpButton;
    bool levelSelectDownButton;
    // levels
    Array *levelsTextCoords;

} MainVariables;

// function to render text _text with font _font and color _color on position _x and _y with SDL_Renderer _renderer
bool renderText(SDL_Renderer *_renderer, char *_text, TTF_Font *_font, SDL_Color _color, int _with, int _height, int _x, int _y) __attribute_warn_unused_result__;

// function to render texture _texture on position _x and _y with _width and _height
bool renderTexture(SDL_Renderer *_renderer, SDL_Texture *_texture, int _x, int _y, int _width, int _height) __attribute_warn_unused_result__;

// render rectangle on position _x and _y with _width and _height with color _color
bool renderRect(SDL_Renderer *_renderer, int _x, int _y, int _width, int _height, SDL_Color _color) __attribute_warn_unused_result__;

// string to lower
void strToLower(char *string);

// load texture
void loadTexture(SDL_Renderer *_renderer, Texture *_texture, char *_path);

// get len of string, because strlen return wrong len for utf8
// https://stackoverflow.com/a/32936928/13157719
size_t count_utf8_code_points(const char *s) __attribute_warn_unused_result__;

#endif