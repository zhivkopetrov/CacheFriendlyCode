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
#include "common/CommonDefines.h"
#include "common/Random.hpp"

#define MOVE_TIMER_INTERVAL 100000 //in microseconds
#define CHANGE_DIR_TIMER_INTERVAL_BASE 200000 //in microseconds (0.2 sec)
#define FRAMES_PER_DIRECTION 4

#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080


Monster::Monster() : _width(40),
					 _height(36),
					 _maxX(MONITOR_WIDTH - _width),
					 _maxY(MONITOR_HEIGHT - _height),
                     _speed(5),
                     _bullets(nullptr),
                     _moveTimer(MOVE_TIMER_INTERVAL),
                     _currDirection(0),
                     _currDirectionFrame(0)
{
    chooseRandomDirection();
}

void Monster::updateState(const int64_t elapsedTime)
{
    _moveTimer -= elapsedTime;
    _changeDirTimer -= elapsedTime;

    if(0 > _moveTimer)
    {
        move();
    }

    if(0 > _changeDirTimer)
    {
        chooseRandomDirection();
    }
}

void Monster::updatePhysics()
{
    const SDL_Rect monsterBoundaryRect = getBoundaryRect();

    for(Bullet & bullet : (*_bullets))
    {
        if(!bullet.isActive())
        {
            continue;
        }

        const SDL_Rect boundaryRect = bullet.getBoundaryRect();
        if(SDL_HasIntersection(&monsterBoundaryRect, &boundaryRect))
        {
            _isActive = false;

            bullet.destroy();

            break;
        }
    }
}

void Monster::chooseRandomDirection()
{
    _currDirection =
          Random::getInstance().generate(Direction::DOWN, Direction::COUNT - 1);

    _currDirectionFrame = _currDirection * FRAMES_PER_DIRECTION;

    _changeDirTimerFrameLimit =
         CHANGE_DIR_TIMER_INTERVAL_BASE * Random::getInstance().generate(1, 20);

    _changeDirTimer = _changeDirTimerFrameLimit;
}

void Monster::move()
{
    _moveTimer = MOVE_TIMER_INTERVAL;

    ++_currDirectionFrame;

    if(_currDirectionFrame >= FRAMES_PER_DIRECTION)
    {
        _currDirectionFrame = 0;
    }

    _drawParams.frame =
                (_currDirection * FRAMES_PER_DIRECTION) + _currDirectionFrame;

    switch (_currDirection)
    {
        case Direction::DOWN:
            _drawParams.pos.y += _speed;

            if(_maxY < _drawParams.pos.y)
            {
                _drawParams.pos.y = _maxY;
            }
            break;

        case Direction::LEFT:
            _drawParams.pos.x -= _speed;

            if(0 > _drawParams.pos.x)
            {
                _drawParams.pos.x = 0;
            }
            break;

        case Direction::RIGHT:
            _drawParams.pos.x += _speed;

            if(_maxX < _drawParams.pos.x)
            {
                _drawParams.pos.x = _maxX;
            }
            break;

        case Direction::UP:
            _drawParams.pos.y -= _speed;

            if(0 > _drawParams.pos.y)
            {
                _drawParams.pos.y = 0;
            }
            break;

        default:
            break;
    }
}



