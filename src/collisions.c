#include "../include/collisions.h"

uint8_t tile_botleft = 0;
uint8_t tile_botright  = 0;
uint8_t tile_topleft = 0;
uint8_t tile_topright = 0;

uint8_t check_tilemap_collision(uint16_t x, uint16_t y)
{
    uint8_t col_left = ((x >> 4)-8) >> 3;
    uint8_t col_right = ((x >> 4)-1) >> 3;
    uint8_t col_top = ((y >> 4)-16) >> 3;
    uint8_t col_bottom = ((y >> 4)-9) >> 3;

    tile_botleft = game.tileMap[col_bottom * game.tileMapW + col_left];
    tile_botright  = game.tileMap[col_bottom * game.tileMapW + col_right];
    tile_topleft = game.tileMap[col_top * game.tileMapW + col_left];
    tile_topright = game.tileMap[col_top * game.tileMapW + col_right];

    return ((tile_botleft >= 2) << 3) |
           ((tile_botright >= 2) << 2) |
           ((tile_topleft >= 2) << 1) |
           ((tile_topright >= 2));
}

uint8_t check_rect_collision(uint16_t x1, uint16_t y1, uint8_t w1, uint8_t h1, uint16_t x2, uint16_t y2, uint8_t w2, uint8_t h2)
{
    return ((x1 < (x2+w2)) && ((x1+w1) > x2) && (y1 < (h2+y2)) && ((y1+h1) > y2));
}
