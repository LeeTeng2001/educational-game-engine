#pragma once

#include "../Component.hpp"
#include "../../helper/Collision.hpp"

class BoxComponent : public Component {
public:
    explicit BoxComponent(class Actor *owner, int updateOrder = 100);
    ~BoxComponent();

    void OnUpdateWorldTransform() override;

    // Getter
    [[nodiscard]] const AABB &GetWorldBox() const { return mWorldBox; }

    // Setter
    void SetObjectBox(const AABB &model) { mObjectBox = model; }
    void SetShouldRotate(bool value) { mShouldRotate = value; }

private:
    AABB mObjectBox{Vector3::Zero, Vector3::Zero};  // object space
    AABB mWorldBox{Vector3::Zero, Vector3::Zero};  // word space, keep changing
    bool mShouldRotate = true;  // rotate based on the world rotation
};
