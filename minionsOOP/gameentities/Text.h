/*
 * Text.h
 *
 *  Created on: 9 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef GAMEENTITIES_TEXT_H_
#define GAMEENTITIES_TEXT_H_

//C system headers

//C++ system headers
#include <cstdint>

//Other libraries headers

//Own components headers
#include "GameEntity.h"

//Forward declarations
class TextureContainer;


class Text : public GameEntity
{
    public:
        Text();
        virtual ~Text() = default;

        int32_t init(RendererProxyInterface * interface,
                     TextureContainer *       textureContainer,
                     const uint8_t            rsrcId,
                     const SDL_Point          startPoint,
                     const char *             startText,
                     const int32_t            fontSize);

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

        void setText(const char * text);

    private:
        TextureContainer * _textureContainer;

        int32_t            _fontSize;

        int32_t            _width;
        int32_t            _height;
};

#endif /* GAMEENTITIES_TEXT_H_ */


