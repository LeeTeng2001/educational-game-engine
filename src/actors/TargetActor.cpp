#include "TargetActor.hpp"
#include "../Game.hpp"
#include "../core/Renderer.hpp"
#include "../components/render/MeshComponent.hpp"
#include "../components/collision/BoxComponent.hpp"
#include "../helper/Mesh.hpp"
#include "../components/gameplay/TargetComponent.hpp"

TargetActor::TargetActor(Game *game) : Actor(game) {
    //SetScale(10.0f);
    SetRotation(Quaternion(Vector3::UnitZ, Math::Pi));
    auto *mc = new MeshComponent(this);
    Mesh *mesh = GetGame()->GetRenderer()->GetMesh("Assets/Target.gpmesh");
    mc->SetMesh(mesh);

    // Add collision box
    auto *bc = new BoxComponent(this);
    bc->SetObjectBox(mesh->GetBox());

    // Target
    new TargetComponent(this);
}
