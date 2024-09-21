
#include "../Inc/Game.hpp"

Game::Game() {
    
    //set start time
    previousGameTime = std::chrono::high_resolution_clock::now();
    GameState = GameStates::MENU;
    //set begin variabels
    previousTime = 0;

    maxPlayerNameLength = 20;
    score = 0;
    frameRate = 60;
    height = 30;
    width = 80;
    speed = (height * width * 3) / 80;
    resizeConsole();
    
    keys = new char[256];

    list = nullptr;
    playerName = new std::string("\0");

    scene = new char* [height];
    for (int i = 0; i < height; i++) {
        scene[i] = new char[width];
    }

    clearFrame();

    //create list array
    list = new std::string * [15];
    for (int i = 0; i < 15; i++)
        list[i] = new std::string[3];
    
    //create food and snake
    food = new Cell((width / 2), 1, '*');
    snake = new Snake(5, 1, 5, (height - 2) * (width - 2));

    //menu states
    menuSetPlayerName = true;
    menuSetup = true;
    menuChooseDifficulty = false;
    menuChooseTheme = false;

    //game states
    gameStatus = true;
    gameWinStatus = false;
    closeGame = false;
    chooseClose = false;
    pauseGame = false;

    //set console title
    wchar_t s[256];
    swprintf_s(s, 256, L"Snake Game / Emircan Hizarci");
    SetConsoleTitle(s);

    hideCursor();
    //set start settings
    createGameFiles();
}

