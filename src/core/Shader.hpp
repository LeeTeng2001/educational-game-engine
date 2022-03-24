#pragma once

#include <string>
#include "glad/glad.h"
#include "../helper/Math.hpp"

class Shader {
public:
    Shader() = default;
    ~Shader() = default;

    // Load the vertex/fragment shaders with the given names
    bool Load(const std::string& vertName, const std::string& fragName);
    void Unload() const;

    // Set this as the active shader program
    void SetActive() const;
    // Sets a Matrix uniform
    void SetMatrixUniform(const char* name, const Matrix4& matrix);
    // Sets a Vector3 uniform
    void SetVectorUniform(const char* name, const Vector3& vector);
    // Sets a float uniform
    void SetFloatUniform(const char* name, float value);

private:
    // Helper function used by Load
    // Tries to compile the specified shader
    static bool CompileShader(const std::string& fileName,
                       GLenum shaderType,
                       GLuint& outShader);
    // Tests whether shader compiled successfully
    static bool IsCompiled(GLuint shader);
    // Tests whether vertex/fragment programs link
    bool IsValidProgram();

private:
    // Store the shader object IDs
    GLuint mVertexShader;
    GLuint mFragShader;
    GLuint mShaderProgram;
};



