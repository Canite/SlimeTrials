#include <stdint.h>
#include <gb/gb.h>
#include <gbdk/platform.h>
#include <gbdk/console.h>
#include <gbdk/incbin.h>

#include "../include/player.h"
#include "../include/game.h"
#include "../include/gfx.h"
#include "../include/input.h"
#include "../include/hUGEDriver.h"
#include "asm/types.h"

void VBL_isr(void)
{
    if (gfx.draw_window)
    {
        WX_REG = DEVICE_WINDOW_PX_OFFSET_X;
        SHOW_WIN;
    }
}

void LCD_isr(void)
{
    HIDE_WIN;
    WX_REG = 0;
}

int main(void)
{
    DISPLAY_OFF;
    BGP_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    OBP0_REG = DMG_PALETTE(DMG_BLACK, DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY);
    OBP1_REG = DMG_PALETTE(DMG_WHITE, DMG_LITE_GRAY, DMG_DARK_GRAY, DMG_BLACK);
    SPRITES_8x8;

    // Enable sound
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x10;

    music_init();

    disable_interrupts();

    CRITICAL {
        STAT_REG |= STATF_LYC;
        music_setup_timer();

        add_VBL(VBL_isr);
        add_LCD(LCD_isr);
        add_low_priority_TIM(music_play_isr);
        LYC_REG = 8;
    }

    enable_interrupts();
    set_interrupts(LCD_IFLAG | VBL_IFLAG | IE_REG | TIM_IFLAG);

    init_title();
    init_gfx();
    init_game();

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;

    game_loop();

    return 1;
}
