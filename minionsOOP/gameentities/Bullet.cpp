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

//Other libraries headers

//Own components headers
#include "common/CommonDefines.h"

#define MOVE_TIMER_INTERVAL 50000 //in microseconds
#define FRAMES_PER_DIRECTION 4

#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080


Bullet::Bullet(const int32_t direction) : _width(4),
								    	  _height(4),
									      _maxX(MONITOR_WIDTH - _width),
									      _maxY(MONITOR_HEIGHT - _height),
									      _speed(15),
									      _moveTimer(MOVE_TIMER_INTERVAL),
									      _currDirection(0)
{
    _currDirection  = direction;
}

void Bullet::updateState(const int64_t elapsedTime)
{
    _moveTimer -= elapsedTime;

    if(0 > _moveTimer)
    {
        move();
    }
}

void Bullet::move()
{
    _moveTimer = MOVE_TIMER_INTERVAL;

    switch (_currDirection)
    {
        case Direction::DOWN:
            _drawParams.pos.y += _speed;

            if(_maxY < _drawParams.pos.y)
            {
                _isActive = false;
            }
            break;

        case Direction::LEFT:
            _drawParams.pos.x -= _speed;

            if(0 > _drawParams.pos.x)
            {
                _isActive = false;
            }
            break;

        case Direction::RIGHT:
            _drawParams.pos.x += _speed;

            if(_maxX < _drawParams.pos.x)
            {
                _isActive = false;
            }
            break;

        case Direction::UP:
            _drawParams.pos.y -= _speed;

            if(0 > _drawParams.pos.y)
            {
                _isActive = false;
            }
            break;

        default:
            break;
    }
}


