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
        if(EXIT_SUCCESS != _background.init(&_renderer,
                                            Textures::TERRAIN,
                                            0,
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
        if(EXIT_SUCCESS != _youWin.init(&_renderer,
                                        Textures::YOU_WIN,
                                        0,
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
        if(EXIT_SUCCESS != _gameOver.init(&_renderer,
                                          Textures::GAME_OVER,
                                          0,
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
        _bulletIcons.resize(MAX_BULLETS);

        const int32_t BULLET_ICON_START_X = 1500;
        const int32_t BULLET_ICON_START_Y = 10;
        const int32_t OFFSET_X = 35;

        for(int32_t i = 0; i < MAX_BULLETS; ++i)
        {
            if(EXIT_SUCCESS != _bulletIcons[i].init(
							&_renderer,
							Textures::BULLET_ICON,
							0,
							SDL_Point{ BULLET_ICON_START_X +
											((MAX_BULLETS - i - 1) * OFFSET_X),
									   BULLET_ICON_START_Y },
							29,  //bullet icon width
							85)) //bullet icon height
            {
                fprintf(stderr, "Error in _bulletIcons[%d].init()\n", i);

                err = EXIT_FAILURE;
                break;
            }
        }
    }

    if(EXIT_SUCCESS == err)
    {
        _monsters.resize(MAX_MONSTERS);

        for(int32_t i = 0; i < MAX_MONSTERS; ++i)
        {
            if(EXIT_SUCCESS != _monsters[i].init(
               &_renderer,
               Textures::MONSTER,
               0,
               SDL_Point{
                   Random::getInstance().generate(0, _monsters[i].getMaxX()),
				   Random::getInstance().generate(0, _monsters[i].getMaxY()) }))
            {
                fprintf(stderr, "Error in _monsters[%d].init()\n", i);

                err = EXIT_FAILURE;
                break;
            }
            else
            {
                _monsters[i].setBulletsVec(&_bullets);
            }
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _hero.init(&_renderer,
                                      Textures::HERO,
                                      0,
                                      SDL_Point{ 0, 0 }))
        {
            fprintf(stderr, "Error in _hero.init()\n");

            err = EXIT_FAILURE;
        }
        else
        {
            _hero.setMonstersVec(&_monsters);
            _hero.setBulletsVec(&_bullets);
            _hero.setBulletIconsVec(&_bulletIcons);

            _hero.chooseNonLosingStartPosition();
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS !=
                _debugConsole.init(&_renderer, _renderer.getTextureContainer()))
        {
            fprintf(stderr, "Error in _debugConsole.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != _countdownText.init(&_renderer,
                                               _renderer.getTextureContainer(),
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

        if(EXIT_SUCCESS != _winTimeoutText.init(&_renderer,
                                                _renderer.getTextureContainer(),
                                                Textures::WIN_TIMEOUT_TEXT,
                                                SDL_Point { 800, 20 },
                                                text.c_str(),
                                                FontSize::SMALL))
        {
            fprintf(stderr, "Error in _winTimeoutText.init()\n");

            err = EXIT_FAILURE;
        }
    }

    if(EXIT_SUCCESS == err)
    {
        _bullets.reserve(MAX_BULLETS);

        //+1 for the hero, +1 for the background, +1 for the winTimeoutText
        //MAX_BULLETS * 2 (for both real bullets and bullet icons)
        _entities.resize(MAX_MONSTERS + (MAX_BULLETS * 2) + 3, nullptr);
    }

    return err;
}

void Game::deinit()
{
    _renderer.deinit();
}

void Game::mainLoop()
{
    volatile bool shouldQuit = false;

    SDL_Event e;
    memset(&e, 0, sizeof(e));

    Time time;

    while(!shouldQuit)
    {
        if(checkWinOrLoseConditions())
        {
            shouldQuit = true;
            continue;
        }

        if(handleUserEvent(e))
        {
            shouldQuit = true;
            continue;
        }

        _renderer.clearScreen();

        const int64_t elapsedTime = time.getElapsed().toMicroseconds();

        updateTimeoutText(elapsedTime);

        if(_debugConsole.isActive())
        {
            _debugConsole.update(elapsedTime);
        }

        invalidateInactiveEntities();

        uint32_t widgetsCount = 0;
        loadActiveFrameEnities(&widgetsCount);

        for(uint32_t i = 0; i < widgetsCount; ++i)
        {
        	_entities[i]->updateState(elapsedTime);

        	_entities[i]->updatePhysics();

            if(_entities[i]->isActive())
            {
            	_entities[i]->draw();
            }
        }

        if(_debugConsole.isActive())
        {
            _debugConsole.draw();
        }

        _renderer.finishFrame();
    }
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

        uint32_t widgetsCounts = 0;
        loadActiveFrameEnities(&widgetsCounts);

        for(uint32_t i = 0; i < widgetsCounts; ++i)
        {
        	_entities[i]->draw();
        }

        _countdownText.draw();

        --secondsRemaining;

        _renderer.finishFrame();

        sleep(1);
    }
}

void Game::updateTimeoutText(const int64_t elapsedTime)
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

        _youWin.draw();

        _renderer.finishFrame();

        sleep(7);

        return true;
    }
    else if(!_hero.isActive())
    {
        _renderer.clearScreen();

        _gameOver.draw();

        _renderer.finishFrame();

        sleep(7);

        return true;
    }

    return false;
}

void Game::loadActiveFrameEnities(uint32_t * outWidgetsCount)
{
	uint32_t currIndex = 0;

    //background must be first - because of image layering
//    _entities[currIndex++] = &_background;

    //background must be first - because of image layering
    _entities[currIndex++] = &_winTimeoutText;

    const uint32_t BULLET_ICONS_SIZE = _bulletIcons.size();
    for(uint32_t i = 0; i < BULLET_ICONS_SIZE; ++i)
    {
        _entities[currIndex++] = &_bulletIcons[i];
    }

    _entities[currIndex++] = &_hero;

    const uint32_t MONSTERS_SIZE = _monsters.size();
    for(uint32_t i = 0; i < MONSTERS_SIZE; ++i)
    {
        _entities[currIndex++] = &_monsters[i];
    }

    const uint32_t BULEETS_SIZE = _bullets.size();
    for(uint32_t i = 0; i < BULEETS_SIZE; ++i)
    {
        _entities[currIndex++] = &_bullets[i];
    }

    *outWidgetsCount = currIndex;
}

void Game::invalidateInactiveEntities()
{
	std::vector<StaticImage>::iterator bulletIconIt;
	for(bulletIconIt = _bulletIcons.begin(); bulletIconIt != _bulletIcons.end();)
	{
		if(bulletIconIt->isActive())
		{
			++bulletIconIt;
			continue;
		}
		else
		{
			bulletIconIt = _bulletIcons.erase(bulletIconIt);
		}
	}

	std::vector<Bullet>::iterator bulletIt;
	for(bulletIt = _bullets.begin(); bulletIt != _bullets.end();)
	{
		if(bulletIt->isActive())
		{
			++bulletIt;
			continue;
		}
		else
		{
			bulletIt = _bullets.erase(bulletIt);
		}
	}

	std::vector<Monster>::iterator monsterIt;
	for(monsterIt = _monsters.begin(); monsterIt != _monsters.end();)
	{
		if(monsterIt->isActive())
		{
			++monsterIt;
			continue;
		}
		else
		{
			monsterIt = _monsters.erase(monsterIt);
		}
	}
}



