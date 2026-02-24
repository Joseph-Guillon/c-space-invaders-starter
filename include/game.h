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

bool init(SDL_Window **window, SDL_Renderer **renderer);
void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active);
void update(Entity *player, Entity *bullet, bool *bullet_active, float dt, bool *game_over, grille grilledennemis, bool *running);
void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, bool bullet_active, grille grilledennemis, bool game_over);
void cleanup(SDL_Window *window, SDL_Renderer *renderer);
void liberegrille(grille grilledennemis);
void gaame_over(grille grilledennemis,bool *running);
grille creer_grille(int n);

#endif
