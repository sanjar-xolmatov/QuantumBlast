#include "quantum.h"

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
  Player player = InitPlayer(playerTexture);

  // Mouse 
  Vector2 mousePosition = { 0 };
  float rotation = 0.0f;

  // Laser array
  InitLasers(lasers, playerLaser);
  // Meteor array
  InitMeteors(meteors, meteorTextures);  

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
        MovePlayer(&player, deltaTime);

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
              bool collisionDetected = CheckCollisionPointRec(
                  (Vector2){
                    lasers[i].position.x,
                    lasers[i].position.y - lasers[i].texture.height/2.0f
                  },
                  (Rectangle){ 
                    meteors[j].position.x - meteors[j].texture.width/2.0f, 
                    meteors[j].position.y - meteors[j].texture.height/2.0f,
                    (float)meteors[j].texture.width,
                    (float)meteors[j].texture.height }
                  );

              if (collisionDetected) {
                lasers[i].active = false;
                meteors[j].hp -= lasers[i].damage; 
                if (meteors[j].hp <= 0) 
                  meteors[j].position = (Vector2){ GetRandomValue(0, screenWidth), GetRandomValue(-750, -10) };
                break;
              }
            }
          }
        }

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
        DrawPlayer(player, rotation);

        // Player HP
        DrawRectangleRec(
            (Rectangle){ 5.0f, 5.0f, (700.0f * player.hp/700.0f), 30.0f },
            RED
        );
        DrawRectangleLinesEx((Rectangle){ 5.0f, 5.0f, 700.0f, 30.0f }, 1.0f, RAYWHITE);

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

