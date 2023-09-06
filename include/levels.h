#ifndef _LEVELS_H
#define _LEVELS_H

#include <stdint.h>
#include <gb/gb.h>
#include <gbdk/incbin.h>

#include "structs.h"
#include "game.h"
#include "key_obj.h"
#include "atl_obj.h"
#include "gfx.h"
#include "../res/caverns.h"
#include "../res/title.h"
#include "../res/title_screen.h"
#include "../res/level1.h"
#include "../res/level2.h"
#include "../res/level3.h"
#include "../res/level4.h"
#include "../res/level5.h"

#define NUM_LEVELS 5

extern const level_t levels[];

void init_title(void);
void init_level(uint8_t level);

#endif
