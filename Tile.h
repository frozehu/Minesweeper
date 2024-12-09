
#ifndef TILE_H
#define TILE_H
#include "TextureManager.h"
#pragma once


class Tile {
    sf::Sprite sprite;
    bool isMine;
    bool hasFlag;
    bool isRevealed;
    int adjacentMines;

    public:
    Tile() : isMine(false), hasFlag(false), isRevealed(false), adjacentMines(0) {}
    Tile(bool isMine, bool hasFlag, bool isRevealed, int adjacentMines, sf::Sprite sprite) :
    isMine(isMine), hasFlag(hasFlag), isRevealed(isRevealed), adjacentMines(adjacentMines), sprite(sprite){}

    bool ifMine() { return isMine; }
    bool ifFlag() { return hasFlag; }
    bool ifRevealed() { return isRevealed; }
    int getAdjacent() { return adjacentMines; }
    sf::Sprite &getSprite() { return sprite; }

    void setTexture(TextureManager &textureManager, string file) {
        sprite.setTexture(textureManager.textures[file]);
    }
    //for gamewin, flags every mine that isnt already flagged
    void mineToFlag() {
        isMine = false;
        hasFlag = true;
    }
    void revealDebug(TextureManager &textureManager, bool debug) {
        if (!isRevealed && !hasFlag) {
            isRevealed = true;
            sprite.setTexture(textureManager.textures["tile_revealed"]);
        } else if (isRevealed && debug){
            isRevealed = false;
            sprite.setTexture(textureManager.textures["tile_hidden"]);
        }
    }
    void reveal() {
        if (!isRevealed) {
            isRevealed = true;
        }
    }
    void revealPause(TextureManager &textureManager, bool pause) {
        if (!isRevealed) {
            sprite.setTexture(textureManager.textures["tile_revealed"]);
        } else if (isRevealed && !pause) {
            sprite.setTexture(textureManager.textures["tile_hidden"]);
        }
    }
    void setFlag() {
        if (!hasFlag && !isRevealed) {
            hasFlag = true;
        }
        else {
            hasFlag = false;
        }
    }
    void setMine(bool value) {
        isMine = value;
    }
    void reset() {
        isMine = false;
        hasFlag = false;
        isRevealed = false;
        adjacentMines = 0;
    }
    void drawTile(sf::RenderWindow &window, int x, int y, TextureManager &textureManager, bool pause) {
        if (pause) {
            sprite.setTexture(textureManager.textures["tile_revealed"]);
            sprite.setPosition(x, y);
            window.draw(sprite);
        } else {
            if (isRevealed) {
                sprite.setTexture(textureManager.textures["tile_revealed"]);
            } else {
                sprite.setTexture(textureManager.textures["tile_hidden"]);
            }
            sprite.setPosition(x, y);
            window.draw(sprite);
            if (isRevealed && !hasFlag) {
                if (isMine) {
                    sf::Sprite overlay;
                    overlay.setPosition(x, y);
                    overlay.setTexture(textureManager.textures["mine"]);
                    window.draw(overlay);
                } else if (adjacentMines > 0) {
                    sf::Sprite overlay;
                    overlay.setPosition(x, y);
                    overlay.setTexture(textureManager.textures["number_" + std::to_string(adjacentMines)]);
                    window.draw(overlay);
                }
            }
            if (hasFlag) {
                sf::Sprite overlay;
                overlay.setPosition(x, y);
                overlay.setTexture(textureManager.textures["flag"]);
                window.draw(overlay);
                if (isRevealed && isMine) {
                    sf::Sprite overlay2;
                    overlay2.setPosition(x, y);
                    overlay2.setTexture(textureManager.textures["mine"]);
                    window.draw(overlay2);
                }
            }
        }
    }
    void setAdjacent(int value) {
        adjacentMines = value;
    }
};


#endif //TILE_H
