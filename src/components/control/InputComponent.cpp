#include "InputComponent.hpp"
#include "../../core/InputSystem.hpp"

InputComponent::InputComponent(class Actor *owner) : MoveComponent(owner){}

void InputComponent::ProcessInput(const InputState& state) {
    // Calculate forward speed for MoveComponent
    float forwardSpeed = 0.0f;

    if (state.Keyboard.GetKeyValue(mForwardKey)) {
        forwardSpeed += mMaxForwardSpeed;
    }
    if (state.Keyboard.GetKeyValue(mBackKey)) {
        forwardSpeed -= mMaxForwardSpeed;
    }
    SetForwardSpeed(forwardSpeed);

    // Calculate angular speed for MoveComponent
    float angularSpeed = 0.0f;
    if (state.Keyboard.GetKeyValue(mClockwiseKey)) {
        angularSpeed += mMaxAngularSpeed;
    }
    if (state.Keyboard.GetKeyValue(mCounterClockwiseKey)) {
        angularSpeed -= mMaxAngularSpeed;
    }
    SetAngularSpeed(angularSpeed);
}
