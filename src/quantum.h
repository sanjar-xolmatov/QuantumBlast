#ifndef QUANTUM_H
#define QUANTUM_H

#include <raylib.h>
#include "../src/meteors/meteors.h"
#include "../src/lasers/lasers.h"
#include "../src/player/player.h"
#include <raylib.h>
#include <math.h>
#include <stdbool.h>

#define SCREEN_WIDTH 1920
#define SCREEN_HEIGHT 1080

typedef enum GameScreen {
  TITLE = 0,
  GAMEPLAY,
} GameScreen;

#endif
