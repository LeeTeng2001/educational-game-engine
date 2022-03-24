#pragma once

#include "MoveComponent.hpp"

class InputComponent : public  MoveComponent{
public:
    // Lower update order updating first
    explicit InputComponent(class Actor* owner);

    void ProcessInput(const struct InputState& keyState) override;

    // Getters/setters for private variables
    [[nodiscard]] float GetMaxForward() const { return mMaxForwardSpeed; }
    [[nodiscard]] float GetMaxAngular() const { return mMaxAngularSpeed; }
    [[nodiscard]] int GetForwardKey() const { return mForwardKey; }
    [[nodiscard]] int GetBackKey() const { return mBackKey; }
    [[nodiscard]] int GetClockwiseKey() const { return mClockwiseKey; }
    [[nodiscard]] int GetCounterClockwiseKey() const { return mCounterClockwiseKey; }

    void SetMaxForwardSpeed(float speed) { mMaxForwardSpeed = speed; }
    void SetMaxAngularSpeed(float speed) { mMaxAngularSpeed = speed; }
    void SetForwardKey(SDL_Scancode key) { mForwardKey = key; }
    void SetBackKey(SDL_Scancode key) { mBackKey = key; }
    void SetClockwiseKey(SDL_Scancode key) { mClockwiseKey = key; }
    void SetCounterClockwiseKey(SDL_Scancode key) { mCounterClockwiseKey = key; }

private:
    // The maximum forward/angular speeds
    float mMaxForwardSpeed = 0;
    float mMaxAngularSpeed = 0;
    // Keys for forward/back movement
    SDL_Scancode mForwardKey;
    SDL_Scancode mBackKey;
    // Keys for angular movement
    SDL_Scancode mClockwiseKey;
    SDL_Scancode mCounterClockwiseKey;
};



