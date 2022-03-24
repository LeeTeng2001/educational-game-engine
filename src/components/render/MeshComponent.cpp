#include "MeshComponent.hpp"
#include "../../core/Shader.hpp"
#include "../../helper/Mesh.hpp"
#include "../../actors/Actor.hpp"
#include "../../Game.hpp"
#include "../../core/Renderer.hpp"
#include "../../helper/Texture.hpp"
#include "../../helper/VertexArray.hpp"

MeshComponent::MeshComponent(Actor* owner) : Component(owner) {
	mOwner->GetGame()->GetRenderer()->AddMeshComp(this);
}

MeshComponent::~MeshComponent() {
    mOwner->GetGame()->GetRenderer()->RemoveMeshGroupRenderer(this, mMesh->GetShaderName());
    mOwner->GetGame()->GetRenderer()->RemoveMeshComp(this);
}

void MeshComponent::Draw(Shader* shader) {
    if (mMesh) {
        // Set the world transform
        shader->SetMatrixUniform("uWorldTransform",
                                 mOwner->GetWorldTransform());
        // Set specular power
        shader->SetFloatUniform("uSpecPower", mMesh->GetSpecPower());
        // Set the active texture
        Texture *t = mMesh->GetTexture(mTextureIndex);
        if (t) {
            t->SetActive();
        }

        // Set the mesh's vertex array as active
        VertexArray *va = mMesh->GetVertexArray();
        va->SetActive();

        // Draw
        glDrawElements(GL_TRIANGLES, va->GetNumIndices(), GL_UNSIGNED_INT, nullptr);
    }
}

void MeshComponent::SetMesh(Mesh *mesh) {
    mMesh = mesh;
    // We add the shader group since we have the shader name already
    mOwner->GetGame()->GetRenderer()->AddMeshGroupRenderer(this, mMesh->GetShaderName());
}
