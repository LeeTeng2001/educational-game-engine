#include <glad/glad.h>
#include <algorithm>
#include <SDL_ttf.h>
#include <fstream>
#include <sstream>
#include <rapidjson/stream.h>
#include <rapidjson/document.h>
#include "Game.hpp"
#include "actors/Actor.hpp"
#include "actors/FPSActor.hpp"
#include "actors/FollowActor.hpp"
#include "actors/PlaneActor.hpp"
#include "actors/OrbitActor.hpp"
#include "actors/SplineActor.hpp"
#include "components/render/SpriteComponent.hpp"
#include "components/render/MeshComponent.hpp"
#include "components/control/AudioComponent.hpp"
#include "core/Shader.hpp"
#include "helper/VertexArray.hpp"
#include "helper/Texture.hpp"
#include "core/Renderer.hpp"
#include "core/InputSystem.hpp"
#include "core/PhysWorld.hpp"
#include "audio/AudioSystem.hpp"
#include "actors/TargetActor.hpp"
#include "ui/Font.hpp"
#include "ui/UIScreen.hpp"
#include "ui/PauseMenu.hpp"
#include "ui/HUD.hpp"


Game::Game() = default;
std::string Game::PROJECT_BASE = "/Users/lunafreya/Programming/CLionProjects/my-minimal-game-engine/src/";

bool Game::Initialize() {
    // Take in a bitwise-or of all subsystems to initialize
    int sdlResult = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_GAMECONTROLLER);
    if (sdlResult) {  // non-zero means error
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());  // like printf
        return false;
    }

    // Create the renderer, move most of the game renderer part to Renderer
    mRenderer = new Renderer(this);
    if (!mRenderer->Initialize(SCREEN_WIDTH, SCREEN_HEIGHT)) {
        SDL_Log("Failed to initialize renderer");
        delete mRenderer;
        mRenderer = nullptr;
        return false;
    }

    // Initialize FMOD audio
    mAudioSystem = new AudioSystem(this);
    if (!mAudioSystem->Initialize()) {
        SDL_Log("Failed to initialize audio system");
        mAudioSystem->Shutdown();
        delete mAudioSystem;
        mAudioSystem = nullptr;
        return false;
    }

    // Initialize input system
    mInputSystem = new InputSystem();
    if (!mInputSystem->Initialize()) {
        SDL_Log("Failed to initialize input system");
        return false;
    }

    // Create the physics world
    mPhysWorld = new PhysWorld(this);

    // Initialize font, similar to sdl image
    if (TTF_Init() != 0) {
        SDL_Log("Failed to initialize SDL_ttf");
        return false;
    }

    // Initialize all actors AFTER everything
    LoadData();

    // Initialize ticks
    mTicksCount = SDL_GetTicks();

    return true;
}

void Game::Shutdown() {
    // Cleanup
    UnloadData();
    TTF_Quit();
    delete mPhysWorld;
    if (mInputSystem) mInputSystem->Shutdown();
    if (mAudioSystem) mAudioSystem->Shutdown();
    if (mRenderer) mRenderer->Shutdown();
    SDL_Quit();
}

void Game::RunLoop() {
    while (mGameState != EQuit) {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput() {
    mInputSystem->PrepareForUpdate();

    // POLL SDL event
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                mGameState = EQuit;
                break;
            case SDL_MOUSEWHEEL:
                mInputSystem->ProcessEvent(event);  // like MOUSEWHEEL
                break;
        }
    }

    // POLL actual event
    mInputSystem->Update();
    const InputState& state = mInputSystem->GetState();

    // Handle keypress in gameplay or ui
    if (mGameState == EGameplay) {
        HandleGameKeyPress(state);
    }
    else if (!mUIStack.empty()) {
        mUIStack.back()->HandleKeyPress(state);
    }

    // Send state to all actor or ui
    if (mGameState == EGameplay) {
        mUpdatingActors = true;
        for (auto actor: mActors) {
            actor->ProcessInput(state);
        }
        mUpdatingActors = false;
    }
    else if (!mUIStack.empty()) {
        mUIStack.back()->ProcessInput(state);
    }
}

