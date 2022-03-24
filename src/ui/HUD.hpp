#pragma once

#include "UIScreen.hpp"
#include <vector>

class HUD : public UIScreen {
public:
    // (Lower draw order corresponds with further back)
    explicit HUD(class Game *game);

    ~HUD() = default;

    void Update(float deltaTime) override;
    void Draw(class Shader *shader) override;

    void AddTargetComponent(class TargetComponent *tc);
    void RemoveTargetComponent(class TargetComponent *tc);

protected:
    void UpdateCrosshair(float deltaTime);
    void UpdateRadar(float deltaTime);

    class Texture *mHealthBar;
    class Texture *mRadar;
    class Texture *mCrosshair;
    class Texture *mCrosshairEnemy;
    class Texture *mBlipTex;
    class Texture *mRadarArrow;

    // All the target components in the game
    std::vector<class TargetComponent *> mTargetComps;
    // 2D offsets of blips relative to radar
    std::vector<Vector2> mBlips;

    float mRadarRange = 2000;  // Adjust range of radar and radius
    float mRadarRadius = 92;
    bool mTargetEnemy = false;  // Whether the crosshair targets an enemy
};
