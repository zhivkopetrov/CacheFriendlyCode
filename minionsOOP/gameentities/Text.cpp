/*
 * Text.cpp
 *
 *  Created on: 9 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "Text.h"

//C system headers

//C++ system headers
#include <cstdlib>
#include <cstdio>

//Other libraries headers

//Own components headers
#include "sdl/TextureContainer.h"


Text::Text() : _textureContainer(nullptr),
               _fontSize(0),
               _width(0),
               _height(0)
{
    _drawParams.frame = 0;
}

int32_t Text::init(RendererProxyInterface * interface,
                   TextureContainer *       textureContainer,
                   const uint8_t            rsrcId,
                   const SDL_Point          startPoint,
                   const char *             startText,
                   const int32_t            fontSize)
{
    int32_t err = EXIT_SUCCESS;

    _textureContainer = textureContainer;
    _fontSize         = fontSize;

    if(EXIT_SUCCESS != GameEntity::init(interface, rsrcId, 0, startPoint))
    {
        fprintf(stderr, "Error in GameEntity::init()");

        err = EXIT_FAILURE;
    }
    else
    {
        _textureContainer->setText(startText,
                                   fontSize,
                                   rsrcId,
                                   &_width,
                                   &_height);
    }

    return err;
}

void Text::setText(const char * text)
{
    _textureContainer->setText(text,
                               _fontSize,
                               _drawParams.rsrcId,
                               &_width,
                               &_height);
}





