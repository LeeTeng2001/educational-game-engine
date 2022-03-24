#pragma once

#include <SDL.h>
#include <vector>
#include <unordered_map>
#include <string>
#include "audio/SoundEvent.hpp"
#include "core/InputSystem.hpp"

using std::vector;

class Game {
public:
    Game();

    // Core functions
    bool Initialize();
    void RunLoop();
    void Shutdown();

    // Create or delete actors
    void AddActor(class Actor* actor);
    void RemoveActor(class Actor* actor);

    // Core Getter
    class Renderer* GetRenderer() { return mRenderer; }
    class AudioSystem* GetAudioSystem() { return mAudioSystem; }
    class InputSystem* GetInputSystem() { return mInputSystem; }
    class PhysWorld* GetPhysWorld() { return mPhysWorld; }

    enum GameState {
        EGameplay,
        EPaused,
        EQuit
    };

    // Game state to route input to gameplay or ui
    [[nodiscard]] GameState GetState() const { return mGameState; }
    void SetState(GameState state) { mGameState = state; }

    // ui functions
    class Font* GetFont(const std::string& fileName);
    void LoadText(const std::string& fileName);
    const std::string& GetText(const std::string& key);
    class HUD* GetHUD() { return mHUD; }

    // Manage UI stack
    const std::vector<class UIScreen*>& GetUIStack() { return mUIStack; }
    void PushUI(class UIScreen* screen) { mUIStack.emplace_back(screen); };

    // Put game specific functions here
    void HandleGameKeyPress(const InputState &key);
    class FPSActor* GetPlayer() { return mFPSActor; }

    // For collision testing against player actor
    void AddPlane(class PlaneActor* plane);
    void RemovePlane(class PlaneActor* plane);
    std::vector<class PlaneActor*>& GetPlanes() { return mPlanes; }

private:
    // Helper functions for the game loop
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    // Responsible to create/delete all the actors in the game world
    void LoadData();
    void UnloadData();

    // All the actors in the game, you need mPending when you're iterating over mActors and create a new one
    vector<class Actor*> mActors;
    vector<class Actor*> mPendingActors;
    bool mUpdatingActors = false;  // Track if we're updating actors right now

    // ui
    std::unordered_map<std::string, class Font*> mFonts;  // filename -> ptr
    std::unordered_map<std::string, std::string> mText;  // Map for text localization
    std::vector<class UIScreen*> mUIStack;  // not actual stack because we need to iterate

    // Game-specific down here -----------------------------------
    class FPSActor* mFPSActor = nullptr;
    class FollowActor* mFollowActor = nullptr;
    class OrbitActor* mOrbitActor = nullptr;
    class SplineActor* mSplineActor = nullptr;
    class SpriteComponent* mCrosshair = nullptr;
    class HUD* mHUD = nullptr; // HUD

    std::vector<class PlaneActor*> mPlanes;

    SoundEvent mMusicEvent;
    SoundEvent mReverbSnap;

private:
    // Core game variables
    class AudioSystem* mAudioSystem = nullptr;  // 3D FMOD audio
    class InputSystem* mInputSystem = nullptr;  // Input system
    class PhysWorld* mPhysWorld = nullptr;
    class Renderer* mRenderer = nullptr;

    GameState mGameState = EGameplay;  // substitute naive isRunning to mGameState
    Uint32 mTicksCount = 0;

public:
    // Useful constant
    constexpr static int SCREEN_WIDTH = 1024;
    constexpr static int SCREEN_HEIGHT = 768;
    static std::string PROJECT_BASE;
};



