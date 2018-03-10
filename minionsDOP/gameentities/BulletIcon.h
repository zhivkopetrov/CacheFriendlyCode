/*
 * BulletIcon.h
 *
 *  Created on: 15 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef GAMEENTITIES_BULLETICON_H_
#define GAMEENTITIES_BULLETICON_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "sdl/DrawParams.h"

//Forward declarations

#define MAX_BULLET_ICONS 10


class BulletIcon
{
	public:
        BulletIcon();

        ~BulletIcon() = default;

        int32_t init(const uint8_t rsrcId);

        inline void deactivateNext()
        {
            --numActive;
        }

        int32_t    numActive;

        DrawParams drawParams[MAX_BULLET_ICONS];
};

#endif /* GAMEENTITIES_BULLETICON_H_ */


