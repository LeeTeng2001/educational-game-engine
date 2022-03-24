#pragma once


class VertexArray {
public:
    VertexArray(const float* verts, unsigned int numVerts,
                const unsigned int* indices, unsigned int numIndices);
    ~VertexArray();

    // Activate this vertex array (so we can draw it)
    void SetActive() const;

    // Getter
    [[nodiscard]] unsigned int GetNumIndices() const { return mNumIndices; }
    [[nodiscard]] unsigned int GetNumVerts() const { return mNumVerts; }

private:
    // How many vertices in the vertex buffer?
    unsigned int mNumVerts = 0;
    // How many indices in the index buffer
    unsigned int mNumIndices = 0;
    // OpenGL ID of the vertex buffer
    unsigned int mVertexBuffer = 0;
    // OpenGL ID of the index buffer
    unsigned int mIndexBuffer = 0;
    // OpenGL ID of the vertex array object
    unsigned int mVertexArray = 0;
};



