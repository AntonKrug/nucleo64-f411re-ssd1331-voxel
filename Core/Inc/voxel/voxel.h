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

#define VOXEL_MAP_HEIGHT_TALL 12.0f        // Height map is 0 to 255, how much real height it is

// Camera / View / Vehicle settings
#define VOXEL_BACKGROUND_COLOR 159         // Background sky color
#define VOXEL_ANIMATION_STEP 0.004f        // The smaller number the smoother the animation is
#define VOXEL_VEHICLE_TILT_FACTOR   250.0f // The bigger number the more tilting is applied
#define VOXEL_VEHICLE_HEIGHT_FACTOR 2.15f  // The higher number the lower the aircraft
#define VOXEL_CAMERA_START_Z 8.0f


// This affect the quality at distance
#define VOXEL_LOD_BEGIN 65                  // At distance 50 start decreasing quality
#define VOXEL_LOD_STEP_INCREMENT 0.1f       // On each step increase the step by
#define VOXEL_MAX_DRAW_DISTANCE 250

#define VOXEL_DEG_TO_RAD 0.0174533f         // 2*pi/360
#define VOXEL_FULL_CIRCLE_IN_RAD 6.2831853f // lazy 2pi estimation


uint32_t voxelAnimationSingleLoop();


#endif /* VOXEL_CONFIGURATION_H_ */
