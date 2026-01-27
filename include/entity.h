#ifndef ENTITY_H
#define ENTITY_H

typedef struct
{
    float x, y;
    float vx, vy;
    int w, h;
} Entity;
typedef struct {
    size_t nbre_ennemis;
    Entity *ennemis;
    bool *vivant;
} grille;
#endif

