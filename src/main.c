#include <stdint.h>
#include <gb/gb.h>
#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <gbdk/incbin.h>

#include "../include/player.h"
#include "../include/game.h"
#include "../include/gfx.h"

INCBIN(slime, "res/slime.2bpp")
INCBIN_EXTERN(slime)
INCBIN(hook, "res/hook.2bpp")
INCBIN_EXTERN(hook)
INCBIN(background_tiles, "res/background_tiles.2bpp")
INCBIN_EXTERN(background_tiles)

void main(void)
{
    DISPLAY_OFF;
    //BGP_REG = OBP0_REG = OBP1_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY);
    OBP1_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    SPRITES_8x8;

    init_game();
    init_camera();

    set_sprite_data(0, 8, slime);
    set_sprite_data(8, 1, hook);
    init_player();

    set_bkg_data(0, 4, background_tiles);
    clear_background();
    set_bkg_submap(0, 0, 20, 18, test_tiles, test_tile_width);

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    game_loop();
}
