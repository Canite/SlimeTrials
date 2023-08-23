#ifndef _STRUCTS_H
#define _STRUCTS_H

#include <gb/gb.h>
#include <stdio.h>

typedef struct level_t 
{
    const unsigned char * tiles;
    const unsigned char * collisions;
    uint16_t tile_width;
    uint16_t tile_height;
    uint16_t spawn_x;
    uint16_t spawn_y;
} level_t;

#endif
