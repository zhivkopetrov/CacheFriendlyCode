/*
 * TextureContainer.cpp
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "TextureContainer.h"

//C system headers

//C++ system headers
#include <cstdlib>

//Other libraries headers
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_video.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

//Own components headers
#include "common/CommonDefines.h"


TextureContainer::TextureContainer()
{
    _fontSmall = nullptr;
    _fontBig   = nullptr;
    _renderer  = nullptr;

    //set green color for text
    _color.r   = 0;
    _color.g   = 255;
    _color.b   = 0;
    _color.a   = 255;
}

int32_t TextureContainer::init(SDL_Renderer * renderer)
{
    int32_t err = EXIT_SUCCESS;

    _renderer = renderer;

    _textures.resize(Textures::COUNT, nullptr);

    _textureFrameRects.resize(Textures::COUNT);

    if(EXIT_SUCCESS != loadTextures())
    {
        fprintf(stderr, "Error in loadTextures()\n");

        err = EXIT_FAILURE;
    }

    return err;
}

void TextureContainer::deinit()
{
    if(_fontSmall) //sanity check
    {
        TTF_CloseFont(_fontSmall);
        _fontSmall = nullptr;
    }

    if(_fontBig) //sanity check
    {
        TTF_CloseFont(_fontBig);
        _fontBig = nullptr;
    }

    for(SDL_Texture * texture : _textures)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }

    _textures.clear();
}

void TextureContainer::setText(const char *  text,
                               const int32_t fontSize,
                               const uint8_t textureId,
                               int32_t *     outTextWidth,
                               int32_t *     outTextHeight)
{
    TTF_Font * font = nullptr;

    FontSize::SMALL == fontSize ?
                            font = _fontSmall :
                            font = _fontBig;

    SDL_Surface * loadedSurface = TTF_RenderText_Solid(font, text, _color);

    if(loadedSurface == nullptr)
    {
        fprintf(stderr, "Unable to load image! SDL_image Error: %s\n",
                                                               IMG_GetError());

        return;
    }

    *outTextWidth = loadedSurface->w;
    *outTextHeight = loadedSurface->h;

    _textureFrameRects[textureId][0].w = loadedSurface->w;
    _textureFrameRects[textureId][0].h = loadedSurface->h;

    //create hardware accelerated texture
    if(EXIT_SUCCESS !=
           loadTextureFromSurface(loadedSurface, _textures[textureId]))
    {
        fprintf(stderr, "Unable to create text texture for ID: %hhu\n",
                                                                    textureId);

        return;
    }
}

int32_t TextureContainer::loadTextures()
{
    int32_t err = EXIT_SUCCESS;

    populateTextureFrameRects();

    //Open the font with font size 30
    _fontSmall = TTF_OpenFont("../assets/orbitron-medium.otf", 30);
    if(nullptr == _fontSmall)
    {
        fprintf(stderr, "Failed to load font! SDL_ttf Error: %s\n",
                                                               TTF_GetError());

        err = EXIT_FAILURE;
    }

    //Open the font with font size 200
    _fontBig = TTF_OpenFont("../assets/orbitron-medium.otf", 200);
    if(nullptr == _fontBig)
    {
        fprintf(stderr, "Failed to load font! SDL_ttf Error: %s\n",
                                                               TTF_GetError());

        err = EXIT_FAILURE;
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS !=
                  loadSingleTexture("../assets/hero.png", Textures::HERO))
        {
            fprintf(stderr, "Error in loadSingleTexture()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS !=
                  loadSingleTexture("../assets/monster.png", Textures::MONSTER))
        {
            fprintf(stderr, "Error in loadSingleTexture()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS !=
                  loadSingleTexture("../assets/bullet.png", Textures::BULLET))
        {
            fprintf(stderr, "Error in loadSingleTexture()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != loadSingleTexture("../assets/bullet_icon.png",
                                             Textures::BULLET_ICON))
        {
            fprintf(stderr, "Error in loadSingleTexture()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS !=
                  loadSingleTexture("../assets/terrain.png", Textures::TERRAIN))
        {
            fprintf(stderr, "Error in loadSingleTexture()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS !=
                  loadSingleTexture("../assets/you_win.png", Textures::YOU_WIN))
        {
            fprintf(stderr, "Error in loadSingleTexture()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS !=
              loadSingleTexture("../assets/game_over.jpg", Textures::GAME_OVER))
        {
            fprintf(stderr, "Error in loadSingleTexture()\n");

            err = EXIT_FAILURE;
        }
    }

    return err;
}

int32_t TextureContainer::loadSingleTexture(const char *  filePath,
                                            const uint8_t textureId)
{
    int32_t err = EXIT_SUCCESS;

    SDL_Surface * surface = nullptr;

    //Load image at specified path
    surface = IMG_Load(filePath);
    if(nullptr == surface)
    {
        fprintf(stderr, "Unable to load image %s! SDL_image Error: %s\n",
                                                    filePath, IMG_GetError());

        err = EXIT_FAILURE;
    }
    else
    {
        //create hardware accelerated texture
        if(EXIT_SUCCESS !=
                loadTextureFromSurface(surface, _textures[textureId]))
        {
            fprintf(stderr, "Unable to create texture with Id: %hhu\n",
                                                                    textureId);

            err = EXIT_FAILURE;
        }
    }

    return err;
}

int32_t TextureContainer::loadTextureFromSurface(SDL_Surface *& surface,
                                                 SDL_Texture *& outTexture)
{
    int32_t err = EXIT_SUCCESS;

    //Create texture from surface pixels
    outTexture = SDL_CreateTextureFromSurface(_renderer, surface);

    if(nullptr == outTexture)
    {
        fprintf(stderr, "Unable to create texture! SDL Error: %s\n",
                                                               SDL_GetError());

        err = EXIT_FAILURE;
    }
    else //success
    {
        SDL_FreeSurface(surface);
        surface = nullptr;
    }

    return err;
}

void TextureContainer::populateTextureFrameRects()
{
    const int32_t HERO_FRAME_COUNT = 24;
    const int32_t HERO_WIDTH = 34;
    const int32_t HERO_HEIGHT = 61;
    const int32_t HERO_FRAMES_PER_ROW = 6;

    _textureFrameRects[Textures::HERO].resize(HERO_FRAME_COUNT);

    for(int32_t i = 0; i < HERO_FRAME_COUNT; ++i)
    {
        _textureFrameRects[Textures::HERO][i].x =
                                    i % HERO_FRAMES_PER_ROW * HERO_WIDTH;

        _textureFrameRects[Textures::HERO][i].y =
                                    i / HERO_FRAMES_PER_ROW * HERO_HEIGHT;

        _textureFrameRects[Textures::HERO][i].w = HERO_WIDTH;
        _textureFrameRects[Textures::HERO][i].h = HERO_HEIGHT;
    }


    const int32_t MONSTER_FRAME_COUNT = 16;
    const int32_t MONSTER_WIDTH = 40;
    const int32_t MONSTER_HEIGHT = 36;
    const int32_t MONSTER_FRAMES_PER_ROW = 4;

    _textureFrameRects[Textures::MONSTER].resize(MONSTER_FRAME_COUNT);

    for(int32_t i = 0; i < MONSTER_FRAME_COUNT; ++i)
    {
        _textureFrameRects[Textures::MONSTER][i].x =
                                    i % MONSTER_FRAMES_PER_ROW * MONSTER_WIDTH;

        _textureFrameRects[Textures::MONSTER][i].y =
                                    i / MONSTER_FRAMES_PER_ROW * MONSTER_HEIGHT;

        _textureFrameRects[Textures::MONSTER][i].w = MONSTER_WIDTH;
        _textureFrameRects[Textures::MONSTER][i].h = MONSTER_HEIGHT;
    }


    const int32_t BULLET_WIDTH = 8;
    const int32_t BULLET_HEIGHT = 8;

    _textureFrameRects[Textures::BULLET].resize(1);

    _textureFrameRects[Textures::BULLET][0].x = 0;
    _textureFrameRects[Textures::BULLET][0].y = 0;
    _textureFrameRects[Textures::BULLET][0].w = BULLET_WIDTH;
    _textureFrameRects[Textures::BULLET][0].h = BULLET_HEIGHT;


    const int32_t BULLET_ICON_WIDTH = 29;
    const int32_t BULLET_ICON_HEIGHT = 85;

    _textureFrameRects[Textures::BULLET_ICON].resize(1);

    _textureFrameRects[Textures::BULLET_ICON][0].x = 0;
    _textureFrameRects[Textures::BULLET_ICON][0].y = 0;
    _textureFrameRects[Textures::BULLET_ICON][0].w = BULLET_ICON_WIDTH;
    _textureFrameRects[Textures::BULLET_ICON][0].h = BULLET_ICON_HEIGHT;


    const int32_t TERRAIN_WIDTH = 1920;
    const int32_t TERRAIN_HEIGHT = 1080;

    _textureFrameRects[Textures::TERRAIN].resize(1);

    _textureFrameRects[Textures::TERRAIN][0].x = 0;
    _textureFrameRects[Textures::TERRAIN][0].y = 0;
    _textureFrameRects[Textures::TERRAIN][0].w = TERRAIN_WIDTH;
    _textureFrameRects[Textures::TERRAIN][0].h = TERRAIN_HEIGHT;


    const int32_t YOU_WIN_WIDTH = 1040;
    const int32_t YOU_WIN_HEIGHT = 1080;

    _textureFrameRects[Textures::YOU_WIN].resize(1);

    _textureFrameRects[Textures::YOU_WIN][0].x = 0;
    _textureFrameRects[Textures::YOU_WIN][0].y = 0;
    _textureFrameRects[Textures::YOU_WIN][0].w = YOU_WIN_WIDTH;
    _textureFrameRects[Textures::YOU_WIN][0].h = YOU_WIN_HEIGHT;


    const int32_t GAME_OVER_WIDTH = 1920;
    const int32_t GAME_OVER_HEIGHT = 1080;

    _textureFrameRects[Textures::GAME_OVER].resize(1);

    _textureFrameRects[Textures::GAME_OVER][0].x = 0;
    _textureFrameRects[Textures::GAME_OVER][0].y = 0;
    _textureFrameRects[Textures::GAME_OVER][0].w = GAME_OVER_WIDTH;
    _textureFrameRects[Textures::GAME_OVER][0].h = GAME_OVER_HEIGHT;


    _textureFrameRects[Textures::FPS_TEXT].resize(1);

    _textureFrameRects[Textures::FPS_TEXT][0].x = 0;
    _textureFrameRects[Textures::FPS_TEXT][0].y = 0;
    _textureFrameRects[Textures::FPS_TEXT][0].w = 0;
    _textureFrameRects[Textures::FPS_TEXT][0].h = 0;


    _textureFrameRects[Textures::COUNTDOWN_TEXT].resize(1);

    _textureFrameRects[Textures::COUNTDOWN_TEXT][0].x = 0;
    _textureFrameRects[Textures::COUNTDOWN_TEXT][0].y = 0;
    _textureFrameRects[Textures::COUNTDOWN_TEXT][0].w = 0;
    _textureFrameRects[Textures::COUNTDOWN_TEXT][0].h = 0;


    _textureFrameRects[Textures::WIN_TIMEOUT_TEXT].resize(1);

    _textureFrameRects[Textures::WIN_TIMEOUT_TEXT][0].x = 0;
    _textureFrameRects[Textures::WIN_TIMEOUT_TEXT][0].y = 0;
    _textureFrameRects[Textures::WIN_TIMEOUT_TEXT][0].w = 0;
    _textureFrameRects[Textures::WIN_TIMEOUT_TEXT][0].h = 0;
}



