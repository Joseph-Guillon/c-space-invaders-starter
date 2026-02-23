#include <SDL2/SDL.h>
#include <stdbool.h>
#include "entity.h"
#include "game.h"
#include <stdlib.h>
struct grille{
    size_t nbre_ennemis;
    Entity *ennemis;
    bool *vivant;
    bool *ennemy_bullet_active;
    Entity * ennemy_bullet;
}
typedef struct grille grille;
//grille creer_grille(int n){
//    grille grilledennemis;
//    grilledennemis.nbre_ennemis= n;
//    grilledennemis.ennemis = malloc(n*sizeof(Entity));
//    grilledennemis.vivant = malloc(n*sizeof(bool));
//    for(size_t i = 0; i<N;i++){//cette fonction ne doit pas être appelée avec autre chose qu'N
//        grilledennemis.vivant[i]=true;
//    }
//    for(size_t i = 0; i<N;i++){
//        float x;
//        float y;
//        while(nook(grilledennemis,x,y,i)){
//            x = SCREEN_WIDTH*(float)rand()/((float)RAND_MAX);
//            y = SCREEN_HEIGHT*(float)rand()/(2*(float)RAND_MAX);//on reste dans la moitié haute de l'écran
//        }
//        grilledennemis.ennemis[i].x = x;
//        grilledennemis.ennemis[i].y = y;
//        grilledennemis.ennemis[i].vx = 0;
//        grilledennemis.ennemis[i].vy = ENNEMY_SPEED;
//        grilledennemis.ennemis[i].w = ENNEMY_WIDTH;
//        grilledennemis.ennemis[i].x = ENNEMY_HEIGHT;
//    };
//    return grilledennemis;
//}
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
void liberegrille(grille grilledennemis){
    free(grilledennemis.ennemis);
    free(grilledennemis.vivant);
}
void game_over(grille grilledennemis,bool *running){
    liberegrille(grilledennemis);
    *running = false;
}