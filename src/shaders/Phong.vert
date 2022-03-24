#version 330
// Same as sprite.vert

// Uniforms for world transform and view-proj
uniform mat4 uWorldTransform;
uniform mat4 uViewProj;

// Vertex attributes
layout(location=0) in vec3 inPosition;
layout(location=1) in vec3 inNormal;
layout(location=2) in vec2 inTexCoord;

// texture coord (original)
out vec2 fragTexCoord;
// Normal (in world space)
out vec3 fragNormal;
// Position (in world space)
out vec3 fragWorldPos;

void main() {
    // Convert position to homogeneous coordinates
    vec4 pos = vec4(inPosition, 1.0);
    // Transform position to world space
    pos = pos * uWorldTransform;
    // Save world position
    fragWorldPos = pos.xyz;
    // Transform to clip space
    gl_Position = pos * uViewProj;

    // Transform normal into world space (w = 0 because it's not a position, make no sense for 1)
    fragNormal = (vec4(inNormal, 0.0f) * uWorldTransform).xyz;

    // Pass along the texture coordinate to frag shader
    fragTexCoord = inTexCoord;
}

