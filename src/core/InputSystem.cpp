#include "InputSystem.hpp"
#include <SDL.h>
#include <algorithm>
#include <cstring>

bool KeyboardState::GetKeyValue(SDL_Scancode keyCode) const {
	return mCurrState[keyCode] == 1;
}

ButtonState KeyboardState::GetKeyState(SDL_Scancode keyCode) const {
    if (mPrevState[keyCode] == 0) {
        // Prev is up
        return mCurrState[keyCode] == 0 ? ENone : EPressed;
    } else {
        // Prev is down
        return mCurrState[keyCode] == 0 ? EReleased : EHeld;
    }
}

bool MouseState::GetButtonValue(int button) const {
    return (SDL_BUTTON(button) & mCurrButtons);
}

ButtonState MouseState::GetButtonState(int button) const {
    int mask = SDL_BUTTON(button);

    if ((mask & mPrevButtons) == 0) {
        return (mask & mCurrButtons) == 0 ? ENone : EPressed;
    } else {
        return (mask & mCurrButtons) == 0 ? EReleased : EHeld;
    }
}

bool ControllerState::GetButtonValue(SDL_GameControllerButton button) const {
    return mCurrButtons[button] == 1;
}

ButtonState ControllerState::GetButtonState(SDL_GameControllerButton button) const {
    if (mPrevButtons[button] == 0) {
        return mCurrButtons[button] == 0 ? ENone : EPressed;
    } else {
        return mCurrButtons[button] == 0 ? EReleased : EHeld;
    }
}

bool InputSystem::Initialize() {
    // Keyboard
    // Assign current state pointer
    mState.Keyboard.mCurrState = SDL_GetKeyboardState(nullptr);
    // Clear previous state memory
    memset(mState.Keyboard.mPrevState, 0,SDL_NUM_SCANCODES);

    // Mouse (just set everything to 0)
    mState.Mouse.mCurrButtons = 0;
    mState.Mouse.mPrevButtons = 0;

    // Get the connected controller, if it exists (return non nullptr)
    mController = SDL_GameControllerOpen(0);
    // Initialize controller state
    mState.Controller.mIsConnected = (mController != nullptr);
    memset(mState.Controller.mCurrButtons, 0,SDL_CONTROLLER_BUTTON_MAX);
    memset(mState.Controller.mPrevButtons, 0,SDL_CONTROLLER_BUTTON_MAX);

    return true;
}

void InputSystem::Shutdown() {
    SDL_GameControllerClose(mController);
}

void InputSystem::PrepareForUpdate() {
    // Copy current state to previous (because SDL overwrite its original key buffer)
    // Keyboard
    memcpy(mState.Keyboard.mPrevState,mState.Keyboard.mCurrState,SDL_NUM_SCANCODES);

    // Mouse
    // mState.Mouse.mIsRelative = false;  // TODO: Bug reset?
    mState.Mouse.mPrevButtons = mState.Mouse.mCurrButtons;
    mState.Mouse.mScrollWheel = Vector2::Zero;  // only triggers on frames where the scroll wheel moves

    // Controller
    memcpy(mState.Controller.mPrevButtons,mState.Controller.mCurrButtons,SDL_CONTROLLER_BUTTON_MAX);
}

void InputSystem::Update() {
    // Mouse
    int x = 0, y = 0;
    if (mState.Mouse.mIsRelative) {
        mState.Mouse.mCurrButtons = SDL_GetRelativeMouseState(&x, &y);
    } else {
        mState.Mouse.mCurrButtons = SDL_GetMouseState(&x, &y);
    }

    mState.Mouse.mMousePos.x = static_cast<float>(x);
    mState.Mouse.mMousePos.y = static_cast<float>(y);

    // Controller --------------------------------------------------------
    // Buttons
    for (int i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++) {
        mState.Controller.mCurrButtons[i] =
                SDL_GameControllerGetButton(mController,
                                            SDL_GameControllerButton(i));
    }

    // Triggers
    mState.Controller.mLeftTrigger =
            Filter1D(SDL_GameControllerGetAxis(mController,
                                               SDL_CONTROLLER_AXIS_TRIGGERLEFT));
    mState.Controller.mRightTrigger =
            Filter1D(SDL_GameControllerGetAxis(mController,
                                               SDL_CONTROLLER_AXIS_TRIGGERRIGHT));

    // Sticks, negates y because SDL report y axis in down positive
    x = SDL_GameControllerGetAxis(mController,
                                  SDL_CONTROLLER_AXIS_LEFTX);
    y = -SDL_GameControllerGetAxis(mController,
                                   SDL_CONTROLLER_AXIS_LEFTY);
    mState.Controller.mLeftStick = Filter2D(x, y);

    x = SDL_GameControllerGetAxis(mController,
                                  SDL_CONTROLLER_AXIS_RIGHTX);
    y = -SDL_GameControllerGetAxis(mController,
                                   SDL_CONTROLLER_AXIS_RIGHTY);
    mState.Controller.mRightStick = Filter2D(x, y);
}

void InputSystem::ProcessEvent(SDL_Event &event) {
    switch (event.type) {
        case SDL_MOUSEWHEEL:
            mState.Mouse.mScrollWheel = Vector2(
                    static_cast<float>(event.wheel.x),
                    static_cast<float>(event.wheel.y));
            break;
        default:
            break;
    }
}

void InputSystem::SetRelativeMouseMode(bool value) {
    SDL_bool set = value ? SDL_TRUE : SDL_FALSE;
    SDL_SetRelativeMouseMode(set);
    if (value) SDL_GetRelativeMouseState(nullptr, nullptr);  // clear out for relative
    mState.Mouse.mIsRelative = value;
}

float InputSystem::Filter1D(int input) {
    // Could make this parameter user configurable
    // A value < dead zone is interpreted as 0%
    const int deadZone = 250;
    // A value > max value is interpreted as 100%
    const int maxValue = 30000;

    float retVal = 0.0f;

    // Take absolute value of input
    int absValue = std::abs(input);
    // Ignore input within dead zone
    if (absValue > deadZone) {
        // Compute fractional value between dead zone and max value
        retVal = static_cast<float>(absValue - deadZone) / (maxValue - deadZone);
        // Make sure sign matches original value
        retVal = input > 0 ? retVal : -1.0f * retVal;
        // Clamp between -1.0f and 1.0f
        retVal = std::clamp(retVal, -1.0f, 1.0f);
    }

    return retVal;
}

Vector2 InputSystem::Filter2D(int inputX, int inputY) {
    const float deadZone = 8000.0f;
    const float maxValue = 30000.0f;

    // Make into 2D vector
    Vector2 dir{};
    dir.x = static_cast<float>(inputX);
    dir.y = static_cast<float>(inputY);

    float length = dir.Length();

    // If length < deadZone, should be no input
    if (length < deadZone) {
        dir = Vector2::Zero;
    } else {
        // Calculate fractional value between
        // dead zone and max value circles
        float f = (length - deadZone) / (maxValue - deadZone);
        // Clamp f between 0.0f and 1.0f
        f = std::clamp(f, 0.0f, 1.0f);
        // Normalize the vector, and then scale it to the
        // fractional value
        dir *= f / length;
    }

    return dir;
}
