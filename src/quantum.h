#ifndef QUANTUM_H
#define QUANTUM_H

#include <raylib.h>

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

typedef struct Meteor {
  Texture2D texture;
  Vector2 position;
  float speed;
  float damage;
  float rotationSpeed;
  float rotation;
  bool active;
} Meteor;

typedef enum GameScreen {
  TITLE = 0,
  GAMEPLAY,
} GameScreen;

#endif
