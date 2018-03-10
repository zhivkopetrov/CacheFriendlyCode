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
#include <vector>

//Other libraries headers

//Own components headers
#include "GameEntity.h"
#include "Monster.h"
#include "Bullet.h"
#include "StaticImage.h"

//Forward declarations
union SDL_Event;


class Hero : public GameEntity
{
    public:
		Hero();

        virtual ~Hero() = default;

        virtual void updateState(const int64_t elapsedTime) override final;

        virtual void updatePhysics() override final;

        virtual inline SDL_Rect getBoundaryRect() override final
        {
            return { _drawParams.pos.x, _drawParams.pos.y, _WIDTH, _HEIGHT };
        }

        void handleEvent(const SDL_Event & e);

        inline void setMonstersVec(std::vector<Monster> * monsterVec)
        {
            _monsters = monsterVec;
        }

        inline void setBulletsVec(std::vector<Bullet> * bulletsVec)
        {
            _bullets = bulletsVec;
        }

        inline void setBulletIconsVec(std::vector<StaticImage> * bulletsIconVec)
        {
            _bulletIcons = bulletsIconVec;
        }

        void chooseNonLosingStartPosition();

    private:
        void move();

        void tryFireNextBullet();

        const int32_t              _WIDTH;
        const int32_t              _HEIGHT;

        const int32_t              _MAX_X;
        const int32_t              _MAX_Y;

        const int32_t              _SPEED;

        std::vector<Monster> *     _monsters;

        std::vector<Bullet> *      _bullets;

        std::vector<StaticImage> * _bulletIcons;

        int64_t                    _moveTimer;

        int32_t                    _currDirection;

        int32_t                    _newDirection;

        int32_t                    _currDirectionFrame;

        int32_t                    _firedBullets;

        bool                       _isMoving;
};

#endif /* GAMEENTITIES_HERO_H_ */
