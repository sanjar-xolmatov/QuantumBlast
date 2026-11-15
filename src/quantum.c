#include "quantum.h"
#include "../src/meteors/meteors.h"
#include "../src/lasers/lasers.h"
#include <raylib.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>


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
  InitLasers(lasers, playerLaser);
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
          FireLaser(lasers, laserSmall, player, rotation);
        }
        UpdateLaserPosition(lasers, deltaTime);

        // Move Player
        if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) player.position.y += player.speed * deltaTime;
        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) player.position.y -= player.speed * deltaTime;
        if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player.position.x += player.speed * deltaTime;
        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player.position.x -= player.speed * deltaTime;

        if (player.position.y <= player.texture.height) player.position.y = player.texture.height;
        if (player.position.y >= screenHeight - player.texture.height) player.position.y = screenHeight - player.texture.height;
        if (player.position.x <= player.texture.width) player.position.x = player.texture.width;
        if (player.position.x >= screenWidth - player.texture.width) player.position.x = screenWidth - player.texture.width;

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
        DrawLasers(lasers, playerLaser);

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

