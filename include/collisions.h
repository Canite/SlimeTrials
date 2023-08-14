#ifndef _COLLISIONS_H
#define _COLLISIONS_H

#include <stdint.h>
#include <gb/gb.h>
#include "player.h"
#include "game.h"

extern uint8_t tile_botleft;
extern uint8_t tile_botright;
extern uint8_t tile_topleft;
extern uint8_t tile_topright;

uint8_t check_tilemap_collision(uint16_t x, uint16_t y);
uint8_t check_rect_collision(uint16_t x1, uint16_t y1, uint8_t w1, uint8_t h1, uint16_t x2, uint16_t y2, uint8_t w2, uint8_t h2);

#endif
