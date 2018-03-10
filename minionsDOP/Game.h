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

//Other libraries headers

//Own components headers
#include "DebugConsole.h"

#include "sdl/Renderer.h"

#include "gameentities/Hero.h"
#include "gameentities/Monster.h"
#include "gameentities/Bullet.h"
#include "gameentities/BulletIcon.h"
#include "gameentities/StaticImage.h"

//Forward declarations
union SDL_Event;


class Game
{
    public:
        Game();
        ~Game() = default;

        int32_t init();

        void deinit();

        inline void start()
        {
            showGetReadyScreen();

            mainLoop();
        }

    private:
        void mainLoop();

        void updateWorldState(const int32_t elapsedTime);

        void updateWorldPhysics();

        void drawWorld();

        bool handleUserEvent(SDL_Event & e);

        void showGetReadyScreen();

        void updateTimeoutText(const int32_t elapsedTime);

        /* returns - should exit flag */
        bool checkWinOrLoseConditions();

        Renderer                  _renderer;

        DebugConsole              _debugConsole;

        StaticImage               _background;

        StaticImage               _youWin;

        StaticImage               _gameOver;

        Text                      _countdownText;

        Text                      _winTimeoutText;

        Hero                      _hero;

        BulletIcon                _bulletIcons;

        Monster                   _monsters;

        Bullet                    _bullets;

        int32_t                   _winTimeout;

        int32_t                   _remainingSecondsToWin;
};

#endif /* GAME_H_ */

