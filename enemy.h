#ifndef ENEMY_H
#define ENEMY_H

#include <stdlib.h>
#include "raylib.h"

typedef enum EnemyType {
  VERTICAL,
  HORIZONTAL,
  CHASER,
} EnemyType;

typedef struct EnemySquare {
  Vector2 position;
  Vector2 velocity;
  Vector2 size;
  Color color;
  EnemyType type;
} Enemy;

#ifndef PLAYER_H
#include "player.h"
#endif



void createEnemy();
void updateEnemies(float);

#endif
