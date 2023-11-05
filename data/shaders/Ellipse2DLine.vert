#version 300 es

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTexCoord;

layout (location = 6) in mat4 ellipseTransform;

out vec3 ourColor;
out vec2 TexCoord;

uniform mat4 mvp;

void main() {
    gl_Position = mvp * ellipseTransform * vec4(aPos, 1.0);
    ourColor = aColor;

    // We will use these to draw the circle.
    TexCoord = aTexCoord;
}
