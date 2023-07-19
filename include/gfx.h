#ifndef _GFX_H
#define _GFX_H

#include <gb/gb.h>
#include <gbdk/platform.h>

#include "utils.h"
#include "player.h"
#include "game.h"

#define BLANK_TILE_INDEX 1

struct Camera
{
    uint16_t x;
    uint16_t y;
    uint16_t oldX;
    uint16_t oldY;
    uint8_t redraw;
};

extern struct Camera camera;

void init_camera();
void update_camera();
void clear_background();
void update_game_sprites();
void draw_hook();
void hide_hook();

#endif