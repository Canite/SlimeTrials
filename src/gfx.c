#include "../include/gfx.h"

struct Camera camera = {0};

void init_camera()
{
    camera.x = 0;
    camera.y = 0;
    camera.oldX = 0;
    camera.oldY = 0;
    camera.redraw = 0;
}

void clear_background()
{
    for (uint16_t i = 0; i < 20; i ++)
    {
        for (uint16_t j = 0; j < 18; j ++)
        {
            set_tile_xy(i, j, BLANK_TILE_INDEX);
        }
    }
}

void update_camera()
{
    if (camera.redraw)
    {
        camera.redraw = 0;

        // update hardware scroll position
        SCY_REG = camera.y; SCX_REG = camera.x; 
        // up or down
        game.mapY = (uint8_t)(camera.y >> 3u);
        if (game.mapY != game.oldMapY)
        {
            if (camera.y < camera.oldY)
            {
                set_bkg_submap(game.mapX, game.mapY, MIN(21u, game.tileMapW-game.mapX), 1, game.tileMap, game.tileMapW);
            }
            else
            {
                if ((game.tileMapH - 18u) > game.mapY) set_bkg_submap(game.mapX, game.mapY + 18u, MIN(21u, game.tileMapW-game.mapX), 1, game.tileMap, game.tileMapW);
            }
            game.oldMapY = game.mapY; 
        }

        // left or right
        game.mapX = (uint8_t)(camera.x >> 3u);
        if (game.mapX != game.oldMapX)
        {
            if (camera.x < camera.oldX)
            {
                set_bkg_submap(game.mapX, game.mapY, 1, MIN(19u, game.tileMapH - game.mapY), game.tileMap, game.tileMapW);
            }
            else
            {
                if ((game.tileMapW - 20u) > game.mapX) set_bkg_submap(game.mapX + 20u, game.mapY, 1, MIN(19u, game.tileMapH - game.mapY), game.tileMap, game.tileMapW);
            }
            game.oldMapX = game.mapX;
        }

        // set old camera position to current camera position
        camera.oldX = camera.x;
        camera.oldY = camera.y;
    }
}

void update_game_sprites()
{
    // Player sprite start
    set_sprite_tile(0, player.animIndex + player.animFrame);
    if ((game.gameFrame & player.animSpeed) == player.animSpeed)
    {
        if (player.animFrame < player.numAnimFrames - 1)
        {
            player.animFrame += 1;
        }
        else
        {
            player.animFrame = 0;
        }
    }

    int16_t playerPixelX = SUBPIXELS_TO_PIXELS(player.x) - camera.x;
    int16_t playerPixelY = SUBPIXELS_TO_PIXELS(player.y) - camera.y;

    if (player.facing)
    {
        set_sprite_prop(0, 1 << 5);
        move_sprite(0, playerPixelX, playerPixelY);
    }
    else
    {
        set_sprite_prop(0, 0);
        move_sprite(0, playerPixelX, playerPixelY);
    }

    if (player.hookState != HS_STOWED)
    {
        draw_hook();
    }
    else
    {
        hide_hook();
    }

    // Player sprite end
}

