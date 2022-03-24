#include "UIScreen.hpp"

#include <utility>
#include "../helper/Texture.hpp"
#include "../core/Shader.hpp"
#include "../Game.hpp"
#include "../core/Renderer.hpp"
#include "Font.hpp"

UIScreen::UIScreen(Game *game) : mGame(game) {
    // Add to UI Stack
    mGame->PushUI(this);

    // Default
    mFont = mGame->GetFont("Assets/Carlito-Regular.ttf");
    mButtonOn = mGame->GetRenderer()->GetTexture("Assets/ButtonYellow.png");
    mButtonOff = mGame->GetRenderer()->GetTexture("Assets/ButtonBlue.png");
}

UIScreen::~UIScreen() {
    if (mTitle) {
        mTitle->Unload();
        delete mTitle;
    }

    for (auto b: mButtons) {
        delete b;
    }

    mButtons.clear();
}

void UIScreen::Update(float deltaTime) {

}

void UIScreen::Draw(Shader *shader) {
    // Draw background (if exists)
    if (mBackground) {
        DrawTexture(shader, mBackground, mBGPos);
    }

    // Draw title (if exists)
    if (mTitle) {
        DrawTexture(shader, mTitle, mTitlePos);
    }

    // Draw buttons
    for (auto b: mButtons) {
        // Draw background of button
        Texture *tex = b->GetHighlighted() ? mButtonOn : mButtonOff;
        DrawTexture(shader, tex, b->GetPosition());
        // Draw text of button
        DrawTexture(shader, b->GetNameTex(), b->GetPosition());
    }

    // Override in subclasses to draw any textures
}

void UIScreen::ProcessInput(const InputState& key) {
    // Do we have buttons?
    if (!mButtons.empty()) {
        auto mousePos = key.Mouse.GetPosition();  // absolute pos,
        // relative from 0, 0 center
        mousePos.x -= mGame->GetRenderer()->GetScreenWidth() * 0.5f;
        mousePos.y = mGame->GetRenderer()->GetScreenHeight() * 0.5f - mousePos.y;

        // Highlight any buttons if it's in bound
        for (auto b: mButtons) {
            b->SetHighlighted(b->ContainsPoint(mousePos));
        }
    }
}

void UIScreen::HandleKeyPress(const InputState &key) {
    if (!mButtons.empty() && key.Mouse.GetButtonState(SDL_BUTTON_LEFT) == EPressed) {
        for (auto b: mButtons) {
            if (b->GetHighlighted()) {
                b->OnClick();
                break;
            }
        }
    }
}

void UIScreen::Close() {
    mState = EClosing;
}

void UIScreen::SetTitle(const std::string &text, const Vector3 &color, int pointSize) {
    // Clear out previous title texture if it exists
    if (mTitle) {
        mTitle->Unload();
        delete mTitle;
        mTitle = nullptr;
    }
    mTitle = mFont->RenderText(text, color, pointSize);
}

void UIScreen::AddButton(const std::string &name, std::function<void()> onClick) {
    Vector2 dims(static_cast<float>(mButtonOn->GetWidth()),
                 static_cast<float>(mButtonOn->GetHeight()));
    auto *b = new Button(name, mFont, std::move(onClick), mNextButtonPos, dims);
    mButtons.emplace_back(b);

    // Update position of next button
    // Move down by height of button plus padding
    mNextButtonPos.y -= mButtonOff->GetHeight() + 20.0f;
}

void UIScreen::DrawTexture(class Shader *shader, class Texture *texture,
                           const Vector2 &offset, float scale) {
    // Scale the quad by the width/height of texture
    Matrix4 scaleMat = Matrix4::CreateScale(
            static_cast<float>(texture->GetWidth()) * scale,
            static_cast<float>(texture->GetHeight()) * scale,
            1.0f);
    // Translate to position on screen
    Matrix4 transMat = Matrix4::CreateTranslation(
            Vector3(offset.x, offset.y, 0.0f));
    // Set world transform
    Matrix4 world = scaleMat * transMat;
    shader->SetMatrixUniform("uWorldTransform", world);
    // Set current texture
    texture->SetActive();
    // Draw quad
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
}

void UIScreen::SetRelativeMouseMode(bool relative) {
    mGame->GetInputSystem()->SetRelativeMouseMode(relative);
}

Button::Button(const std::string &name, Font *font, std::function<void()> onClick,
               const Vector2 &pos, const Vector2 &dims)
        : mOnClick(std::move(onClick)), mFont(font), mPosition(pos), mDimensions(dims) {
    SetName(name);
}

Button::~Button() {
    if (mNameTex) {
        mNameTex->Unload();
        delete mNameTex;
    }
}

void Button::SetName(const std::string &name) {
    mName = name;

    if (mNameTex) {
        mNameTex->Unload();
        delete mNameTex;
        mNameTex = nullptr;
    }

    mNameTex = mFont->RenderText(mName);
}

bool Button::ContainsPoint(const Vector2 &pt) const {
    bool no = pt.x<(mPosition.x - mDimensions.x / 2.0f) || pt.x>(mPosition.x + mDimensions.x / 2.0f) ||
              pt.y<(mPosition.y - mDimensions.y / 2.0f) || pt.y>(mPosition.y + mDimensions.y / 2.0f);
    return !no;
}

void Button::OnClick() {
    // Call attached handler, if it exists
    if (mOnClick) {
        mOnClick();
    }
}
