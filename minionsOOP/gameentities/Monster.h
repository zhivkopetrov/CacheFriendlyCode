/*
 * Monster.h
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef GAMEENTITIES_MONSTER_H_
#define GAMEENTITIES_MONSTER_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "GameEntity.h"
#include "Bullet.h"

//Forward declarations

#define MAX_MONSTERS 50


class Monster : public GameEntity
{
    public:
        Monster();

        virtual ~Monster() = default;

        virtual void updateState(const int64_t elapsedTime) override final;

        virtual void updatePhysics() override final;

        virtual inline SDL_Rect getBoundaryRect() override final
        {
            return { _drawParams.pos.x, _drawParams.pos.y, _width, _height };
        }

        inline void setBulletsVec(std::vector<Bullet> * bulletsVec)
        {
            _bullets = bulletsVec;
        }

        inline int32_t getMaxX() const
        {
        	return _maxX;
        }

        inline int32_t getMaxY() const
        {
        	return _maxY;
        }

    private:
        void chooseRandomDirection();

        void move();

        int32_t               _width;
        int32_t               _height;

        int32_t               _maxX;
        int32_t         	  _maxY;

        int32_t         	  _speed;

        std::vector<Bullet> * _bullets;

        int64_t               _moveTimer;

        int64_t               _changeDirTimer;

        int64_t               _changeDirTimerFrameLimit;

        int32_t               _currDirection;

        int32_t               _currDirectionFrame;
};

#endif /* GAMEENTITIES_MONSTER_H_ */

