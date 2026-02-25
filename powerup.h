#ifndef POWERUP_H
#define POWERUP_H

#include "raylib.h"


typedef struct PowerUpSquare {
  Vector2 position;
  Vector2 velocity;
  Vector2 size;
  Color color;
} PowerUp;

PowerUp *createPowerUp();
void updatePowerUp(float);

#endif