void Game::UpdateGame() {
    // Simple solution for frame limiting.
    // Wait until 16ms has elapsed since last frame
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16));

    // Get delta in second
    float deltaTime = static_cast<float>(SDL_GetTicks() - mTicksCount) / 1000.0f;
    mTicksCount = SDL_GetTicks();

    // Clamp maximum delta to prevent huge delta time (ex, when stepping through debugger)
    deltaTime = std::min(deltaTime, 0.05f);

    // Only update in gameplay mode
    if (mGameState == EGameplay) {
        // Update all existing actors
        mUpdatingActors = true;
        for (auto &actor: mActors)
            actor->Update(deltaTime);
        mUpdatingActors = false;

        // Add pending vector
        for (auto &pending: mPendingActors) {
            pending->ComputeWorldTransform();  // have their world transform calculated
            mActors.emplace_back(pending);
        }
        mPendingActors.clear();

        // Check dead vector and remove
        vector<Actor *> deadActors;
        for (auto &actor: mActors) {
            if (actor->GetState() == Actor::EDead)
                deadActors.emplace_back(actor);
        }

        for (auto actor: deadActors) {
            delete actor;
        }
    }

    // Update audio system
    mAudioSystem->Update(deltaTime);

    // Update UI screens
    for (auto ui: mUIStack) {
        if (ui->GetState() == UIScreen::EActive) {
            ui->Update(deltaTime);
        }
    }

    // Delete any UIScreens that are closed
    auto iter = mUIStack.begin();
    while (iter != mUIStack.end()) {
        if ((*iter)->GetState() == UIScreen::EClosing) {
            delete *iter;
            iter = mUIStack.erase(iter);
        } else {
            ++iter;
        }
    }
}

void Game::GenerateOutput() {
    mRenderer->Draw();
}

void Game::AddActor(class Actor *actor) {
    if (mUpdatingActors) {
        mPendingActors.emplace_back(actor);
    }
    else {
        mActors.emplace_back(actor);
    }
}

void Game::RemoveActor(class Actor *actor) {
    // Check pending and normal actor
    // Swap to end of vector and pop off (avoid erase copies)

    auto iter = std::find(mPendingActors.begin(), mPendingActors.end(), actor);
    if (iter != mPendingActors.end()) {
        std::iter_swap(iter, mPendingActors.end() - 1);
        mPendingActors.pop_back();
    }

    iter = std::find(mActors.begin(), mActors.end(), actor);
    if (iter != mActors.end()) {
        std::iter_swap(iter, mActors.end() - 1);
        mActors.pop_back();
    }
}

void Game::AddPlane(PlaneActor *plane) {
    mPlanes.emplace_back(plane);
}

void Game::RemovePlane(PlaneActor *plane) {
    auto iter = std::find(mPlanes.begin(), mPlanes.end(), plane);
    mPlanes.erase(iter);
}

void Game::LoadData() {
    // Default UI Language
    LoadText("Assets/English.gptext");
    // LoadText("Assets/Russian.gptext");

    // Create actors
    auto* a = new Actor(this);
    a->SetPosition(Vector3(200.0f, 75.0f, 0.0f));
    a->SetScale(100.0f);

    Quaternion q(Vector3::UnitY, -Math::PiOver2);
    q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::Pi + Math::Pi / 4.0f));
    a->SetRotation(q);
    auto* mc = new MeshComponent(a);
    mc->SetMesh(mRenderer->GetMesh("Assets/Cube.gpmesh"));

    a = new Actor(this);
    a->SetPosition(Vector3(200.0f, -75.0f, 0.0f));
    a->SetScale(3.0f);
    mc = new MeshComponent(a);
    mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));

    // Setup floor
    const float start = -1250.0f;
    const float size = 250.0f;
    for (int i = 0; i < 10; i++) {
        for (int j = 0; j < 10; j++) {
            a = new PlaneActor(this);
            a->SetPosition(Vector3(start + i * size, start + j * size, -100.0f));
        }
    }

    // Left/right walls
    q = Quaternion(Vector3::UnitX, Math::PiOver2);
    for (int i = 0; i < 10; i++) {
        a = new PlaneActor(this);
        a->SetPosition(Vector3(start + i * size, start - size, 0.0f));
        a->SetRotation(q);

        a = new PlaneActor(this);
        a->SetPosition(Vector3(start + i * size, -start + size, 0.0f));
        a->SetRotation(q);
    }

    q = Quaternion::Concatenate(q, Quaternion(Vector3::UnitZ, Math::PiOver2));
    // Forward/back walls
    for (int i = 0; i < 10; i++) {
        a = new PlaneActor(this);
        a->SetPosition(Vector3(start - size, start + i * size, 0.0f));
        a->SetRotation(q);

        a = new PlaneActor(this);
        a->SetPosition(Vector3(-start + size, start + i * size, 0.0f));
        a->SetRotation(q);
    }

    // Setup lights
    mRenderer->SetAmbientLight(Vector3(0.2f, 0.2f, 0.2f));
    DirectionalLight& dir = mRenderer->GetDirectionalLight();
    dir.mDirection = Vector3(0.0f, -0.707f, -0.707f);
    dir.mDiffuseColor = Vector3(0.78f, 0.88f, 1.0f);
    dir.mSpecColor = Vector3(0.8f, 0.8f, 0.8f);

    // UI elements
    mHUD = new HUD(this);

    // Start music
    mMusicEvent = mAudioSystem->PlayEvent("event:/Music");

    // Create spheres with audio components playing different sounds
    a = new Actor(this);
    a->SetPosition(Vector3(500.0f, -75.0f, 0.0f));
    a->SetScale(1.0f);
    mc = new MeshComponent(a);
    mc->SetMesh(mRenderer->GetMesh("Assets/Sphere.gpmesh"));
    auto* ac = new AudioComponent(a);
    ac->PlayEvent("event:/FireLoop");

    // Camera actor ------------------------------------------------
    mFPSActor = new FPSActor(this);
    // mFollowActor = new FollowActor(this);
    // mOrbitActor = new OrbitActor(this);
    // mSplineActor = new SplineActor(this);

    // Create target actors
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, 0.0f, 100.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, 0.0f, 400.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, -500.0f, 200.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(1450.0f, 500.0f, 200.0f));
    a = new TargetActor(this);
    a->SetPosition(Vector3(0.0f, -1450.0f, 200.0f));
    a->SetRotation(Quaternion(Vector3::UnitZ, Math::PiOver2));
    a = new TargetActor(this);
    a->SetPosition(Vector3(0.0f, 1450.0f, 200.0f));
    a->SetRotation(Quaternion(Vector3::UnitZ, -Math::PiOver2));

}

