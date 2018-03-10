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
#include "Monster.h"
#include "Bullet.h"
#include "BulletIcon.h"

#include "common/CommonDefines.h"
#include "common/Random.hpp"

#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080

#define MOVE_TIMER_INTERVAL 50000 //in microseconds
#define FRAMES_PER_DIRECTION 6

#define HERO_SPEED 5
#define HERO_MAX_X 1886 //1920 - 34
#define HERO_MAX_Y 1019 //1080 - 61


Hero::Hero() : _monsters(nullptr),
			   _bullets(nullptr),
			   _bulletIcons(nullptr),
			   _moveTimer(MOVE_TIMER_INTERVAL),
			   _firedBullets(0),
			   _currDirection(0),
			   _newDirection(0),
			   _currDirectionFrame(0),
			   _isMoving(false),
			   _isActive(true)
{

}

int32_t Hero::init(const uint8_t   rsrcId,
				   const uint8_t   startFrame,
				   const SDL_Point startPoint)
{
    drawParams.rsrcId = rsrcId;
    drawParams.frame  = startFrame;
    drawParams.pos    = startPoint;

    return EXIT_SUCCESS;
}

void Hero::updateState(const int32_t elapsedTime)
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
    const SDL_Rect heroBoundaryRect =
    			{ drawParams.pos.x, drawParams.pos.y, HERO_WIDTH, HERO_HEIGHT };

    SDL_Rect monsterBoundaryRect = { 0, 0, MONSTER_WIDTH, MONSTER_HEIGHT };

    SDL_Point monsterPos;

    for(int32_t i = 0; i < _monsters->numActive; ++i)
    {
        monsterPos = _monsters->drawParams[i].pos;

        monsterBoundaryRect.x = monsterPos.x;
        monsterBoundaryRect.y = monsterPos.y;

        if(SDL_HasIntersection(&heroBoundaryRect, &monsterBoundaryRect))
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

        drawParams.pos.x = Random::getInstance().generate(0, HERO_MAX_X);
        drawParams.pos.y = Random::getInstance().generate(0, HERO_MAX_Y);

        const SDL_Rect heroBoundaryRect = getBoundaryRect();
        SDL_Rect monsterBoundaryRect;

        const int32_t numMonsters = _monsters->numActive;
        for(int32_t i = 0; i < numMonsters; ++i)
        {
            monsterBoundaryRect = _monsters->getBoundaryRect(i);
            if(SDL_HasIntersection(&heroBoundaryRect, &monsterBoundaryRect))
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

    drawParams.frame =
                (_currDirection * FRAMES_PER_DIRECTION) + _currDirectionFrame;

    switch (_currDirection)
    {
        case Direction::DOWN:
            drawParams.pos.y += HERO_SPEED;

            if(HERO_MAX_Y < drawParams.pos.y)
            {
                drawParams.pos.y = HERO_MAX_Y;
            }
            break;

        case Direction::LEFT:
            drawParams.pos.x -= HERO_SPEED;

            if(0 > drawParams.pos.x)
            {
                drawParams.pos.x = 0;
            }
            break;

        case Direction::RIGHT:
            drawParams.pos.x += HERO_SPEED;

            if(HERO_MAX_X < drawParams.pos.x)
            {
                drawParams.pos.x = HERO_MAX_X;
            }
            break;

        case Direction::UP:
            drawParams.pos.y -= HERO_SPEED;

            if(0 > drawParams.pos.y)
            {
                drawParams.pos.y = 0;
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

    ++_firedBullets;

    _bullets->fireNext(drawParams.pos.x + HERO_WIDTH / 2,
                       drawParams.pos.y + HERO_HEIGHT / 2,
                       _currDirection);

    _bulletIcons->deactivateNext();
}



