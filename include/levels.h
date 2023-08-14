#ifndef _LEVELS_H
#define _LEVELS_H

#include <stdint.h>
#include <gb/gb.h>
#include <gbdk/incbin.h>

#include "game.h"
#include "../res/title_screen.h"
#include "../res/level1.h"
#include "../res/level2.h"
#include "../res/level3.h"

INCBIN_EXTERN(background_tiles)
INCBIN_EXTERN(title)

void init_title();
void init_level(uint8_t level);

#endif
