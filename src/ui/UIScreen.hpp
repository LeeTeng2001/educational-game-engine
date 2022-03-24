#pragma once

#include "../helper/Math.hpp"
#include "../core/InputSystem.hpp"
#include <cstdint>
#include <string>
#include <functional>
#include <vector>

class Button {
public:
    Button(const std::string &name, class Font *font,
           std::function<void()> onClick,
           const Vector2 &pos, const Vector2 &dims);
    ~Button();

    // Getters
    class Texture *GetNameTex() { return mNameTex; }
    [[nodiscard]] const Vector2 &GetPosition() const { return mPosition; }
    [[nodiscard]] bool ContainsPoint(const Vector2 &pt) const; // Returns true if the point is within the button's bounds
    [[nodiscard]] bool GetHighlighted() const { return mHighlighted; }

    // Setter
    void SetName(const std::string &name);  // name of button
    void SetHighlighted(bool sel) { mHighlighted = sel; }

    // Called when button is clicked
    void OnClick();

private:
    std::function<void()> mOnClick;
    std::string mName;

    class Texture *mNameTex = nullptr;
    class Font *mFont;

    Vector2 mPosition;
    Vector2 mDimensions;
    bool mHighlighted = false;
};

class UIScreen {
public:
    explicit UIScreen(class Game *game);
    virtual ~UIScreen();

    // UIScreen subclasses can override these
    virtual void Update(float deltaTime);
    virtual void Draw(class Shader *shader);
    virtual void ProcessInput(const InputState& key);  // handle input during pause
    virtual void HandleKeyPress(const InputState& key);  // handle anytime

    // Tracks if the UI is active or closing
    enum UIState {
        EActive,
        EClosing
    };

    // Set state to closing
    void Close();

    // Getter
    [[nodiscard]] UIState GetState() const { return mState; }

    // Setter
    void SetTitle(const std::string &text,
                  const Vector3 &color = Color::White, int pointSize = 40);  // Change the title text

    // Helper function, add a button to this screen
    void AddButton(const std::string &name, std::function<void()> onClick);

protected:
    // Helper to draw a texture since it's not an actor
    void DrawTexture(class Shader *shader, class Texture *texture,
                     const Vector2 &offset = Vector2::Zero,
                     float scale = 1.0f);

    // Sets the mouse mode to relative or not
    void SetRelativeMouseMode(bool relative);

    class Game *mGame = nullptr;
    class Font *mFont = nullptr;
    class Texture *mTitle = nullptr;
    class Texture *mBackground = nullptr;

    class Texture *mButtonOn = nullptr;
    class Texture *mButtonOff = nullptr;

    // Configure positions
    Vector2 mTitlePos{0.0f, 300.0f};
    Vector2 mNextButtonPos{0.0f, 200.0f};
    Vector2 mBGPos{0.0f, 250.0f};

    // State
    UIState mState = EActive;
    std::vector<Button *> mButtons;  // Vertical list of buttons
};
