#ifndef _UTILS_H
#define _UTILS_H

#include <stdint.h>
#include <gb/gb.h>
#include "player.h"
#include "game.h"

// coordinate translation macros
#define SUBPIXELS_TO_PIXELS(v) (v >> 4)
#define PIXELS_TO_SUBPIXELS(v) (v << 4)

#define TOP_RIGHT_COL 0x1
#define TOP_LEFT_COL  0x2
#define BOT_RIGHT_COL 0x4
#define BOT_LEFT_COL  0x8

int16_t abs16(int16_t x);
int8_t abs8(int8_t x);
uint16_t abs_sub(uint16_t x, uint16_t y);
int16_t sign(int16_t x);

#endif
