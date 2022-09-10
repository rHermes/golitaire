#version 300 es
precision mediump float;


in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

uniform sampler2D texture1;
uniform sampler2D texture2;

void main()
{
    // FragColor = texture(ourTexture, TexCoord) * vec4(ourColor, 1.0);

    FragColor = mix(
        texture(texture1, TexCoord),
        texture(texture2, TexCoord),
        0.2
    );
}
