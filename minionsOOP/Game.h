/*
 * Game.h
 *
 *  Created on: 7 Feb 2018
 *      Author: Zhivko Petrov
 */

#ifndef GAME_H_
#define GAME_H_

//C system headers

//C++ system headers
#include <cstdint>
#include <vector>

//Other libraries headers

//Own components headers
#include "DebugConsole.h"

#include "sdl/Renderer.h"

#include "gameentities/Hero.h"
#include "gameentities/Monster.h"
#include "gameentities/Bullet.h"
#include "gameentities/StaticImage.h"

//Forward declarations
union SDL_Event;


class Game
{
    public:
        Game();
        virtual ~Game() = default;

        int32_t init();

        void deinit();

        inline void start()
        {
            showGetReadyScreen();

            mainLoop();
        }

    private:
        void mainLoop();

        bool handleUserEvent(SDL_Event & e);

        void showGetReadyScreen();

        void updateTimeoutText(const int64_t elapsedTime);

        /* returns - should exit flag */
        bool checkWinOrLoseConditions();

        void loadActiveFrameEnities(uint32_t * outWidgetsCount);

        void invalidateInactiveEntities();

        Renderer                  _renderer;

        DebugConsole              _debugConsole;

        StaticImage               _background;

        StaticImage               _youWin;

        StaticImage               _gameOver;

        Text                      _countdownText;

        Text                      _winTimeoutText;

        Hero                      _hero;

        std::vector<StaticImage>  _bulletIcons;

        std::vector<Monster>      _monsters;

        std::vector<Bullet>       _bullets;

        std::vector<GameEntity *> _entities;

        int64_t                   _winTimeout;

        int64_t                   _remainingSecondsToWin;
};

#endif /* GAME_H_ */

