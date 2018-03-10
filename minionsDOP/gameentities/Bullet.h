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
#include <SDL2/SDL_rect.h>

//Own components headers
#include "sdl/DrawParams.h"

//Forward declarations

#define MAX_BULLETS 10
#define BULLET_WIDTH 4
#define BULLET_HEIGHT 4

class Bullet
{
    public:
        Bullet();

        ~Bullet() = default;

        int32_t init(const uint8_t rsrcId);

        void updateState(const int32_t elapsedTime);

        inline SDL_Rect getBoundaryRect(const int32_t index)
        {
            return { drawParams[index].pos.x,
                     drawParams[index].pos.y,
                     BULLET_WIDTH,
                     BULLET_HEIGHT };
        }

        void fireNext(const int32_t startX,
        		      const int32_t startY,
				      const uint8_t startDirection);

        void deactivate(const int32_t index);

        int32_t    numActive;

        DrawParams drawParams[MAX_BULLETS];

    private:
        void move(const int32_t index);

        int32_t    _moveTimer[MAX_BULLETS];
        uint8_t    _currDirection[MAX_BULLETS];
};

#endif /* GAMEENTITIES_BULLET_H_ */


