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
#include "GameEntity.h"

//Forward declarations


class StaticImage : public GameEntity
{
    public:
        StaticImage();

        virtual ~StaticImage() = default;

        int32_t init(RendererProxyInterface * interface,
                     const uint8_t            rsrcId,
                     const uint8_t            startFrame,
                     const SDL_Point          startPoint,
                     const int32_t            width,
                     const int32_t            height);

#pragma GCC diagnostic push  // require GCC 4.6
#pragma GCC diagnostic ignored "-Wunused-parameter"
        virtual void updateState(const int64_t elapsedTime) override final {}
#pragma GCC diagnostic pop   // require GCC 4.6

        virtual void updatePhysics() override final {}

        virtual inline SDL_Rect getBoundaryRect() override final
        {
            return { _drawParams.pos.x, _drawParams.pos.y, _width, _height };
        }

        inline void hide()
        {
        	_isActive = false;
        }

    private:
        int32_t _width;
        int32_t _height;
};

#endif /* GAMEENTITIES_STATICIMAGE_H_ */


