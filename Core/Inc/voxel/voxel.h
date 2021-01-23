// (c) 2021 anton.krug@gmail.com
// This code is licensed under MIT license (see LICENSE.txt for details)

#ifndef VOXEL_CONFIGURATION_H_
#define VOXEL_CONFIGURATION_H_

#include "color.h"
#include "height.h"


#define WIDTH  96
#define HEIGHT_BITS 6
#define HEIGHT (1 << (HEIGHT_BITS)) // 64

// Needs to match MAP settings of the included files
#define VOXEL_MAP_BITS 8            // 256 x 256
#define VOXEL_MAP_RESOLUTION (1 << (VOXEL_MAP_BITS))

#define VOXEL_MAP_COLOR  color_map
#define VOXEL_MAP_HEIGHT height_map

#define FIXED_POINT_BITS             15                                     // it's 15 as it will be shifted twice and still has to fit into 32-bit (signed int has 1bit for sign, so we have to fit into 31-bit)
#define FIXED_POINT_MAJOR_MASK       (0xffffffff ^ ((1<<FIXED_POINT_BITS)-1))           // it's 15 as it will be shifted twice and still has to fit into 32-bit (signed int has 1bit for sign, so we have to fit into 31-bit)
#define FIXED_POINT_ONE_FOR_DIVISION (1<<(2*FIXED_POINT_BITS))              // 1 shifted twice by FIXED_POINT_BITS to prepare it for a division
#define FIXED_POINT_INVERSE(X)       (FIXED_POINT_ONE_FOR_DIVISION/(X))
#define FLOAT_TO_FIXED_POINT(X)      ((int32_t)((X)*(1<<FIXED_POINT_BITS)))
#define INT_TO_FIXED_POINT(X)        ((int32_t)((X)<<FIXED_POINT_BITS))
#define FIXED_POINT_TO_INT(X)        ((X)>>FIXED_POINT_BITS)

#define VOXEL_MAP_HEIGHT_TALL 12           // Height map is 0 to 255, how much real height it is

// Camera / View / Vehicle settings
#define VOXEL_BACKGROUND_COLOR      159                      // Background sky color
#define VOXEL_FOG_START             INT_TO_FIXED_POINT(150)  // Distance at where it will start fading
#define VOXEL_ANIMATION_STEP        0.004f                   // The smaller number the smoother the animation is
#define VOXEL_VEHICLE_TILT_FACTOR   250.0f                   // The bigger number the more tilting is applied
#define VOXEL_VEHICLE_HEIGHT_FACTOR 2.15f                    // The higher number the lower the aircraft


#define VOXEL_CAMERA_START_Z FLOAT_TO_FIXED_POINT(8.0f)

// This affect the quality at distance
#define VOXEL_LOD_BEGIN          FLOAT_TO_FIXED_POINT(65.0f)   // At distance 50 start decreasing quality
#define VOXEL_LOD_STEP_INCREMENT FLOAT_TO_FIXED_POINT(0.1f)    // On each step increase the step by
#define VOXEL_MAX_DRAW_DISTANCE  FLOAT_TO_FIXED_POINT(250.0f)  // Where the camera distance ends rendering

#define VOXEL_DEG_TO_RAD 0.0174533f         // 2*pi/360
#define VOXEL_FULL_CIRCLE_IN_RAD 6.2831853f // lazy 2pi estimation

#define VOXEL_FULL_CIRCLE_STEPS ((int)(VOXEL_FULL_CIRCLE_IN_RAD / VOXEL_ANIMATION_STEP))


typedef struct {
	int32_t x;
	int32_t y;
} Pair32;


typedef struct {
	int32_t x;
	int32_t y;
} PairFixedPoint;


typedef struct {
	float x;
	float y;
} PairFloat;


uint32_t voxelAnimationSingleLoop();


#endif /* VOXEL_CONFIGURATION_H_ */
