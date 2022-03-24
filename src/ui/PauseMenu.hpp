#pragma once

#include "UIScreen.hpp"

class PauseMenu : public UIScreen {
public:
    explicit PauseMenu(class Game *game);
    ~PauseMenu() override;

    void HandleKeyPress(const InputState& key) override;
};
