#ifndef _LEVELS_H
#define _LEVELS_H

#include <stdint.h>
#include <gb/gb.h>
#include <gbdk/incbin.h>

#include "structs.h"
#include "game.h"
#include "../res/caverns.h"
#include "../res/title.h"
#include "../res/title_screen.h"
#include "../res/level1.h"
#include "../res/level2.h"
#include "../res/level3.h"

#define NUM_LEVELS 3

extern const level_t levels[];

void init_title(void);
void init_level(uint8_t level);

#endif
