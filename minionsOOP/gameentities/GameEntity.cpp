/*
 * GameEntity.cpp
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "GameEntity.h"

//C system headers

//C++ system headers
#include <cstdio>

//Other libraries headers

//Own components headers


GameEntity::GameEntity() : _rendererInterface(nullptr),
                           _isActive(true)
{

}

int32_t GameEntity::init(RendererProxyInterface * interface,
                         const uint8_t            rsrcId,
                         const uint8_t            startFrame,
                         const SDL_Point          startPoint)
{
    _rendererInterface = interface;
    _drawParams.rsrcId = rsrcId;
    _drawParams.frame  = startFrame;
    _drawParams.pos    = startPoint;

    return EXIT_SUCCESS;
}


