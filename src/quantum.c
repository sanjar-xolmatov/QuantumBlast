#include "quantum.h"
#include "collisions/collisions.h"

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
        CollisionPlayerAndMeteors(&player, meteors);

        // Check collision between meteor and laser
        CollisionMeteorAndLasers(lasers, meteors);

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

