#include <stdio.h> 
#include <time.h>
#include <stdlib.h>
#include "raylib.h"

/** Constants **/
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const Color BACKGROUND_COLOR = { .r=0, .g=0, .b=0, .a=0xFF };
const Color TEXT_COLOR = {.r=0, .g=0xFF, .b=0xFF, .a=0xFF};
const float ACC = 10;
const float ENEMY_ACC = 4;
const float ENEMY_SPAWN_RATE = 0.02f;
const float SPAWN_RATE_MULTI = 0.01f;
const float POWER_UP_SPAWN_RATE = 0.01f;


/** Data Defs **/
typedef enum EnemyType {
  VERTICAL,
  HORIZONTAL,
  CHASER,
} EnemyType;

typedef struct PlayerSquare {
  bool alive;
  Vector2 position;
  Vector2 velocity;
  Vector2 size;
  Color color;
} Player;

typedef struct EnemySquare {
  Vector2 position;
  Vector2 velocity;
  Vector2 size;
  Color color;
  EnemyType type;
} Enemy;

typedef struct PowerUpSquare {
  Vector2 position;
  Vector2 velocity;
  Vector2 size;
  Color color;
} PowerUp;


/** Global Vars **/
Player *player;
Enemy **enemies;
PowerUp *powerup;
int enemyCount;


/** Functions **/
bool checkPlayerCollisionEnemy(Enemy *e) {
  if (player->position.x < e->position.x +  e->size.x  &&
      player->position.x + player->size.x > e->position.x &&
      player->position.y < e->position.y +  e->size.y &&
      player->position.y + player->size.y > e->position.y ) {
    return true;
  }
  return false;
}

bool checkPlayerCollisionPowerUp(PowerUp *p) {
  if (player->position.x < p->position.x +  p->size.x  &&
      player->position.x + player->size.x > p->position.x &&
      player->position.y < p->position.y +  p->size.y &&
      player->position.y + player->size.y > p->position.y ) {
    return true;
  }
  return false;
}

Player *createPlayer() {
  Player *player = malloc(sizeof(Player));
  Vector2 pos = {.x=100,  .y=100};
  Vector2 vel = {.x=0,    .y=0};
  Vector2 siz = {.x=20,   .y=20};
  Color   col = {.r=0xA0, .g=0, .b=0xA0, .a=0xFF};

  player->alive = true;
  player->position = pos;
  player->velocity = vel;
  player->size     = siz;
  player->color    = col;
  return player;
}

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

  int rX = GetRandomValue(0, SCREEN_HEIGHT - 20);
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


void updatePlayer(float dt) {
  Vector2 nextPos = {0};

  if (IsKeyDown(KEY_W)) {
    player->velocity.y -= ACC;
  }
  if (IsKeyDown(KEY_S)) {
    player->velocity.y += ACC;
  }
  if (IsKeyDown(KEY_A)) {
    player->velocity.x -= ACC;
  }
  if (IsKeyDown(KEY_D)) {
    player->velocity.x += ACC;
  }

  if (!IsKeyDown(KEY_W) &&
      !IsKeyDown(KEY_S) && !IsKeyDown(KEY_A) &&
      !IsKeyDown(KEY_D)) {

    if (player->velocity.x > 0)
      player->velocity.x -= ACC;
    else if (player->velocity.x < 0)
      player->velocity.x += ACC;

    if (player->velocity.y > 0)
      player->velocity.y -= ACC;
    else if (player->velocity.y < 0)
      player->velocity.y += ACC;
  }

  Vector2 newPos = {
    .x=player->position.x + (player->velocity.x*dt),
    .y=player->position.y + (player->velocity.y*dt)
  };

  if (newPos.x < 0 || newPos.x > SCREEN_WIDTH - player->size.x) {
    newPos.x = player->position.x;
  }
  if (newPos.y < 0 || newPos.y > SCREEN_HEIGHT - player->size.y) {
    newPos.y = player->position.y;
  }

  player->position.x=newPos.x;
  player->position.y=newPos.y;
}

