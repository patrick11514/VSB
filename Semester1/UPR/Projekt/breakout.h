#ifndef BREAKOUT_H
#define BREAKOUT_H 1

#define WINDOW_WIDTH 720
#define WINDOW_HEIGHT 900

#define MOVE_FPS 60.0f

#define HIGHSCORES_PER_PAGE 8
#define LEVELS_PER_PAGE 8

#define PADDLE_SPEED 35.0f

// default includes
#include <stdbool.h>

// sdl
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

// my includes
#include "assets.h"
#include "dynamicarray.h"

typedef struct Highscores
{
    int count;
    Array *scores;
    Array *players;
} Highscores;

typedef enum
{
    // Main menu window
    MainMenu = 0,
    // Game window
    Game = 1,
    // Settings window
    Settings = 2,
    // Highscores window
    Highscore = 3,
    // Level select window
    LevelSelect = 4,
    GameOver = 5,
    // Info after level is selected, but before actual game
    LevelInfo = 6,
    // Controls menu
    Controls = 7
} MenuTypes;

typedef struct Colors
{
    SDL_Color white;
    SDL_Color yellow;
    SDL_Color red;
    SDL_Color green;
    SDL_Color blue;
    SDL_Color black;
    SDL_Color orange;
} Colors;

typedef struct Textures
{
    // paddle
    Texture *paddle;
    // buttons
    Texture *buttonUp;
    Texture *buttonUpHover;
    Texture *buttonDown;
    Texture *buttonDownHover;
    // bricks
    Texture *brickYellow;
    Texture *brickLime;
    Texture *brickGray;
    Texture *brickRed;
    Texture *brickPurple;
    Texture *brickBlue;
    // other
    Texture *heart;
    Texture *ball;
} Textures;

typedef struct TextCoords
{
    int x;
    int y;
    int width;
    int height;
    bool hover;
} TextCoords;

typedef struct Brick
{
    int x;
    int y;
    Texture *texture;
    bool destroyed;
    int lives;
} Brick;

typedef struct BrickHealth
{
    int lives;
    char identifier;
    Texture *texture;
} BrickHealth;

typedef struct Level
{
    int score;
    int health;
    int scorePerBrick;
    char *description;
    Array *brickHealths;
    Array *bricks;
    int ballSpeed;
    int ballSpeedModifier;
    int ballSpeedMax;
} Level;

typedef struct WindowProperties
{
    // scale of window
    float scale;
    // Roboto Font
    TTF_Font *font;
    // current FPS text
    char currentFPS[10];
    // colors
    Colors *colors;
    // current menu
    MenuTypes currentMenu;
    // loaded textures
    Textures *textures;
    // highscores
    Highscores *highscores;
    // levels
    Array *levels;
    // the game
    Level *currentLevel;
    int score;
    int lives;
    float ballX;
    float ballY;
    float ballSpeedX;
    float ballSpeedY;
    double ballSpeedModifier;
    float paddleX;
    float paddleY;
    float paddleSpeed;
    char *currentUserName;
} WindowProperties;

// function ran on every SDL event
void checkEvents(SDL_Event *e, bool *quit, WindowProperties *windowProperties, MainVariables *mainVars);

// function run every frame
void tick(int *frames, SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *vars);

// calculate and display current FPS
void calculateFPS(unsigned long *prevTime, int *frames, char *currentFPS, MainVariables *vars);

// main menu
void renderMainMenu(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *vars);

// settings
void renderSettings(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars);

// highscore screen
void renderHighscore(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars);

// level select screen
void renderLevelSelect(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars);

// level info before level starts
void renderLevelInfo(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars);

// the game
void renderGame(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars);

// the screen after winning or losing game
void renderGameOver(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars);

// controls menu
void renderControls(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars);

// render title function
void renderTitle(SDL_Renderer *renderer, WindowProperties *windowProperties, MainVariables *mainVars, TextCoords *textCoords);

#endif