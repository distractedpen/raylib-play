
#include "enemy.h"

extern const int SCREEN_WIDTH;
extern const int SCREEN_HEIGHT;
extern Player *player;
extern Enemy **enemies;
extern int enemycount;
extern float spawnrate;

const float ENEMY_ACC = 4;

Enemy *createHorizontalEnemy() 
{
    int dir;
    int rX;
    if (GetRandomValue(0, 1) == 0) {
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

    Enemy *e = malloc(sizeof(Enemy));
    e->type       = HORIZONTAL;
    e->position.x = rX;
    e->position.y = rY;
    e->velocity.x = (rV * dir);
    e->velocity.y = 0;
    e->size.x     = 20;
    e->size.y     = 20;
    e->color.r    = 0xFF;
    e->color.g    = 0;
    e->color.b    = 0;
    e->color.a    = 0xFF;
    return e;
}



Enemy *createChaserEnemy() 
{
    int xdir;
    int rX;
    if (GetRandomValue(0, 1) == 0)
    {
        xdir = -1;
        rX = SCREEN_WIDTH;
    }
    else
    {
        xdir = 1;
        rX = 0;
    }

    int ydir;
    int rY;
    if (GetRandomValue(0, 1) == 0)
    {
        ydir = -1;
        rY = SCREEN_WIDTH;
    }
    else
    {
        ydir = 1;
        rY = 0;
    }
    int vX = GetRandomValue(50, 150);
    int vY = GetRandomValue(50, 150);

    Enemy *e = malloc(sizeof(Enemy));
    e->type       = CHASER;
    e->position.x = rX;
    e->position.y = rY;
    e->velocity.x = (vX * xdir);
    e->velocity.y = (vY * ydir);
    e->size.x     = 20;
    e->size.y     = 20;
    e->color.r    = 0;
    e->color.g    = 0;
    e->color.b    = 0xFF;
    e->color.a    = 0xFF;
    return e;
}

Enemy *createVerticalEnemy() 
{
    int dir;
    int rY;
    if (GetRandomValue(0, 1) == 0) {
        dir = -1;
        rY = SCREEN_HEIGHT;
    }
    else
    {
        dir = 1;
        rY = 0;
    }

    int rX = GetRandomValue(0, SCREEN_WIDTH - 20);
    int rV = GetRandomValue(50, 150);

    Enemy *e = malloc(sizeof(Enemy));
    e->type       = VERTICAL;
    e->position.x = rX;
    e->position.y = rY;
    e->velocity.x = 0;
    e->velocity.y = (rV * dir);
    e->size.x     = 20;
    e->size.y     = 20;
    e->color.r    = 0xFF;
    e->color.g    = 0;
    e->color.b    = 0;
    e->color.a    = 0xFF;
    return e;
}

void updateEnemies(float dt) {


    for (int i = 0; i < enemycount; i++) {
        Enemy *e = enemies[i];

        if (e != NULL) {
            // Remove enemies off screen
            if (e->position.x < (-1 * e->size.x) || 
                    e->position.x > SCREEN_WIDTH ||
                    e->position.y < (-1 * e->size.y) ||
                    e->position.y > SCREEN_HEIGHT) {
                enemies[i] = NULL;
                for (int j = i; j < enemycount - 1; j++) {
                    enemies[j] = enemies[j + 1];
                }
                enemycount--;
                continue;
            }

            switch(e->type) {
                case CHASER:
                    {
                        int dirx;
                        if (player->position.x < e->position.x) {
                            dirx = -1;
                        } else { dirx = 1;
                        }

                        int diry;
                        if (player->position.y < e->position.y) {
                            diry = -1;
                        } else {
                            diry = 1;
                        }

                        e->velocity.x += dirx * ENEMY_ACC;
                        e->velocity.y += diry * ENEMY_ACC;
                        e->position.x += e->velocity.x * dt;
                        e->position.y += e->velocity.y * dt;
                        break;
                    }
                case HORIZONTAL:
                case VERTICAL:
                    e->position.x += e->velocity.x*dt;
                    e->position.y += e->velocity.y*dt;
                    break;
                default:
                    break;
            }
        }

    }
}

void createEnemy() {

    int enemyType = GetRandomValue(0,3);
    Enemy *e = NULL;
    switch (enemyType) {
        case 0:
            e = createHorizontalEnemy();
            break;
        case 1:
            e = createVerticalEnemy();
            break;
        case 2:
            e = createChaserEnemy();
            break;
        default:
            break;
    }

    if (e != NULL) {
        enemies = realloc(enemies, sizeof(Enemy *) * (enemycount + 1));
        enemies[enemycount] = e;
        enemycount++;
    }
}

