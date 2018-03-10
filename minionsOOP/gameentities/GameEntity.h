/*
 * GameEntity.h
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef GAMEENTITIES_GAMEENTITY_H_
#define GAMEENTITIES_GAMEENTITY_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "sdl/DrawParams.h"
#include "sdl/RendererProxyInterface.hpp"

//Forward declarations


class GameEntity
{
    public:
        GameEntity();

        virtual ~GameEntity() = default;

        virtual void updateState(const int64_t elapsedTime) = 0;

        virtual void updatePhysics() = 0;

        virtual SDL_Rect getBoundaryRect() = 0;

        int32_t init(RendererProxyInterface * interface,
                     const uint8_t            rsrcId,
                     const uint8_t            startFrame,
                     const SDL_Point          startPoint);

        inline void draw()
        {
            _rendererInterface->drawTexture(&_drawParams);
        }

        inline bool isActive() const
        {
            return _isActive;
        }

    protected:
        DrawParams               _drawParams;

        RendererProxyInterface * _rendererInterface;

        bool                     _isActive;
};

#endif /* GAMEENTITIES_GAMEENTITY_H_ */


