#include <SDL2/SDL.h>
#include "game.h"
#include "entity.h"
#include <stdio.h>
// grille creer_grille(size_t n){
//     grille grilledennemis;
//     grilledennemis.nbre_ennemis= n;
//     grilledennemis.ennemis = malloc(n*sizeof(Entity));
//     grilledennemis.vivant = malloc(n*sizeof(bool));
//     for(size_t i = 0; i<N;i++){
//         grilledennemis.vivant[i]=true;
//     }
//     for(size_t i = 0; i<N;i++){
//         float x;
//         float y;
//         bool nook(float x,float y){
//             for(size_t j = 0;j<i-1;j++){
//                 if(fabs(x-grilledennemis.ennemis[j].x)<ENNEMY_WIDTH){
//                     return false;
//                 }elseif(fabs(y-grilledennemis.ennemis[j].y)<ENNEMY_HEIGHT){
//                     return false;
//                 };
//              };
//              return true;
//         };
//         while(nook(x,y)){
//             x = SCREEN_WIDTH*(float)rand()/((float)RAND_MAX);
//             y = SCREEN_HEIGHT*(float)rand()/(2*(float)RAND_MAX);//on reste dans la moitié haute de l'écran
//         }
//         grilledennemis.ennemis[i].x = x;
//         grilledennemis.ennemis[i].y = y;
//         grilledennemis.ennemis[i].vx = 0;
//         grilledennemis.ennemis[i].vy = ENNEMY_SPEED;
//         grilledennemis.ennemis[i].w = ENNEMY_WIDTH;
//         grilledennemis.ennemis[i].x = ENNEMY_HEIGHT;
//     };
//     return grilledennemis;
//}
char spv[5];
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
        grilledennemis.ennemis[i].vy = -ENNEMY_SPEED;
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

void update(Entity *player, Entity *bullet, bool *bullet_active, float dt, bool *game_over, grille grilledennemis, bool *encoredesennemis,int *pv){
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
       }
       *encoredesennemis = false;
       for(size_t i = 0;i<grilledennemis.nbre_ennemis;i++){
        if(grilledennemis.vivant[i]){
            *encoredesennemis = true;
            grilledennemis.ennemis[i].y +=grilledennemis.ennemis[i].vy *dt;
            if(grilledennemis.ennemis[i].y + grilledennemis.ennemis[i].h > SCREEN_HEIGHT){
                *game_over = true;
            }
            if(*bullet_active&&fabs(grilledennemis.ennemis[i].y-bullet->y)<(bullet->h+grilledennemis.ennemis[i].h)&&fabs(grilledennemis.ennemis[i].x-bullet->x)<bullet->w+grilledennemis.ennemis[i].w){
                *bullet_active = false;
                grilledennemis.vivant[i] = false;
                grilledennemis.ennemy_bullet_active[i]=false;
            }
            if (!grilledennemis.ennemy_bullet_active[i])//condition a completer pour eviter tir en continu
               {
                    Entity *ennemi = &(grilledennemis.ennemis[i]);
                    grilledennemis.ennemy_bullet_active[i] = true;
                    Entity *buullet = &(grilledennemis.ennemy_bullet[i]);
                    buullet->x = ennemi->x + ennemi->w / 2 - BULLET_WIDTH / 2;
                    buullet->y = ennemi->y;
                    buullet->w = BULLET_WIDTH;
                    buullet->h = BULLET_HEIGHT;
                    buullet->vy = BULLET_SPEED;
                }else {
            Entity *buullet = &(grilledennemis.ennemy_bullet[i]);
            buullet->y += buullet->vy * dt;
            if (buullet->y + buullet->h > SCREEN_HEIGHT)
                *grilledennemis.ennemy_bullet_active = false;
            if(fabs(buullet->y-player->y)<buullet->h+player->h&&fabs(buullet->x-player->x)<buullet->w+player->w){
                *pv -= 1;
                grilledennemis.ennemy_bullet_active[i] = false;
            }
            }
        }
       }
        if(!*encoredesennemis||*pv == 0){
            *game_over = true;//on ne distingue pas victoire et defaite, pour le moment 
        }
    };
}

void render(SDL_Renderer *renderer, Entity *player, Entity *bullet, bool bullet_active, grille grilledennemis, int *pv){
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
        if (grilledennemis.ennemy_bullet_active[i]){
            SDL_Rect ennemi_bullet_rect = {
                (int)grilledennemis.ennemy_bullet[i].x, (int)grilledennemis.ennemy_bullet[i].y,
                grilledennemis.ennemy_bullet[i].w, grilledennemis.ennemy_bullet[i].h};
            SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
            SDL_RenderFillRect(renderer, &ennemi_bullet_rect);
        }
    }

    SDL_Surface* coeur = SDL_LoadBMP("coeur.bmp");
    if(coeur == NULL){
        SDL_Log("ERREUR>%s\n",SDL_GetError());
    };
    SDL_Texture* coeur_tampon = SDL_CreateTextureFromSurface(renderer,coeur);
    SDL_FreeSurface(coeur);
    SDL_Rect dimensions_coeur = {700,5,100,50};
    SDL_RenderCopy(renderer, coeur_tampon, NULL, &dimensions_coeur);

    
    snprintf(spv, 10, "%d.bmp", *pv);
    SDL_Surface* nbre = SDL_LoadBMP(spv);
    if(nbre == NULL){
        SDL_Log("ERREUR>%s\n",SDL_GetError());
    };
    SDL_Texture* nbre_tampon = SDL_CreateTextureFromSurface(renderer,nbre);
    SDL_FreeSurface(nbre);
    SDL_Rect dimensions_nbre = {650,5,50,50};
    SDL_RenderCopy(renderer, nbre_tampon, NULL, &dimensions_nbre);

    SDL_RenderPresent(renderer);

};

void gaame_over(bool *running, SDL_Renderer *renderer, bool *encoredesennemis){
    //la partie logique de la fin de partie
    
    SDL_Surface* go = SDL_LoadBMP("GameOver.bmp");
    if(go == NULL){
        SDL_Log("ERREUR>%s\n",SDL_GetError());
    };
    SDL_Texture* gameover_tampon = SDL_CreateTextureFromSurface(renderer,go);
    SDL_FreeSurface(go);
    SDL_Rect dimensions_gameover = {50,0,640,480};
    SDL_RenderCopy(renderer, gameover_tampon, NULL, &dimensions_gameover);
    
    SDL_Surface* resultat;
    if(*encoredesennemis){
        resultat = SDL_LoadBMP("defaite.bmp");
    }else{
        resultat = SDL_LoadBMP("victoire.bmp"); 
    };
    if(resultat == NULL){
        SDL_Log("ERREUR>%s\n",SDL_GetError());
    };
    SDL_Texture* resultat_tampon = SDL_CreateTextureFromSurface(renderer,resultat);
    SDL_FreeSurface(resultat);
    SDL_Rect dimensions_resultat = {50,300,640,200};
    SDL_RenderCopy(renderer, resultat_tampon, NULL, &dimensions_resultat);   

    SDL_RenderPresent(renderer);
    SDL_DestroyTexture(gameover_tampon);
    SDL_DestroyTexture(resultat_tampon);
    SLEEP(5000);
    *running = false;
}

void cleanup(SDL_Window *window, SDL_Renderer *renderer, grille grilledennemis){//la partie gique de la fin de partie
    if (renderer)
        SDL_DestroyRenderer(renderer);
    if (window)
        SDL_DestroyWindow(window);
    SDL_Quit();
    liberegrille(grilledennemis);
    //TTF_Quit();
}
