/*
 * Game.cpp
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

//Corresponding header
#include "Game.h"

//C system headers
#include <unistd.h>

//C++ system headers
#include <cstdlib>
#include <cstdio>

//Other libraries headers
#include <SDL2/SDL_events.h>

//Own components headers
#include "common/CommonDefines.h"
#include "common/Time.h"
#include "common/Random.hpp"

#define MONITOR_WIDTH 1920
#define MONITOR_HEIGHT 1080


Game::Game()
{
    _winTimeout            = 40000000; //in microseconds (40 seconds)
    _remainingSecondsToWin = _winTimeout / MICROSECOND;
}

int32_t Game::init()
{
    int32_t err = EXIT_SUCCESS;

    //initialise monitor to FullHD
    if(EXIT_SUCCESS != _renderer.init(0, 0, MONITOR_WIDTH, MONITOR_HEIGHT))
    {
        fprintf(stderr, "Error in _renderer.init() \n");

        err = EXIT_FAILURE;
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _background.init(Textures::TERRAIN,
                                            SDL_Point{ 0, 0 },
                                            MONITOR_WIDTH,
                                            MONITOR_HEIGHT))
        {
            fprintf(stderr, "Error in _background.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _youWin.init(Textures::YOU_WIN,
                                        SDL_Point{ 440, 0 },
                                        880,
                                        MONITOR_HEIGHT))
        {
            fprintf(stderr, "Error in _youWin.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _gameOver.init(Textures::GAME_OVER,
                                          SDL_Point{ 0, 0 },
                                          MONITOR_WIDTH,
                                          MONITOR_HEIGHT))
        {
            fprintf(stderr, "Error in _background.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _bulletIcons.init(Textures::BULLET_ICON))
        {
            fprintf(stderr, "Error in _bulletIcons.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _bullets.init(Textures::BULLET))
        {
            fprintf(stderr, "Error in _bullets.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _monsters.init(Textures::MONSTER))
        {
            fprintf(stderr, "Error in _monsters.init()\n");

            err = EXIT_FAILURE;
        }
        else
        {
            _monsters.setBulletsInstance(&_bullets);
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _hero.init(Textures::HERO,
                                      0,
                                      SDL_Point{ 0, 0 }))
        {
            fprintf(stderr, "Error in _hero.init()\n");

            err = EXIT_FAILURE;
        }
        else
        {
            _hero.setMonstersInstance(&_monsters);
            _hero.setBulletsInstance(&_bullets);
            _hero.setBulletIconInstance(&_bulletIcons);

            _hero.chooseNonLosingStartPosition();
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS !=  _debugConsole.init(_renderer.getTextureContainer()))
        {
            fprintf(stderr, "Error in _debugConsole.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _countdownText.init(_renderer.getTextureContainer(),
                                               Textures::COUNTDOWN_TEXT,
                                               SDL_Point { 880, 450 },
                                               "3",
                                               FontSize::BIG))
        {
            fprintf(stderr, "Error in _countdownText.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        std::string text = "Survive for: ";
        text.append(std::to_string(_remainingSecondsToWin)).append(" s");

        if(EXIT_SUCCESS != _winTimeoutText.init(_renderer.getTextureContainer(),
                                                Textures::WIN_TIMEOUT_TEXT,
                                                SDL_Point { 800, 20 },
                                                text.c_str(),
                                                FontSize::SMALL))
        {
            fprintf(stderr, "Error in _winTimeoutText.init()\n");

            err = EXIT_FAILURE;
        }
    }

    return err;
}

void Game::deinit()
{
    _renderer.deinit();
}

void Game::mainLoop()
{
    SDL_Event e;
    memset(&e, 0, sizeof(e));

    Time time;

    while(true)
    {
        if(checkWinOrLoseConditions())
        {
        	break;
        }

        if(handleUserEvent(e))
        {
        	break;
        }

        updateWorldState(time.getElapsed().toMicroseconds());
        updateWorldPhysics();

        drawWorld();
    }
}

void Game::updateWorldState(const int32_t elapsedTime)
{
    if(_debugConsole.isActive())
    {
        _debugConsole.update(elapsedTime);
    }

    _hero.updateState(elapsedTime);
    _monsters.updateState(elapsedTime);
    _bullets.updateState(elapsedTime);

    updateTimeoutText(elapsedTime);
}

void Game::updateWorldPhysics()
{
    _hero.updatePhysics();
    _monsters.updatePhysics();
}

void Game::drawWorld()
{
    _renderer.clearScreen();

//    _renderer.drawTexture(&_background.drawParams);
    _renderer.drawTexture(&_hero.drawParams);

    _renderer.drawTextureArr(_bulletIcons.drawParams,
    												_bulletIcons.numActive);
    _renderer.drawTextureArr(_monsters.drawParams, _monsters.numActive);
    _renderer.drawTextureArr(_bullets.drawParams, _bullets.numActive);

    _renderer.drawTexture(&_winTimeoutText.drawParams);

    if(_debugConsole.isActive())
    {
        _renderer.drawTexture(_debugConsole.getDrawParams());
    }

    _renderer.finishFrame();
}

bool Game::handleUserEvent(SDL_Event & e)
{
    if(0 != SDL_PollEvent(&e))
    {
        if((SDLK_ESCAPE == e.key.keysym.sym && e.type == SDL_KEYUP) ||
           (SDL_QUIT == e.type))
        {
            return true;
        }

        _hero.handleEvent(e);
        _debugConsole.handleEvent(e);
    }

    return false;
}

void Game::showGetReadyScreen()
{
    int32_t secondsRemaining = 3;

    while(secondsRemaining)
    {
        _renderer.clearScreen();

        _countdownText.setText(std::to_string(secondsRemaining).c_str());

//        _renderer.drawTexture(&_background.drawParams);
        _renderer.drawTexture(&_hero.drawParams);

        _renderer.drawTextureArr(_bulletIcons.drawParams,
        												_bulletIcons.numActive);
        _renderer.drawTextureArr(_monsters.drawParams, _monsters.numActive);
        _renderer.drawTextureArr(_bullets.drawParams, _bullets.numActive);

        _renderer.drawTexture(&_winTimeoutText.drawParams);

        _renderer.drawTexture(&_countdownText.drawParams);

        _renderer.finishFrame();

        --secondsRemaining;

        sleep(1);
    }
}

void Game::updateTimeoutText(const int32_t elapsedTime)
{
    _winTimeout -= elapsedTime;

    const int64_t currRemainigSecondsToWin = _winTimeout / MICROSECOND;

    if(_remainingSecondsToWin != currRemainigSecondsToWin)
    {
        _remainingSecondsToWin = currRemainigSecondsToWin;

        std::string text = "Survive for: ";
        text.append(std::to_string(_remainingSecondsToWin)).append(" s");

        _winTimeoutText.setText(text.c_str());
    }
}

bool Game::checkWinOrLoseConditions()
{
    if(0 >= _winTimeout)
    {
        _renderer.clearScreen();

        _renderer.drawTexture(&_youWin.drawParams);

        _renderer.finishFrame();

        sleep(7);

        return true;
    }
    else if(!_hero.isActive())
    {
        _renderer.clearScreen();

        _renderer.drawTexture(&_gameOver.drawParams);

        _renderer.finishFrame();

        sleep(7);

        return true;
    }

    return false;
}



