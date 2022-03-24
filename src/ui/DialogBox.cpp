#include "DialogBox.hpp"
#include "../Game.hpp"
#include "../core/Renderer.hpp"

DialogBox::DialogBox(Game *game, const std::string &text, const std::function<void()>& onOK) : UIScreen(game) {

    // Adjust positions for dialog box
    mBGPos = Vector2(0.0f, 0.0f);
    mTitlePos = Vector2(0.0f, 100.0f);
    mNextButtonPos = Vector2(0.0f, 0.0f);

    mBackground = mGame->GetRenderer()->GetTexture("Assets/DialogBG.png");
    SetTitle(text, Vector3::Zero, 30);

    AddButton("OKButton", [onOK]() {
        onOK();
    });
    AddButton("CancelButton", [this]() {
        Close();
    });
}

