#pragma once
#ifndef GAME_HPP
#define GAME_HPP


#include <iostream>
#include "../Inc/Snake.hpp"
#include <random>
#include <conio.h>
#include <windows.h>
#include <fstream>
#include <chrono>
#include <thread>
#include <limits>
#include <filesystem>
#include <direct.h>
//using namespace std;

enum class GameDifficultys {
    EASY = 0,
    HARD = 1,
    SPECIAL = 2
};
enum class GameThemes {
    CIRCLE,
    SQUARE
};

enum class GameStates {
    SELECTTHEME,
    SELECTDIFFICULTY,
    MENU,
    GAMEPLAY,
    PAUSED,
    GAMEOVER
};

class Game{
private:
    Snake* snake;
    Cell* food;
    GameDifficultys Difficulty;
    GameThemes Theme;
    GameStates GameState;
    unsigned long int previousTime;
    unsigned short int gameMinute;
    unsigned short int gameSecond;
    std::chrono::high_resolution_clock::time_point pausedTime;
    std::chrono::high_resolution_clock::time_point unpausedTime;
    std::chrono::high_resolution_clock::time_point previousGameTime;
    int speed;
    std::string** list;
    std::string* playerName;
    
    char** scene;
    char* keys;
    unsigned short int frameRate;
    unsigned short int maxPlayerNameLength;
    unsigned short int height;
    unsigned short int width;
    unsigned short int score;
    char borderType;
    
    bool gameWinStatus;
    bool gameStatus;
    bool closeGame;
    bool chooseClose;
    bool pauseGame;

    bool border;
    bool eatCell;
    bool bordersCheck;
    bool bodyCheck;
    
    bool menuSetup;
    bool menuSetPlayerName;
    bool menuChooseDifficulty;
    bool menuChooseTheme;
    void settingsMenu();
    void gotoxy(short x, short y);
    void hideCursor();
    void setup();
    void buildBorders();
    void setconsolecolor(int textColor, int bgColor);
    void drawFrame();
    void menu();
    void difficultyMenu();
    void themeMenu();
    int  mainMenu(std::string* options, short length, std::string* promt);
    void createFood();
    void listMenu();
    void outMenu();
    void clearFrame();
    void checkKeyboard();
    void checkGameStatus();
    bool checkBorders();
    bool checkFood();
    bool checkFoodSpawn(const int& _x, const int& _y);
    void setScore();
    void setDifficulty(GameDifficultys _Difficulty);
    void setTheme(GameThemes _Theme);
    void saveNewList();
    void getOldPlayerList();
    void assignScoreToList();
    void resizeConsole();
    void readSettingsFile();
    void writeSettingsFile();
    void createGameFiles();
    unsigned long int setTime();
    GameDifficultys getDifficulty();
    GameThemes getTheme();
public:
    Game();
    Game(const int& _width, const int& _height);
    ~Game() {
        delete[] keys;
        delete snake;
        delete food;
        for (int i = 0; i < height; i++) {
            delete[] scene[i];
        }

        for (int i = 0; i < 15; i++)
            delete[] list[i];
        delete[] list;
        delete playerName;
        delete[] scene;
    }
    void loop();
};

#endif // !GAME_HPP