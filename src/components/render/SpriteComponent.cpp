#include "../../actors/Actor.hpp"
#include "../../Game.hpp"
#include "../../core/Shader.hpp"
#include "SpriteComponent.hpp"
#include "../../helper/Texture.hpp"
#include "../../core/Renderer.hpp"

SpriteComponent::SpriteComponent(Actor *owner, int drawOrder)
        : Component(owner), mDrawOrder(drawOrder) {
    mOwner->GetGame()->GetRenderer()->AddSprite(this);
}

SpriteComponent::~SpriteComponent() {
    mOwner->GetGame()->GetRenderer()->RemoveSprite(this);
}

void SpriteComponent::Draw(Shader *shader) {
    if (mTexture) {
        // Scale the quad by the width/height of texture
        Matrix4 scaleMat = Matrix4::CreateScale(
                static_cast<float>(mTexWidth),
                static_cast<float>(mTexHeight),
                1.0f);

        Matrix4 world = scaleMat * mOwner->GetWorldTransform();

        // Since all sprites use the same shader/vertices,
        // the game first sets them active before any sprite draws

        // Set world transform
        shader->SetMatrixUniform("uWorldTransform", world);

        // Set current texture
        mTexture->SetActive();
        // Draw quad
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);
    }
}

void SpriteComponent::SetTexture(Texture *texture) {
    mTexture = texture;
    // Set width/height using query texture
    mTexWidth = texture->GetWidth();
    mTexHeight = texture->GetHeight();
}