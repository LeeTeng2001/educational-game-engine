#include <algorithm>
#include "Renderer.hpp"
#include "../helper/Texture.hpp"
#include "../helper/Mesh.hpp"
#include "Shader.hpp"
#include "../helper/VertexArray.hpp"
#include "../Game.hpp"
#include "../components/render/SpriteComponent.hpp"
#include "../components/render/MeshComponent.hpp"
#include "../audio/AudioSystem.hpp"
#include "../ui/UIScreen.hpp"

Renderer::Renderer(Game* game) : mGame(game) {}

Renderer::~Renderer() = default;

bool Renderer::Initialize(float screenWidth, float screenHeight) {
    mScreenWidth = screenWidth;
    mScreenHeight = screenHeight;

    // Set opengl attribute BEFORE creating the window, it's like glfw
    // Use the core OpenGL profile
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Specify version 3.3
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    // Request a color buffer with 8-bits per RGBA channel
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);  // Depth buffer
    // Double buffering, one present one being drawn into
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    // Force OpenGL to use hardware acceleration
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 6)", 100, 100,
                               static_cast<int>(mScreenWidth), static_cast<int>(mScreenHeight), SDL_WINDOW_OPENGL);
    if (!mWindow) {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    // Create an OpenGL context
    mContext = SDL_GL_CreateContext(mWindow);

    // Initialize glad and make all extension function supported available
    // Load GL extensions using glad
    if (!gladLoadGLLoader((GLADloadproc) SDL_GL_GetProcAddress)) {
        SDL_Log("Failed to initialize the OpenGL context.");
        return false;
    }

    // Loaded OpenGL successfully.
    SDL_Log("OpenGL version loaded: %d.%d", GLVersion.major, GLVersion.minor);

    // On some platforms, GLEW will emit a benign error code,
    // so clear it
    glGetError();

    // Make sure we can create/compile shaders
    if (!LoadShaders()) {
        SDL_Log("Failed to load shaders.");
        return false;
    }

    // Create quad for drawing sprites
    CreateSpriteVerts();

    return true;
}

void Renderer::Shutdown() {
    delete mSpriteVerts;
    mSpriteShader->Unload();
    delete mSpriteShader;

    // Delete shaders for meshes
    for (const auto &item : mNameToShader) {
        item.second->Unload();
        delete item.second;
    }

    SDL_GL_DeleteContext(mContext);
    SDL_DestroyWindow(mWindow);
}

void Renderer::UnloadData() {
    // Destroy textures
    for (auto i : mTextures) {
        i.second->Unload();
        delete i.second;
    }
    mTextures.clear();

    // Destroy meshes
    for (auto i : mMeshes) {
        i.second->Unload();
        delete i.second;
    }
    mMeshes.clear();
}

void Renderer::Draw() {
    // Calculate current color
    // Set draw colour, clear back buffer to current colour
    glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
    // Clear color buffer & depth buffer
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Main render logic ----------------------------------------------------------------

    // Draw mesh components
    // Enable depth buffering/disable alpha blend (must know reason!)
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_BLEND);

    // Group shader render
    auto viewProjectionCache = mView * mProjection;
    for (const auto &shaderGroup : mShaderGroup) {
        // Set mesh shader active, update view matrix
        auto &curShader = shaderGroup.first;
        curShader->SetActive();
        curShader->SetMatrixUniform("uViewProj", viewProjectionCache);

        // Update lighting uniforms
        SetLightUniforms(curShader);
        for (auto mc : shaderGroup.second) {
            if (mc->GetVisible())
                mc->Draw(curShader);
        }
    }

    // Draw all sprite components
    // Disable depth buffering & enable blend mode for sprite
    glDisable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    // Enable alpha blending on the color buffer, look at the book for func explanation
    // we want outputColor = alpha * newColor + (1-alpha) * oriColor
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    // Why replace blend func with this?
    glBlendEquationSeparate(GL_FUNC_ADD, GL_FUNC_ADD);
    glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ZERO);

    // Set shader/vao as active
    mSpriteShader->SetActive();
    mSpriteVerts->SetActive();
    for (auto sprite : mSprites) {
        if (sprite->GetVisible())
            sprite->Draw(mSpriteShader);
    }

    // Share same shader as sprite, draw UI
    for (auto ui : mGame->GetUIStack()) {
        ui->Draw(mSpriteShader);
    }

    // Swap the buffers
    SDL_GL_SwapWindow(mWindow);
}

