#include "quantum.h"
#include "../src/meteors/meteors.h"
#include <raylib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_LASERS 50

int main(void) {
  const int screenHeight = 1080;
  const int screenWidth = 1920;

  Laser lasers[MAX_LASERS] = { 0 };
  Meteor meteors[MAX_METEORS] = { 0 };

  InitWindow(screenWidth, screenHeight, "Quantum Blast");
  InitAudioDevice();

  ToggleFullscreen();
  HideCursor();

  GameScreen currentScreen = GAMEPLAY;

  // Textures 
  Texture2D background = LoadTexture("./assets/textures/background/background.png");
  Texture2D playerTexture = LoadTexture("./assets/textures/player/playerShip1_blue.png");
  Texture2D cursorTexture = LoadTexture("./assets/textures/ui/cursors/cursor_e.png");
  Texture2D playerLaser = LoadTexture("./assets/textures/lasers/laserBlue01.png");

  Texture2D meteorTextures[] = {
    LoadTexture("./assets/textures/meteors/meteorBrown_small1.png"),
    LoadTexture("./assets/textures/meteors/meteorBrown_med1.png"),
    LoadTexture("./assets/textures/meteors/meteorBrown_big2.png")
  };

  // Sounds
  Sound laserSmall = LoadSound("./assets/audio/laserSmall_004.ogg");

  // Player 
  Player player = { playerTexture, (Vector2){ (screenWidth - playerTexture.width)/2.0f, (screenHeight - playerTexture.height)/2.0f + 200.0f }, 400.0f, 500.0f };

  // Mouse 
  Vector2 mousePosition = { 0 };
  float rotation = 0.0f;

  // Laser array
  for (int i = 0; i < MAX_LASERS; i++) {
    Laser laser = { playerLaser, (Vector2){ 0, 0 }, (Vector2){ 800.0f * cosf(rotation * (M_PI/180.0f)), 800.0f * sinf(rotation * (M_PI/180.0f)) }, 100.0f, 0.0f, false };
    lasers[i] = laser;
  }
  // Meteor array
  InitMeteors(meteors, meteorTextures);  

  // Player HP text
  char hpText[100];

  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    float deltaTime = GetFrameTime();

    mousePosition = GetMousePosition();
    rotation = atan2f(mousePosition.y - player.position.y, mousePosition.x - player.position.x) * (180 / M_PI);

    switch (currentScreen) {

      case TITLE:
      {
        break;
      }
      case GAMEPLAY:
      {
        // Fire Laser
        if (IsKeyPressed(KEY_E) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
          for (int i = 0; i < MAX_LASERS; i++) {
            if (lasers[i].active == false) {
              PlaySound(laserSmall);

              lasers[i].active = true;
              lasers[i].rotation = rotation;
              lasers[i].position.x = player.position.x + (40.0f * cosf(lasers[i].rotation * (M_PI/180.0f)));
              lasers[i].position.y = player.position.y + (40.0f * sinf(lasers[i].rotation * (M_PI/180.0f)));
              lasers[i].velocity = (Vector2){ 800.0f * cosf(lasers[i].rotation * (M_PI/180.0f)), 800.0f * sinf(lasers[i].rotation * (M_PI/180.0f)) };
              break;
            }
          }
        }

        // Move Player
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) player.position.y += player.speed * deltaTime;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) player.position.y -= player.speed * deltaTime;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.position.x += player.speed * deltaTime;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.position.x -= player.speed * deltaTime;

        if (player.position.y <= player.texture.height) player.position.y = player.texture.height;
        if (player.position.y >= screenHeight - player.texture.height) player.position.y = screenHeight - player.texture.height;
        if (player.position.x <= player.texture.width) player.position.x = player.texture.width;
        if (player.position.x >= screenWidth - player.texture.width) player.position.x = screenWidth - player.texture.width;

        for (int i = 0; i < MAX_LASERS; i++) {
          if (lasers[i].active == true) {

            lasers[i].position.x += lasers[i].velocity.x * deltaTime;
            lasers[i].position.y += lasers[i].velocity.y * deltaTime;
            
            if (lasers[i].position.x - lasers[i].texture.width - 30.0f > screenWidth) lasers[i].active = false;
            if (lasers[i].position.x + 30.0f < 0) lasers[i].active = false;
            if (lasers[i].position.y - lasers[i].texture.height - 30.0f > screenHeight) lasers[i].active = false;
            if (lasers[i].position.y + 30.0f < 0) lasers[i].active = false;
          }
        }

        // Spawn meteors
        SpawnMeteors(meteors, deltaTime);

        // Check collision between player and meteors
        for (int i = 0; i < MAX_METEORS; i++) {
          bool collisionDetected = CheckCollisionRecs(
              (Rectangle){ 
                player.position.x - player.texture.width/2.0f,
                player.position.y - player.texture.height/2.0f,
                (float)player.texture.width,
                (float)player.texture.height }, 
              (Rectangle){ 
                meteors[i].position.x - meteors[i].texture.width/2.0f, 
                meteors[i].position.y - meteors[i].texture.height/2.0f,
                (float)meteors[i].texture.width,
                (float)meteors[i].texture.height }
            );
          if (collisionDetected) {
            player.hp -= meteors[i].damage;
            meteors[i].position = (Vector2){ GetRandomValue(0, screenWidth), GetRandomValue(-750, -10) };
          }
        }

        // Check collision between meteor and laser
        for (int i = 0; i < MAX_LASERS; i++) {
          if (lasers[i].active == false) continue;
          for (int j = 0; j < MAX_METEORS; j++) {
            if (lasers[i].active == true && meteors[j].active == true) {
              bool collisionDetected = CheckCollisionRecs(
                  (Rectangle){ 
                  lasers[i].position.x - lasers[i].texture.width/2.0f,
                  lasers[i].position.y - lasers[i].texture.height/2.0f,
                  (float)lasers[i].texture.width,
                  (float)lasers[i].texture.height }, 
                  (Rectangle){ 
                  meteors[j].position.x - meteors[j].texture.width/2.0f, 
                  meteors[j].position.y - meteors[j].texture.height/2.0f,
                  (float)meteors[j].texture.width,
                  (float)meteors[j].texture.height }
                  );

              if (collisionDetected) {
                lasers[i].active = false;
                meteors[j].position = (Vector2){ GetRandomValue(0, screenWidth), GetRandomValue(-750, -10) };
                break;
              }
            }
          }
        }

        sprintf(hpText, "HEALTH: %.0f", player.hp);

        break;
      }
    }
    
    BeginDrawing();
    switch (currentScreen) {
      case TITLE:
      {
        break;
      }
      case GAMEPLAY:
      {
        ClearBackground(WHITE);  

        DrawTexture(background, 0, 0, WHITE);

        // Draw Lasers
        for (int i = 0; i < MAX_LASERS; i++) {
          if (lasers[i].active == true) {
            DrawTexturePro(
                playerLaser,
                (Rectangle){ 0, 0, playerLaser.width, playerLaser.height },
                (Rectangle){ lasers[i].position.x, lasers[i].position.y, lasers[i].texture.width, lasers[i].texture.height },
                (Vector2){ lasers[i].texture.width/2.0f, lasers[i].texture.height/2.0f },
                lasers[i].rotation + 90.0f,
                WHITE);
          }
        }

        // Draw Meteors
        DrawMeteors(meteors);

        // Draw Player
        DrawTexturePro(
            player.texture,
            (Rectangle){ 0, 0, player.texture.width, player.texture.height },
            (Rectangle){ player.position.x, player.position.y, player.texture.width, player.texture.height },
            (Vector2){ player.texture.width/2.0f, player.texture.height/2.0f },
            rotation + 90.0f,
            WHITE);

        DrawText(hpText, 10, 10, 30, RED);

        break;
      }
    }

    DrawTexture(cursorTexture, mousePosition.x, mousePosition.y, WHITE);

    EndDrawing();
  }

  UnloadTexture(background);
  UnloadTexture(playerTexture);
  UnloadTexture(cursorTexture);
  UnloadTexture(playerLaser);

  for (long unsigned int i = 0; i < sizeof(meteorTextures) / sizeof(meteorTextures[0]); i++) {
    UnloadTexture(meteorTextures[i]);
  }

  UnloadSound(laserSmall);

  CloseAudioDevice();

  CloseWindow();

  return 0;

}

