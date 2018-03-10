/*
 * BulletIcon.cpp
 *
 *  Created on: 15 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "BulletIcon.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstdio>
#include <algorithm>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"
#include "common/Random.hpp"


BulletIcon::BulletIcon() : numActive(MAX_BULLET_ICONS)
{

}

int32_t BulletIcon::init(const uint8_t rsrcId)
{
    const int32_t BULLET_ICON_START_X = 1500;
    const int32_t BULLET_ICON_START_Y = 10;
    const int32_t OFFSET_X = 35;

    for(int32_t i = 0; i < MAX_BULLET_ICONS; ++i)
    {
        drawParams[i].rsrcId = rsrcId;
        drawParams[i].frame  = 0;
        drawParams[i].pos.x  = BULLET_ICON_START_X +
                                        ((MAX_BULLET_ICONS - i - 1) * OFFSET_X);
        drawParams[i].pos.y  = BULLET_ICON_START_Y;
    }

    return EXIT_SUCCESS;
}



