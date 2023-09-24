#version 410 core

in vec3 PosColor;

out vec4 FragColor;

uniform float blue;

void main()
{
    FragColor = vec4(PosColor.xy, blue, 1.0f);
}
