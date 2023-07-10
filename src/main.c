#include <gb/gb.h>
#include <gbdk/platform.h>
#include <gb/drawing.h>
#include <gbdk/console.h>
#include <gbdk/incbin.h>
#include <stdint.h>
#include <stdio.h>

//void __func_sprites() /*__banked __naked*/ { __asm _sprites:: 1$: .incbin "res/sprites.bin" 2$: ___size_sprites = (2$-1$) .globl ___size_sprites .local b___func_sprites ___bank_sprites = b___func_sprites .globl ___bank_sprites __endasm; }
INCBIN(sprites, "res/sprites.bin")
INCBIN_EXTERN(sprites)

// update user input macro
#define INPUT_PROCESS (old_joy=joy,joy=joypad()) 
// check button down
#define INPUT_KEY(key) (joy&(key))
// check button press
#define INPUT_KEYPRESS(key) ((joy & ~old_joy) & (key))
// check button release
#define INPUT_KEYRELEASE(key) ((~joy & old_joy) & (key))

// coordinate translation macros
#define SUBPIXELS_TO_PIXELS(v) (v >> 4)
#define PIXELS_TO_SUBPIXELS(v) (v << 4)

// physics constants
#define MAX_X_SPEED_IN_SUBPIXELS 45
#define MAX_X_GROUND_SPEED_IN_SUBPIXELS 40
#define X_ACCELERATION_IN_SUBPIXELS 5
#define X_DECELERATION_IN_SUBPIXELS 3
#define MAX_Y_SPEED_IN_SUBPIXELS 48
#define Y_ACCELERATION_IN_SUBPIXELS 2
#define GRAVITY_CONST 2
#define JUMP_ACCELERATION_IN_SUBPIXELS 48

// Game state
enum GameState
{
    GS_TITLE_LOAD,
    GS_TITLE,
    GS_INGAME,
    GS_PAUSE
};

uint16_t nGameState = GS_TITLE_LOAD;

// new and previous values of the joypad input
uint8_t joy  = 0, old_joy;

struct Player
{
    int16_t x;
    int16_t y;
    uint8_t w;
    uint8_t h;
    int16_t xSpd;
    int16_t ySpd;
    uint8_t facing;
    uint8_t rot;
    uint8_t animFrame;
    uint8_t grounded;
};

struct Player player = {0};

