#ifndef _PHYSICS_H
#define _PHYSICS_H

#include "utils.h"
#include "collisions.h"
#include "player.h"
#include "game.h"
#include "key_obj.h"

// physics constants
#define MAX_X_SPEED_IN_SUBPIXELS 28
#define MAX_X_GROUND_SPEED_IN_SUBPIXELS 24
#define X_ACCELERATION_IN_SUBPIXELS 2
#define X_GROUND_ACCELERATION_IN_SUBPIXELS 4
#define X_DECELERATION_IN_SUBPIXELS 1
#define X_GROUND_DECELERATION_IN_SUBPIXELS 3
#define MAX_Y_SPEED_IN_SUBPIXELS 32
#define Y_ACCELERATION_IN_SUBPIXELS 2
#define GRAVITY_CONST 3
#define JUMP_ACCELERATION_IN_SUBPIXELS 48
#define INPUT_ANGULAR_ACC 5
#define MAX_ANGULAR_VELOCITY 80
#define MIN_ANGULAR_VELOCITY -80
#define MIN_HOOK_DISTANCE 112 // 6 pixels
#define MAX_HOOK_DISTANCE 1792 // 80 pixels
#define MAX_STRAIGHT_HOOK_DISTANCE 1280 // 80 pixels
#define MAX_HOOK_LENGTH 70 // 80 pixels
#define MIN_HOOK_LENGTH 12 // 12 pixels
#define MAX_ADJUST_ANGLE 24

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))
#define CLAMP(a, min, max) (((a) < (min)) ? (min) : (((a) > (max)) ? (max) : (a)))

extern const int8_t sine_wave[256];

#define SIN(a)  (sine_wave[(uint8_t)(a)])
#define COS(a)  (sine_wave[(uint8_t)((uint8_t)(a) + 64u)])

#define ANGLE_0DEG      0
#define ANGLE_45DEG     32
#define ANGLE_90DEG     64
#define ANGLE_135DEG    96
#define ANGLE_180DEG    128
#define ANGLE_225DEG    160
#define ANGLE_270DEG    192
#define ANGLE_315DEG    224
#define ANGLE_360DEG    255

void apply_physics(void);
uint16_t isqrt(uint16_t x) NONBANKED;

#endif
