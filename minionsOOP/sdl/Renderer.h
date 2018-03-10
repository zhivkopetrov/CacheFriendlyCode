/*
 * Renderer.h
 *
 *  Created on: 6 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef SDL_RENDERER_H_
#define SDL_RENDERER_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "RendererProxyInterface.hpp"
#include "TextureContainer.h"
#include "sdl/DrawParams.h"

//Forward declarations
struct SDL_Window;
struct SDL_Renderer;


class Renderer : public RendererProxyInterface
{
    public:
        Renderer();

        //forbid the copy and move constructors
        Renderer(const Renderer & other) = delete;
        Renderer(Renderer && other) = delete;

        //forbid the copy and move assignment operators
        Renderer & operator=(const Renderer & other) = delete;
        Renderer & operator=(Renderer && other) = delete;

        virtual ~Renderer() = default;

        /** @brief used to initialise actual renderer
         *
         *  @param const int32_t - monitor X coordinate
         *  @param const int32_t - monitor Y coordinate
         *  @param const int32_t - monitor width
         *  @param const int32_t - monitor height
         *
         *  @returns int32_t     - error code
         * */
        int32_t init(const int32_t windowX,
                     const int32_t windowY,
                     const int32_t windowWidth,
                     const int32_t windowHeight);

        /** @brief used to destroy renderer and window
         * */
        void deinit();

        //=====================================================================
        /** @brief Every frame should start with this function call
         * */
        void clearScreen();

        /** All the drawing functions calls should be encapsulated
        *   between clearScreen() and finishFrame() functions
        * */

        /** @brief Every frame should end with this function call
         * */
        void finishFrame();
        //=====================================================================

        inline TextureContainer * getTextureContainer()
        {
            return &_textureContainer;
        }

        /** @brief transfer draw specific data from Textures to renderer
         *
         *  @param DrawParams * - draw specific data for a single Texture
         * */
        virtual void drawTexture(DrawParams * drawParams) override final;

    private:
        //The window we'll be rendering to
        SDL_Window *            _window;

        //The Hardware Accelerated Renderer
        SDL_Renderer *          _sdlRenderer;

        //container holding all the graphical textures
        TextureContainer        _textureContainer;

        //counter for Textures in each individual frame
        uint64_t                _currWidgetCounter;

        /* A container for all draw specific widget data.
         * Since rendering will be the most heavy operation we need to prepare
         * draw specific data in the most comfortable way.
         *
         * A std::vector is chosen here, so we can guarantee that we will have
         * 0(1) lookup time and constant cache hits, because of the
         * std::vector sequential memory layout
         * */
        std::vector<DrawParams> _widgets;
};

#endif /* SDL_RENDERER_H_ */


