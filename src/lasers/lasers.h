#ifndef LASERS_H
#define LASERS_H

#include <raylib.h>
#include <math.h>
#include "../../src/player/player.h"

#define MAX_LASERS 50

typedef struct Laser {
  Texture2D texture;
  Vector2 position;
  Vector2 velocity;
  float damage;
  float rotation;
  bool active;
} Laser;

void InitLasers(Laser lasers[], Texture2D texture);
void FireLaser(Laser lasers[], Sound sound, Player player, float rotation);
void UpdateLaserPosition(Laser lasers[], float deltaTime);
void DrawLasers(Laser lasers[], Texture2D texture);

#endif
