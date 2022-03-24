#include "Texture.hpp"
#include <glad/glad.h>
#include <stb/stb_image.h>
#include <SDL.h>

bool Texture::Load(const std::string &fileName) {
    // // because Opengl and stb read image in different direction
    stbi_set_flip_vertically_on_load(true);
    unsigned char *bytes = stbi_load(fileName.c_str(), &mWidth, &mHeight, &mChannel, 0);
    if (!bytes) {
        SDL_Log("stb_image failed to load image %s", fileName.c_str());
        return false;
    }

    int format = mChannel == 4 ? GL_RGBA : GL_RGB;

    // Generate textures
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, format, mWidth, mHeight, 0, format, GL_UNSIGNED_BYTE, bytes);

    stbi_image_free(bytes);

    // Enable bilinear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    return true;
}

void Texture::CreateFromSurface(SDL_Surface *surface) {
    mWidth = surface->w;
    mHeight = surface->h;

    // Generate a GL texture
    glGenTextures(1, &mTextureID);
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, mWidth, mHeight, 0, GL_BGRA, GL_UNSIGNED_BYTE, surface->pixels);

    // Use linear filtering
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Texture::Unload() {
    glDeleteTextures(1, &mTextureID);
}

void Texture::SetActive() const {
    glBindTexture(GL_TEXTURE_2D, mTextureID);
}
