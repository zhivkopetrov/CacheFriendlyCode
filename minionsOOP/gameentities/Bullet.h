/*
 * Bullet.h
 *
 *  Created on: 8 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef GAMEENTITIES_BULLET_H_
#define GAMEENTITIES_BULLET_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "GameEntity.h"

//Forward declarations

#define MAX_BULLETS 10


class Bullet : public GameEntity
{
    public:
        Bullet(const int32_t direction);

        Bullet() = delete;

        virtual ~Bullet() = default;

        virtual void updateState(const int64_t elapsedTime) override final;

        virtual inline void updatePhysics() override final { }

        virtual inline SDL_Rect getBoundaryRect() override final
        {
            return { _drawParams.pos.x, _drawParams.pos.y, _width, _height };
        }

        inline void destroy()
        {
            _isActive = false;
        }

    private:
        void move();

        int32_t _width;
        int32_t _height;

        int32_t _maxX;
        int32_t _maxY;

        int32_t _speed;

        int64_t _moveTimer;

        int32_t _currDirection;
};

#endif /* GAMEENTITIES_BULLET_H_ */


