#ifndef _GFX_H
#define _GFX_H

#include <gb/gb.h>
#include <gbdk/platform.h>

#include "utils.h"
#include "player.h"
#include "game.h"

#define BLANK_TILE_INDEX 1

void clear_background();
void update_game_sprites();
void draw_hook();
void hide_hook();

#endif