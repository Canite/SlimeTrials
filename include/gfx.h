#ifndef _GFX_H
#define _GFX_H

#include <gb/gb.h>
#include <gbdk/platform.h>

#include "utils.h"
#include "player.h"
#include "game.h"

#define BLANK_TILE_INDEX 0
#define OPEN_DOOR_TILE1_INDEX 1
#define OPEN_DOOR_TILE2_INDEX 2
#define CLOSED_DOOR_TILE1_INDEX 28
#define CLOSED_DOOR_TILE2_INDEX 29

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
inline void hide_key(void);
inline void hide_hook(void);
void draw_hook_indicator(void);

#endif
