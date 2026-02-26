#include <stdio.h> 
#include <time.h>
#include <stdlib.h>
#include "raylib.h"
#include "player.h"
#include "enemy.h"
#include "powerup.h"

/** Constants **/
const int SCREEN_WIDTH = 800; const int SCREEN_HEIGHT = 600;
const Color BACKGROUND_COLOR = { .r=0, .g=0, .b=0, .a=0xFF };
const Color TEXT_COLOR = {.r=0, .g=0xFF, .b=0xFF, .a=0xFF};

typedef enum GameState {
    TitleScreen = 0,
    GameActive,
    GameOver,
    ScoreScreen
} GameState;
GameState state;

/** Globals **/
Player *player;
Enemy **enemies;
PowerUp *powerup;

int enemycount;
float spawnrate = 0.02f;
float powerupSpawnRate = 0.02f;
int score = 0;
char playerScoreText[80];
char enemyCountText[80];
char effectiveSpawnRateText[80];
double lastTime = 0.0;
double elapsedTime = 0.0;

void initgame() 
{
    player = createPlayer();
    enemies = NULL;
    enemycount = 0;
    state = TitleScreen;
}


void update(float dt, float currTime) {

    double deltaTime = currTime - lastTime;
    elapsedTime += deltaTime;

    switch (state) {
        case TitleScreen:
            {
                if (GetKeyPressed() == KEY_SPACE) {
                    state = GameActive;
                }
                break;
            }
        case GameActive:
            {
                // Spawn Checks
                if (GetRandomValue(0, 99) < (int) (100 * spawnrate)) {
                    createEnemy();
                    spawnrate += 0.0005f;
                    if (spawnrate >= 1) spawnrate = 0.02f;
                }
                if (powerup == NULL && GetRandomValue(0, 1000) < powerupSpawnRate) {
                    powerup = createPowerUp();
                    powerupSpawnRate += 0.005f;
                    if (powerupSpawnRate >= 1) powerupSpawnRate = 0.02f;
                } 

                // Update Entities
                updateEnemies(dt);
                updatePlayer(dt);
                updatePowerUp(dt);

                // Collision Checks
                if (powerup != NULL && checkPlayerCollisionPowerUp(player, powerup)) {
                    for (int i = 0; i < enemycount; i++) {
                        enemies[i] = NULL;
                    }
                    powerup = NULL;
                    enemycount = 0;
                }

                for (int i = 0; i < enemycount; i++) {
                    Enemy *enemy = enemies[i];
                    if (checkPlayerCollisionEnemy(player, enemy)) {
                        player->alive = false;
                        break;
                    }
                }

                if (elapsedTime >= 1) {
                    score++;
                    elapsedTime = 0.0;
                }

                // Update HUD
                sprintf(enemyCountText, "Enemies: %d\n", enemycount);
                sprintf(playerScoreText, "Score: %d\n", score);
                sprintf(effectiveSpawnRateText, "Spawn Rate: %f\n", spawnrate);
            }
        case GameOver:
            {

                if (elapsedTime >= 2 && GetKeyPressed() == KEY_SPACE) {
                    state = ScoreScreen;
                    elapsedTime = 0.0;
                }
                break;
            }
        case ScoreScreen:
            {
                if (elapsedTime >= 2 && GetKeyPressed() == KEY_SPACE) {
                    elapsedTime = 0.0;
                    initgame();
                }
                break;
            }
    }

    lastTime = currTime;
}

void draw(void) {
        // Draw
        BeginDrawing();
        ClearBackground(BACKGROUND_COLOR);

        switch(state) {
            case TitleScreen:
            {
                DrawText("Dodge the Block!", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 40, TEXT_COLOR);
                DrawText("Press <Space> to Start.", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 + 40, 40, TEXT_COLOR);
                break;
            }
            case GameActive:
            {
                for (int i = 0; i < enemycount; i++) {
                    DrawRectangleV(enemies[i]->position, enemies[i]->size, enemies[i]->color);
                }
                DrawRectangleV(player->position, player->size, player->color);
                if (powerup != NULL) {
                    DrawRectangleV(powerup->position, powerup->size, powerup->color);
                }
                DrawText(playerScoreText, 20, 20, 20, TEXT_COLOR);
                DrawText(enemyCountText, 20, 40, 20, TEXT_COLOR);
                DrawText(effectiveSpawnRateText, 20, 60, 20, TEXT_COLOR);
                break;
            }
            case GameOver:
            {
                for (int i = 0; i < enemycount; i++) {
                    DrawRectangleV(enemies[i]->position, enemies[i]->size, enemies[i]->color);
                }
                DrawRectangleV(player->position, player->size, player->color);
                if (powerup != NULL) {
                    DrawRectangleV(powerup->position, powerup->size, powerup->color);
                }
                DrawText(playerScoreText, 20, 20, 20, TEXT_COLOR);
                DrawText(enemyCountText, 20, 40, 20, TEXT_COLOR);
                DrawText(effectiveSpawnRateText, 20, 60, 20, TEXT_COLOR);
                DrawText("GAME OVER", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 40, TEXT_COLOR);
                DrawText("Press <Space> to Continue", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 + 40, 40, TEXT_COLOR);
                break;
            }
            case ScoreScreen:
            {
                DrawText(playerScoreText, SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4, 40, TEXT_COLOR);
                DrawText("Press <Space> to Continue", SCREEN_WIDTH / 4, SCREEN_HEIGHT / 4 + 40, 40, TEXT_COLOR);
                break;
            }
        }

        EndDrawing();
}

int main(void)
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Dodge the Block!");
    SetRandomSeed(time(NULL));
    SetTargetFPS(60);

    initgame();

    while (!WindowShouldClose())
    {
        if (state == GameActive && !player->alive) {
            state = GameOver;
        }
        else if (state == ScoreScreen) {
        } 
        update(GetFrameTime(), GetTime());
        draw();
    }

    CloseWindow();

    return 0;
}
