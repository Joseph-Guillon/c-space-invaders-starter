#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"
#include <stdlib.h>
struct grille{
    size_t nbre_ennemis;
    entity *ennemis;
    bool *vivant;
}
typedef struct grille grille;
grille creer_grille(N){
    grille grilledennemis;
    grilledennemis.nbre_ennemis= N;
    grilledennemis.ennemis = malloc(N*sizeof(entity));
    grilledennemis.vivant = malloc(N*sizeof(bool));
    for(size_t i = 0; i<N;i++){
        grilledennemis.vivant[i]=true;
    }
    for(size_t i = 0; i<N;i++){
        float x;
        float y;
        bool nook(float x,float y){
            for(size_t j = 0,j<i-1,j++){
                if(fabs(x-grilledennemis.ennemis.x[j])<ENNEMY_WIDTH){
                    return false;
                }elseif(x-grilledennemis.ennemis.y[j])<ENNEMY_HEIGHT){
                    return false;
                };
             };
             return true
        };
        while(nook(x,y)){
            x = SCREEN_WIDTH*(float)rand()/((float)RAND_MAX);
            y = SCREEN_HEIGHT*(float)rand()/(2*(float)RAND_MAX);//on reste dans la moitié haute de l'écran
        }
        grilledennemis.ennemis[i].x = x;
        grilledennemis.ennemis[i].y = y;
        grilledennemis.ennemis[i].vx = 0;
        grilledennemis.ennemis[i].vy = ENNEMY_SPEED;
        grilledennemis.ennemis[i].w = ENNEMY_WIDTH;
        grilledennemis.ennemis[i].x = ENNEMY_HEIGHT;
    };
    return grilledennemis;
}
void liberegrille(grille grilledennemis){
    free(grilledennemis.ennemis);
    free(grilledennemis.vivant);
}
void game_over(grille grilledennemis,bool *running){
    liberegrille(grilledennemis);
    *running = false;
}