Game::Game(const int& _width, const int& _height) {
    //set start time
    previousGameTime = std::chrono::high_resolution_clock::now();
    GameState = GameStates::MENU;
    //set begin variabels
    previousTime = 0;

    maxPlayerNameLength = 20;
    score = 0;
    frameRate = 60;
    height = _height;
    width = _width;
    speed = (height * width * 3) / 80;
    resizeConsole();
    
    list = nullptr;
    playerName = new std::string("\0");
    

    keys = new char[256];
    //create screen array
    scene = new char* [height + 1];
    for (int i = 0; i <= height; i++) {
        scene[i] = new char[width + 1];
    }
    scene[0][width] = '\0';
    scene[height][0] = '\0';
    clearFrame();

    //create list array
    list = new std::string * [15];
    for (int i = 0; i < 15; i++)
        list[i] = new std::string[3];

    //create food and snake
    food = new Cell((width / 2), 1, '*');
    snake = new Snake(5, 1, 5, (height - 2) * (width - 2));


    //menu states
    menuSetPlayerName = true;
    menuSetup = true;
    menuChooseDifficulty = false;
    menuChooseTheme = false;

    //game states
    gameStatus = true;
    gameWinStatus = false;
    closeGame = false;
    chooseClose = false;
    pauseGame = false;

    //set console title
    wchar_t s[256];
    swprintf_s(s, 256, L"Snake Game / Emircan Hizarci");
    SetConsoleTitle(s);

    hideCursor();
    //set start settings
    createGameFiles();
}
void Game::setup() {
    delete snake;
    snake = new Snake(5, 1, 1, (height - 2) * (width - 2));
    setTheme(Theme);
    system("CLS");
    gameWinStatus = false;
    chooseClose = false;
    gameStatus = true;
    
    while (_kbhit())
        _getch();

    while (menuSetPlayerName) {
        try {
            std::string temp;
            std::cin.clear();
            gotoxy((width / 2) - 16, height / 2);
            std::cout << "Please enter your player name: ";
            std::getline(std::cin, temp,'\n');
            *playerName = temp;
            if (playerName->length() > maxPlayerNameLength) throw std::string("Your name lenght cant be bigger then ");
            if (playerName->find(' ') != -1)
                throw 404;
            if (playerName->empty()) throw 404;
            system("CLS");
            menuSetPlayerName = false;
            menuChooseTheme = true;
            clearFrame();
            Sleep(300);
        }
        catch (std::string s) {
            system("CLS");
            gotoxy((width / 2) - 20, (height / 2) - 2);
            std::cout << s << maxPlayerNameLength << std::endl;
        }
        catch (int) {
            system("CLS");
            gotoxy((width / 2) - 15, (height / 2) - 2);
            std::cout << "Your name cant take a blank" << std::endl;
        }
    }
    system("CLS");
    menuSetup = false;
    int x = width + width / 10;
    int y = height / 6;

    gotoxy(x, y);
    setconsolecolor(1, 0);
    std::cout << "player: ";
    setconsolecolor(15, 0);
    std::cout << *playerName;
    gotoxy(x, y + (height) / 6);
    setconsolecolor(1, 0);
    std::cout << "Score: ";
    gotoxy(x, y + ((2 * height) / 6));
    std::cout << "Time:  ";
    gotoxy(x, y + (3 * height)/6);
    std::cout << "Difficulty: ";
    setconsolecolor(15, 0);
    switch (Difficulty)
    {
    case GameDifficultys::EASY:
        setconsolecolor(10, 0);
        std::cout << "EASY";
        setconsolecolor(15, 0);
        break;
    case GameDifficultys::HARD:
        setconsolecolor(4,0);
        std::cout << "HARD";
        setconsolecolor(15, 0);
        break;
    case GameDifficultys::SPECIAL:
        setconsolecolor(6, 0);
        std::cout << "SPECIAL";
        setconsolecolor(15, 0);
        break;
    default:
        break;
    }

    setScore();
    GameState = GameStates::GAMEPLAY;

    //start game time
    previousGameTime = std::chrono::high_resolution_clock::now();
    pausedTime = previousGameTime;
    unpausedTime = pausedTime;
}
void Game::gotoxy(short x, short y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}
void Game::setconsolecolor(int textColor, int bgColor) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (textColor + (bgColor * 16)));
}
void Game::hideCursor() {
    HANDLE out = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursoreInfo;
    GetConsoleCursorInfo(out, &cursoreInfo);
    cursoreInfo.bVisible = false;
    SetConsoleCursorInfo(out, &cursoreInfo);
}
int Game::mainMenu(std::string* options, short length, std::string* promt = nullptr) {
    short int selectIndex = 0;
    bool flag = true;
    bool flag2 = true;
    char temp[3] = "><";
    if (promt != nullptr) {
        gotoxy((width - promt->length()) / 2, ((height / 2) - length) / 2);
        std::cout << *promt;
    }
    do {
        for (short i = 0; i < length; i++) {
            if (selectIndex == i) {
                temp[0] = '>';
                temp[1] = '<';
            }
            else {
                temp[0] = ' ';
                temp[1] = ' ';
            }
            
            gotoxy((width / 2) - (options[i].length() / 2) - (short)6, (height / 2) - (length)+i * 3);
            std::cout << temp[0] << "   " << options[i] << "   " << temp[1];
        }

        if (((GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S')) & 0x8000) && flag) {
            flag = false;
            selectIndex++;
            if (selectIndex == length)
                selectIndex = 0;
        }
        else if (!((GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S')) & 0x8000) && !flag) {
            flag = true;
        }
        else if (((GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W')) & 0x8000) && flag2) {
            flag2 = false;
            selectIndex--;
            if (selectIndex == -1)
                selectIndex = length - 1;
        }
        else if (!((GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W')) & 0x8000) && !flag2) {
            flag2 = true;
        }
        while (_kbhit()) {
            _getch();
        }

    } while (!(GetAsyncKeyState(VK_RETURN) & 0x8000));
    return selectIndex;
}
void Game::menu() {
    std::string main[] = { "Play","List","Settings","Quite "};
    std::string promt = "Main Menu  ";
    short int selectIndex = mainMenu(main, 4,&promt);
    switch (selectIndex)
    {
    case 0:
        Sleep(300);
        system("CLS");
        setup();
        break;
    case 1:
        Sleep(300);
        system("CLS");
        listMenu();
        break;
    case 2:
        Sleep(300);
        system("CLS");
        settingsMenu();
        break;
    case 3:
        Sleep(200);
        system("CLS");
        gameStatus = false;
        closeGame = true;
        break;
    default:
        break;
    }
}
void Game::difficultyMenu() {
    std::string main[] = { "EASY","HARD","SPECIAL" };
    std::string promt = "Difficulty Menu";
    short int selectIndex = mainMenu(main, 3,&promt);
    switch (selectIndex)
    {
    case 0:
        Sleep(300);
        setDifficulty(GameDifficultys::EASY);
        writeSettingsFile();
        system("CLS");
        settingsMenu();
        break;
    case 1:
        Sleep(300);
        setDifficulty(GameDifficultys::HARD);
        writeSettingsFile();
        system("CLS");
        settingsMenu();
        break;
    case 2:
        Sleep(300);
        setDifficulty(GameDifficultys::SPECIAL);
        writeSettingsFile();
        system("CLS");
        settingsMenu();
        break;
    default:
        break;
    }
}
void Game::themeMenu() {
    char temp = (char)219;
    char temp2 = (char)219;
    short int selectIndex = 0;
    bool flag = true;
    bool flag2 = true;
    std::string options[] = { "CIRCLE","SQUARE" };
    do {
        for (short i = 0; i < 2; i++) {
            if (i == selectIndex) {
                temp = (char)219;
            }
            else {
                temp = ' ';
            }
            gotoxy(1 + i*(width/2), 1);
            for (int i = 0; i < ((width/2) - 8)/2; i++) {
                std::cout << temp;
            }
            gotoxy((((width / 2) - 8) / 2) + 1 + i * (width/2), 1);
            std::cout << options[i];
            gotoxy((((width / 2) - 8) / 2) + 7 + i * (width / 2), 1);
            for (int i = 0; i < ((width / 2) - 8) / 2; i++) {
                std::cout << temp;
            }
        }
        gotoxy(0, 0);
        for (int i = 0; i < width; i++)
            std::cout << "-";
        std::cout << std::endl;
        std::cout << "|";
        gotoxy(width - 1, 1);
        std::cout << "|";
        for (int i = 0; i < width; i++) {
            gotoxy(i, 2);
            std::cout << "-";
        }
        for (int i = 3; i < (height * 2) / 3; i++) {
            gotoxy(0, i);
            std::cout << (char)219;
        }
        for (int i = 3; i < (height * 2) / 3; i++) {
            gotoxy(width - 1, i);
            std::cout << (char)178;
        }
        for (int i = 3; i < (height * 2) / 3; i++) {
            gotoxy(width / 2, i);
            std::cout << "|";
        }
        for (int i = 0; i < width; i++) {
            gotoxy(i, (height * 2) / 3);
            std::cout << "-";
        }
        for (int i = ((height * 2) / 3) + 1; i < height - 1; i++) {
            gotoxy(0, i);
            std::cout << "|";
        }
        for (int i = ((height * 2) / 3) + 1; i < height - 1; i++) {
            gotoxy(width - 1, i);
            std::cout << "|";
        }
        for (int i = 0; i < width; i++) {
            gotoxy(i, height - 1);
            std::cout << "-";
        }
        gotoxy((width * 3) / 4, (height * 2) / 7);
        std::cout << "*FOOD";
        gotoxy((width * 1) / 4, (height * 2)/7);
        std::cout << (char)162 << "FOOD";

        for (int i = 5; i < width / 4; i++) {
            gotoxy(i, height / 2);
            std::cout << "o";
        }
        std::cout << "0";

        for (int i = 5; i < (width / 4) - 1; i++) {
            gotoxy((width / 2) + i, height / 2);
            std::cout << (char)176;
        }
        std::cout << (char)219;
       
        if (((GetAsyncKeyState(VK_RIGHT) | GetAsyncKeyState('D')) & 0x8000) && flag) {
            flag = false;
            selectIndex++;
            if (selectIndex == 2)
                selectIndex = 0;
        }
        else if (!((GetAsyncKeyState(VK_RIGHT) | GetAsyncKeyState('D')) & 0x8000) && !flag) {
            flag = true;
        }
        else if (((GetAsyncKeyState(VK_LEFT) | GetAsyncKeyState('A')) & 0x8000) && flag2) {
            flag2 = false;
            selectIndex--;
            if (selectIndex == -1)
                selectIndex = 1;
        }
        else if (!((GetAsyncKeyState(VK_LEFT) | GetAsyncKeyState('A')) & 0x8000) && !flag2) {
            flag2 = true;
        }
    } while (!(GetAsyncKeyState(VK_RETURN) & 0x8000));

    switch (selectIndex)
    {
    case 0:
        setTheme(GameThemes::CIRCLE);
        writeSettingsFile();
        Sleep(200);
        system("CLS");
        settingsMenu();
        break;
    case 1:
        setTheme(GameThemes::SQUARE);
        writeSettingsFile();
        Sleep(200);
        system("CLS");
        settingsMenu();
        break;
    default:
        break;
    }
}
void Game::outMenu() {
    short y = (height) / 12;
    gotoxy(((width / 2) - 5), (height / 12));
    if (gameWinStatus)
        std::cout << "You Win!!";
    else
        std::cout << "Game Over";
    short x = (((width / 2) - (short)playerName->length() / 2) - 5);
    y += (height) / 12;
    y += (height) / 12;
    short _x = (width / 2) - ((33 + (short)playerName->length()) / 2);
    gotoxy(_x, y);
    std::cout << "Player: " << *playerName << "   Score: " << score << "   Time: ";
    if (gameMinute < 10)
        std::cout << "0" << gameMinute;
    else
        std::cout << gameMinute;
    std::cout << ":";
    if (gameSecond < 10)
        std::cout << "0" << gameSecond;
    else
        std::cout << gameSecond;
    y += (height) / 12;
    y += (height) / 12;
    _x = (width / 6);
    gotoxy(_x, y);
    switch (Difficulty)
    {
    case GameDifficultys::EASY:
        std::cout << "PlayerName (EASY) \tScore \t\tTime";
        y += (height) / 12;
        gotoxy(0, y);
        for (int i = 0; i < width; i++)
            std::cout << "-";
        y += (height) / 12;
        for (int i = 0; i < 5; i++) {
            gotoxy(_x, y);
            std::cout << list[i][0];

            size_t length = list[i][0].length();
            if (length >= 14 && length <= 20)
            {
                std::cout << "\t";
            }
            else if (length > 5 && length < 14) {
                std::cout << "\t\t";
            }
            else if (length <= 5)
                std::cout << "\t\t\t";
            else
                std::cout << "\t\t\t\t";

            std::cout << list[i][1] << "\t\t" << list[i][2];
            y += (height) / 12;
        }
        break;
    case GameDifficultys::HARD:
        std::cout << "PlayerName (HARD) \tScore \t\tTime";
        y += (height) / 12;
        gotoxy(0, y);
        for (int i = 0; i < width; i++)
            std::cout << "-";
        y += (height) / 12;
        for (int i = 5; i < 10; i++) {
            gotoxy(_x, y);
            std::cout << list[i][0];

            size_t length = list[i][0].length();
            if (length >= 14 && length <= 20)
            {
                std::cout << "\t";
            }
            else if (length > 5 && length < 14) {
                std::cout << "\t\t";
            }
            else if (length <= 5)
                std::cout << "\t\t\t";
            else
                std::cout << "\t\t\t\t";

            std::cout << list[i][1] << "\t\t" << list[i][2];
            y += (height) / 12;
        }
        break;
    case GameDifficultys::SPECIAL:
        gotoxy(_x - 2, y);
        std::cout << "PlayerName  (SPECIAL) \tScore \t\tTime";
        y += (height) / 12;
        gotoxy(0, y);
        for (int i = 0; i < width; i++)
            std::cout << "-";
        y += (height) / 12;
        for (int i = 10; i < 15; i++) {
            gotoxy(_x - 2, y);
            std::cout << list[i][0];

            size_t length = list[i][0].length();
            if (length >= 14 && length <= 20)
            {
                std::cout << "\t";
            }
            else if (length > 7 && length < 14) {
                std::cout << "\t\t";
            }
            else if (length <= 7)
                std::cout << "\t\t\t";
            else
                std::cout << "\t\t\t\t";

            std::cout << list[i][1] << "\t\t" << list[i][2];
            y += (height) / 12;
        }
        break;
    default:
        break;
    }
    y += (height) / 12;
    short y_ = 0;
    bool flag = true;
    bool flag2 = true;
    short int selectIndex = 0;
    char temp[3] = "><";
    std::string options[] = { "New Game","Restart ","Quite " };
    do {
        y_ = y;
        for (short i = 0; i < 3; i++) {

            if (i == selectIndex) {
                temp[0] = '>';
                temp[1] = '<';
            }
            else {
                temp[0] = ' ';
                temp[1] = ' ';
            }
            y += ((height) / 12) + (i);
            gotoxy((width / 2) - (options[i].length() / 2) - (short)5, y);
            std::cout << temp[0] << "   " << options[i] << "   " << temp[1];
        }
        y = y_;
        if (((GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S')) & 0x8000) && flag) {
            flag = false;
            selectIndex++;
            if (selectIndex == 3)
                selectIndex = 0;
        }
        else if (!((GetAsyncKeyState(VK_DOWN) | GetAsyncKeyState('S')) & 0x8000) && !flag) {
            flag = true;
        }
        else if (((GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W')) & 0x8000) && flag2) {
            flag2 = false;
            selectIndex--;
            if (selectIndex == -1)
                selectIndex = 2;
        }
        else if (!((GetAsyncKeyState(VK_UP) | GetAsyncKeyState('W')) & 0x8000) && !flag2) {
            flag2 = true;
        }
    } while (!(GetAsyncKeyState(VK_RETURN) & 0x8000));

    saveNewList();

    switch (selectIndex)
    {
    case 0:
        Sleep(300);
        system("CLS");
        gameStatus = false;
        closeGame = false;
        menuSetPlayerName = true;
        break;
    case 1:
        Sleep(300);
        system("CLS");
        gameStatus = true;
        closeGame = false;
        menuSetPlayerName = false;
        setup();
        break;
    case 2:
        gameStatus = false;
        closeGame = true;
        break;
    default:
        break;
    }
}
void Game::readSettingsFile() {
    std::ifstream file("bin\\Settings.txt", std::ios::in);
    std::string* temp = new std::string;
    std::string* temp2 = new std::string;
    if (!file.is_open()) {
        setDifficulty(GameDifficultys::EASY);
        setTheme(GameThemes::CIRCLE);
        file.close();
        writeSettingsFile();
        return;
    }
    file.seekg(0, std::ios::beg);
    for (int i = 0; i < 2; i++) {
        file >> *temp >> *temp2;
        if (*temp == "Difficulty") {
            int dif = stoi(*temp2);
            if (dif >= 0 && dif <= 2)
                setDifficulty(static_cast<GameDifficultys>(stoi(*temp2)));
            else
                setDifficulty(GameDifficultys::EASY);
        }
        else if (*temp == "Theme") {
            int intTheme = stoi(*temp2);
            if (intTheme >= 0 && intTheme < 2)
                setTheme(static_cast<GameThemes>(intTheme));
            else
                setTheme(GameThemes::CIRCLE);
        }
        else {
            setDifficulty(GameDifficultys::EASY);
            setTheme(GameThemes::CIRCLE);
        }
    }
    file.close();
    delete temp;
    delete temp2;
}
void Game::writeSettingsFile() {
    std::fstream file("bin\\Settings.txt", std::ios::out);

    if (!file.is_open()) {
        file.close();
        std::cout << "Dosya acilamadi." << std::endl;
        return;
    }
    std::string newLine = "Theme ";
    std::string options[] = { "Difficulty","Theme" };
    file.seekg(0, std::ios::beg);
    for (int i = 0; i < 2; i++) {
        file << options[i] << " ";
        if (options[i] == "Difficulty") {
            file << static_cast<int>(Difficulty) <<std::endl;
        }
        else if (options[i] == "Theme") {
            file << static_cast<int>(Theme) << std::endl;
        }
    }
    file.close();
}
void Game::resizeConsole() {
    HWND console = GetConsoleWindow();
    CONSOLE_FONT_INFO cfi;
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, width * 12, height * 23, TRUE);
}
void Game::createFood() {
    while (true) {
        try {
            if (gameWinStatus) throw std::string("win");
            std::random_device rd;
            std::uniform_int_distribution<int> dist(1, width - 2);
            int x = dist(rd);
            std::uniform_int_distribution<int> dist1(1, height - 2);
            int y = dist1(rd);
            if (checkFoodSpawn(x, y)) {
                throw 404;
            }
            else {
                food->setX(x);
                food->setY(y);
                return;
            }
        }
        catch (int) {

        }
        catch (std::string) {
            food->setX(0);
            food->setY(0);
            return;
        }
    }
}
void Game::buildBorders() {
    for (int x = 0; x < width; x++) {
        scene[0][x] = borderType;
        scene[height - 1][x] = borderType;
    }
    for (int y = 0; y < height; y++) {
        scene[y][0] = borderType;
        scene[y][width - 1] = borderType;
    }
}
void Game::drawFrame() {
    hideCursor();
    gotoxy(0, 0);
    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {
            std::cout << scene[y][x];
        }
        std::cout << std::endl;
    }
}
void Game::listMenu() {

    getOldPlayerList();
    int y = 0;
    gotoxy(0, y);
    for (int i = 0; i < width; i++)
        std::cout << "-";
    y += (height) / 12;
    int _x = (width / 6);
    gotoxy(_x, y);
        std::cout << "PlayerName (EASY) \tScore \t\tTime";
        y += (height) / 12;
        gotoxy(0, y);
        for (int i = 0; i < width; i++)
            std::cout << "-";
        y += (height) / 12;
        for (int i = 0; i < 5; i++) {
            gotoxy(_x, y);
            std::cout << list[i][0];

            size_t length = list[i][0].length();
            if (length >= 14 && length <= 20)
            {
                std::cout << "\t";
            }
            else if (length > 5 && length < 14) {
                std::cout << "\t\t";
            }
            else if (length <= 5)
                std::cout << "\t\t\t";
            else
                std::cout << "\t\t\t\t";

            std::cout << list[i][1] << "\t\t" << list[i][2];
            y += (height) / 12;
        }
        gotoxy(0, y);
        for (int i = 0; i < width; i++)
            std::cout << "-";
        y += (height) / 12;
        gotoxy(_x, y);
        std::cout << "PlayerName (HARD) \tScore \t\tTime";
        y += (height) / 12;
        gotoxy(0, y);
        for (int i = 0; i < width; i++)
            std::cout << "-";
        y += (height) / 12;
        for (int i = 5; i < 10; i++) {
            gotoxy(_x, y);
            std::cout << list[i][0];

            size_t length = list[i][0].length();
            if (length >= 14 && length <= 20)
            {
                std::cout << "\t";
            }
            else if (length > 5 && length < 14) {
                std::cout << "\t\t";
            }
            else if (length <= 5)
                std::cout << "\t\t\t";
            else
                std::cout << "\t\t\t\t";

            std::cout << list[i][1] << "\t\t" << list[i][2];
            y += (height) / 12;
        }
        gotoxy(0, y);
        for (int i = 0; i < width; i++)
            std::cout << "-";
        y += (height) / 12;
        gotoxy(_x - 2, y);
        std::cout << "PlayerName  (SPECIAL) \tScore \t\tTime";
        y += (height) / 12;
        gotoxy(0, y);
        for (int i = 0; i < width; i++)
            std::cout << "-";
        y += (height) / 12;
        for (int i = 10; i < 15; i++) {
            gotoxy(_x - 2, y);
            std::cout << list[i][0];

            size_t length = list[i][0].length();
            if (length >= 14 && length <= 20)
            {
                std::cout << "\t";
            }
            else if (length > 7 && length < 14) {
                std::cout << "\t\t";
            }
            else if (length <= 7)
                std::cout << "\t\t\t";
            else
                std::cout << "\t\t\t\t";

            std::cout << list[i][1] << "\t\t" << list[i][2];
            y += (height) / 12;
        }
        gotoxy((width / 2)-4, y);
    std::cout << "> Back <";
    while (!(GetAsyncKeyState(VK_RETURN) & 0x8000));
    
    Sleep(300);
    system("CLS");
    menu();

}
void Game::settingsMenu() {
    std::string settings[] = { " Difficulty","Theme","Back" };
    std::string promt = "Settings  ";
    short int selectIndex = mainMenu(settings,3,&promt);
    
    switch (selectIndex)
    {
    case 0:
        Sleep(300);
        system("CLS");
        difficultyMenu();
        break;
    case 1:
        Sleep(250);
        system("CLS");
        themeMenu();
        break;
    case 2:
        Sleep(200);
        system("CLS");
        menu();
        break;
    default:
        break;
    }
}
void Game::clearFrame() {
    for (int y = 0; y < height; y++) {

        for (int x = 0; x < width; x++) {
            scene[y][x] = ' ';
        }
    }
}
void Game::checkKeyboard() {
    Cell_Direction _direction = snake->cells[0]->getDirection();
    if (_kbhit()) {
        switch (_getch()) {
        case 'p':
            if (!menuSetup && !chooseClose) {
                int x = width + width / 10;
                int y = (height * 5) / 6;
                if (!pauseGame) {
                    pauseGame = true;
                    GameState = GameStates::PAUSED;
                    gotoxy(x, y);
                    std::cout << "-GAME PAUSED-";
                    pausedTime = std::chrono::high_resolution_clock::now();
                }
                else {
                    pauseGame = false;
                    GameState = GameStates::GAMEPLAY;
                    gotoxy(x, y);
                    std::cout << "             ";
                    unpausedTime = std::chrono::high_resolution_clock::now();
                    previousGameTime += (unpausedTime - pausedTime);
                }
            }
            break;
        case 50:
            if (!menuSetup && !chooseClose) {
                int x = width + width / 10;
                int y = (height * 5) / 6;
                if (!pauseGame) {
                    pauseGame = true;
                    gotoxy(x, y);
                    std::cout << "-GAME PAUSED-";
                    pausedTime = std::chrono::high_resolution_clock::now();
                }
                else {
                    pauseGame = false;
                    gotoxy(x, y);
                    std::cout << "             ";
                    unpausedTime = std::chrono::high_resolution_clock::now();
                    previousGameTime += (unpausedTime - pausedTime);
                }
            }
            break;
        case 'w':
            if (GameState == GameStates::GAMEPLAY) {
                if (_direction != Cell_Direction::DOWN && _direction != Cell_Direction::UP) {
                    snake->cells[0]->setDirection(Cell_Direction::UP);
                }
            }
            break;
        case 'W':
            if (GameState == GameStates::GAMEPLAY) {
                if (_direction != Cell_Direction::DOWN && _direction != Cell_Direction::UP)
                    snake->cells[0]->setDirection(Cell_Direction::UP);
            }
            break;
        case 72:
            break;
        case 'a':
            if (GameState == GameStates::GAMEPLAY) {
                if (_direction != Cell_Direction::RIGHT && _direction != Cell_Direction::LEFT) {
                    snake->cells[0]->setDirection(Cell_Direction::LEFT);
                }
            }
            break;
        case 'A':
            if (GameState == GameStates::GAMEPLAY) {
                if (_direction != Cell_Direction::RIGHT && _direction != Cell_Direction::LEFT)
                    snake->cells[0]->setDirection(Cell_Direction::LEFT);
            }
            break;
        case 75:
            break;
        case 's':
            if (GameState == GameStates::GAMEPLAY) {
                if (_direction != Cell_Direction::UP && _direction != Cell_Direction::DOWN) {
                    snake->cells[0]->setDirection(Cell_Direction::DOWN);
                }
            }
            break;
        case 'S':
            if (GameState == GameStates::GAMEPLAY) {
                if (_direction != Cell_Direction::UP && _direction != Cell_Direction::DOWN)
                    snake->cells[0]->setDirection(Cell_Direction::DOWN);
            }
            break;
        case 80:
            break;
        case 'd':
            if (GameState == GameStates::GAMEPLAY) {
                if (_direction != Cell_Direction::LEFT && _direction != Cell_Direction::RIGHT) {
                    snake->cells[0]->setDirection(Cell_Direction::RIGHT);
                }
            }
            break;
        case 'D':
            if (GameState == GameStates::GAMEPLAY) {
                if (_direction != Cell_Direction::LEFT && _direction != Cell_Direction::RIGHT)
                    snake->cells[0]->setDirection(Cell_Direction::RIGHT);
            }
            break;
        case 77:
            break;
        default:
            break;
        }
    }
}
void Game::setScore() {
    score = (snake->getSnakeLength() * 10);
    if (score == 65526)
        score = 0;

    //draw player score
    int x = width + width / 10;
    int y = (height * 2) / 6;
    gotoxy(x + 7, y);
    std::cout << "    ";
    gotoxy(x + 7, y);
    std::cout << score;
}
void Game::setTheme(GameThemes _Theme) {
    Theme = _Theme;
    switch (Theme)
    {
    case GameThemes::CIRCLE:
        snake->setType('o');
        food->setType((char)162);
        borderType = (char)219;
        break;
    case GameThemes::SQUARE:
        snake->setType((char)176);
        food->setType('*');
        borderType = (char)178;
        break;
    default:
        break;
    }
}
void Game::setDifficulty(GameDifficultys _Difficulty) {
    Difficulty = _Difficulty;
    switch (Difficulty)
    {
    case GameDifficultys::EASY:
        bordersCheck = false;
        bodyCheck = true;
        eatCell = false;
        speed = ((height * width) *12)/1000;
        break;
    case GameDifficultys::HARD:
        bordersCheck = true;
        bodyCheck = true;
        speed = ((height * width) * 13) / 1000;
        eatCell = false;
        break;
    case GameDifficultys::SPECIAL:
        bordersCheck = true;
        bodyCheck = false;
        eatCell = true;
        speed = ((height * width) * 13) / 1000;
        break;
    default:
        break;
    }
}
GameDifficultys Game::getDifficulty() {
    return Difficulty;
}
GameThemes Game::getTheme() {
    return Theme;
}
bool Game::checkBorders() {
    bool status = false;
    int length = snake->getSnakeLength();
    for (int i = 0; i < length; i++) {
        int x = snake->cells[i]->getX();
        int y = snake->cells[i]->getY();
        if (y == 0) {
            snake->cells[i]->setY(height - 2);
            if(i == 0)
            status = true;
        }
        else if (y == (height - 1)) {
            snake->cells[i]->setY(1);
            if (i == 0)
            status = true;
        }
        else if (x == 0) {
            snake->cells[i]->setX(width - 2);
            if (i == 0)
            status = true;
        }
        else if (x == (width - 1)) {
            snake->cells[i]->setX(1);
            if (i == 0)
            status = true;
        }
    }
    return status;
}
void Game::checkGameStatus() {
    int body = snake->checkBody();
    if (body) {
        int length = snake->getSnakeLength();
        for (int i = 0; i < length - body; i++) {
            snake->removeTail();
        }
        if(eatCell)
            setScore();
    }

    if ((bordersCheck && border) || ((body && bodyCheck)) || gameWinStatus || closeGame) {
        GameState = GameStates::MENU;
        if (gameWinStatus)
            Sleep(1000);
        gameStatus = false;
        chooseClose = true;
        getOldPlayerList();
        assignScoreToList();
        while (_kbhit())
            _getch();
        system("CLS");
        outMenu();
    }
}
bool Game::checkFoodSpawn(const int& _x, const int& _y) {
    int length = snake->getSnakeLength();
    for (int i = 0; i < length; i++) {
        if ((_x == snake->cells[i]->getX()) && (_y == snake->cells[i]->getY()))
            return 1;
    }
    return 0;
}
bool Game::checkFood() {
    if ((snake->cells[0]->getX() == food->getX()) && (snake->cells[0]->getY() == food->getY())) {
        snake->addTail();
        setScore();
        if ((score + 50) >= snake->getMaxSnakeLength() * 10)
            gameWinStatus = true;
        createFood();
        return 1;
    }
    else
        return 0;
}
void Game::saveNewList() {
    std::ofstream outfile;
    outfile.open("log\\Save.txt", std::ios::out);
    if (outfile.is_open()) {
        for(int i = 0; i < 70; i++)
            outfile << "-";
        outfile << std::endl;
        outfile << "PlayerName" << "\t(EASY)" << "\t\t\tScore \t\tTime" << std::endl;
        for (int i = 0; i < 70; i++)
            outfile << "-";
        outfile << std::endl;
        for (int i = 0; i < 5; i++) {
            if (list[i][0].empty()) {
                outfile << "0";
                outfile << " ";
                outfile << "\t\t\t\t\t0";
                outfile << "\t\t0";
                outfile << std::endl;
            }
            else if (!list[i][0].empty()) {
                outfile << list[i][0];
                size_t a = list[i][0].length();
                for (int j = 0; j < maxPlayerNameLength - a + 1; j++)
                    outfile << " ";
                outfile << "\t\t\t" << list[i][1];
                outfile << "\t\t" << list[i][2];
                outfile << std::endl;
            } 
        }
        for (int i = 0; i < 70; i++)
            outfile << "-";
        outfile << std::endl;
        outfile << "PlayerName" << "\t(HARD)" << "\t\t\tScore \t\tTime" << std::endl;
        for (int i = 0; i < 70; i++)
            outfile << "-";
        outfile << std::endl;
        for (int i = 5; i < 10; i++) {
            if (list[i][0].empty()) {
                outfile << "0";
                outfile << " " ;
                outfile << "\t\t\t\t\t0";
                outfile << "\t\t0";
                outfile << std::endl;
            }
            else if (!list[i][0].empty()) {
                outfile << list[i][0];
                size_t a = list[i][0].length();
                for (int j = 0; j < maxPlayerNameLength - a + 1; j++)
                    outfile << " ";
                outfile << "\t\t\t" << list[i][1];
                outfile << "\t\t" << list[i][2];
                outfile << std::endl;
            }
        }
        for (int i = 0; i < 70; i++)
            outfile << "-";
        outfile << std::endl;
        outfile << "PlayerName"<< "\t(SPECÝAL)" << "\t\tScore \t\tTime" << std::endl;
        for (int i = 0; i < 70; i++)
            outfile << "-";
        outfile << std::endl;
        for (int i = 10; i < 15; i++) {
            if (list[i][0].empty()) {
                outfile << "0";
                outfile << " ";
                outfile << "\t\t\t\t\t0";
                outfile << "\t\t0";
                outfile << std::endl;
            }
            else if (!list[i][0].empty()) {
                outfile << list[i][0];
                size_t a = list[i][0].length();
                for (int j = 0; j < maxPlayerNameLength - a + 1; j++)
                    outfile << " ";
                outfile << "\t\t\t" << list[i][1];
                outfile << "\t\t" << list[i][2];
                outfile << std::endl;
            }
        }
    }
    
    outfile.close();

    
    std::ofstream file("bin\\Datas.bin", std::ios::binary | std::ios::out | std::ios::trunc);

    if (!file.is_open()) {
        file.close();
        return;
    }

    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 3; ++j) {
            file.write(list[i][j].c_str(), sizeof(std::string));
        }
    }
    file.close();
}
void Game::getOldPlayerList() {
    std::ifstream file("bin\\Datas.bin", std::ios::binary | std::ios::in);

    if (!file.is_open()) {
        file.close();

        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 3; ++j) {
                list[i][j] = "\0";
            }
        }
        return;
    }

    file.seekg(0, std::ios::end);
    std::streampos fileSize = file.tellg();
    if (fileSize != (sizeof(std::string) * 45)) {
        /* close Corrupted file */
        file.close();

        /* set lists null */
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 3; ++j) {
                list[i][j] = "\0";
            }
        }

        return;
    }
    else {
        file.seekg(0, std::ios::beg);
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 3; ++j) {
                char* buffer = new char[256];
                file.read(buffer, sizeof(std::string));
                list[i][j] = buffer;
                delete[] buffer;
            }
        }
        file.close();
    }

}
void Game::createGameFiles() {
    _mkdir("bin");
    _mkdir("log");
    system("CLS");
    readSettingsFile();
    getOldPlayerList();
    saveNewList();
}
unsigned long int Game::setTime() {
    std::chrono::duration<int, std::milli> gameMilliSeconds;
    if (!pauseGame) {
        std::chrono::high_resolution_clock::time_point currentTime = std::chrono::high_resolution_clock::now();
        gameMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - previousGameTime);
    }
    else {
        gameMilliSeconds = std::chrono::duration_cast<std::chrono::milliseconds>(pausedTime - previousGameTime);
    }
    
    gameMinute = std::chrono::duration_cast<std::chrono::minutes>(gameMilliSeconds).count();
    gameSecond = std::chrono::duration_cast<std::chrono::seconds>(gameMilliSeconds).count() % 60;

    int x = width + width / 10;
    int y = (height) / 6;
    gotoxy(x + 7, y + (2 * height) / 6);
    if (gameMinute < 10)
        std::cout << "0" << gameMinute;
    else
        std::cout << gameMinute;
    std::cout << ":";
    if (gameSecond < 10)
        std::cout << "0" << gameSecond;
    else
        std::cout << gameSecond;

    return gameMilliSeconds.count();
}
void Game::assignScoreToList() {
    int second = (gameMinute * 60) + gameSecond;
    switch (Difficulty)
    {
    case GameDifficultys::EASY:
        for (int i = 0; i < 5; i++) {
            int oldScore = atoi(list[i][1].c_str());
            if (score > oldScore) {
                for (int j = 3; j >= i; j--) {
                    list[j + 1][0] = list[j][0];
                    list[j + 1][1] = list[j][1];
                    list[j + 1][2] = list[j][2];
                }
                list[i][0] = *playerName;
                list[i][1] = std::to_string(score);
                if (gameMinute < 10)
                    list[i][2] = "0" + std::to_string(gameMinute);
                else
                    list[i][2] = std::to_string(gameMinute);
                list[i][2] += ":";
                if (gameSecond < 10)
                    list[i][2] += "0" + std::to_string(gameSecond);
                else
                    list[i][2] += std::to_string(gameSecond);

                break;
            }
            else if (score == oldScore) {
                int oldMinute = atoi(list[i][2].substr(0, list[i][2].find(':')).c_str());
                int oldSecond = atoi(list[i][2].substr(list[i][2].find(':') + 1, list[i][2].length()).c_str());
                oldSecond += oldMinute * 60;


                if (second <= oldSecond) {
                    for (int j = 3; j >= i; j--) {
                        list[j + 1][0] = list[j][0];
                        list[j + 1][1] = list[j][1];
                        list[j + 1][2] = list[j][2];
                    }
                    list[i][0] = *playerName;
                    list[i][1] = std::to_string(score);
                    if (gameMinute < 10)
                        list[i][2] = "0" + std::to_string(gameMinute);
                    else
                        list[i][2] = std::to_string(gameMinute);
                    list[i][2] += ":";
                    if (gameSecond < 10)
                        list[i][2] += "0" + std::to_string(gameSecond);
                    else
                        list[i][2] += std::to_string(gameSecond);

                    break;
                }
            }
            else if (list[i][0].empty())
            {
                list[i][0] = *playerName;
                list[i][1] = std::to_string(score);
                break;
            }
        }
        break;
    case GameDifficultys::HARD:
        for (int i = 5; i < 10; i++) {
            int oldScore = atoi(list[i][1].c_str());
            if (score > oldScore) {
                for (int j = 8; j >= i; j--) {
                    list[j + 1][0] = list[j][0];
                    list[j + 1][1] = list[j][1];
                    list[j + 1][2] = list[j][2];

                }
                list[i][0] = *playerName;
                list[i][1] = std::to_string(score);
                if (gameMinute < 10)
                    list[i][2] = "0" + std::to_string(gameMinute);
                else
                    list[i][2] = std::to_string(gameMinute);
                list[i][2] += ":";
                if (gameSecond < 10)
                    list[i][2] += "0" + std::to_string(gameSecond);
                else
                    list[i][2] += std::to_string(gameSecond);

                break;
            }
            else if (score == oldScore) {
                int oldMinute = atoi(list[i][2].substr(0, list[i][2].find(':')).c_str());
                int oldSecond = atoi(list[i][2].substr(list[i][2].find(':') + 1, list[i][2].length()).c_str());
                oldSecond += oldMinute * 60;

                
                if (second <= oldSecond) {
                    for (int j = 8; j >= i; j--) {
                        list[j + 1][0] = list[j][0];
                        list[j + 1][1] = list[j][1];
                        list[j + 1][2] = list[j][2];

                    }
                    list[i][0] = *playerName;
                    list[i][1] = std::to_string(score);
                    if (gameMinute < 10)
                        list[i][2] = "0" + std::to_string(gameMinute);
                    else
                        list[i][2] = std::to_string(gameMinute);
                    list[i][2] += ":";
                    if (gameSecond < 10)
                        list[i][2] += "0" + std::to_string(gameSecond);
                    else
                        list[i][2] += std::to_string(gameSecond);

                    break;
                }
            }
            else if (list[i][0].empty())
            {
                list[i][0] = *playerName;
                list[i][1] = std::to_string(score);
                break;
            }
        }
        break;
    case GameDifficultys::SPECIAL:
        for (int i = 10; i < 15; i++) {
            int oldScore = atoi(list[i][1].c_str());
            if (score >= oldScore) {
                for (int j = 13; j >= i; j--) {
                    list[j + 1][0] = list[j][0];
                    list[j + 1][1] = list[j][1];
                    list[j + 1][2] = list[j][2];

                }
                list[i][0] = *playerName;
                list[i][1] = std::to_string(score);
                if (gameMinute < 10)
                    list[i][2] = "0" + std::to_string(gameMinute);
                else
                    list[i][2] = std::to_string(gameMinute);
                list[i][2] += ":";
                if (gameSecond < 10)
                    list[i][2] += "0" + std::to_string(gameSecond);
                else
                    list[i][2] += std::to_string(gameSecond);

                break;
            }
            else if (score == oldScore) {
                int oldMinute = atoi(list[i][2].substr(0, list[i][2].find(':')).c_str());
                int oldSecond = atoi(list[i][2].substr(list[i][2].find(':') + 1, list[i][2].length()).c_str());
                oldSecond += oldMinute * 60;


                if (second <= oldSecond) {
                    for (int j = 13; j >= i; j--) {
                        list[j + 1][0] = list[j][0];
                        list[j + 1][1] = list[j][1];
                        list[j + 1][2] = list[j][2];

                    }
                    list[i][0] = *playerName;
                    list[i][1] = std::to_string(score);
                    if (gameMinute < 10)
                        list[i][2] = "0" + std::to_string(gameMinute);
                    else
                        list[i][2] = std::to_string(gameMinute);
                    list[i][2] += ":";
                    if (gameSecond < 10)
                        list[i][2] += "0" + std::to_string(gameSecond);
                    else
                        list[i][2] += std::to_string(gameSecond);

                    break;
                }
            }
            else if (list[i][0].empty())
            {
                list[i][0] = *playerName;
                list[i][1] = std::to_string(score);
                break;
            }
        }
        break;
    default:
        break;
    }
}
void Game::loop() {

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();
    float fElapsedTime = 0.0f;

    while (!closeGame) {
        menu();
        while (gameStatus) {
            setTime();
            if (!pauseGame) {
                tp2 = std::chrono::system_clock::now();
                std::chrono::duration<float> elapsedTime = tp2 - tp1;
                
                fElapsedTime = elapsedTime.count();
                
                clearFrame();
                snake->build(scene);
                food->build(scene);
                buildBorders();
                drawFrame();
                
                if ((fElapsedTime) >= (1.0f / speed)) {
                    checkKeyboard();
                    snake->move();
                    border = checkBorders();
                    checkFood();
                    checkGameStatus();
                    tp1 = tp2;
                }
            }
            else {
                checkKeyboard();
            }
        }
    }
}
