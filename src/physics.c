#include "../include/physics.h"

const int8_t sine_wave[256] = {
    0,3,6,9,12,16,19,22,25,28,31,34,37,40,43,46,49,51,54,57,60,
    63,65,68,71,73,76,78,81,83,85,88,90,92,94,96,98,100,102,104,
    106,107,109,111,112,113,115,116,117,118,120,121,122,122,123,
    124,125,125,126,126,126,127,127,127,127,127,127,127,126,126,
    126,125,125,124,123,122,122,121,120,118,117,116,115,113,112,
    111,109,107,106,104,102,100,98,96,94,92,90,88,85,83,81,78,76,
    73,71,68,65,63,60,57,54,51,49,46,43,40,37,34,31,28,25,22,19,
    16,12,9,6,3,0,-3,-6,-9,-12,-16,-19,-22,-25,-28,-31,-34,-37,
    -40,-43,-46,-49,-51,-54,-57,-60,-63,-65,-68,-71,-73,-76,-78,
    -81,-83,-85,-88,-90,-92,-94,-96,-98,-100,-102,-104,-106,-107,
    -109,-111,-112,-113,-115,-116,-117,-118,-120,-121,-122,-122,
    -123,-124,-125,-125,-126,-126,-126,-127,-127,-127,-127,-127,
    -127,-127,-126,-126,-126,-125,-125,-124,-123,-122,-122,-121,
    -120,-118,-117,-116,-115,-113,-112,-111,-109,-107,-106,-104,
    -102,-100,-98,-96,-94,-92,-90,-88,-85,-83,-81,-78,-76,-73,-71,
    -68,-65,-63,-60,-57,-54,-51,-49,-46,-43,-40,-37,-34,-31,-28,
    -25,-22,-19,-16,-12,-9,-6,-3
};

uint16_t isqrt(uint16_t x) NONBANKED
{
    uint16_t m, y, b;
    m = 0x4000;
    y = 0;
    while (m != 0) {
        b = y | m;
        y >>= 1;
        if (x >= b) {
            x -= b;
            y |= m;
        }
        m >>= 2;
    }
    return y;
}

