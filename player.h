#ifndef PLAYER_H
#define PLAYER_H

#include <stdlib.h>
#include "raylib.h"
#include "powerup.h"

#ifndef ENEMY_H
#include "enemy.h"
#endif

typedef struct PlayerSquare {
  bool alive;
  Vector2 position;
  Vector2 velocity;
  Vector2 size;
  Color color;
} Player;


Player *createPlayer();
bool checkPlayerCollisionPowerUp(Player*, PowerUp*);
bool checkPlayerCollisionEnemy(Player*, Enemy*);
void updatePlayer(float dt);

#endif
