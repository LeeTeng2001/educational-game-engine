#version 330

// Layout should match vertex array attributes
in vec3 inPosition;

void main() {
    gl_Position = vec4(inPosition, 1.0);
}

