//
// Created by Elizabeth Gonzalez on 11/21/24.
//

#ifndef TEXTUREMANAGER_H
#define TEXTUREMANAGER_H
#include <map>
#include <string>
using namespace std;
#include <SFML/Graphics/Texture.hpp>

struct TextureManager {
    map<string, sf::Texture> textures;
    public:
    TextureManager() : textures() {}

    void loadTexture(const string &filename) {
        //filename "files/images/" + filename + ".png";
        if (textures.find(filename) == textures.end()) {
            textures[filename].loadFromFile("files/images/" + filename + ".png");
        }
    }
    void clearTexture() {
        textures.clear();
    }

};

#endif //TEXTUREMANAGER_H