void Renderer::AddSprite(SpriteComponent* sprite) {
    // Find the insertion point in the sorted vector
    // (The first element with a higher draw order than me)
    int myDrawOrder = sprite->GetDrawOrder();
    auto iter = mSprites.begin();
    while (iter != mSprites.end()) {
        if (myDrawOrder < (*iter)->GetDrawOrder())
            break;
        ++iter;
    }

    // Inserts element before position of iterator
    mSprites.insert(iter, sprite);
}

void Renderer::RemoveSprite(SpriteComponent* sprite) {
    auto iter = std::find(mSprites.begin(), mSprites.end(), sprite);
    mSprites.erase(iter);
}

void Renderer::AddMeshComp(MeshComponent* mesh) {
    mMeshComps.emplace_back(mesh);
}

void Renderer::RemoveMeshComp(MeshComponent* mesh) {
    auto iter = std::find(mMeshComps.begin(), mMeshComps.end(), mesh);
    mMeshComps.erase(iter);
}

Texture* Renderer::GetTexture(const std::string& fileName) {
    Texture *tex = nullptr;

    std::string filePath = Game::PROJECT_BASE + fileName;

    auto iter = mTextures.find(filePath);
    if (iter != mTextures.end()) {
        tex = iter->second;
    } else {
        tex = new Texture();
        if (tex->Load(filePath)) {
            mTextures.emplace(filePath, tex);
        } else {
            delete tex;
            tex = nullptr;
        }
    }
    return tex;
}

Mesh* Renderer::GetMesh(const std::string &fileName) {
    Mesh *m = nullptr;

    std::string filePath = Game::PROJECT_BASE + fileName;

    auto iter = mMeshes.find(filePath);
    if (iter != mMeshes.end()) {
        m = iter->second;
    } else {
        m = new Mesh();
        if (m->Load(filePath, this)) {
            mMeshes.emplace(filePath, m);
        } else {
            delete m;
            m = nullptr;
        }
    }
    return m;
}

bool Renderer::LoadShaders() {
    // Create sprite shader
    mSpriteShader = new Shader();
    if (!mSpriteShader->Load("shaders/Sprite.vert", "shaders/Sprite.frag")) {
        return false;
    }

    mSpriteShader->SetActive();
    // Set the view-projection matrix
    Matrix4 viewProj = Matrix4::CreateSimpleViewProj(mScreenWidth, mScreenHeight);
    mSpriteShader->SetMatrixUniform("uViewProj", viewProj);


    // Create default mesh shader
    mMeshShader = new Shader();
    if (!mMeshShader->Load("shaders/" + mDefaultShaderName + ".vert", "shaders/" + mDefaultShaderName + ".frag")) {
        return false;
    }
    mNameToShader[mDefaultShaderName] = mMeshShader;

    mMeshShader->SetActive();
    // Set the view-projection matrix
    mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
    mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
                                                mScreenWidth, mScreenHeight, 25.0f, 10000.0f);
    mMeshShader->SetMatrixUniform("uViewProj", mView * mProjection);
    return true;
}

