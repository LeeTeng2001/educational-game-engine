#version 330

// Must have the same name as out in vert
in vec2 fragTexCoord;

out vec4 outColor;

// This is used for the texture sampling
uniform sampler2D uTexture;

void main() {
    // RGBA of 100% blue, 100% opaque
    outColor = texture(uTexture, fragTexCoord);
}
