#version 300 es


precision mediump float;

out vec4 FragColor;

in vec3 vertexColor;

void main()
{
    FragColor = vec4(vertexColor, 1.0);
}
