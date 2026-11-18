#include "quantum.h"
#include "collisions/collisions.h"
#include <raylib.h>

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
  Texture2D cursorCrossTexture = LoadTexture("./assets/textures/ui/cursors/cursor_e.png");
  Texture2D cursorNormalTexture = LoadTexture("./assets/textures/ui/cursors/cursor.png");
  Texture2D playerLaser = LoadTexture("./assets/textures/lasers/laserBlue01.png");

  Texture2D meteorTextures[] = {
    LoadTexture("./assets/textures/meteors/meteorBrown_small1.png"),
    LoadTexture("./assets/textures/meteors/meteorBrown_med1.png"),
    LoadTexture("./assets/textures/meteors/meteorBrown_big2.png")
  };

  // Fonts
  Font kenvectorFuture = LoadFont("./assets/fonts/kenvector_future.ttf");

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
        if (IsKeyPressed(KEY_M)) currentScreen = LOST;
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
        CollisionPlayerAndMeteors(&player, meteors, &currentScreen);

        // Check collision between meteor and laser
        CollisionMeteorAndLasers(lasers, meteors);

        break;
      }
      case LOST:
      {
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
        Rectangle playerHPIndicator = { 5.0f, 5.0f, (700 * player.hp/player.maxHP), 30.0f };

        DrawRectangleRec( playerHPIndicator, RED );

        playerHPIndicator.width = 700.0f;
        DrawRectangleLinesEx(playerHPIndicator, 2.0f, RAYWHITE);

        DrawTexture(cursorCrossTexture, mousePosition.x, mousePosition.y, WHITE);
        break;
      }
      case LOST:
      {
        ClearBackground(BLACK);
        DrawTextEx(kenvectorFuture, "LOST", (Vector2){ (GetScreenWidth() - MeasureText("LOST", 80))/2.0f, (GetScreenHeight() - 80)/2.0f }, 80, 1.0f, RED); 

        DrawTexture(cursorNormalTexture, mousePosition.x, mousePosition.y, WHITE);

        break;
      }
    }


    EndDrawing();
  }

  UnloadTexture(background);
  UnloadTexture(playerTexture);
  UnloadTexture(cursorCrossTexture);
  UnloadTexture(cursorNormalTexture);
  UnloadTexture(playerLaser);

  for (long unsigned int i = 0; i < sizeof(meteorTextures) / sizeof(meteorTextures[0]); i++) {
    UnloadTexture(meteorTextures[i]);
  }

  UnloadFont(kenvectorFuture);

  UnloadSound(laserSmall);

  CloseAudioDevice();

  CloseWindow();

  return 0;

}

