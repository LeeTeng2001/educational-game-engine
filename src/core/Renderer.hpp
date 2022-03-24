#pragma once

#include <SDL_render.h>
#include <string>
#include <unordered_map>
#include <vector>
#include "../helper/Math.hpp"

struct DirectionalLight {
    Vector3 mDirection; // Direction of light
    Vector3 mDiffuseColor; // Diffuse color
    Vector3 mSpecColor; // Specular color
};

class Renderer {
public:
    explicit Renderer(class Game* game);
    ~Renderer();

    // Main function called by Game
    bool Initialize(float screenWidth, float screenHeight);
    void Shutdown();
    void UnloadData();
    void Draw();

    void AddSprite(class SpriteComponent* sprite);
    void RemoveSprite(class SpriteComponent* sprite);

    void AddMeshComp(class MeshComponent* mesh);
    void RemoveMeshComp(class MeshComponent* mesh);

    // Mesh group renderer to support multiple shaders with different meshes
    void AddMeshGroupRenderer(class MeshComponent* mesh, const std::string &shaderName);
    void RemoveMeshGroupRenderer(class MeshComponent* mesh, const std::string &shaderName);

    class Texture* GetTexture(const std::string& fileName);
    class Mesh* GetMesh(const std::string& fileName);

    // 3D render related
    void SetViewMatrix(const Matrix4& view) { mView = view; }
    void SetAmbientLight(const Vector3& ambient) { mAmbientLight = ambient; }
    DirectionalLight& GetDirectionalLight() { return mDirLight; }

    [[nodiscard]] float GetScreenWidth() const { return mScreenWidth; }
    [[nodiscard]] float GetScreenHeight() const { return mScreenHeight; }

    // Given a screen space point, un-projects it into world space,
    // Expected ranges:
    // x = [-screenWidth/2, +screenWidth/2]
    // y = [-screenHeight/2, +screenHeight/2]
    // z = [0, 1) -- 0 is closer to camera, 1 is further
    [[nodiscard]] Vector3 Unproject(const Vector3& screenPoint) const;
    void GetScreenDirection(Vector3& outStart, Vector3& outDir) const;

private:
    // responsible for Opengl shader
    bool LoadShaders();
    void CreateSpriteVerts();
    void SetLightUniforms(class Shader* shader);

    // Map of textures & meshes loaded
    std::unordered_map<std::string, class Texture*> mTextures;
    std::unordered_map<std::string, class Mesh*> mMeshes;

    // All the sprite, meshes components to draw
    std::vector<class SpriteComponent*> mSprites;
    std::vector<class MeshComponent*> mMeshComps;

    // Shaders and group meshes
    std::unordered_map<std::string, class Shader*> mNameToShader;
    std::unordered_map<class Shader*, std::vector<class MeshComponent*>> mShaderGroup;
    std::string mDefaultShaderName = "BasicMesh";

    // Mesh & sprites shader
    class Shader* mSpriteShader = nullptr;
    class Shader* mMeshShader = nullptr;

    // vertex array for sprites
    class VertexArray* mSpriteVerts = nullptr;

    // View/projection for 3D shaders
    Matrix4 mView;
    Matrix4 mProjection;

    // Width/height of screen
    float mScreenWidth = 0;
    float mScreenHeight = 0;

    // Lighting data
    Vector3 mAmbientLight{};
    DirectionalLight mDirLight{};

private:
    // Core members
    // Game
    class Game* mGame = nullptr;
    // Window
    SDL_Window* mWindow = nullptr;
    // OpenGL context
    SDL_GLContext mContext = nullptr;
};



