/*
 *     Console Snake Game C++
 *
 *     Author: Emircan Hizarci
 * 
 */

#include <iostream>

#include "lib/Inc/Game.hpp"

using namespace std;


int main() {

    Game SnakeGame(65, 20);

    SnakeGame.loop();

    return 0;
}