#include <string>

class Texture {
public:
    Texture() = default;
    ~Texture() = default;

    bool Load(const std::string &fileName);
    void Unload();

    // Convert from SDL surface to opengl texture
    void CreateFromSurface(struct SDL_Surface* surface);

    // Setter
    void SetActive() const;

    // Setter
    [[nodiscard]] int GetWidth() const { return mWidth; }
    [[nodiscard]] int GetHeight() const { return mHeight; }

private:
    // OpenGL ID of this texture
    unsigned int mTextureID = 0;
    // Width/height of the texture
    int mWidth = 0;
    int mHeight = 0;
    int mChannel = 0;
};
