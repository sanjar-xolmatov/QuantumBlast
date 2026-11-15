#ifndef QUANTUM_H
#define QUANTUM_H

#include <raylib.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef struct Player {
  Texture2D texture;
  Vector2 position;
  float speed;
  float hp;
} Player;

typedef struct Laser {
  Texture2D texture;
  Vector2 position;
  Vector2 velocity;
  float damage;
  float rotation;
  bool active;
} Laser;

typedef enum GameScreen {
  TITLE = 0,
  GAMEPLAY,
} GameScreen;

#endif
