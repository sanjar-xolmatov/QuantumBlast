#include "meteors.h"

int InitMeteors(Meteor meteors[], Texture2D textures[]) {
  for (int i = 0; i < MAX_METEORS; i++) {
    int meteorType = GetRandomValue(0, 2);
    if (meteorType == 0) {
      Meteor meteor = {
        .texture=textures[0],
        .position=(Vector2){ GetRandomValue(20, GetScreenWidth() - 20), GetRandomValue(-750, -10) },
        .speed=220.0f,
        .damage=25.0f,
        .rotationSpeed=GetRandomValue(-50, 50),
        .rotation=0,
        .hp=100.0f,
        .active=true
      };
      meteors[i] = meteor;
    } else if (meteorType == 1) {
      Meteor meteor = {
        .texture=textures[1],
        .position=(Vector2){ GetRandomValue(20, GetScreenWidth() - 20), GetRandomValue(-750, -10) },
        .speed=170.0f,
        .damage=50.0f,
        .rotationSpeed=GetRandomValue(-40, 40),
        .rotation=0,
        .hp=200.0f,
        .active=true
      };
      meteors[i] = meteor;
    } else {
      Meteor meteor = {
        .texture=textures[2],
        .position=(Vector2){ GetRandomValue(20, GetScreenWidth() - 20), GetRandomValue(-750, -10) },
        .speed=130.0f,
        .damage=100.0f,
        .rotationSpeed=GetRandomValue(-25, 25),
        .rotation=0,
        .hp=350.0f,
        .active=true
      };
      meteors[i] = meteor;
    }
  }
  return 0;
}

int SpawnMeteors(Meteor meteors[], float deltaTime) {
  for (int i = 0; i < MAX_METEORS; i++) {
      meteors[i].position.y += meteors[i].speed * deltaTime;
      meteors[i].rotation += meteors[i].rotationSpeed * deltaTime;

      if (meteors[i].position.y - meteors[i].texture.height - 30.0f >= GetScreenHeight())
        meteors[i].position = (Vector2){ GetRandomValue(0, GetScreenHeight()), GetRandomValue(-100, -10) };
  }

  return 0;
}

void DrawMeteors(Meteor meteors[]) {
  for (int i = 0; i < MAX_METEORS; i++) {
    DrawTextureEx(
        meteors[i].texture,
        meteors[i].position, 
        meteors[i].rotation,
        1.0f,
        WHITE);
  }
}