void apply_physics()
{
    // Unhooked physics
    if (player.hookState != HS_ATTACHED)
    {
        int8_t xSpdSign = sign(player.xSpd);
        int8_t ySpdSign = sign(player.ySpd);
        int16_t xTmp = 0;
        int16_t yTmp = 0;
        player.grounded = 0;
        while (xTmp != player.xSpd || yTmp != player.ySpd)
        {
            if ((xSpdSign < 0 && (xTmp - 128) < player.xSpd) || (xSpdSign >= 0 && (xTmp + 128) > player.xSpd))
            {
                xTmp = player.xSpd;
            }
            else
            {
                xTmp += xSpdSign << 7;
            }

            if ((ySpdSign < 0 && (yTmp - 128) < player.ySpd) || (ySpdSign >= 0 && (yTmp + 128) > player.ySpd))
            {
                yTmp = player.ySpd;
            }
            else
            {
                yTmp += ySpdSign << 7;
            }

            // Check at least 1 pixel offset
            uint16_t yColOffset = yTmp;
            if (yTmp < 0) yColOffset = MIN(-16, yTmp);
            if (yTmp > 0) yColOffset = MAX(16, yTmp);
            uint16_t xColOffset = xTmp;
            if (xTmp < 0) xColOffset = MIN(-16, xTmp);
            if (xTmp > 0) xColOffset = MAX(16, xTmp);

            uint8_t col_flags = check_tilemap_collision(player.x + xColOffset, player.y + yColOffset, test_tiles, test_tile_width);
            player.colFlags = col_flags;
            // Check corners first
            uint8_t cornerCollision = handle_collision_v_corners(yTmp, col_flags);
            if (cornerCollision == 1)
            {
                handle_collision_h(xTmp, col_flags);
            }
            else if (!cornerCollision)
            {
                if ((col_flags == (BOT_LEFT_COL | TOP_LEFT_COL)) || (col_flags == (BOT_RIGHT_COL | TOP_RIGHT_COL)))
                {
                    handle_collision_h(xTmp, col_flags);
                    col_flags = check_tilemap_collision(player.x, player.y + yColOffset, test_tiles, test_tile_width);
                    handle_collision_v(yTmp, col_flags);
                }
                else
                {
                    handle_collision_v(yTmp, col_flags);
                    col_flags = check_tilemap_collision(player.x + xColOffset, player.y, test_tiles, test_tile_width);
                    handle_collision_h(xTmp, col_flags);
                }
            }
        }

        player.x += player.xSpd;
        player.y += player.ySpd;

        player.ySpd += GRAVITY_CONST;

        // decelerate Y and X
        if (player.grounded)
        {
            if (player.xSpd < X_GROUND_DECELERATION_IN_SUBPIXELS && player.xSpd > -X_GROUND_DECELERATION_IN_SUBPIXELS)
            {
                player.xSpd = 0;
            }
            else if (player.xSpd < 0)
            {
                player.xSpd += X_GROUND_DECELERATION_IN_SUBPIXELS;
            }
            else if (player.xSpd)
            {
                player.xSpd -= X_GROUND_DECELERATION_IN_SUBPIXELS; 
            }
        }
        else
        {
            if (player.xSpd < X_DECELERATION_IN_SUBPIXELS && player.xSpd > -X_DECELERATION_IN_SUBPIXELS)
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
        }
    }
    // Hook physics
    else
    {
        player.angularAcc = SIN(player.hookAngle);
        // Division with negative numbers does not work properly in gbdk
        // So, change to positive before division (algorithm needs -sin() anyways)
        // Divide by the number of hook segements (the hookLength is too aggressive adjustment)
        if (player.angularAcc < 0)
        {
            player.angularAcc = -1 * player.angularAcc;
            player.angularAcc /= (player.hookSegments << 1);
        }
        else
        {
            player.angularAcc /= (player.hookSegments << 1);
            player.angularAcc = -1 * player.angularAcc;
        }

        player.angularVel += player.angularAcc + sign(player.angularAcc);
        // Taper off to angle 0 even if there is no more velocity
        // this ensures that we always settle down to angle 0 with no input
        if (player.angularVel >> 1 == 0 && player.hookAngle > 0 && player.hookAngle <= 8)
        {
            player.hookAngle -= 1;
        }
        else if (player.angularVel >> 1 == 0  && player.hookAngle > 247 /* && player.hookAngle <= 255 //always true */)
        {
            player.hookAngle += 1;
        }
        else
        {
            player.hookAngle += (player.angularVel) >> 4;
        }

        // Hook length needs to be << 4 and sin(a) needs >> 7, so after mult it's >> 3
        player.x = ((player.hookLength * SIN(player.hookAngle)) >> 3) + player.hookX;
        player.y = ((player.hookLength * COS(player.hookAngle)) >> 3) + player.hookY;

        // Decelerate, otherwise we'll swing back and forth forever
        if (player.angularVel < 0)
        {
            player.angularVel += 1;
        }
        else if (player.angularVel)
        {
            player.angularVel -= 1;
        }
    }
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
    uint16_t yOffset = (((player.y + yTmp) >> 7) << 7);
    if (col_flags == BOT_LEFT_COL || col_flags == BOT_RIGHT_COL)
    {
        if (yTmp > 0 && player.y - yOffset <= 24)
        {
            player.y = yOffset;
            yTmp = 0;
            player.ySpd = 0;
            player.grounded = 1;
            return 2;
        }
        return 1;
    }

    if (col_flags == TOP_LEFT_COL || col_flags == TOP_RIGHT_COL)
    {
        yOffset += 128;
        if (yTmp < 0 && yOffset - player.y <= 24)
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