#include <stdlib.h>
#include "raylib.h"

#include "powerup.h"

#ifndef ENEMY_H
#include "enemy.h"
#endif


extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern Player *player;
extern Enemy **enemies;
extern PowerUp *powerup;


PowerUp *createPowerUp()
{
    int dir;
    int rX;
    if (GetRandomValue(0, 1) == 0)
    {
        dir = -1;
        rX = SCREEN_WIDTH;
    }
    else
    {
        dir = 1;
        rX = 0;
    }

    int rY = GetRandomValue(0, SCREEN_HEIGHT - 20);
    int rV = GetRandomValue(50, 150);

    PowerUp *p = malloc(sizeof(PowerUp)); 
    p->position.x = rX;
    p->position.y = rY;
    p->velocity.x = (rV * dir);
    p->velocity.y = 0;
    p->size.x     = 20;
    p->size.y     = 20;
    p->color.r    = 0;
    p->color.g    = 0xFF;
    p->color.b    = 0;
    p->color.a    = 0xFF;
    return p;
}


void updatePowerUp(float dt) 
{
    if (powerup == NULL) return;

    powerup->position.x += powerup->velocity.x*dt;
    powerup->position.y += powerup->velocity.y*dt;

    if (powerup->position.x < (-1 * powerup->size.x) || 
            powerup->position.x > SCREEN_WIDTH ||
            powerup->position.y < (-1 * powerup->size.y) ||
            powerup->position.y > SCREEN_HEIGHT) {
        powerup = NULL;
    }
}