void Game::UnloadData() {
    // Because ~Actor calls RemoveActor, have to use a different style loop
    while (!mActors.empty()) {
        delete mActors.back();
    }

    // Remember to unload renderer data
    if (mRenderer) {
        mRenderer->UnloadData();
    }
}


Font *Game::GetFont(const std::string &fileName) {
    auto iter = mFonts.find(fileName);

    // similar loading mechanism as texture
    if (iter != mFonts.end()) {
        return iter->second;
    } else {
        Font *font = new Font(this);
        if (font->Load(fileName)) {
            mFonts.emplace(fileName, font);
        } else {
            font->Unload();
            delete font;
            font = nullptr;
        }

        return font;
    }
}

void Game::LoadText(const std::string &fileName) {
    // Clear the existing map, if already loaded
    mText.clear();

    // Try to open the file
    std::ifstream file(PROJECT_BASE + fileName);
    if (!file.is_open()) {
        SDL_Log("Text file %s not found", fileName.c_str());
        return;
    }

    // Read the entire file to a string stream
    std::stringstream fileStream;
    fileStream << file.rdbuf();
    std::string contents = fileStream.str();

    // Open this file in rapidJSON
    rapidjson::StringStream jsonStr(contents.c_str());
    rapidjson::Document doc;
    doc.ParseStream(jsonStr);
    if (!doc.IsObject()) {
        SDL_Log("Text file %s is not valid JSON", fileName.c_str());
        return;
    }

    // Parse the text map
    const rapidjson::Value &actions = doc["TextMap"];
    for (rapidjson::Value::ConstMemberIterator itr = actions.MemberBegin();
         itr != actions.MemberEnd(); ++itr) {
        if (itr->name.IsString() && itr->value.IsString()) {
            mText.emplace(itr->name.GetString(), itr->value.GetString());
        }
    }
}

const std::string &Game::GetText(const std::string &key) {
    static std::string errorMsg("**KEY NOT FOUND**");
    // Find this text in the map, if it exists
    auto iter = mText.find(key);
    if (iter != mText.end()) {
        return iter->second;
    } else {
        SDL_Log("Key not found for %s", key.c_str());
        return errorMsg;
    }
}


void Game::HandleGameKeyPress(const InputState& key) {
    if (key.Keyboard.GetKeyState(SDL_SCANCODE_MINUS) == EPressed) {
        // Reduce master volume
        float volume = mAudioSystem->GetBusVolume("bus:/");
        volume = fmax(0.0f, volume - 0.1f);
        mAudioSystem->SetBusVolume("bus:/", volume);
    }
    else if (key.Keyboard.GetKeyState(SDL_SCANCODE_EQUALS) == EPressed) {
        // Increase master volume
        float volume = mAudioSystem->GetBusVolume("bus:/");
        volume = fmin(1.0f, volume + 0.1f);
        mAudioSystem->SetBusVolume("bus:/", volume);
    }
    else if (key.Keyboard.GetKeyState(SDL_SCANCODE_E) == EPressed) {
        // Play explosion
        mAudioSystem->PlayEvent("event:/Explosion2D");
    }
    else if (key.Keyboard.GetKeyState(SDL_SCANCODE_M) == EPressed) {
        // Toggle music pause state
        mMusicEvent.SetPaused(!mMusicEvent.GetPaused());
    }
    else if (key.Keyboard.GetKeyState(SDL_SCANCODE_R) == EPressed) {
        // Stop or start reverb snapshot
        if (!mReverbSnap.IsValid()) {
            mReverbSnap = mAudioSystem->PlayEvent("snapshot:/WithReverb");
        } else {
            mReverbSnap.Stop();
        }
    }
    else if (key.Mouse.GetButtonState(SDL_BUTTON_LEFT) == EPressed) {
        mFPSActor->Shoot();
    } // game play
    else if (key.Keyboard.GetKeyState(SDL_SCANCODE_ESCAPE) == EPressed) {
        new PauseMenu(this);
    }
}