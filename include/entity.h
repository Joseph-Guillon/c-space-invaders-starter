#ifndef ENTITY_H
#define ENTITY_H

typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
} Entity;

#endif
typedef struct {
    size_t nbre_ennemis;
    Entity *ennemis;
    bool *vivant;
    bool *ennemy_bullet_active;
    Entity * ennemy_bullet;
} grille;
