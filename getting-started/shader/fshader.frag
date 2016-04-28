#version 330 core

in vec3 custom_color;

out vec4 color;

void main()
{
    color = vec4(custom_color, 1.0);
}
