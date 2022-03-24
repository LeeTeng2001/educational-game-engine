#pragma once

#include <unordered_map>
#include "SoundEvent.hpp"

// Forward declarations to avoid including FMOD header
namespace FMOD {
    class System;
    namespace Studio {
        class Bank;
        class EventDescription;
        class EventInstance;
        class System;
        class Bus;
    };
};

class AudioSystem {
public:
    explicit AudioSystem(class Game* game);
    ~AudioSystem() = default;

    bool Initialize();
    void Shutdown();
    void Update(float deltaTime);

    // Load/unload banks
    void LoadBank(const std::string& name);
    void UnloadBank(const std::string& name);
    void UnloadAllBanks();

    // Play event, increment id and return sound wrapper
    SoundEvent PlayEvent(const std::string& name);

    // For positional audio
    void SetListener(const Matrix4& viewMatrix);

    // Control buses
    [[nodiscard]] float GetBusVolume(const std::string& name) const;
    [[nodiscard]] bool GetBusPaused(const std::string& name) const;
    void SetBusVolume(const std::string& name, float volume);
    void SetBusPaused(const std::string& name, bool pause);

protected:
    // Prevent everybody from having access to event instance, but SoundEvent needed this
    FMOD::Studio::EventInstance* GetEventInstance(unsigned int id);
    friend class SoundEvent;

private:
    // Map of loaded banks
    std::unordered_map<std::string, FMOD::Studio::Bank*> mBanks;
    // Map of event name to EventDescription
    std::unordered_map<std::string, FMOD::Studio::EventDescription*> mEvents;
    // Map of event id to EventInstance
    std::unordered_map<unsigned int, FMOD::Studio::EventInstance*> mEventInstances;
    // Map of buses
    std::unordered_map<std::string, FMOD::Studio::Bus*> mBuses;

    // Tracks the next ID to use for event instances
    static unsigned int sNextID;

private:
    // Core
    class Game* mGame;
    // FMOD studio system
    FMOD::Studio::System* mSystem = nullptr;
    // FMOD Low-level system (in case needed)
    FMOD::System* mLowLevelSystem = nullptr;
};



