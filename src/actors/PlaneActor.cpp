#include "PlaneActor.hpp"
#include "../Game.hpp"
#include "../core/Renderer.hpp"
#include "../helper/Mesh.hpp"
#include "../components/render/MeshComponent.hpp"
#include "../components/collision/BoxComponent.hpp"

PlaneActor::PlaneActor(Game* game) : Actor(game) {
	SetScale(10.0f);
	auto* mc = new MeshComponent(this);
    auto* mesh = GetGame()->GetRenderer()->GetMesh("Assets/Plane.gpmesh");
	mc->SetMesh(mesh);

    // Add collision box
    mBox = new BoxComponent(this);
    mBox->SetObjectBox(mesh->GetBox());

    game->AddPlane(this);
}

PlaneActor::~PlaneActor() {
    GetGame()->RemovePlane(this);
}
