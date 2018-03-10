/*
 * Hero.cpp
 *
 *  Created on: Feb 8, 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "Hero.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstdio>

//Other libraries headers
#include <SDL2/SDL_events.h>
#include <SDL2/SDL_rect.h>

//Own components headers
#include "common/CommonDefines.h"
#include "common/Random.hpp"

#define MOVE_TIMER_INTERVAL 50000 //in microseconds
#define FRAMES_PER_DIRECTION 6

#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080


Hero::Hero() : _WIDTH(34),
			   _HEIGHT(61),
			   _MAX_X(1920 - _WIDTH),
			   _MAX_Y(1080 - _HEIGHT),
			   _SPEED(5),
			   _monsters(nullptr),
			   _bullets(nullptr),
			   _bulletIcons(nullptr),
			   _moveTimer(MOVE_TIMER_INTERVAL),
			   _currDirection(0),
			   _newDirection(0),
			   _currDirectionFrame(0),
			   _firedBullets(0),
			   _isMoving(false)
{

}

void Hero::updateState(const int64_t elapsedTime)
{
    _moveTimer -= elapsedTime;

    if(0 > _moveTimer)
    {
        _moveTimer = MOVE_TIMER_INTERVAL;

        if(_isMoving)
        {
            move();
        }
    }
}

void Hero::updatePhysics()
{
    const SDL_Rect heroBoundaryRect = getBoundaryRect();

    for(Monster & monster : (*_monsters))
    {
        const SDL_Rect boundaryRect = monster.getBoundaryRect();
        if(SDL_HasIntersection(&heroBoundaryRect, &boundaryRect))
        {
            _isActive = false;

            break;
        }
    }
}

void Hero::handleEvent(const SDL_Event & e)
{
    if(SDL_KEYUP == e.type)
    {
        _isMoving = false;
    }
    else if(SDL_KEYDOWN == e.type)
    {
        switch (e.key.keysym.sym)
        {
            case SDLK_DOWN:
                _newDirection = Direction::DOWN;
                _isMoving = true;
                break;

            case SDLK_LEFT:
                _newDirection = Direction::LEFT;
                _isMoving = true;
                break;

            case SDLK_RIGHT:
                _newDirection = Direction::RIGHT;
                _isMoving = true;
                break;

            case SDLK_UP:
                _newDirection = Direction::UP;
                _isMoving = true;
                break;

            case SDLK_SPACE:
                tryFireNextBullet();
                break;

            default:
                break;
        }
    }
}

void Hero::chooseNonLosingStartPosition()
{
    bool isValidPosition = true;

    do
    {
        isValidPosition = true;

        _drawParams.pos.x = Random::getInstance().generate(0, _MAX_X);
        _drawParams.pos.y = Random::getInstance().generate(0, _MAX_Y);

        const SDL_Rect heroBoundaryRect = getBoundaryRect();

        for(Monster & monster : (*_monsters))
        {
            const SDL_Rect boundaryRect = monster.getBoundaryRect();
            if(SDL_HasIntersection(&heroBoundaryRect, &boundaryRect))
            {
                isValidPosition = false;

                break;
            }
        }
    } while(!isValidPosition);
}

void Hero::move()
{
    if(_newDirection == _currDirection)
    {
        ++_currDirectionFrame;

        if(_currDirectionFrame >= FRAMES_PER_DIRECTION)
        {
            _currDirectionFrame = 0;
        }
    }
    else
    {
        _currDirection = _newDirection;
        _currDirectionFrame = 0;
    }

    _drawParams.frame =
                (_currDirection * FRAMES_PER_DIRECTION) + _currDirectionFrame;

    switch (_currDirection)
    {
        case Direction::DOWN:
            _drawParams.pos.y += _SPEED;

            if(_MAX_Y < _drawParams.pos.y)
            {
                _drawParams.pos.y = _MAX_Y;
            }
            break;

        case Direction::LEFT:
            _drawParams.pos.x -= _SPEED;

            if(0 > _drawParams.pos.x)
            {
                _drawParams.pos.x = 0;
            }
            break;

        case Direction::RIGHT:
            _drawParams.pos.x += _SPEED;

            if(_MAX_X < _drawParams.pos.x)
            {
                _drawParams.pos.x = _MAX_X;
            }
            break;

        case Direction::UP:
            _drawParams.pos.y -= _SPEED;

            if(0 > _drawParams.pos.y)
            {
                _drawParams.pos.y = 0;
            }
            break;

        default:
            break;
    }
}

void Hero::tryFireNextBullet()
{
    //no bullets left
    if(MAX_BULLETS <= _firedBullets)
    {
        return;
    }

    (*_bullets).emplace_back(_currDirection);

    if(EXIT_SUCCESS != (*_bullets).back().init(
    						   _rendererInterface,
							   Textures::BULLET,
							   0,
							   SDL_Point{ _drawParams.pos.x + _WIDTH / 2,
										  _drawParams.pos.y + _HEIGHT / 2 }))
    {
        fprintf(stderr, "Error in _bullets.init()\n");
    }

    (*_bulletIcons).back().hide();

    ++_firedBullets;
}



