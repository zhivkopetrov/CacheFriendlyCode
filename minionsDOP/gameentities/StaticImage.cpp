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


StaticImage::StaticImage() : _width(0),
                             _height(0)
{

}

int32_t StaticImage::init(const uint8_t   rsrcId,
                          const SDL_Point startPoint,
                          const int32_t   width,
                          const int32_t   height)
{
    drawParams.rsrcId = rsrcId;
    drawParams.frame  = 0;
    drawParams.pos    = startPoint;

    _width            = width;
    _height           = height;

    return EXIT_SUCCESS;
}