void Renderer::CreateSpriteVerts() {
    // most image file formats store their data starting at the top left row
    // Opengl starts from bottom left, we follow because we inverse when loading with std_image
    float vertices[] = {
            -0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, // top left
            0.5f, 0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 1.f, // top right
            0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 1.f, 0.f, // bottom right
            -0.5f, -0.5f, 0.f, 0.f, 0.f, 0.f, 0.f, 0.f  // bottom left
    };

    unsigned int indices[] = {
            0, 1, 2,
            2, 3, 0
    };

    mSpriteVerts = new VertexArray(vertices, 4, indices, 6);
}

void Renderer::SetLightUniforms(Shader* shader) {
    // Camera position is from inverted view
    Matrix4 invView = mView;
    invView.Invert();
    shader->SetVectorUniform("uCameraPos", invView.GetTranslation());
    // Ambient light
    shader->SetVectorUniform("uAmbientLight", mAmbientLight);
    // Directional light, notice we use dot notation to access struct
    shader->SetVectorUniform("uDirLight.mDirection", mDirLight.mDirection);
    shader->SetVectorUniform("uDirLight.mDiffuseColor",mDirLight.mDiffuseColor);
    shader->SetVectorUniform("uDirLight.mSpecColor", mDirLight.mSpecColor);
}

void Renderer::AddMeshGroupRenderer(MeshComponent *mesh, const std::string &shaderName) {
    // 1. find if the shader path exist
    auto shader = mNameToShader.find(shaderName);
    if (shader != mNameToShader.end()) {  // shader exist, add to existing group
        mShaderGroup[shader->second].push_back(mesh);
        return;
    }

    // 2. We don't have that shader in cache, try to create one
    auto newShader = new Shader();
    if (!newShader->Load("shaders/" + shaderName + ".vert", "shaders/" + shaderName + ".frag")) {
        // 3. Doesn't exist, use default
        delete newShader;
        mShaderGroup[mMeshShader].push_back(mesh);
    }
    else {
        // 4. Exist a shader file, store it and do initial setting
        mNameToShader[shaderName] = newShader;
        mShaderGroup[newShader].push_back(mesh);

        // Same code as default shader setup
        newShader->SetActive();
        // Set the view-projection matrix
        mView = Matrix4::CreateLookAt(Vector3::Zero, Vector3::UnitX, Vector3::UnitZ);
        mProjection = Matrix4::CreatePerspectiveFOV(Math::ToRadians(70.0f),
                                                    mScreenWidth, mScreenHeight, 25.0f, 10000.0f);
        newShader->SetMatrixUniform("uViewProj", mView * mProjection);
    }
}

void Renderer::RemoveMeshGroupRenderer(MeshComponent *mesh, const std::string &shaderName) {
    // remove mesh renderer from group, ugly C++
    auto shader = mNameToShader.find(shaderName);
    if (shader == mNameToShader.end()) {
        shader = mNameToShader.find(mDefaultShaderName);
    }

    mShaderGroup[shader->second].erase(
            std::remove(mShaderGroup[shader->second].begin(),
                        mShaderGroup[shader->second].end(), mesh),
                        mShaderGroup[shader->second].end()
            );
}

Vector3 Renderer::Unproject(const Vector3 &screenPoint) const {
    // Convert screenPoint to device coordinates (between -1 and +1)
    Vector3 deviceCoord = screenPoint;
    deviceCoord.x /= (mScreenWidth) * 0.5f;
    deviceCoord.y /= (mScreenHeight) * 0.5f;

    // Transform vector by un-projection matrix
    Matrix4 unprojection = mView * mProjection;
    unprojection.Invert();
    return Vector3::TransformWithPerspDiv(deviceCoord, unprojection);
}

void Renderer::GetScreenDirection(Vector3 &outStart, Vector3 &outDir) const {
    // Get start point (in center of screen on near plane)
    Vector3 screenPoint(0.0f, 0.0f, 0.0f);
    outStart = Unproject(screenPoint);
    // Get end point (in center of screen, between near and far)
    screenPoint.z = 0.9f;
    Vector3 end = Unproject(screenPoint);
    // Get direction vector
    outDir = end - outStart;
    outDir.Normalize();
}
