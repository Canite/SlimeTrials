#include <gb/gb.h>
#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <gbdk/incbin.h>
#include <stdint.h>

#include "../include/player.h"
#include "../include/game.h"

INCBIN(sprites, "res/sprites.bin")
INCBIN_EXTERN(sprites)

void main(void)
{
    DISPLAY_OFF;
    BGP_REG = OBP0_REG = OBP1_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_LITE_GRAY, DMG_WHITE);
    SPRITES_8x8;

    set_sprite_data(0, 4, sprites);
    init_player();

    /*
    set_bkg_data(0x66, 2, tiles);
    VBK_REG = 1;
    VBK_REG = 0;
    set_bkg_tiles(0, 0, testWidth, testHeight, test);
    */

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    game_loop();
}