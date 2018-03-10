/*
 * StaticImage.cpp
 *
 *  Created on: 8 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "StaticImage.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstdio>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"

#define MOVE_TIMER_INTERVAL 50000 //in microseconds
#define FRAMES_PER_DIRECTION 4

#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080


StaticImage::StaticImage() : _width(0),
                             _height(0)
{
    _drawParams.frame = 0;
}

int32_t StaticImage::init(RendererProxyInterface * interface,
                          const uint8_t            rsrcId,
                          const uint8_t            startFrame,
                          const SDL_Point          startPoint,
                          const int32_t            width,
                          const int32_t            height)
{
    _width  = width;
    _height = height;

    return GameEntity::init(interface, rsrcId, startFrame, startPoint);
}


