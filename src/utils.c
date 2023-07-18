#include "../include/utils.h"

int16_t abs16(int16_t x)
{
    if (x < 0)
    {
        return -1 * x;
    }

    return x;
}

int8_t abs8(int8_t x)
{
    if (x < 0)
    {
        return -1 * x;
    }

    return x;
}

uint16_t abs_sub(uint16_t x, uint16_t y)
{
    if (x < y)
    {
        return y - x;
    }

    return x - y;
}

int16_t sign(int16_t x)
{
    if (x < 0) return -1;
    if (x > 0) return 1;
    return 0;
}

uint8_t check_tilemap_collision(uint16_t x, uint16_t y, unsigned char tilemap[], uint8_t tilemap_w)
{
    uint8_t col_left = ((x >> 4)-8) >> 3;
    uint8_t col_right = ((x >> 4)-1) >> 3;
    uint8_t col_top = ((y >> 4)-16) >> 3;
    uint8_t col_bottom = ((y >> 4)-9) >> 3;

    return ((tilemap[col_bottom * tilemap_w + col_left] == 2) << 3) |
           ((tilemap[col_bottom * tilemap_w + col_right] == 2) << 2) |
           ((tilemap[col_top * tilemap_w + col_left] == 2) << 1) |
           ((tilemap[col_top * tilemap_w + col_right] == 2));
}

uint8_t check_rect_collision(uint16_t x1, uint16_t y1, uint8_t w1, uint8_t h1, uint16_t x2, uint16_t y2, uint8_t w2, uint8_t h2)
{
    return ((x1 < (x2+w2)) && ((x1+w1) > x2) && (y1 < (h2+y2)) && ((y1+h1) > y2));
}
