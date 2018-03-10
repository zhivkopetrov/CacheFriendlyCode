/*
 * Bullet.cpp
 *
 *  Created on: 8 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "Bullet.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstdio>
#include <algorithm>

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"

#define MOVE_TIMER_INTERVAL 50000 //in microseconds
#define FRAMES_PER_DIRECTION 4

#define BULLET_SPEED 15
#define BULLET_MAX_X 1916 //1920 - 4
#define BULLET_MAX_Y 1076 //1080 - 4


Bullet::Bullet() : numActive(0)
{
	for(int32_t i = 0; i < MAX_BULLETS; ++i)
	{
		_moveTimer[i] = MOVE_TIMER_INTERVAL;
	}

    memset(_currDirection, 0, sizeof(_currDirection));
}

int32_t Bullet::init(const uint8_t rsrcId)
{
    for(int32_t i = 0; i < MAX_BULLETS; ++i)
    {
        drawParams[i].rsrcId = rsrcId;
        drawParams[i].frame  = 0;
        drawParams[i].pos.x  = 0;
        drawParams[i].pos.y  = 0;
    }

    return EXIT_SUCCESS;
}

void Bullet::updateState(const int32_t elapsedTime)
{
    const int32_t NUM_ACTIVE = numActive;

    for(int32_t i = 0; i < NUM_ACTIVE; ++i)
    {
        _moveTimer[i] -= elapsedTime;
    }

    for(int32_t i = 0; i < NUM_ACTIVE; ++i)
    {
        if(0 > _moveTimer[i])
        {
            move(i);
        }
    }
}

void Bullet::fireNext(const int32_t startX,
				      const int32_t startY,
			          const uint8_t startDirection)
{
	drawParams[numActive].pos.x = startX;
	drawParams[numActive].pos.y = startY;
	_currDirection[numActive]   = startDirection;

    ++numActive;
}

void Bullet::move(const int32_t index)
{
    _moveTimer[index] = MOVE_TIMER_INTERVAL;

    switch (_currDirection[index])
    {
        case Direction::DOWN:
            drawParams[index].pos.y += BULLET_SPEED;

            if(BULLET_MAX_Y < drawParams[index].pos.y)
            {
                deactivate(index);
            }
            break;

        case Direction::LEFT:
            drawParams[index].pos.x -= BULLET_SPEED;

            if(0 > drawParams[index].pos.x)
            {
                deactivate(index);
            }
            break;

        case Direction::RIGHT:
            drawParams[index].pos.x += BULLET_SPEED;

            if(BULLET_MAX_X < drawParams[index].pos.x)
            {
                deactivate(index);
            }
            break;

        case Direction::UP:
            drawParams[index].pos.y -= BULLET_SPEED;

            if(0 > drawParams[index].pos.y)
            {
                deactivate(index);
            }
            break;

        default:
            break;
    }
}

void Bullet::deactivate(const int32_t index)
{
    --numActive;

    std::swap(drawParams[index], drawParams[numActive]);
    std::swap(_moveTimer[index], _moveTimer[numActive]);
    std::swap(_currDirection[index], _currDirection[numActive]);
}


