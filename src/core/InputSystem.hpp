#pragma once
#include <SDL_scancode.h>
#include <SDL_gamecontroller.h>
#include <SDL_mouse.h>
#include "../helper/Math.hpp"

// The different button states (See the picture)
enum ButtonState {
	ENone,
	EPressed,
	EReleased,
	EHeld
};

// Helper for keyboard input
class KeyboardState {
public:
	// Friend so InputSystem can easily update it
	friend class InputSystem;

	// Get just the boolean true/false value of key
	[[nodiscard]] bool GetKeyValue(SDL_Scancode keyCode) const;
	// Get a state based on current and previous frame
	[[nodiscard]] ButtonState GetKeyState(SDL_Scancode keyCode) const;

private:
	const Uint8* mCurrState;
	Uint8 mPrevState[SDL_NUM_SCANCODES];  // buffer to save state before update
};

// Helper for mouse input
class MouseState {
public:
	friend class InputSystem;

	// For buttons
	[[nodiscard]] bool GetButtonValue(int button) const;
	[[nodiscard]] ButtonState GetButtonState(int button) const;

	// For mouse specific
	[[nodiscard]] const Vector2& GetPosition() const { return mMousePos; }
	[[nodiscard]] const Vector2& GetScrollWheel() const { return mScrollWheel; }
	[[nodiscard]] bool IsRelative() const { return mIsRelative; }

private:
	// Store button data
	Uint32 mCurrButtons;
	Uint32 mPrevButtons;
	// Store current mouse position
	Vector2 mMousePos;
	// Motion of scroll wheel, two axis as most mouse can move in both direction
	Vector2 mScrollWheel;
	// Are we in relative mouse mode
	bool mIsRelative;
};

// Helper for controller input
class ControllerState {
public:
	friend class InputSystem;

	// For buttons
	[[nodiscard]] bool GetButtonValue(SDL_GameControllerButton button) const;
	[[nodiscard]] ButtonState GetButtonState(SDL_GameControllerButton button) const;

    // Controller specific
	[[nodiscard]] const Vector2& GetLeftStick() const { return mLeftStick; }
	[[nodiscard]] const Vector2& GetRightStick() const { return mRightStick; }
	[[nodiscard]] float GetLeftTrigger() const { return mLeftTrigger; }
	[[nodiscard]] float GetRightTrigger() const { return mRightTrigger; }
	[[nodiscard]] bool GetIsConnected() const { return mIsConnected; }

private:
	// Current/previous buttons
	Uint8 mCurrButtons[SDL_CONTROLLER_BUTTON_MAX];
	Uint8 mPrevButtons[SDL_CONTROLLER_BUTTON_MAX];
	// Left/right sticks
	Vector2 mLeftStick;
	Vector2 mRightStick;
	// Left/right trigger
	float mLeftTrigger;
	float mRightTrigger;
	// Is this controller connected?
	bool mIsConnected;
};

// Wrapper that contains current state of input
struct InputState {
	KeyboardState Keyboard;
	MouseState Mouse;
	ControllerState Controller;
};


class InputSystem {
public:
    // Like Game
	bool Initialize();
	void Shutdown();

	// Called right before SDL_PollEvents loop
	void PrepareForUpdate();
	// Called after SDL_PollEvents loop
	void Update();
	// Called to process an SDL event in input system (like mouse wheel)
	void ProcessEvent(union SDL_Event& event);

    // GET SET
	[[nodiscard]] const InputState& GetState() const { return mState; }
	void SetRelativeMouseMode(bool value);

private:
	static float Filter1D(int input);
	static Vector2 Filter2D(int inputX, int inputY);

	InputState mState{};
	SDL_GameController* mController = nullptr;
};
