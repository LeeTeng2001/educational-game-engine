#pragma once

#include "../Component.hpp"

class SpriteComponent : public Component {
public:
    explicit SpriteComponent(class Actor* owner, int updateOrder = 100);
    ~SpriteComponent() override;

    virtual void Draw(class Shader *renderer);
    virtual void SetTexture(class Texture *texture);

    // Getter
    [[nodiscard]] int GetDrawOrder() const { return mDrawOrder; }
    [[nodiscard]] int GetTexHeight() const { return mTexHeight; }
    [[nodiscard]] int GetTexWidth() const { return mTexWidth; }
    [[nodiscard]] bool GetVisible() const { return mVisible; }

    // Setter
    void SetVisible(bool visible) { mVisible = visible; }

protected:
    // Texture to draw
    class Texture* mTexture = nullptr;

    // Draw order by painting algorithm, dimension of texture
    int mDrawOrder;
    int mTexWidth = 0;
    int mTexHeight = 0;
    bool mVisible = true;
};



