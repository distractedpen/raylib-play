#include "player.h"
#include "enemy.h"

extern Player *player;
extern Enemy **enemies;
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;

const float ACC = 10;

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


bool checkPlayerCollisionEnemy(Player *player, Enemy *e) {
  if (player->position.x < e->position.x +  e->size.x  &&
      player->position.x + player->size.x > e->position.x &&
      player->position.y < e->position.y +  e->size.y &&
      player->position.y + player->size.y > e->position.y ) {
    return true;
  }
  return false;
}


bool checkPlayerCollisionPowerUp(Player *player, PowerUp *p) {
  if (player->position.x < p->position.x +  p->size.x  &&
      player->position.x + player->size.x > p->position.x &&
      player->position.y < p->position.y +  p->size.y &&
      player->position.y + player->size.y > p->position.y ) {
    return true;
  }
  return false;
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
