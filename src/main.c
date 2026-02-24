#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"

int main(void)
{
    // if(TTF_Init()==-1) 
    // {
    // printf("TTF_Init: %s\n", TTF_GetError());
    // exit(2);
    // }
    // TTF_Font* vera = TTF_OpenFont("etl24-unicode.bdf",16);
    SDL_Window *window = NULL;
    SDL_Renderer *renderer = NULL;

    if (!init(&window, &renderer))
    {
        return 1;
    }

    bool running = true;
    bool game_over = false;
    bool encoredesennemis = true;
    grille grilledennemis = creer_grille(N);
    Uint32 last_ticks = SDL_GetTicks();

    Entity player = {
        .x = SCREEN_WIDTH / 2 - PLAYER_WIDTH / 2,
        .y = SCREEN_HEIGHT - 60,
        .w = PLAYER_WIDTH,
        .h = PLAYER_HEIGHT,
        .vx = 0,
        .vy = 0};

    Entity bullet = {0};
    bool bullet_active = false;

    while (running)
    {
        Uint32 ticks = SDL_GetTicks();
        float dt = (ticks - last_ticks) / 1000.0f;
        if (dt > 0.05f)
            dt = 0.05f;
        last_ticks = ticks;

        SDL_PumpEvents();
        const Uint8 *keys = SDL_GetKeyboardState(NULL);
        handle_input(&running, keys, &player, &bullet, &bullet_active);
        update(&player, &bullet, &bullet_active, dt, &game_over, grilledennemis,&encoredesennemis);
        render(renderer, &player, &bullet, bullet_active,grilledennemis);
        if(game_over){
            gaame_over(grilledennemis, &running, renderer,&encoredesennemis);
        }
    }
    printf("fini");
    cleanup(window, renderer);
    return 0;
}
