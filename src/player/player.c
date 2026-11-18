#include "player.h"
#include <raylib.h>

Player InitPlayer(Texture2D texture) {
  Player player = { texture, (Vector2){ (GetScreenWidth() - texture.width)/2.0f, (GetScreenHeight() - texture.height)/2.0f + 200.0f }, 400.0f, 700.0f };
  return player;
}

void MovePlayer(Player *player, float deltaTime) {
  if (IsKeyDown(KEY_DOWN) || IsKeyDown(KEY_S)) player->position.y += player->speed * deltaTime;
  if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) player->position.y -= player->speed * deltaTime;
  if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) player->position.x += player->speed * deltaTime;
  if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) player->position.x -= player->speed * deltaTime;

  if (player->position.y <= player->texture.height) player->position.y = player->texture.height;
  if (player->position.y >= GetScreenHeight() - player->texture.height) player->position.y = GetScreenHeight() - player->texture.height;
  if (player->position.x <= player->texture.width) player->position.x = player->texture.width;
  if (player->position.x >= GetScreenWidth()- player->texture.width) player->position.x = GetScreenWidth() - player->texture.width;
}

void DrawPlayer(Player player, float rotation) {
  DrawTexturePro(
      player.texture,
      (Rectangle){ 0, 0, player.texture.width, player.texture.height },
      (Rectangle){ player.position.x, player.position.y, player.texture.width, player.texture.height },
      (Vector2){ player.texture.width/2.0f, player.texture.height/2.0f },
      rotation + 90.0f,
      WHITE
  );
}

