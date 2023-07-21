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
    if (player.fallDelay)
    {
        player.fallDelay -= 1;
    }

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
            if ((xSpdSign < 0 && (xTmp - 112) < player.xSpd) || (xSpdSign >= 0 && (xTmp + 112) > player.xSpd))
            {
                xTmp = player.xSpd;
            }
            else
            {
                xTmp += (((player.xSpd) < (0)) ? (-112) : (112));
            }

            if ((ySpdSign < 0 && (yTmp - 112) < player.ySpd) || (ySpdSign >= 0 && (yTmp + 112) > player.ySpd))
            {
                yTmp = player.ySpd;
            }
            else
            {
                yTmp += (((player.ySpd) < (0)) ? (-112) : (112));
            }

            // Check at least 1 pixel offset
            uint16_t yColOffset = yTmp;
            if (yTmp < 0) yColOffset = MIN(-16, yTmp);
            if (yTmp > 0) yColOffset = MAX(16, yTmp);
            uint16_t xColOffset = xTmp;
            if (xTmp < 0) xColOffset = MIN(-16, xTmp);
            if (xTmp > 0) xColOffset = MAX(16, xTmp);

            uint8_t col_flags = check_tilemap_collision(player.x + xColOffset, player.y + yColOffset);
            player.colFlags = col_flags;
            // Check corners first
            uint8_t cornerCollision = handle_collision_v_corners(yTmp, col_flags);
            if (cornerCollision == 2)
            {
                yTmp = 0;
                col_flags = check_tilemap_collision(player.x + xColOffset, player.y);
                if (handle_collision_h(xColOffset, col_flags)) xTmp = 0;
            }
            else if (cornerCollision == 1)
            {
                if (handle_collision_h(xColOffset, col_flags)) xTmp = 0;
                col_flags = check_tilemap_collision(player.x, player.y + yColOffset);
                if (handle_collision_v(yColOffset, col_flags)) yTmp = 0;
            }
            else
            {
                if ((col_flags == (BOT_LEFT_COL | TOP_LEFT_COL)) || (col_flags == (BOT_RIGHT_COL | TOP_RIGHT_COL)))
                {
                    if (handle_collision_h(xColOffset, col_flags)) xTmp = 0;
                    col_flags = check_tilemap_collision(player.x, player.y + yColOffset);
                    if (handle_collision_v(yColOffset, col_flags)) yTmp = 0;
                }
                else
                {
                    if (handle_collision_v(yColOffset, col_flags)) yTmp = 0;
                    col_flags = check_tilemap_collision(player.x + xColOffset, player.y);
                    if (handle_collision_h(xColOffset, col_flags)) xTmp = 0;
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

            if (player.animIndex != AIR_IDLE_ANIM_INDEX)
            {
                player.animIndex = AIR_IDLE_ANIM_INDEX;
                player.numAnimFrames = AIR_IDLE_ANIM_FRAMES;
                player.animSpeed = AIR_IDLE_ANIM_SPEED;
                player.animFrame = 0;
                player.fallDelay = 6;
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
            player.angularAcc /= (player.hookSegments + (player.hookSegments >> 2));
        }
        else
        {
            player.angularAcc /= (player.hookSegments + (player.hookSegments >> 2));
            player.angularAcc = -1 * player.angularAcc;
        }

        if (sign(player.angularAcc) != sign(player.angularVel)) player.angularAcc += -(player.angularAcc >> 2);
        player.angularVel += player.angularAcc;
        player.angularVel = CLAMP(player.angularVel, -MAX_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY);

        // Taper off to angle 0 even if there is no more velocity
        // this ensures that we always settle down to angle 0 with no input
        // unless we are blocked by a wall, then we can't settle to 0
        int8_t angleSettleAlignment = 0;
        uint8_t oldAngle = player.hookAngle;
        if (player.angularVel >> 4 == 0 && player.hookAngle > 0 && player.hookAngle <= 9)
        {
            player.hookAngle -= 1;
            angleSettleAlignment = 1;
        }
        else if (player.angularVel >> 4 == 0  && player.hookAngle >= 246 /* && player.hookAngle <= 255 //always true */)
        {
            player.hookAngle += 1;
            angleSettleAlignment = -1;
        }
        else
        {
            player.hookAngle += (player.angularVel) >> 4;
        }

        // Hook length needs to be << 4 and sin(a) needs >> 7, so after mult it's >> 3
        int16_t xOffset = (player.hookLength * SIN(player.hookAngle)) >> 3;
        int16_t yOffset = (player.hookLength * COS(player.hookAngle)) >> 3;
        player.x = player.hookX + xOffset;
        player.y = player.hookY + yOffset;
        uint8_t col_flags = check_tilemap_collision(player.x, player.y);
        uint8_t nAttempts = 12;
        int8_t tmpAngle = 0;
        int8_t angularVelSign = sign(player.angularVel);
        if (col_flags)
        {
            while (col_flags && nAttempts > 0)
            {
                if (angleSettleAlignment == 0 || player.oldHookLength != player.hookLength)
                {
                    if (angularVelSign < 0)
                    {
                        tmpAngle += 1;
                    }
                    else
                    {
                        tmpAngle -= 1;
                    }
                }
                else
                {
                    tmpAngle += angleSettleAlignment;
                }

                xOffset = (player.hookLength * SIN(player.hookAngle + tmpAngle)) >> 3;
                yOffset = (player.hookLength * COS(player.hookAngle + tmpAngle)) >> 3;
                player.x = player.hookX + xOffset;
                player.y = player.hookY + yOffset;
                col_flags = check_tilemap_collision(player.x, player.y);
                nAttempts -= 1;
                // If we're adjusting hook length, check both directions for a valid path
                if (col_flags && (nAttempts & 1u) && player.oldHookLength != player.hookLength)
                {
                    tmpAngle = -tmpAngle + sign(tmpAngle);
                    angularVelSign = -angularVelSign;
                    if (angularVelSign == 0) angularVelSign = -1;
                }
            }

            // Failed to update due to collision
            if (nAttempts == 0)
            {
                if (player.oldHookLength != player.hookLength)
                {
                    player.hookLength = player.oldHookLength;
                }
                player.hookAngle = oldAngle;
                xOffset = (player.hookLength * SIN(player.hookAngle)) >> 3;
                yOffset = (player.hookLength * COS(player.hookAngle)) >> 3;
                player.x = player.hookX + xOffset;
                player.y = player.hookY + yOffset;
            }
            else
            {
                player.hookAngle = player.hookAngle + tmpAngle;
                player.angularVel = -(player.angularVel >> 2);
            }
        }

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

    if (player.x != player.oldX || player.y != player.oldY)
    {
        player.oldX = player.x;
        player.oldY = player.y;

        uint16_t playerPixelX = (player.x >> 4);
        uint16_t playerPixelY = (player.y >> 4);

        if (playerPixelX > 76) camera.x = playerPixelX - 76;
        if (playerPixelY > 144) camera.y = playerPixelY - 144;
        camera.redraw = 1;
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
