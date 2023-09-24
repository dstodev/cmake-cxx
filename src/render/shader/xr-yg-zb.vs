#version 410 core

layout (location = 0)
in vec3 Position;

out vec3 PosColor;

void main()
{
    gl_Position = vec4(Position, 1.0);
    PosColor = (Position + 1) / 2;// move range from [-1, 1] to [0, 0.5]
}
