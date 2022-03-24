#pragma once

#include "../Component.hpp"
#include "../../helper/Math.hpp"

class CircleComponent : public Component {
public:
    explicit CircleComponent(class Actor *owner);

    // Getter & Setter
    void SetRadius(float radius) { mRadius = radius; }
    [[nodiscard]] float GetRadius() const;
    [[nodiscard]] const Vector2& GetCenter() const;

    static bool Intersect(const CircleComponent& a, const CircleComponent& b);
private:
    float mRadius = 0;
};


