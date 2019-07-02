//
// Created by alex on 21/01/19.
//

#pragma once

#include <GL/glew.h>
#include <string>
#include <unordered_map>

#include "texture.h"

// Manages loading/unloading of textures

class TextureManager {
private:
    std::unordered_map<std::string, Texture> textures_;

public:
    TextureManager();
    ~TextureManager();

    bool load_texture(const char* path, const char* name);

    Texture get_texture(const char* name);
};