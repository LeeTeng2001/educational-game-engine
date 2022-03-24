#pragma once

#include "Actor.hpp"
#include "../audio/SoundEvent.hpp"

class FPSActor : public Actor {
public:
    explicit FPSActor(class Game* game);

    void UpdateActor(float deltaTime) override;
    void ActorInput(const struct InputState &keys) override;

    // setter
    void SetFootstepSurface(float value);
    void SetVisible(bool visible);

    // Game
    void Shoot();
    void FixCollisions();

private:
    class MoveComponent* mMoveComp = nullptr;
    class AudioComponent* mAudioComp = nullptr;
    class MeshComponent* mMeshComp = nullptr;
    class FPSCamera* mCameraComp = nullptr;
    class BoxComponent* mBoxComp = nullptr;
    class Actor* mFPSModel = nullptr;
    SoundEvent mFootstep;
    float mLastFootstep;
};



