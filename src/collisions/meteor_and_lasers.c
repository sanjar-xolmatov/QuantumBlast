#include "collisions.h"
#include <raylib.h>

void CollisionMeteorAndLasers(Laser lasers[], Meteor meteors[]) {
  for (int i = 0; i < MAX_LASERS; i++) {
    if (lasers[i].active == false) continue;

    for (int j = 0; j < MAX_METEORS; j++) {
      if (meteors[j].active == true) {
        bool collisionDetected = CheckCollisionRecs(
            (Rectangle){ 
              lasers[i].position.x - lasers[i].texture.width/2.0f,
              lasers[i].position.y - lasers[i].texture.height/2.0f,
              (float)lasers[i].texture.width,
              (float)lasers[i].texture.height,
            }, (Rectangle){
              meteors[j].position.x - meteors[j].texture.width/2.0f, 
              meteors[j].position.y - meteors[j].texture.height/2.0f,
              (float)meteors[j].texture.width,
              (float)meteors[j].texture.height
            }
            );

        if (collisionDetected) {
          lasers[i].active = false;
          meteors[j].hp -= lasers[i].damage; 
          if (meteors[j].hp <= 0) {
            meteors[j].position = (Vector2){ GetRandomValue(0, GetScreenWidth()), GetRandomValue(-750, -10) };
            meteors[j].hp = meteors[j].maxHP;
          }
          break;
        }
      }
    }
  }
}

