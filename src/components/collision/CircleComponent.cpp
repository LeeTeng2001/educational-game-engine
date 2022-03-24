#include "CircleComponent.hpp"
#include "../../actors/Actor.hpp"

CircleComponent::CircleComponent(class Actor *owner) : Component(owner) {}

const Vector2 &CircleComponent::GetCenter() const {
    return mOwner->GetPosition();
}

float CircleComponent::GetRadius() const {
    return mOwner->GetScale() * mRadius;
}

bool CircleComponent::Intersect(const CircleComponent &a, const CircleComponent &b) {
    // Calculate distance squared for efficient computation
    Vector2 diff = a.GetCenter() - b.GetCenter();
    float distSq = diff.LengthSq();

    // Calculate sum of radii squared
    float radiiSq = a.GetRadius() + b.GetRadius();
    radiiSq *= radiiSq;

    return distSq <= radiiSq;
}
