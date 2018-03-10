/*
 * Hero.h
 *
 *  Created on: Feb 8, 2018
 *      Author: Zhivko Petrov
 */

#ifndef GAMEENTITIES_HERO_H_
#define GAMEENTITIES_HERO_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "StaticImage.h"

#include "sdl/DrawParams.h"

//Forward declarations
class Monster;
class Bullet;
class BulletIcon;
union SDL_Event;

#define HERO_WIDTH 34
#define HERO_HEIGHT 61


class Hero
{
    public:
		Hero();

        ~Hero() = default;

        int32_t init(const uint8_t   rsrcId,
        		     const uint8_t   startFrame,
        		     const SDL_Point startPoint);

        void updateState(const int32_t elapsedTime);

        void updatePhysics();

        inline SDL_Rect getBoundaryRect()
        {
            return
            	{ drawParams.pos.x, drawParams.pos.y, HERO_WIDTH, HERO_HEIGHT };
        }

        void handleEvent(const SDL_Event & e);

        inline void setMonstersInstance(Monster * instance)
        {
            _monsters = instance;
        }

        inline void setBulletsInstance(Bullet * instance)
        {
            _bullets = instance;
        }

        inline void setBulletIconInstance(BulletIcon * instance)
        {
            _bulletIcons = instance;
        }

        void chooseNonLosingStartPosition();

        inline bool isActive() const
        {
            return _isActive;
        }

        DrawParams    drawParams;

    private:
        void move();

        void tryFireNextBullet();

        Monster *    _monsters;

        Bullet *     _bullets;

        BulletIcon * _bulletIcons;

        int32_t      _moveTimer;

        int32_t      _firedBullets;

        uint8_t      _currDirection;

        uint8_t      _newDirection;

        uint8_t      _currDirectionFrame;

        bool         _isMoving;

        bool         _isActive;
};

#endif /* GAMEENTITIES_HERO_H_ */


