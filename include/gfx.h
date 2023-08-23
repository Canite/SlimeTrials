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

void init_camera(void);
void update_camera(void);
void clear_background(void);
void update_game_sprites(void);
void draw_hook(void);
void hide_hook(void);
void draw_hook_indicator(void);

#endif
