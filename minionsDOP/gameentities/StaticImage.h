/*
 * StaticImage.h
 *
 *  Created on: 8 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef GAMEENTITIES_STATICIMAGE_H_
#define GAMEENTITIES_STATICIMAGE_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "sdl/DrawParams.h"

//Forward declarations


class StaticImage
{
    public:
        StaticImage();

        ~StaticImage() = default;

        int32_t init(const uint8_t   rsrcId,
                     const SDL_Point startPoint,
                     const int32_t   width,
                     const int32_t   height);

        inline SDL_Rect getBoundaryRect()
        {
            return { drawParams.pos.x, drawParams.pos.y, _width, _height };
        }

        DrawParams drawParams;

    private:
        int32_t    _width;
        int32_t    _height;
};

#endif /* GAMEENTITIES_STATICIMAGE_H_ */


