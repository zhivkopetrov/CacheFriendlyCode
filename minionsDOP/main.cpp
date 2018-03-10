/*
 * main.cpp
 *
 *  Created on: 6 Feb 2018
 *      Author: Zhivko Petrov
 */


//C system headers

//C++ system headers
#include <cstdint>
#include <cstdlib>
#include <cstdio>

//Other libraries headers

//Own components headers
#include "Game.h"
#include "sdl/SDLLoader.h"


int32_t main()
{
    int32_t err = EXIT_SUCCESS;

    Game game;

    if(EXIT_SUCCESS != SDLLoader::init())
    {
        fprintf(stderr, "Error in SDLLoader::init() -> Terminating ...\n");

        err = EXIT_FAILURE;
    }

    if(EXIT_SUCCESS == err)
    {
        if(EXIT_SUCCESS != game.init())
        {
            fprintf(stderr, "game.init() failed\n");

            err = EXIT_FAILURE;
        }
        else
        {
            game.start();
        }
    }

    game.deinit();

    //close SDL libraries
    SDLLoader::deinit();

    return err;
}