void main(void)
{
    DISPLAY_OFF;
    BGP_REG = OBP0_REG = OBP1_REG = DMG_PALETTE(DMG_BLACK, DMG_DARK_GRAY, DMG_LITE_GRAY, DMG_WHITE);
    SPRITES_8x8;

    set_sprite_data(0, 4, sprites);
    set_sprite_tile(0, player.animFrame);
    set_sprite_tile(1, player.animFrame + 1);
    set_sprite_tile(2, player.animFrame + 2);
    set_sprite_tile(3, player.animFrame + 3);

    /*
    set_bkg_data(0x66, 2, tiles);
    VBK_REG = 1;
    VBK_REG = 0;
    set_bkg_tiles(0, 0, testWidth, testHeight, test);
    */

    player.x = player.y = PIXELS_TO_SUBPIXELS(64);
    player.xSpd = player.ySpd = PIXELS_TO_SUBPIXELS(0);

    SHOW_BKG;
    SHOW_SPRITES;
    DISPLAY_ON;
    // Loop forever
    while(TRUE)
    {
        INPUT_PROCESS;

        switch(nGameState)
        {
            case GS_TITLE_LOAD:
                gotoxy(7u, 5u);
                printf("UNKNOWN");
                nGameState = GS_TITLE;
            case GS_TITLE:
                if (INPUT_KEYPRESS(J_START))
                {
                    nGameState = GS_INGAME;
                    gotoxy(7u, 5u);
                    printf("       ");
                }
                wait_vbl_done();
                continue;

                break;
            case GS_INGAME:
                break;
            case GS_PAUSE:

                if (INPUT_KEYPRESS(J_START))
                {
                    nGameState = GS_INGAME;
                    gotoxy(7u, 5u);
                    printf("       ");
                }
                else
                {
                    gotoxy(7u, 5u);
                    printf("PAUSE");
                }

                wait_vbl_done();
                continue;

                break;
            default:
                break;
        }

        if (INPUT_KEYPRESS(J_START))
        {
            nGameState = GS_PAUSE;
        }

        if (INPUT_KEY(J_UP))
        {
        }
        else if (INPUT_KEY(J_DOWN))
        {
        }

        if (INPUT_KEY(J_LEFT))
        {
            player.facing = 1;
            player.xSpd -= X_ACCELERATION_IN_SUBPIXELS;
            if (player.grounded)
            {
                if (player.xSpd < -MAX_X_GROUND_SPEED_IN_SUBPIXELS) player.xSpd = -MAX_X_GROUND_SPEED_IN_SUBPIXELS;
            } 
            else
            {
                if (player.xSpd < -MAX_X_SPEED_IN_SUBPIXELS) player.xSpd = -MAX_X_SPEED_IN_SUBPIXELS;
            }
        }
        else if (INPUT_KEY(J_RIGHT))
        {
            player.facing = 0;
            player.xSpd += X_ACCELERATION_IN_SUBPIXELS;
            if (player.grounded)
            {
                if (player.xSpd > MAX_X_GROUND_SPEED_IN_SUBPIXELS) player.xSpd = MAX_X_GROUND_SPEED_IN_SUBPIXELS;
            } 
            else
            {
                if (player.xSpd > MAX_X_SPEED_IN_SUBPIXELS) player.xSpd = MAX_X_SPEED_IN_SUBPIXELS;
            }
        }

        // check button keypress
        if (player.grounded && INPUT_KEYPRESS(J_A))
        {
            player.ySpd = -JUMP_ACCELERATION_IN_SUBPIXELS;
        }
        else if (player.ySpd < 0 && !player.grounded && INPUT_KEYRELEASE(J_A))
        {
            player.ySpd -= player.ySpd >> 1;
        }

        player.x += player.xSpd;
        player.y += player.ySpd;

        if (player.y > 2304) player.y = 2304;

        set_sprite_tile(0, player.animFrame);
        set_sprite_tile(1, player.animFrame + 1);
        set_sprite_tile(2, player.animFrame + 2);
        set_sprite_tile(3, player.animFrame + 3);

        if (player.facing)
        {
            set_sprite_prop(0, 1 << 5);
            set_sprite_prop(1, 1 << 5);
            set_sprite_prop(2, 1 << 5);
            set_sprite_prop(3, 1 << 5);
            move_sprite(1, SUBPIXELS_TO_PIXELS(player.x), SUBPIXELS_TO_PIXELS(player.y));
            move_sprite(0, SUBPIXELS_TO_PIXELS(player.x) + 8, SUBPIXELS_TO_PIXELS(player.y));
            move_sprite(3, SUBPIXELS_TO_PIXELS(player.x), SUBPIXELS_TO_PIXELS(player.y) + 8);
            move_sprite(2, SUBPIXELS_TO_PIXELS(player.x) + 8, SUBPIXELS_TO_PIXELS(player.y) + 8);
        }
        else
        {
            set_sprite_prop(0, 0);
            set_sprite_prop(1, 0);
            set_sprite_prop(2, 0);
            set_sprite_prop(3, 0);
            move_sprite(0, SUBPIXELS_TO_PIXELS(player.x), SUBPIXELS_TO_PIXELS(player.y));
            move_sprite(1, SUBPIXELS_TO_PIXELS(player.x) + 8, SUBPIXELS_TO_PIXELS(player.y));
            move_sprite(2, SUBPIXELS_TO_PIXELS(player.x), SUBPIXELS_TO_PIXELS(player.y) + 8);
            move_sprite(3, SUBPIXELS_TO_PIXELS(player.x) + 8, SUBPIXELS_TO_PIXELS(player.y) + 8);
        }

        // decelerate Y and X
        if (player.y < 2304)
        {
            player.grounded = 0;
            player.ySpd += GRAVITY_CONST;
        }
        else
        {
            player.grounded = 1;
            player.ySpd = 0;
        }

        if (player.xSpd == 1 || player.xSpd == -1)
        {
            player.xSpd = 0;
        }
        else if (player.xSpd < 0)
        {
            player.xSpd += X_DECELERATION_IN_SUBPIXELS;
        }
        else if (player.xSpd)
        {
            player.xSpd -= X_DECELERATION_IN_SUBPIXELS; 
        }

        //printf("        ");
        //gotoxy(1u, 1u);
        //printf("%d %d", player.xSpd, player.ySpd);
		// Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
    }
}