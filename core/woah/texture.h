#pragma once

#include "../ew/external/glad.h"

#define STB_IMAGE_IMPLEMENTATION
#include "../ew/external/stb_image.h"

#include <string>
#include <iostream>

class Texture {
public:
    Texture(const char* filePath, int filterMode, int wrapMode);
    ~Texture() {};
    void Bind(unsigned int slot = 0); //Bind to a specific texture unit
private:
    unsigned int m_id; //GL texture handle
    int m_width, m_height;
};