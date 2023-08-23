#include "../include/collisions.h"

uint8_t tile_botleft = 0;
uint8_t tile_botright  = 0;
uint8_t tile_topleft = 0;
uint8_t tile_topright = 0;

uint8_t collision_botleft = 0;
uint8_t collision_botright  = 0;
uint8_t collision_topleft = 0;
uint8_t collision_topright = 0;

// For solid ground collisions
uint8_t check_collision(uint16_t x, uint16_t y)
{
    uint8_t col_left = ((x >> 4)-8) >> 3;
    uint8_t col_right = ((x >> 4)-1) >> 3;
    uint8_t col_top = ((y >> 4)-16) >> 3;
    uint8_t col_bottom = ((y >> 4)-9) >> 3;

    collision_botleft = game.level_data.collisions[col_bottom * game.level_data.tile_width + col_left];
    collision_botright  = game.level_data.collisions[col_bottom * game.level_data.tile_width + col_right];
    collision_topleft = game.level_data.collisions[col_top * game.level_data.tile_width + col_left];
    collision_topright = game.level_data.collisions[col_top * game.level_data.tile_width + col_right];

    col_left = (collision_topleft == COL_SOLID);
    col_right = (collision_topright == COL_SOLID);
    col_top = (collision_botleft == COL_SOLID);
    col_bottom = (collision_botright == COL_SOLID);

    return ((col_top) << 3) |
           ((col_bottom) << 2) |
           ((col_left) << 1) |
           ((col_right));
}

// For special collisions
void update_tilemap_collision(uint16_t x, uint16_t y)
{
    uint8_t col_left = ((x >> 4)-8) >> 3;
    uint8_t col_right = ((x >> 4)-1) >> 3;
    uint8_t col_top = ((y >> 4)-16) >> 3;
    uint8_t col_bottom = ((y >> 4)-9) >> 3;

    tile_botleft = game.level_data.tiles[col_bottom * game.level_data.tile_width + col_left];
    tile_botright  = game.level_data.tiles[col_bottom * game.level_data.tile_width + col_right];
    tile_topleft = game.level_data.tiles[col_top * game.level_data.tile_width + col_left];
    tile_topright = game.level_data.tiles[col_top * game.level_data.tile_width + col_right];
}

uint8_t check_rect_collision(uint16_t x1, uint16_t y1, uint8_t w1, uint8_t h1, uint16_t x2, uint16_t y2, uint8_t w2, uint8_t h2)
{
    return ((x1 < (x2+w2)) && ((x1+w1) > x2) && (y1 < (h2+y2)) && ((y1+h1) > y2));
}

uint8_t handle_collision_h(int16_t xTmp, uint8_t col_flags)
{
    if (xTmp < 0 && (col_flags & (BOT_LEFT_COL | TOP_LEFT_COL)))
    {
        player.x = (((player.x + xTmp) >> 7) << 7) + 128;
        xTmp = 0;
        player.xSpd = 0;
        return 1;
    }

    if (xTmp > 0 && (col_flags & (BOT_RIGHT_COL | TOP_RIGHT_COL)))
    {
        player.x = (((player.x + xTmp) >> 7) << 7);
        xTmp = 0;
        player.xSpd = 0;
        return 1;
    }

    return 0;
}

uint8_t handle_collision_v(int16_t yTmp, uint8_t col_flags)
{
    if (yTmp > 0 && (col_flags & (BOT_LEFT_COL | BOT_RIGHT_COL)))
    {
        player.y = (((player.y + yTmp) >> 7) << 7);
        yTmp = 0;
        player.ySpd = 0;
        player.grounded = 1;
        return 1;
    }

    if (yTmp < 0 && (col_flags & (TOP_LEFT_COL | TOP_RIGHT_COL)))
    {
        player.y = (((player.y + yTmp) >> 7) << 7) + 128;
        yTmp = 0;
        player.ySpd = 0;
        return 1;
    }

    return 0;
}

uint8_t handle_collision_v_corners(int16_t yTmp, uint8_t col_flags)
{
    if (yTmp == 0)
    {
        return 1;
    }

    // Tile position _above_ the colliding tile
    int16_t yOffset = (((player.y + yTmp) >> 7) << 7);
    if (col_flags == BOT_LEFT_COL || col_flags == BOT_RIGHT_COL)
    {
        // 2 pixel leniency when coming from below (snap up to ledge)
        // also handles coming down from above adjusting y first
        if (((int16_t)player.y - yOffset) <= 32)
        {
            player.y = yOffset;
            yTmp = 0;
            player.ySpd = 0;
            player.grounded = 1;
            return 2;
        }

        // there _is_ a collision, but handle horizontal first
        return 1;
    }

    if (col_flags == TOP_LEFT_COL || col_flags == TOP_RIGHT_COL)
    {
        yOffset += 128;
        if ((yOffset - (int16_t)player.y) <= 32)
        {
            player.y = yOffset;
            yTmp = 0;
            player.ySpd = 0;
            return 2;
        }
        return 1;
    }

    return 0;
}