int updateEnemies(float dt) {

  for (int i = 0; i < enemyCount; i++) {
    Enemy *e = enemies[i];

    if (e != NULL) {
      // Remove enemies off screen
      if (e->position.x < (-1 * e->size.x) || 
	  e->position.x > SCREEN_WIDTH ||
	  e->position.y < (-1 * e->size.y) ||
	  e->position.y > SCREEN_HEIGHT) {
	enemies[i] = NULL;
	for (int j = i; j < enemyCount - 1; j++) {
	  enemies[j] = enemies[j + 1];
	}
	enemyCount--;
	continue;
      }

      switch(e->type) {
      case CHASER:
	int dirx;
	if (player->position.x < e->position.x) {
	  dirx = -1;
	} else {
	  dirx = 1;
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
  return enemyCount;
}


void updatePowerUp(float dt) 
{
  powerup->position.x += powerup->velocity.x*dt;
  powerup->position.y += powerup->velocity.y*dt;

  if (powerup->position.x < (-1 * powerup->size.x) || 
      powerup->position.x > SCREEN_WIDTH ||
      powerup->position.y < (-1 * powerup->size.y) ||
      powerup->position.y > SCREEN_HEIGHT) {
    powerup = NULL;
  }

}

int main(void)
{
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Raylib playtime!");
  SetRandomSeed(time(NULL));
  SetTargetFPS(60);

  float spawnRate = ENEMY_SPAWN_RATE;
  int score = 0;
  char playerScoreText[80];
  char enemyCountText[80];
  char effectiveSpawnRateText[80];
  bool collisionExists = false;
  player = createPlayer();
	
  enemies = NULL;
  enemyCount = 0;

  double lastTime = 0.0;
  double elapsedTime = 0.0;
  while (!WindowShouldClose())
    {
      if (!player->alive) {
	break;
      }
      float dt = GetFrameTime();
      double currTime = GetTime();

      if (GetRandomValue(0, 99) < (int) (100 * spawnRate)) {
	int enemyType = GetRandomValue(0,3);
	Enemy *e = NULL;
	switch (enemyType) {
	case 0:
	    e = createHorizontalEnemy();
	    enemies = realloc(enemies, sizeof(Enemy *) * (enemyCount + 1));
	    enemies[enemyCount] = e;
	    enemyCount++;
	  break;
	case 1:
	    e = createVerticalEnemy();
	    enemies = realloc(enemies, sizeof(Enemy *) * (enemyCount + 1));
	    enemies[enemyCount] = e;
	    enemyCount++;
	  break;
	case 2:
	    e = createChaserEnemy();
	    enemies = realloc(enemies, sizeof(Enemy *) * (enemyCount + 1));
	    enemies[enemyCount] = e;
	    enemyCount++;
	  break;
	default:
	  break;
	}

	if (GetRandomValue(0, 99) < 10) {
	    spawnRate += spawnRate * SPAWN_RATE_MULTI;
	}
      }

      // chance to spawn power up
      if (powerup == NULL && GetRandomValue(0, 1000) < POWER_UP_SPAWN_RATE) {
	powerup = createPowerUp();
      } 

      if (powerup != NULL)
	{
	  updatePowerUp(dt);
	  if (powerup != NULL && checkPlayerCollisionPowerUp(powerup)) {
	    for (int i = 0; i < enemyCount; i++) {
	      enemies[i] = NULL;
	    }
	    powerup = NULL;
	    enemyCount = 0;
	  }
	}

      updatePlayer(dt);
      enemyCount = updateEnemies(dt);

      for (int i = 0; i < enemyCount; i++) {
	Enemy *enemy = enemies[i];
	if (checkPlayerCollisionEnemy(enemy)) {
	  player->alive = false;
	  break;
	}
      }

      double deltaTime = currTime - lastTime;
      elapsedTime += deltaTime;
      if (elapsedTime >= 1) {
	score++;
	elapsedTime = 0.0;
      }
      lastTime = currTime;

      sprintf(enemyCountText, "Enemies: %d\n", enemyCount);
      sprintf(playerScoreText, "Score: %d\n", score);
      sprintf(effectiveSpawnRateText, "Spawn Rate: %f\n", spawnRate);
		
		
      // Draw
      BeginDrawing();
      ClearBackground(BACKGROUND_COLOR);
      for (int i = 0; i < enemyCount; i++) {
	DrawRectangleV(enemies[i]->position, enemies[i]->size, enemies[i]->color);
      }
      DrawRectangleV(player->position, player->size, player->color);
      if (powerup != NULL) {
	DrawRectangleV(powerup->position, powerup->size, powerup->color);
      }
      DrawText(playerScoreText, 20, 20, 20, TEXT_COLOR);
      DrawText(enemyCountText, 20, 40, 20, TEXT_COLOR);
      DrawText(effectiveSpawnRateText, 20, 60, 20, TEXT_COLOR);
      EndDrawing();
    }

  CloseWindow();

  return 0;
}
