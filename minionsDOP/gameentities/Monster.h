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
#include "sdl/DrawParams.h"

//Forward declarations
class Bullet;

#define MAX_MONSTERS 50
#define MONSTER_WIDTH 40
#define MONSTER_HEIGHT 36


class Monster
{
    public:
        Monster();

        ~Monster() = default;

        int32_t init(const uint8_t rsrcId);

        void updateState(const int32_t elapsedTime);

        void updatePhysics();

        inline void setBulletsInstance(Bullet * instance)
        {
            _bullets = instance;
        }

        inline SDL_Rect getBoundaryRect(const int32_t index)
        {
            return { drawParams[index].pos.x,
                     drawParams[index].pos.y,
                     MONSTER_WIDTH,
                     MONSTER_HEIGHT };
        }

        int32_t    numActive;

        DrawParams drawParams[MAX_MONSTERS];

    private:

        void chooseRandomDirection(const int32_t index);

        void move(const int32_t index);

        void deactivate(const int32_t index);

        int32_t    _moveTimer[MAX_MONSTERS];
        int32_t    _changeDirTimer[MAX_MONSTERS];
        int32_t    _changeDirTimerFrameLimit[MAX_MONSTERS];
        uint8_t    _currDirection[MAX_MONSTERS];
        uint8_t    _currDirectionFrame[MAX_MONSTERS];

        Bullet *   _bullets;
};

#endif /* GAMEENTITIES_MONSTER_H_ */

