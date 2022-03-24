#pragma once

#include <vector>
#include <string>
#include "Collision.hpp"

class Mesh {
public:
    Mesh() : mBox(Vector3::Infinity, Vector3::NegInfinity) {};
    ~Mesh() = default;

    // Load/unload mesh
    bool Load(const std::string &fileName, class Renderer *renderer);
    void Unload();

    // Get the vertex array associated with this mesh
    class VertexArray *GetVertexArray() { return mVertexArray; }
    // Get a texture from specified index
    class Texture *GetTexture(size_t index);
    // Get name of shader
    [[nodiscard]] const std::string &GetShaderName() const { return mShaderName; }
    // Get object space bounding sphere radius
    [[nodiscard]] float GetRadius() const { return mRadius; }
    // Get specular power of mesh
    [[nodiscard]] float GetSpecPower() const { return mSpecPower; }
    // Get object space bounding box
    [[nodiscard]] const AABB& GetBox() const { return mBox; }

private:
    // Textures associated with this mesh
    std::vector<class Texture *> mTextures;
    // Vertex array associated with this mesh
    class VertexArray *mVertexArray = nullptr;
    // Name of shader specified by mesh
    std::string mShaderName;
    // Stores object space bounding sphere radius, distance between
    // the object space origin and the point furthest away from the origin
    float mRadius = 0;
    // Specular power of surface
    float mSpecPower = 100.0f;
    // AABB collision
    AABB mBox;
};