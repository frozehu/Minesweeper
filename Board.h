//
// Created by Elizabeth Gonzalez on 11/19/24.
//


#ifndef BOARD_H
#define BOARD_H
#include "Tile.h"
#include <random>
#include <ctime>
#pragma once

class Board {
    int rows;
    int cols;
    int mines;
    vector<vector<Tile*>> board;

    public:
    Board() : rows(0), cols(0), mines(0) {}
    Board(int rows, int cols, int mines) : rows(rows), cols(cols), mines(mines) {};
    ~Board() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                delete board[i][j];
            }
        }
    };
    void initializeBoard(TextureManager &textureManager) {
        board.resize(rows, vector<Tile*>(cols, nullptr));
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                board[i][j] = new Tile();
                board[i][j]->getSprite().setTexture(textureManager.textures["tile_hidden"]);
            }
        }
        assignMines();
        findAdjacent();
    }

    void revealAdjacent(TextureManager &textureManager, Tile* tile, int x, int y) {
        tile = getTile(x, y);
        if (!tile->ifMine() && !tile->ifFlag()) {
            if (!tile->ifRevealed()) {
                tile->reveal();
            } if (tile->getAdjacent() >0) {
                return;
            }
            for (int otherRow = -1; otherRow <=1; otherRow++) {
                for (int otherCol = -1; otherCol <=1; otherCol++) {
                    if (otherRow == 0 && otherCol == 0) { continue;}
                    int adjacentRow = x + otherRow;
                    int adjacentCol = y + otherCol;
                    if (adjacentRow >=0 && adjacentRow < rows
                        && adjacentCol >=0 && adjacentCol < cols) {
                        Tile* newTile = getTile(adjacentRow, adjacentCol);
                        if (!newTile->ifRevealed() && !newTile->ifFlag()) {
                            revealAdjacent(textureManager, newTile, adjacentRow, adjacentCol);
                        }
                    }
                }
            }
        }
    }
    void drawBoard(sf::RenderWindow& window, TextureManager& textureManager, bool pause){
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                //board[i][j]->getSprite().setTexture(textureManager.textures["tile_hidden"]);
                board[i][j]->drawTile(window, j*32, i*32, textureManager, pause);
            }
        }
    }
    Tile* getTile(int row, int col) {
        return board[row][col];
    }
    void assignMines() {
        srand(static_cast<unsigned>(time(NULL)));
        int placedMines = 0;
        while (placedMines < mines) {
            int randomRow = rand () % rows;
            int randomCol = rand () % cols;
            Tile* tile = getTile(randomRow, randomCol);
            if (!tile->ifMine()) {
                tile->setMine(true);
                placedMines++;
            }
        }
    }
    void findAdjacent() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Tile* tile = getTile(i, j);
                if (tile->ifMine()) {
                    continue;
                }
                int count = 0;
                for (int otherRow = -1; otherRow <=1; otherRow++) {
                    for (int otherCol = -1; otherCol <=1; otherCol++) {
                        if (otherRow == 0 && otherCol == 0) { continue;}
                        int adjacentRow = i + otherRow;
                        int adjacentCol = j + otherCol;
                        if (adjacentRow >=0 && adjacentRow < rows
                            && adjacentCol >=0 && adjacentCol < cols) {
                            Tile* newTile = getTile(adjacentRow, adjacentCol);
                            if (newTile->ifMine()) {
                                count++;
                            }
                        }
                    }
                }

                tile->setAdjacent(count);
            }
        }
    }
    bool checkWin() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Tile* tile = getTile(i, j);
                if (!tile->ifMine() && tile->ifRevealed()) {
                    continue;
                } else if (!tile->ifMine() && !tile->ifRevealed()) {
                    return false;
                }
            }
        }
        return true;
    }
    void reset(TextureManager &textureManager) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Tile* tile = getTile(i, j);
                tile->reset();
            }
        }
        initializeBoard(textureManager);
    }
    void debugMode(TextureManager &textureManager, bool debug) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Tile* tile = getTile(i, j);
                if (tile->ifMine()) {
                    if (debug) {
                        tile->revealDebug(textureManager, debug);
                    } else {
                        tile->revealDebug(textureManager, debug);
                    }
                }
            }
        }
    }
    void revealAllMines() {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Tile* tile = getTile(i, j);
                if (tile->ifMine()) {
                    tile->reveal();
                }
            }
        }
    }
    void flagAllMines(TextureManager &textureManager) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Tile* tile = getTile(i, j);
                if (tile->ifMine()) {
                    tile->mineToFlag();
                }
            }
        }
    }
    void pauseMode(TextureManager &textureManager, bool pause) {
        for (int i = 0; i < rows; i++) {
            for (int j = 0; j < cols; j++) {
                Tile* tile = getTile(i, j);
                if (tile) {
                    if (pause) {
                        tile->revealPause(textureManager, pause);
                    } else {
                        tile->revealPause(textureManager, pause);
                    }
                }
            }
        }
    }

};



#endif //BOARD_H
