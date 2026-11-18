#ifndef PLAYER_H
#define PLAYER_H

#include <raylib.h>

typedef struct Player {
  Texture2D texture;
  Vector2 position;
  float speed;
  float hp;
} Player;

Player InitPlayer(Texture2D texture);
void MovePlayer(Player *player, float deltaTime);
void DrawPlayer(Player player, float rotation);

#endif
