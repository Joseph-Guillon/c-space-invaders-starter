#include <SDL2/SDL.h>
#include "game.h"
#include <stdio.h>
//bool nook(grille grilledennemis, float x,float y, size_t i){
//            for(size_t j = 0;j<i-1;j++){
//                if(fabs(x-grilledennemis.ennemis[j].x)<ENNEMY_WIDTH){
//                    return false;
//                }else if(fabs(x-grilledennemis.ennemis[j].y)<ENNEMY_HEIGHT){
//                    return false;
//                };
//             };
//             return true;
//        };
grille creer_grille(int n){
    grille grilledennemis;
    grilledennemis.nbre_ennemis= n;
    grilledennemis.ennemis = malloc(n*sizeof(Entity));
    grilledennemis.vivant = malloc(n*sizeof(bool));
    grilledennemis.ennemy_bullet_active = malloc(n*sizeof(bool));
    grilledennemis.ennemy_bullet = malloc(n*sizeof(Entity));
    for(size_t i = 0; i<n;i++){
        grilledennemis.vivant[i]=true;
    }
    for(size_t i = 0; i<n;i++){
        float x = i*SCREEN_WIDTH/n;
        float y = SCREEN_HEIGHT/4;
        grilledennemis.ennemis[i].x = x;
        grilledennemis.ennemis[i].y = y;
        grilledennemis.ennemis[i].vx = 0;
        grilledennemis.ennemis[i].vy = ENNEMY_SPEED;
        grilledennemis.ennemis[i].w = ENNEMY_WIDTH;
        grilledennemis.ennemis[i].h = ENNEMY_HEIGHT;
        grilledennemis.ennemy_bullet_active[i] = false;
    };
    return grilledennemis;
}
void liberegrille(grille grilledennemis){
    free(grilledennemis.ennemis);
    free(grilledennemis.vivant);
}
void gaame_over(grille grilledennemis,bool *running){
    liberegrille(grilledennemis);
    *running = false;
}
bool init(SDL_Window **window, SDL_Renderer **renderer)
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        SDL_Log("Erreur SDL_Init: %s", SDL_GetError());
        return false;
    }

    *window = SDL_CreateWindow("Space Invaders (SDL)", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                               SCREEN_WIDTH, SCREEN_HEIGHT, 0);
    if (!*window)
    {
        SDL_Log("Erreur SDL_CreateWindow: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    *renderer = SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    if (!*renderer)
    {
        SDL_Log("Erreur SDL_CreateRenderer: %s", SDL_GetError());
        SDL_DestroyWindow(*window);
        SDL_Quit();
        return false;
    }

    return true;
}

void handle_input(bool *running, const Uint8 *keys, Entity *player, Entity *bullet, bool *bullet_active)
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT)
            *running = false;
    }

    player->vx = 0.0f;
    if (keys[SDL_SCANCODE_LEFT])
        player->vx = -PLAYER_SPEED;
    if (keys[SDL_SCANCODE_RIGHT])
        player->vx = PLAYER_SPEED;

    if (keys[SDL_SCANCODE_SPACE] && !*bullet_active)
    {
        *bullet_active = true;
        bullet->x = player->x + player->w / 2 - BULLET_WIDTH / 2;
        bullet->y = player->y;
        bullet->w = BULLET_WIDTH;
        bullet->h = BULLET_HEIGHT;
        bullet->vy = -BULLET_SPEED;
    }
}

void update(Entity *player, Entity *bullet, bool *bullet_active, float dt, bool *game_over, grille grilledennemis, bool *running){
    if(!*game_over){
        player->x += player->vx * dt;

        if (player->x < 0)
            player->x = 0;
        if (player->x + player->w > SCREEN_WIDTH)
            player->x = SCREEN_WIDTH - player->w;

        if (*bullet_active)
        {
            bullet->y += bullet->vy * dt;
            if (bullet->y + bullet->h < 0)
                *bullet_active = false;
       };
       bool encoredesennemis = false;
       for(size_t i = 0;i<grilledennemis.nbre_ennemis;i++){
        if(grilledennemis.vivant[i]){
            encoredesennemis = true;
            Entity *ennemi = &grilledennemis.ennemis[i];
            ennemi->y +=ennemi->vy *dt;
            ennemi->vy += ENNEMY_ACCELERATION;
            if(ennemi->y + ennemi->h > SCREEN_HEIGHT){
                *game_over = true;
            }
            if(*bullet_active&&fabs(ennemi->y-bullet->y)<(bullet->h+ennemi->h)&&fabs(ennemi->x-bullet->x)<bullet->w+ennemi->w){
                *bullet_active = false;
                grilledennemis.vivant[i] = false;
            }
            if (!grilledennemis.ennemy_bullet_active[i])//condition a completer pour eviter tir en continu
               {
                    grilledennemis.ennemy_bullet_active[i] = true;
                    Entity *buullet = &grilledennemis.ennemy_bullet[i];
                    buullet->x = ennemi->x + ennemi->w / 2 - BULLET_WIDTH / 2;
                    buullet->y = ennemi->y;
                    buullet->w = BULLET_WIDTH;
                    buullet->h = BULLET_HEIGHT;
                    buullet->vy = BULLET_SPEED;
                };
            if (grilledennemis.ennemy_bullet_active[i])
                    {
                        Entity *buullet = &grilledennemis.ennemy_bullet[i];
                        buullet->y += buullet->vy * dt;
                        if (buullet->y + buullet->h > SCREEN_HEIGHT)
                            grilledennemis.ennemy_bullet_active[i] = false;
                        if(fabs(player->y-buullet->y)<(buullet->h+player->h)&&fabs(player->x-buullet->x)<buullet->w+player->w){
                            grilledennemis.ennemy_bullet_active[i] = false;
                            *game_over = true; //on verra apres por les HP
                               };        
                    }
       }
       if(!encoredesennemis){
            *game_over = true;//on ne distingue pas vicroitr et defaite
       }
    }else{
        return gaame_over(grilledennemis,running);
    };
};

void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, bool bullet_active, grille grilledennemis){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    SDL_Rect player_rect = {
        (int)player->x, (int)player->y,
        player->w, player->h};
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &player_rect);

    if (bullet_active)
    {
        SDL_Rect bullet_rect = {
            (int)bullet->x, (int)bullet->y,
            bullet->w, bullet->h};
        SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
        SDL_RenderFillRect(renderer, &bullet_rect);
    }
    for(size_t i = 0;i<grilledennemis.nbre_ennemis;i++){
        if(grilledennemis.vivant[i]){
            Entity ennemi = grilledennemis.ennemis[i];
            SDL_Rect ennemi_rect = {
                (int)ennemi.x, (int)ennemi.y,
                ennemi.w, ennemi.h};
            SDL_SetRenderDrawColor(renderer, 0, 0, 255, 255);
            SDL_RenderFillRect(renderer, &ennemi_rect);
        }
    }

    SDL_RenderPresent(renderer);
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer)
{
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
}
