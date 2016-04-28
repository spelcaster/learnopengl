#version 330 core

out vec4 color;

uniform vec4 dynamic_color;

void main()
{
    color = dynamic_color;
}
