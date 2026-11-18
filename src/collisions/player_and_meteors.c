#include "collisions.h"
#include "../quantum.h"

void CollisionPlayerAndMeteors(Player *player, Meteor meteors[], GameScreen *currentScreen) {
  for (int i = 0; i < MAX_METEORS; i++) {
    bool collisionDetected = CheckCollisionRecs(
        (Rectangle){ 
          player->position.x - player->texture.width/2.0f,
          player->position.y - player->texture.height/2.0f,
          (float)player->texture.width,
          (float)player->texture.height }, 
        (Rectangle){ 
          meteors[i].position.x - meteors[i].texture.width/2.0f, 
          meteors[i].position.y - meteors[i].texture.height/2.0f,
          (float)meteors[i].texture.width,
          (float)meteors[i].texture.height }
      );
    if (collisionDetected) {
      player->hp -= meteors[i].damage;

      if (player->hp <= 0) {
        *currentScreen = LOST;
      }

      meteors[i].position = (Vector2){ GetRandomValue(0, GetScreenWidth()), GetRandomValue(-750, -10) };
      meteors[i].hp = meteors[i].maxHP;
    }
  }
}
