/*
 * Monster.cpp
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "Monster.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstdio>

//Other libraries headers

//Own components headers
#include "Bullet.h"

#include "common/CommonDefines.h"
#include "common/Random.hpp"

#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080

#define MOVE_TIMER_INTERVAL 100000 //in microseconds
#define CHANGE_DIR_TIMER_INTERVAL_BASE 200000 //in microseconds (0.2 sec)
#define FRAMES_PER_DIRECTION 4

#define MONSTER_MAX_X 1880 //1920 - 40
#define MONSTER_MAX_Y 1044 //1080 - 36

#define MONSTER_SPEED 5


Monster::Monster() : numActive(MAX_MONSTERS),
					 _bullets(nullptr)
{
	for(int32_t i = 0; i < MAX_MONSTERS; ++i)
	{
		_moveTimer[i] = MOVE_TIMER_INTERVAL;
	}

    memset(_changeDirTimer, 0, sizeof(_changeDirTimer));
    memset(_changeDirTimerFrameLimit, 0, sizeof(_changeDirTimerFrameLimit));
    memset(_currDirection, 0, sizeof(_currDirection));
    memset(_currDirectionFrame, 0, sizeof(_currDirectionFrame));
}

int32_t Monster::init(const uint8_t rsrcId)
{
    for(int32_t i = 0; i < MAX_MONSTERS; ++i)
    {
        drawParams[i].rsrcId = rsrcId;
        drawParams[i].frame = 0;
        drawParams[i].pos.x = Random::getInstance().generate(0, MONSTER_MAX_X);
        drawParams[i].pos.y = Random::getInstance().generate(0, MONSTER_MAX_Y);
    }

    for(int32_t i = 0; i < MAX_MONSTERS; ++i)
    {
        chooseRandomDirection(i);
    }

    return EXIT_SUCCESS;
}

void Monster::updateState(const int32_t elapsedTime)
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

    for(int32_t i = 0; i < NUM_ACTIVE; ++i)
    {
        _changeDirTimer[i] -= elapsedTime;
    }

    for(int32_t i = 0; i < NUM_ACTIVE; ++i)
    {
        if(0 > _changeDirTimer[i])
        {
            chooseRandomDirection(i);
        }
    }
}

void Monster::updatePhysics()
{
    SDL_Rect bulletRect = { 0, 0, BULLET_WIDTH, BULLET_HEIGHT };
    SDL_Rect monsterRect = { 0, 0, MONSTER_WIDTH, MONSTER_HEIGHT };

    for(int32_t i = 0; i < _bullets->numActive; ++i)
    {
        bulletRect.x = _bullets->drawParams[i].pos.x;
        bulletRect.y = _bullets->drawParams[i].pos.y;

        for(int32_t j = 0; j < numActive; ++j)
        {
            monsterRect.x = drawParams[j].pos.x;
            monsterRect.y = drawParams[j].pos.y;

            if(SDL_HasIntersection(&bulletRect, &monsterRect))
            {
                deactivate(j);

                _bullets->deactivate(i);
                --i;

                break;
            }
        }
    }
}

void Monster::chooseRandomDirection(const int32_t index)
{
    _currDirection[index] =
          Random::getInstance().generate(Direction::DOWN, Direction::COUNT - 1);

    _currDirectionFrame[index] = _currDirection[index] * FRAMES_PER_DIRECTION;

    _changeDirTimerFrameLimit[index] =
         CHANGE_DIR_TIMER_INTERVAL_BASE * Random::getInstance().generate(1, 20);

    _changeDirTimer[index] = _changeDirTimerFrameLimit[index];
}

void Monster::move(const int32_t index)
{
    _moveTimer[index] = MOVE_TIMER_INTERVAL;

    ++_currDirectionFrame[index];

    if(_currDirectionFrame[index] >= FRAMES_PER_DIRECTION)
    {
        _currDirectionFrame[index] = 0;
    }

    drawParams[index].frame = (_currDirection[index] * FRAMES_PER_DIRECTION) +
                                                    _currDirectionFrame[index];

    switch (_currDirection[index])
    {
        case Direction::DOWN:
            drawParams[index].pos.y += MONSTER_SPEED;

            if(MONSTER_MAX_Y < drawParams[index].pos.y)
            {
                drawParams[index].pos.y = MONSTER_MAX_Y;
            }
            break;

        case Direction::LEFT:
            drawParams[index].pos.x -= MONSTER_SPEED;

            if(0 > drawParams[index].pos.x)
            {
                drawParams[index].pos.x = 0;
            }
            break;

        case Direction::RIGHT:
            drawParams[index].pos.x += MONSTER_SPEED;

            if(MONSTER_MAX_X < drawParams[index].pos.x)
            {
                drawParams[index].pos.x = MONSTER_MAX_X;
            }
            break;

        case Direction::UP:
            drawParams[index].pos.y -= MONSTER_SPEED;

            if(0 > drawParams[index].pos.y)
            {
                drawParams[index].pos.y = 0;
            }
            break;

        default:
            break;
    }
}

void Monster::deactivate(const int32_t index)
{
    --numActive;

    std::swap(drawParams[index], drawParams[numActive]);
    std::swap(_moveTimer[index], _moveTimer[numActive]);
    std::swap(_changeDirTimer[index], _changeDirTimer[numActive]);
    std::swap(_changeDirTimerFrameLimit[index],
                                         _changeDirTimerFrameLimit[numActive]);
    std::swap(_currDirection[index], _currDirection[numActive]);
    std::swap(_currDirectionFrame[index], _currDirectionFrame[numActive]);
}



