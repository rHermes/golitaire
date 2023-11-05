#version 300 es
precision mediump float;


in vec3 ourColor;
in vec2 TexCoord;

out vec4 FragColor;

// uniform sampler2D texture1;

float sdCircle( in vec2 p, in float r )
{
    return length(p)-r;
}


void main()
{
    float cir = sdCircle(TexCoord - vec2(0.5, 0.5), 0.5);
    if (cir <= 0.0) {
        FragColor = vec4(ourColor, 1.0);
    } else {
        FragColor = vec4(0.0, 0.0, 0.0, 0.0);
    }
}
