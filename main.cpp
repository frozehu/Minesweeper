#include <iostream>
using namespace std;
#include <string>
#include <fstream>
#include <vector>
#include <SFML/Graphics.hpp>
#include "Board.h"
#include "Leaderboard.h"

void setText(sf::Text &text,float x, float y){
    sf::FloatRect textRect = text.getLocalBounds();
    text.setOrigin(textRect.left + textRect.width/2.0f, textRect.top + textRect.height/2.0f);
    text.setPosition(sf::Vector2f(x, y));
}

//all code for my timer was taken from the demo video with code written by
// Gabriela Tabaoda!
map<int, sf::Sprite> parseDigits(sf::Sprite digits) {
    map<int, sf::Sprite> digitsMap;
    for (int i = 0; i <= 10; i++) {
        sf::IntRect rect(i*21, 0, 21, 32);
        digits.setTextureRect(rect);
        sf::Sprite sprite = digits;
        digitsMap.emplace(i, sprite);
    }
    return digitsMap;
}



int main() {
    TextureManager textureManager;
    textureManager.loadTexture("debug");
    textureManager.loadTexture("digits");
    textureManager.loadTexture("face_happy");
    textureManager.loadTexture("face_lose");
    textureManager.loadTexture("face_win");
    textureManager.loadTexture("flag");
    textureManager.loadTexture("leaderboard");
    textureManager.loadTexture("mine");
    textureManager.loadTexture("number_1");
    textureManager.loadTexture("number_2");
    textureManager.loadTexture("number_3");
    textureManager.loadTexture("number_4");
    textureManager.loadTexture("number_5");
    textureManager.loadTexture("number_6");
    textureManager.loadTexture("number_7");
    textureManager.loadTexture("number_8");
    textureManager.loadTexture("pause");
    textureManager.loadTexture("play");
    textureManager.loadTexture("tile_hidden");
    textureManager.loadTexture("tile_revealed");

    string rowsS, colsS, minesS;
    ifstream numFile("files/config.cfg");
    getline(numFile, colsS);
    getline(numFile, rowsS);
    getline(numFile, minesS);
    int rows = stoi(rowsS);
    int cols = stoi(colsS);
    int mines = stoi(minesS);

    float width = cols*32;
    float height = rows*32 + 100;

    bool gameOver = false;
    bool gameWon = false;
    Player player;

    sf::RenderWindow welcomeWindow(sf::VideoMode(width, height), "Minesweeper");

    sf::Font font;
    font.loadFromFile("files/font.ttf");
    sf::Text welcomeText("WELCOME TO MINESWEEPER!", font, 24);
    welcomeText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(welcomeText, width/2, height/2 -150);

    sf::Text nameText("Enter your name:", font, 20);
    nameText.setStyle(sf::Text::Bold);
    setText(nameText, width/2, height/2 - 75);

    sf::Text inputText("", font, 18);
    inputText.setFillColor(sf::Color::Yellow);
    inputText.setStyle(sf::Text::Bold);
    string name;

    sf::Text cursor("|", font, 18);
    cursor.setFillColor(sf::Color::Yellow);
    cursor.setStyle(sf::Text::Bold);

    auto start_time = chrono::high_resolution_clock::now();
    auto pauseTime = chrono::high_resolution_clock::now();
    auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();

    sf::Text lbText("LEADERBOARD", font, 20);
    lbText.setStyle(sf::Text::Bold | sf::Text::Underlined);
    setText(lbText, (cols*16)/2, (rows*16 + 50)/2 -120);
    bool lbOpen = false;


    sf::Sprite digits;
    digits.setTexture(textureManager.textures["digits"]);
    map<int, sf::Sprite> digitsMap = parseDigits(digits);

    int count = mines;

    sf::Sprite debugButton;
    debugButton.setTexture(textureManager.textures["debug"]);
    debugButton.setPosition((cols*32)-304, (rows+0.5)*32);
    bool debug = false;

    sf::Sprite pauseButton;
    pauseButton.setTexture(textureManager.textures["pause"]);
    pauseButton.setPosition((cols*32)-240, (rows+0.5)*32);
    bool pause = false;
    bool pauseMode = false;

    sf::Sprite resetButton;
    resetButton.setTexture(textureManager.textures["face_happy"]);
    resetButton.setPosition(((cols/2.0)*32)-32, (rows+0.5)*32);

    sf::Sprite leaderboardButton;
    leaderboardButton.setTexture(textureManager.textures["leaderboard"]);
    leaderboardButton.setPosition((cols*32)-176, (rows+0.5)*32);
    Leaderboard leaderboard;



    while(welcomeWindow.isOpen()) {
        sf::Event event;
        while(welcomeWindow.pollEvent(event)) {
            if(event.type == sf::Event::Closed) {
                welcomeWindow.close();
                break;
            }
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode < 128) {
                    char key = static_cast<char>(event.text.unicode);
                    if (isalpha(key)) {
                        if (name.empty()) {
                            key = toupper(key);
                        } else { key = tolower(key); }
                        if (name.size() <= 9) {
                            name += key;
                        }
                }
            }
        }
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.scancode == sf::Keyboard::Scan::Backspace && !name.empty()) {
                    name.pop_back();
                }
                if (event.key.code == sf::Keyboard::Enter && !name.empty()) {
                    welcomeWindow.close();

            auto start_time = chrono::high_resolution_clock::now();
            auto pauseTime = chrono::high_resolution_clock::now();
            auto elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();

            Board board(rows, cols, mines);
            board.initializeBoard(textureManager);
            sf::RenderWindow gameWindow(sf::VideoMode(width, height), "Minesweeper");
            while(gameWindow.isOpen()) {
                sf::Event event;
                while(gameWindow.pollEvent(event)) {
                    if(event.type == sf::Event::Closed) {
                        gameWindow.close();
                    }
                        if (event.type == sf::Event::MouseButtonPressed) {
                            if (event.mouseButton.button == sf::Mouse::Left) {
                                sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                                if (debugButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                    if (!gameOver) {
                                        debug = !debug;
                                        board.debugMode(textureManager, debug);
                                    }
                                } else if (pauseButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                    pauseMode = !pauseMode;
                                    if (!gameOver){
                                    pause = !pause;
                                    board.pauseMode(textureManager, pause);
                                    if (pause) {
                                        pauseButton.setTexture(textureManager.textures["play"]);
                                        pauseTime = chrono::high_resolution_clock::now();
                                    } else {
                                        pauseButton.setTexture(textureManager.textures["pause"]);
                                        auto unPausedTime = chrono::steady_clock::now();
                                        elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count();
                                    }
                                }
                                } else if (resetButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                    if (pause) {
                                        pause = false;
                                        board.pauseMode(textureManager, pause);
                                        pauseButton.setTexture(textureManager.textures["pause"]);
                                    }
                                    board.reset(textureManager);
                                    resetButton.setTexture(textureManager.textures["face_happy"]);
                                    count = mines;
                                    start_time = chrono::high_resolution_clock::now();
                                    pauseTime = chrono::high_resolution_clock::now();
                                    elapsed_paused_time = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - pauseTime).count();
                                    gameOver = false;
                                }
                                else if (leaderboardButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                                    if (!gameOver && !pause){
                                        pause = true;
                                        board.pauseMode(textureManager, pause);
                                        pauseButton.setTexture(textureManager.textures["play"]);
                                    }
                                    lbOpen = !lbOpen;
                                    pauseTime = chrono::high_resolution_clock::now();
                                }
                                if (!pause && !gameOver) {
                                    int col = mousePos.x / 32;
                                    int row = mousePos.y / 32;
                                    if (row < rows && col < cols) {
                                        Tile* tile = board.getTile(row, col);
                                        if (!tile->ifFlag()) {
                                            tile->reveal();
                                            board.revealAdjacent(textureManager, tile, row, col);
                                            if(tile->ifMine()) {
                                                resetButton.setTexture(textureManager.textures["face_lose"]);
                                                board.revealAllMines();
                                                gameOver = true;
                                            }
                                        }
                                    }
                                }
                            } else if (event.mouseButton.button == sf::Mouse::Right) {
                                if (!gameOver){
                                    sf::Vector2i mousePos = sf::Mouse::getPosition(gameWindow);
                                    int col = mousePos.x / 32;
                                    int row = mousePos.y / 32;
                                    if (row < rows && col < cols) {
                                        Tile* tile = board.getTile(row, col);
                                        if (!tile->ifRevealed()) {
                                            tile->setFlag();
                                            if (tile->ifFlag()) {
                                                count--;
                                            } else {
                                                count++;
                                            }
                                        }
                                    }
                                }
                            }
                    }
                }
                    if (!pause) {
                    if (board.checkWin()) {
                        gameOver = true;
                        gameWon = true;
                        auto game_duration = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
                        int total_time = game_duration.count();
                        int minutes;
                        int seconds;
                        total_time = total_time - elapsed_paused_time;
                        minutes = total_time / 60;
                        seconds = total_time % 60;
                        int minutes0 = minutes/10 % 10;
                        int minutes1 = minutes % 10;
                        int seconds0 = seconds/10 % 10;
                        int seconds1 = seconds % 10;
                        player = Player(name + "*", to_string(minutes0) + to_string(minutes1) + ":"
                            + to_string(seconds0) + to_string(seconds1));
                        leaderboard.read();
                        leaderboard.add(player);
                        if (leaderboard.isFastest(player)) {
                            leaderboard.sortbyTime();
                            leaderboard.remove();
                            leaderboard.write();
                        }

                        resetButton.setTexture(textureManager.textures["face_win"]);
                        board.flagAllMines(textureManager);
                        count = 0;
                    }

                }
                gameWindow.clear(sf::Color::White);
                auto game_duration = chrono::duration_cast<chrono::seconds>(chrono::high_resolution_clock::now() - start_time);
                int total_time = game_duration.count();
                int minutes;
                int seconds;
                if (!pause && !gameOver && !lbOpen) {
                    total_time = total_time - elapsed_paused_time;
                    minutes = total_time / 60;
                    seconds = total_time % 60;
                }
                int minutes0 = minutes/10 % 10;
                int minutes1 = minutes % 10;
                int seconds0 = seconds/10 % 10;
                int seconds1 = seconds % 10;

                digitsMap[minutes0].setPosition((cols*32)-97, (rows+0.5)*32 + 16);
                gameWindow.draw(digitsMap[minutes0]);
                digitsMap[minutes1].setPosition((cols*32)-76, (rows+0.5)*32 + 16);
                gameWindow.draw(digitsMap[minutes1]);
                digitsMap[seconds0].setPosition((cols*32)-54, (rows+0.5)*32 + 16);
                gameWindow.draw(digitsMap[seconds0]);
                digitsMap[seconds1].setPosition((cols*32)-33, (rows+0.5)*32 + 16);
                gameWindow.draw(digitsMap[seconds1]);


                if (count <0) {
                    int newCount = count*-1;
                    int count3 = newCount/100 %10;
                    int count1 = newCount/10 % 10;
                    int count2 = newCount%10;
                    int count0 = 10;
                    digitsMap[count2].setPosition(75, (rows+0.5)*32 +16);
                    gameWindow.draw(digitsMap[count2]);
                    digitsMap[count1].setPosition(54, (rows+0.5)*32 +16);
                    gameWindow.draw(digitsMap[count1]);
                    digitsMap[count3].setPosition(33, (rows+0.5)*32 +16);
                    gameWindow.draw(digitsMap[count3]);
                    digitsMap[count0].setPosition(12, (rows+0.5)*32 +16);
                    gameWindow.draw(digitsMap[count0]);
                } else {
                    int count3 = count /100 % 10;
                    int count1 = count/10 % 10;
                    int count2 = count%10;
                    digitsMap[count2].setPosition(54, (rows+0.5)*32 +16);
                    gameWindow.draw(digitsMap[count2]);
                    digitsMap[count1].setPosition(33, (rows+0.5)*32 +16);
                    gameWindow.draw(digitsMap[count1]);
                    digitsMap[count3].setPosition(12, (rows+0.5)*32 +16);
                    gameWindow.draw(digitsMap[count3]);
                }



                gameWindow.draw(debugButton);
                gameWindow.draw(pauseButton);
                gameWindow.draw(leaderboardButton);
                gameWindow.draw(resetButton);
                board.drawBoard(gameWindow, textureManager, pause);

                gameWindow.display();
                if (lbOpen || gameWon) {
                    sf::RenderWindow lbWindow(sf::VideoMode(cols*16, (rows*16) +50), "Minesweeper");
                    while(lbWindow.isOpen()) {
                        sf::Event lbEvent;
                        while(lbWindow.pollEvent(lbEvent)) {
                            if (lbEvent.type == sf::Event::Closed) {
                                lbOpen = false;
                                if (!pauseMode && pause) {
                                    pause = false;
                                    board.pauseMode(textureManager, pause);
                                    pauseButton.setTexture(textureManager.textures["pause"]);
                                }
                                if (!gameOver && !gameWon) {
                                    auto unPausedTime = chrono::steady_clock::now();
                                    elapsed_paused_time += chrono::duration_cast<chrono::seconds>(unPausedTime - pauseTime).count();
                                }
                                lbWindow.close();
                                if (gameWon) {
                                    gameWon = false;
                                }
                            }
                        }

                        lbWindow.clear(sf::Color::Blue);
                        if (!gameWon) {
                            leaderboard.read();
                        }
                        vector<Player> playerVect = leaderboard.getPlayers();
                        sf::Text playerText(
                            "1.\t" + playerVect[0].getScore() + "\t"+
                        playerVect[0].getName() + "\n\n" +
                        "2.\t" + playerVect[1].getScore() + "\t"+
                        playerVect[1].getName() + "\n\n" +
                        "3.\t" + playerVect[2].getScore() + "\t"+
                                playerVect[2].getName() + "\n\n" +
                                "4.\t" + playerVect[3].getScore() + "\t"+
                                playerVect[3].getName() + "\n\n" +
                                "5.\t" + playerVect[4].getScore() + "\t"+
                                playerVect[4].getName() + "\n\n", font, 18);
                        playerText.setStyle(sf::Text::Bold);
                        setText(playerText, (cols*16)/2, (rows*16 + 50)/2 + 20);
                        lbWindow.draw(playerText);
                        lbWindow.draw(lbText);
                        lbWindow.display();
                    }
                }
            }
        }
    }
            inputText.setString(name);
            setText(inputText, width/2, height/2 - 45);
            setText(cursor, inputText.getPosition().x + (inputText.getGlobalBounds().width/2)+5,
                inputText.getPosition().y);


            welcomeWindow.clear(sf::Color::Blue);
            welcomeWindow.draw(welcomeText);
            welcomeWindow.draw(nameText);
            welcomeWindow.draw(inputText);
            welcomeWindow.draw(cursor);
            welcomeWindow.display();
        }
    }
    return 0;
}