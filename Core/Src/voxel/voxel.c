// (c) 2021 anton.krug@gmail.com
// This code is licensed under MIT license (see LICENSE.txt for details)

#include <math.h>
#include <string.h>

#include "voxel/voxel.h" // The map settings need to match the included map

#include "drivers/oled_for_c.h"

#define FIXED_POINT_BITS 16


uint8_t buffer[2][WIDTH * HEIGHT]  __attribute__ ((aligned));
uint8_t zBuffer[WIDTH] __attribute__ ((aligned));
uint32_t frame = 0;


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


void lineVertical(uint32_t x, int32_t yBegin, uint32_t yEnd, uint8_t color) {
	if (yBegin < 0) yBegin = 0; // When start is outside the screen, limit it
	uint32_t offset = yBegin |  ( x << HEIGHT_BITS);
	uint8_t *bufferCur = &buffer[frame%2][offset];

	for (uint32_t index = yBegin; index < yEnd; ++index, bufferCur++) {
		*bufferCur = color;
	}
}


void cleanZbuffer() {
	// Clean z buffer fully
	for (uint32_t index = 0; index < WIDTH; ++index) {
		zBuffer[index] = HEIGHT;
	}
}



void cleanBuffer(uint8_t* buffer) {
	for (uint32_t ix = 0; ix < WIDTH * HEIGHT; ++ix, buffer++) {
		*buffer = VOXEL_BACKGROUND_COLOR;
	}
}


void cleanTheSkybox() {
	// Wipe only section which need to be (as little as you get away with)
	uint8_t *bufferCurLine = buffer[frame%2];

	for (uint32_t ix = 0; ix < WIDTH; ++ix) {
		uint8_t *bufferCur = bufferCurLine;
		for (uint32_t iy = 0; iy < zBuffer[ix]; ++iy, bufferCur++) {
			*bufferCur = VOXEL_BACKGROUND_COLOR;
		}
		bufferCurLine += HEIGHT;
	}
}


uint32_t calculateMapOffset(uint32_t x, uint32_t y)  {
	// Calculate index depending on the current coordinates and resolution
	return (x % VOXEL_MAP_RESOLUTION) | ((y % VOXEL_MAP_RESOLUTION) << VOXEL_MAP_BITS);
}


PairFloat infiniteSymbolPath(float time) {
	// Quick and dirty infinity symbol estimation
	// https://gamedev.stackexchange.com/questions/43691
	const PairFloat ret = {
		(110 + 2 * VOXEL_MAP_RESOLUTION) + 147 * cosf(time),
		(70 + 2 * VOXEL_MAP_RESOLUTION) + 115 * sinf(time*2.0f)
	};

	return ret;
}


float angleOfTwoPoints(PairFloat a, PairFloat b) {
	// Calculating angle and offset it to make it work in our reference
	return (VOXEL_FULL_CIRCLE_IN_RAD / 4) - atan2f(a.y - b.y, a.x - b.x);
}


uint32_t calculateAltitude(float time, uint32_t previousAltitude) {
	// Look up the the trajectory ahead of the time and smooth out the
	// altitude difference so the camera will not jump up/down in erratic manner
	const uint32_t altitudeOffset = 60;
	uint32_t maxAltitude = 0;

	// Detect the highest point closely ahead of the path
	for (uint32_t index = 0; index < 5; ++index) {
		// Look up only few steps ahead
		PairFloat camera          = infiniteSymbolPath(time);
		uint32_t  probingAltitude = altitudeOffset + VOXEL_MAP_HEIGHT[calculateMapOffset(camera.x, camera.y)];
		if (maxAltitude < probingAltitude) {
			maxAltitude = probingAltitude;
		}
		time += VOXEL_ANIMATION_STEP;
	}

	if ((previousAltitude > (maxAltitude-5)) & (previousAltitude < (maxAltitude+5))) {
		// The altitudes are close enough to be the same and do not do anythin
		return previousAltitude;
	} else if (previousAltitude > maxAltitude) {
		// We are above the future altitude, glide down
		return previousAltitude-1;
	} else {
		// We are below future altitude, slowly climb up
		return (uint32_t)((maxAltitude - previousAltitude) / 10.0f) + previousAltitude;
	}
}


