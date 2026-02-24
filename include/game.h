#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"
//#include <SDL_TTF.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

#define PLAYER_WIDTH 50
#define PLAYER_HEIGHT 20
#define PLAYER_SPEED 400.0f

#define BULLET_WIDTH 10
#define BULLET_HEIGHT 20
#define BULLET_SPEED 600.0f

#define ENNEMY_HEIGHT 20
#define ENNEMY_WIDTH 50
#define ENNEMY_SPEED -50.0f
#define N 2

#define VIE 5

bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
void update(Entity *player, Entity *bullet, bool *bullet_active, float dt, bool *game_over, grille grilledennemis, bool* encoredesennemis, int *pv);
void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, bool bullet_active, grille grilledennemis);
void cleanup(SDL_Window *window, SDL_Renderer *renderer, grille grilledennemis);
void liberegrille(grille grilledennemis);
grille creer_grille(int n);
void gaame_over(bool *running, SDL_Renderer *renderer, bool *encoredesennemis);

#ifdef _WIN32
//  For Windows (32- and 64-bit)
#   include <windows.h>
#   define SLEEP(msecs) Sleep(msecs)
#elif __unix
//  For linux, OSX, and other unixes
#   define _POSIX_C_SOURCE 199309L // or greater
#   include <time.h>
#   define SLEEP(msecs) do {            \
        struct timespec ts;             \
        ts.tv_sec = msecs/1000;         \
        ts.tv_nsec = msecs%1000*1000;   \
        nanosleep(&ts, NULL);           \
        } while (0)
#else
#   error "Unknown system"
#endif

#endif
