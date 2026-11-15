#include "lasers.h"
#include <raylib.h>

void InitLasers(Laser lasers[], Texture2D texture) {
  for (int i = 0; i < MAX_LASERS; i++) {
    Laser laser = { texture, (Vector2){ 0, 0 }, (Vector2){ 0, 0 }, 100.0f, 0.0f, false };
    lasers[i] = laser;
  }
}

void FireLaser(Laser lasers[], Sound sound, Player player, float rotation) {
  for (int i = 0; i < MAX_LASERS; i++) {
    if (lasers[i].active == false) {
      PlaySound(sound);

      lasers[i].active = true;
      lasers[i].rotation = rotation;
      lasers[i].position.x = player.position.x + (40.0f * cosf(lasers[i].rotation * (M_PI/180.0f)));
      lasers[i].position.y = player.position.y + (40.0f * sinf(lasers[i].rotation * (M_PI/180.0f)));
      lasers[i].velocity = (Vector2){ 800.0f * cosf(lasers[i].rotation * (M_PI/180.0f)), 800.0f * sinf(lasers[i].rotation * (M_PI/180.0f)) };
      break;
    }
  }
}

void UpdateLaserPosition(Laser lasers[], float deltaTime) {
  for (int i = 0; i < MAX_LASERS; i++) {
    if (lasers[i].active == true) {

      lasers[i].position.x += lasers[i].velocity.x * deltaTime;
      lasers[i].position.y += lasers[i].velocity.y * deltaTime;
        
        if (lasers[i].position.x - lasers[i].texture.width - 30.0f > GetScreenWidth()) lasers[i].active = false;
        if (lasers[i].position.x + 30.0f < 0) lasers[i].active = false;
        if (lasers[i].position.y - lasers[i].texture.height - 30.0f > GetScreenHeight()) lasers[i].active = false;
        if (lasers[i].position.y + 30.0f < 0) lasers[i].active = false;
      }
    }
}

void DrawLasers(Laser lasers[], Texture2D texture) {
  for (int i = 0; i < MAX_LASERS; i++) {
    if (lasers[i].active == true) {
      DrawTexturePro(
          texture,
          (Rectangle){ 0, 0, texture.width, texture.height },
          (Rectangle){ lasers[i].position.x, lasers[i].position.y, lasers[i].texture.width, lasers[i].texture.height },
          (Vector2){ lasers[i].texture.width/2.0f, lasers[i].texture.height/2.0f },
          lasers[i].rotation + 90.0f,
          WHITE);
    }
  }
}

