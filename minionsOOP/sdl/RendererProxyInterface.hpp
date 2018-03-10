/*
 * RendererProxyInterface.h
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef SDL_RENDERERPROXYINTERFACE_HPP_
#define SDL_RENDERERPROXYINTERFACE_HPP_

//C system headers

//C++ system headers

//Other libraries headers

//Own components headers

//Forward declarations
struct DrawParams;


class RendererProxyInterface
{
    public:
        RendererProxyInterface() = default;
        virtual ~RendererProxyInterface() = default;

        virtual void drawTexture(DrawParams * drawParams) = 0;
};

#endif /* SDL_RENDERERPROXYINTERFACE_HPP_ */
