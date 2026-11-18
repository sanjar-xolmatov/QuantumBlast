#ifndef METEORS_H
#define METEORS_H

#include <raylib.h>

#define MAX_METEORS 20

typedef struct Meteor {
  Texture2D texture;
  Vector2 position;
  float speed;
  float damage;
  float rotationSpeed;
  float rotation;
  float hp;
  float maxHP;
  bool active;
} Meteor;

int InitMeteors(Meteor meteors[], Texture2D textures[]);
int SpawnMeteors(Meteor meteors[], float deltaTime);
void DrawMeteors(Meteor meteors[]);

#endif