void renderScreen(Pair32 screen, PairFloat origin, float angle, int32_t altitude, uint32_t pitchBegin, uint32_t pitchEnd) {
    const float   angleSin   = sinf(angle);
    const float   angleCos   = cosf(angle);
    const float   pitchStep  = ((float)pitchEnd - (float)pitchBegin) / (float)screen.x;
          float   zIncrement = 1.0f; // starting z increment value


	for (float z = VOXEL_CAMERA_START_Z; z < VOXEL_MAX_DRAW_DISTANCE; z = z + zIncrement) {
		// Start from close by to far, but first few steps as they not visible
		const float inverseZ  = 1.0f / (float)(z) * VOXEL_MAP_HEIGHT_TALL;

		PairFixedPoint begin = {
				((-angleCos - angleSin) * z + origin.x) * (1 << FIXED_POINT_BITS),
				((angleSin - angleCos) * z + origin.y)  * (1 << FIXED_POINT_BITS)
		};

		PairFixedPoint end = {
				((angleCos - angleSin) * z + origin.x) * (1 << FIXED_POINT_BITS),
				((-angleSin - angleCos) * z + origin.y) * (1 << FIXED_POINT_BITS)
		};

		// When begin and end locations on the map are calculated, calculate
		// how big the steps needs to be made to get from begin to end when
		// going through the screen horizontally
		PairFixedPoint step = {
				(end.x - begin.x) / screen.x,
				(end.y - begin.y) / screen.x
		};

		PairFixedPoint current = {begin.x, begin.y};

		float pitch = pitchBegin;
		for (uint32_t x = 0; x < screen.x; ++x, pitch += pitchStep) {
			// For each horizontal point on the screen calculate the corresponding voxel
			uint32_t mapOffset      = calculateMapOffset(current.x >> FIXED_POINT_BITS, current.y >> FIXED_POINT_BITS);
			int32_t  heightOnScreen = fmaxf(0.0f, (altitude - VOXEL_MAP_HEIGHT[mapOffset]) * inverseZ + pitch);

			if (heightOnScreen < zBuffer[x]) {
				// Render the location when it's not obscured by previous voxel
				uint8_t color = VOXEL_MAP_COLOR[mapOffset];
				if ( z > VOXEL_FOG_START ) {
					// When we are far away apply fast low-quality fading effect
					color = ((color & 0xDA) >> 1 ) | 0x92;
				}

				// Start/end and the color of the line calculated
				lineVertical(x, heightOnScreen, zBuffer[x], color);

				// Mark this part of the screen as obscured from now on
				zBuffer[x] = heightOnScreen;
			}
			current.x += step.x;
			current.y += step.y;
		}

		if (z > VOXEL_LOD_BEGIN * 2) {
			// When even further away decrease the quality even more
			zIncrement += VOXEL_LOD_STEP_INCREMENT * 4;
		} else if (z > VOXEL_LOD_BEGIN) {
			// When far away decrease the quality
			zIncrement += VOXEL_LOD_STEP_INCREMENT;
		}
	}
}


uint32_t voxelAnimationSingleLoop() {
	const Pair32 screenResolution = { WIDTH, HEIGHT };
	PairFloat cameraNow = infiniteSymbolPath(0.0f);
	PairFloat cameraNext;
	float pointingToOld = 0.0f;
	static uint32_t altitudeOld = 255;

	// Clean the buffers for the very first updates
	cleanBuffer(buffer[0]);
	cleanBuffer(buffer[1]);

	frame = 0;
	for (float step = 0.0f; step < VOXEL_FULL_CIRCLE_IN_RAD; step += VOXEL_ANIMATION_STEP) {
		cleanZbuffer();
		uint32_t altitude = calculateAltitude(step, altitudeOld);
		cameraNext = infiniteSymbolPath(step + VOXEL_ANIMATION_STEP);

		// Following the curve, calculate angle of the camera where to point
		float pointingTo = angleOfTwoPoints(cameraNow, cameraNext);

		// Depending on old and current camera angle, estimated the roll
		float roll = (pointingTo - pointingToOld) * VOXEL_VEHICLE_TILT_FACTOR;

		// Significantly amplify the estimated roll
		renderScreen(screenResolution, cameraNow, pointingTo, altitude,
				(HEIGHT / VOXEL_VEHICLE_HEIGHT_FACTOR) - roll,
				(HEIGHT / VOXEL_VEHICLE_HEIGHT_FACTOR) + roll);

		// Only fill leftover vertical lines with the skybox
//		cleanTheSkybox();

		// Flush the calculated buffer into the screen
		oledUpdateScreenFromBuffer(buffer[frame%2],  WIDTH * HEIGHT);

		// Store the values for the next iteration
		cameraNow     = cameraNext;
		pointingToOld = pointingTo;
		altitudeOld   = altitude;
		frame++;
	}

	return frame;
}

