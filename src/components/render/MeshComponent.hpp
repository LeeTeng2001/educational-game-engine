#pragma once

#include "../Component.hpp"
#include <cstddef>

class MeshComponent : public Component {
public:
    explicit MeshComponent(class Actor *owner);
    ~MeshComponent();

    // Draw this mesh component with specified shader
    virtual void Draw(class Shader *shader);

    // Set the mesh/texture index used by mesh component
    virtual void SetMesh(class Mesh *mesh);

    // Setter
    void SetTextureIndex(size_t index) { mTextureIndex = index; }
    void SetVisible(bool visible) { mVisible = visible; }

    // Getter
    [[nodiscard]] bool GetVisible() const { return mVisible; }

protected:
    class Mesh *mMesh = nullptr;
    size_t mTextureIndex = 0;
    bool mVisible = true;
};
