#pragma once

#include "../control/MoveComponent.hpp"

class BallMove : public MoveComponent {
public:
    explicit BallMove(class Actor *owner);

    void Update(float deltaTime) override;

    void SetPlayer(Actor *player) { mPlayer = player; }

protected:
    class Actor *mPlayer = nullptr;
};