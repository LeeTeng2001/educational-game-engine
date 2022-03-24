#include "PauseMenu.hpp"
#include "../Game.hpp"
#include "DialogBox.hpp"
#include <SDL.h>

PauseMenu::PauseMenu(Game *game) : UIScreen(game) {
    mGame->SetState(Game::EPaused);

    mGame->GetInputSystem()->SetRelativeMouseMode(false);  // use absolute pos!
    SetTitle("PauseTitle");

    // Buttons + callbacks
    AddButton("ResumeButton", [this]() {
        Close();
    });

    AddButton("QuitButton", [this]() {
        new DialogBox(mGame, "QuitText",
                      [this]() {
                          mGame->SetState(Game::EQuit);
                      });
    });
}

PauseMenu::~PauseMenu() {
    mGame->GetInputSystem()->SetRelativeMouseMode(true);
    mGame->SetState(Game::EGameplay);
}

void PauseMenu::HandleKeyPress(const InputState& key) {
    UIScreen::HandleKeyPress(key);

    if (key.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EPressed) {
        Close();  // close lead to game destroy pause menu, lead to game mode to EGameplay
    }
}
