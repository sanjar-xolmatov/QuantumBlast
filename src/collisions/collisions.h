#ifndef COLLISIONS_H
#define COLLISIONS_H

#include <raylib.h>
#include "../quantum.h"

void CollisionMeteorAndLasers(Laser lasers[], Meteor meteors[]);
void CollisionPlayerAndMeteors(Player *player, Meteor meteors[], GameScreen *currentScreen);

#endif
