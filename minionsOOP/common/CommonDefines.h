/*
 * CommonDefines.h
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef COMMON_COMMONDEFINES_H_
#define COMMON_COMMONDEFINES_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers

//Forward declarations


namespace Textures
{
    enum : uint8_t
    {
        HERO,
        MONSTER,
        BULLET,
        BULLET_ICON,
        TERRAIN,
        YOU_WIN,
        GAME_OVER,
        FPS_TEXT,
        COUNTDOWN_TEXT,
        WIN_TIMEOUT_TEXT,

        COUNT
    };
}

namespace Direction
{
    enum
    {
        DOWN,
        LEFT,
        RIGHT,
        UP,

        COUNT
    };
}

namespace FontSize
{
    enum
    {
        SMALL,
        BIG
    };
}

#endif /* COMMON_COMMONDEFINES_H_ */


