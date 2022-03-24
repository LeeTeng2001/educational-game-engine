#pragma once
#include "../Component.hpp"

class MoveComponent : public Component {
public:
    // Lower update order to update first
    explicit MoveComponent(class Actor* owner, int updateOrder = 10);

    void Update(float deltaTime) override;

    [[nodiscard]] float GetAngularSpeed() const { return mAngularSpeed; }
    [[nodiscard]] float GetForwardSpeed() const { return mForwardSpeed; }
    [[nodiscard]] float GetStrafeSpeed() const { return mStrafeSpeed; }
    void SetAngularSpeed(float speed) { mAngularSpeed = speed; }
    void SetForwardSpeed(float speed) { mForwardSpeed = speed; }
    void SetStrafeSpeed(float speed) { mStrafeSpeed = speed; }

private:
    // Controls rotation (radians/second)
    float mAngularSpeed = 0;
    // Controls forward movement (units/second)
    float mForwardSpeed = 0;
    // Controls side movement (units/second)
    float mStrafeSpeed = 0;
};



