#include "meteors.h"

int InitMeteors(Meteor meteors[], Texture2D textures[]) {
  for (int i = 0; i < MAX_METEORS; i++) {
    int meteorType = GetRandomValue(0, 2);
    if (meteorType == 0) {
      Meteor meteor = { textures[0], (Vector2){ GetRandomValue(20, GetScreenWidth() - 20), GetRandomValue(-750, -10) }, 200.0f, 50.0f, GetRandomValue(-50, 50), 0, true };
      meteors[i] = meteor;
    } else if (meteorType == 1) {
      Meteor meteor = { textures[1], (Vector2){ GetRandomValue(20, GetScreenWidth() - 20), GetRandomValue(-750, -10) }, 100.0f, 100.0f, GetRandomValue(-25, 25), 0, true };
      meteors[i] = meteor;
    } else {
      Meteor meteor = { textures[2], (Vector2){ GetRandomValue(20, GetScreenWidth() - 20), GetRandomValue(-750, -10) }, 50.0f, 200.0f, GetRandomValue(-10, 10), 0, true };
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

