#ifndef _GFX_H
#define _GFX_H

#include <gb/gb.h>
#include <stdlib.h>
#include <gbdk/platform.h>

#include "utils.h"
#include "player.h"
#include "game.h"
#include "../res/font_bold.h"

#define BLANK_TILE_INDEX 0
#define OPEN_DOOR_TILE1_INDEX 1
#define OPEN_DOOR_TILE2_INDEX 2
#define CLOSED_DOOR_TILE1_INDEX 28
#define CLOSED_DOOR_TILE2_INDEX 29
#define FONT_START_TILE_INDEX 42
#define FONT_COLON_TILE_INDEX 52
#define FONT_CLOCK_TILE_INDEX 53
#define FONT_SKULL_TILE_INDEX 54

struct Camera
{
    uint16_t x;
    uint16_t y;
    uint16_t oldX;
    uint16_t oldY;
    uint8_t redraw;
};

struct WindowData
{
    uint16_t timer_frames;
    uint8_t timer_minutes;
    uint8_t timer_seconds;
    uint8_t drawn_deaths;
    uint8_t level_timer_tiles[6];
    uint8_t death_counter_tiles[3];
};

struct GraphicsInfo
{
    uint8_t fade_delay;
    uint8_t fade_step_length;
    uint8_t sprites_inited;
    uint8_t draw_window;
};


extern struct Camera camera;
extern struct WindowData window;
extern struct GraphicsInfo gfx;

void init_gfx(void);
void init_camera(void);
void init_window(void);
void update_window(void);
void update_camera(void);
void clear_background(void);
void fade_out(void);
void fade_in(void);
void update_game_sprites(void);
void draw_hook(void);
inline void hide_atl(void);
inline void hide_key(void);
inline void hide_hook(void);
void draw_hook_indicator(void);

#endif
