#version 410 core

in vec3 PosColor;

out vec4 FragColor;

void main()
{
    FragColor = vec4(PosColor, 1.0f);
}
