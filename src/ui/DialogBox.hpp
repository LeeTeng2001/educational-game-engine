#pragma once

#include "UIScreen.hpp"

class DialogBox : public UIScreen {
public:
    // (Lower draw order corresponds with further back)
    DialogBox(class Game *game, const std::string &text, const std::function<void()>& onOK);

    ~DialogBox() = default;
};