void draw_hook()
{
    int8_t xOffset = (player.hookAngle) >> 2;
    int8_t yOffset = 8;
    //uint8_t spriteProp = 0;
    //uint8_t gfxHookIndex = xOffset;
    if (player.hookAngle > ANGLE_315DEG)
    {
        xOffset = (player.hookAngle - ANGLE_360DEG) >> 2;
        yOffset = 8;
        //spriteProp = S_FLIPX;
        //gfxHookIndex = (ANGLE_360DEG - player.hookAngle) >> 2;
    }
    else if (player.hookAngle > ANGLE_270DEG)
    {
        xOffset = -8;
        yOffset = (player.hookAngle - ANGLE_270DEG) >> 2;
        //spriteProp = S_FLIPX;
        //gfxHookIndex = (ANGLE_360DEG - player.hookAngle) >> 2;
    }
    else if (player.hookAngle > ANGLE_225DEG)
    {
        xOffset = -8;
        yOffset = (player.hookAngle - ANGLE_270DEG) >> 2;
        //spriteProp = S_FLIPX | S_FLIPY;
        //gfxHookIndex = (player.hookAngle - ANGLE_180DEG) >> 2;
    }
    else if (player.hookAngle > ANGLE_180DEG)
    {
        xOffset = (ANGLE_180DEG - player.hookAngle) >> 2;
        yOffset = -8;
        //spriteProp = S_FLIPX | S_FLIPY;
        //gfxHookIndex = (player.hookAngle - ANGLE_180DEG) >> 2;
    }
    else if (player.hookAngle > ANGLE_135DEG)
    {
        xOffset = (ANGLE_180DEG - player.hookAngle) >> 2;
        yOffset = -8;
        //spriteProp = S_FLIPY;
        //gfxHookIndex = (ANGLE_180DEG - player.hookAngle) >> 2;
    }
    else if (player.hookAngle > ANGLE_90DEG)
    {
        xOffset = 8;
        yOffset = (ANGLE_90DEG - player.hookAngle) >> 2;
        //spriteProp = S_FLIPY;
        //gfxHookIndex = (ANGLE_180DEG - player.hookAngle) >> 2;
    }
    else if (player.hookAngle > ANGLE_45DEG)
    {
        xOffset = 8;
        yOffset = (ANGLE_90DEG - player.hookAngle) >> 2;
    }

    //if (gfxHookIndex > 15) gfxHookIndex = 15;

    /* If I ever want to try manually drawing the line..
    uint8_t hookX = player.hookX >> 4;
    uint8_t hookY = player.hookY >> 4;
    uint8_t playerX = player.x >> 4;
    uint8_t playerY = player.y >> 4;
    uint8_t drawY = 0;
    uint8_t drawX = 0;
    int16_t a = (player.hookY - player.y) >> 3;
    int16_t b = a - ((player.hookX - player.x) >> 3);
    int16_t p = a - ((player.hookX - player.x) >> 4);
    uint8_t spriteData[16];
    */
    uint16_t hookPixelX = SUBPIXELS_TO_PIXELS(player.hookX) - camera.x;
    uint16_t hookPixelY = SUBPIXELS_TO_PIXELS(player.hookY) - camera.y;
    for (uint8_t i = 0; i < player.hookSegments; i++)
    {
        /*
        // 2 bytes per row
        for (uint8_t j = 0; j < 16; j += 2)
        {
            spriteData[j] = 1;
            spriteData[j + 1] = 0;
        }

        set_sprite_data(HOOK_SPRITE_INDEX + i, 1, spriteData);
        set_sprite_tile(i + 1, HOOK_SPRITE_INDEX + i);
        */

        set_sprite_tile(i + 1, HOOK_SPRITE_TILE_INDEX);
        //set_sprite_prop(i + 1, spriteProp);
        move_sprite(i + 1, hookPixelX + (xOffset * i), hookPixelY + (yOffset * i));
    }

    if ((abs8(xOffset) + abs8(yOffset)) * player.hookSegments > 80)
    {
        hide_sprite(HOOK_SPRITE_INDEX + player.hookSegments - 1);
    }
}

// Dead code for drawing line sprites with 16 frame angle difference
/*
void draw_hook_sprite()
{
    int16_t xOffset = (player.x - player.hookX);
    if (xOffset < 0)
    {
        xOffset = -1 * xOffset;
        xOffset /= player.hookSegments;
        xOffset = -1 * xOffset;
    }
    else
    {
        xOffset /= player.hookSegments;
    }

    xOffset = xOffset >> 4;

    int16_t yOffset = (player.y - player.hookY);
    if (yOffset < 0)
    {
        yOffset = -1 * yOffset;
        yOffset /= player.hookSegments;
        yOffset = -1 * yOffset;
    }
    else
    {
        yOffset /= player.hookSegments;
    }

    yOffset = (yOffset >> 4);

    uint8_t gfxHookIndex = player.hookAngle >> 2;
    uint8_t spriteProp = 0;
    if (player.hookAngle > ANGLE_270DEG)
    {
        spriteProp = S_FLIPX;
        gfxHookIndex = (ANGLE_360DEG - player.hookAngle) >> 2;
    }
    else if (player.hookAngle > ANGLE_180DEG && player.hookAngle <= ANGLE_270DEG)
    {
        spriteProp = S_FLIPX | S_FLIPY;
        gfxHookIndex = (player.hookAngle - ANGLE_180DEG) >> 2;
    }
    else if (player.hookAngle > ANGLE_90DEG && player.hookAngle <= ANGLE_180DEG)
    {
        spriteProp = S_FLIPY;
        gfxHookIndex = (ANGLE_180DEG - player.hookAngle) >> 2;
    }

    if (gfxHookIndex > 15) gfxHookIndex = 15;

    for (uint8_t i = 0; i < player.hookSegments; i++)
    {
        set_sprite_tile(i + 1, HOOK_SPRITE_INDEX + gfxHookIndex);
        set_sprite_prop(i + 1, spriteProp);
        if (spriteProp & S_FLIPX)
        {
            if (spriteProp & S_FLIPY)
            {
                move_sprite(i + 1, (player.hookX >> 4) + (xOffset * i) - 3, (player.hookY >> 4) + (yOffset * i) - 3);
            }
            else
            {
                move_sprite(i + 1, (player.hookX >> 4) + (xOffset * i) - 3, (player.hookY >> 4) + (yOffset * i) + 3);
            }
        }
        else if (spriteProp & S_FLIPY)
        {
            move_sprite(i + 1, (player.hookX >> 4) + (xOffset * i) + 3, (player.hookY >> 4) + (yOffset * i) - 3);
        }
        else
        {
            move_sprite(i + 1, (player.hookX >> 4) + (xOffset * i) + 3, (player.hookY >> 4) + (yOffset * i) + 3);
        }
    }
}
*/

void hide_hook()
{
    for (uint8_t i = 0; i < player.hookSegments; i++)
    {
        hide_sprite(HOOK_SPRITE_INDEX + i);
    }
}
