#pragma once

#include <SDL.h>

class Component {
public:
    // (the lower the update order, the earlier the component updates)
    explicit Component(class Actor* owner, int updateOrder = 100);
    virtual ~Component();

    // Update this component by delta time
    virtual void Update(float deltaTime);

    // Process input for components
    virtual void ProcessInput(const struct InputState& keyState);

    // Notify when parent's being updated
    virtual void OnUpdateWorldTransform() {};

    // Getter
    [[nodiscard]] class Actor* GetOwner() { return mOwner; }
    [[nodiscard]] int GetUpdateOrder() const { return mUpdateOrder; }

protected:
    // Owning actor
    class Actor* mOwner;
    // Update order of component
    int mUpdateOrder;
};



