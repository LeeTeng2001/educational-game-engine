#include "Font.hpp"
#include "../helper/Texture.hpp"
#include <vector>
#include "../Game.hpp"

Font::Font(class Game *game) : mGame(game) { }

bool Font::Load(const std::string &fileName) {
    // We support these font sizes
    std::vector<int> fontSizes = {
            8, 9,
            10, 11, 12, 14, 16, 18,
            20, 22, 24, 26, 28,
            30, 32, 34, 36, 38,
            40, 42, 44, 46, 48,
            52, 56,
            60, 64, 68,
            72
    };

    std::string filePath = Game::PROJECT_BASE + fileName;

    for (auto size: fontSizes) {
        TTF_Font *font = TTF_OpenFont(filePath.c_str(), size);

        if (font == nullptr) {
            SDL_Log("Failed to load font %s in size %d", filePath.c_str(), size);
            return false;
        }
        mFontData.emplace(size, font);
    }
    return true;
}

void Font::Unload() {
    for (auto &font: mFontData) {
        TTF_CloseFont(font.second);
    }
}

namespace {
    // Add padding one side, return ceiling
    int ceil_power_two(int val, int &offSetOneSide) {
        int ceil = 1;
        while((ceil *= 2) < val);
        // SDL_Log("%d to %d", val, ans);
        offSetOneSide = (ceil - val) / 2;
        return ceil;
    }

    void flip_surface(SDL_Surface *surface) {
        SDL_LockSurface(surface);

        int pitch = surface->pitch; // row size
        char *temp = new char[pitch]; // intermediate buffer
        char *pixels = (char *) surface->pixels;

        for (int i = 0; i < surface->h / 2; ++i) {
            // get pointers to the two rows to swap
            char *row1 = pixels + i * pitch;
            char *row2 = pixels + (surface->h - i - 1) * pitch;

            // swap rows
            memcpy(temp, row1, pitch);
            memcpy(row1, row2, pitch);
            memcpy(row2, temp, pitch);
        }

        delete[] temp;
        SDL_UnlockSurface(surface);
    }
}

Texture *Font::RenderText(const std::string &textKey,
                          const Vector3 &color /*= Color::White*/,
                          int pointSize /*= 24*/) {
    Texture *texture = nullptr;

    // Convert our vector to SDL_Color
    SDL_Color sdlColor;
    sdlColor.r = static_cast<Uint8>(color.x * 255);
    sdlColor.g = static_cast<Uint8>(color.y * 255);
    sdlColor.b = static_cast<Uint8>(color.z * 255);
    sdlColor.a = 255;

    // Find the font data for this point size
    auto iter = mFontData.find(pointSize);

    if (iter != mFontData.end()) {
        TTF_Font *font = iter->second;
        const std::string &actualText = mGame->GetText(textKey);  // Translation

        // Draw this to a surface (blended for alpha)
        SDL_Surface *surf = TTF_RenderUTF8_Blended(font, actualText.c_str(), sdlColor);

        if (surf != nullptr) {
            // Convert surface to Opengl compatible (power 2)
            SDL_Rect offset;
            int w = ceil_power_two(surf->w, offset.x);
            int h = ceil_power_two(surf->h, offset.y);

            // Create a surf to the correct size in RGB format, and copy the old image + flip (to opengl buffer order)
            SDL_Surface *surCorrection = SDL_CreateRGBSurface(0, w, h, 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000);
            SDL_BlitSurface(surf, nullptr, surCorrection, &offset);
            flip_surface(surCorrection);
            // SDL_Log("Original (%d, %d), new (%d, %d), correction (%d, %d)", surf->w, surf->h, surCorrection->w, surCorrection->h, offset.x, offset.y);

            // Convert from surface to texture
            texture = new Texture();
            texture->CreateFromSurface(surCorrection);

            SDL_FreeSurface(surf);  // release resource
            SDL_FreeSurface(surCorrection);
        }
        else {
            SDL_Log("cannot render ttf to sdl surface");
        }
    }
    else {
        SDL_Log("Point size %d is unsupported", pointSize);
    }

    return texture;
}
