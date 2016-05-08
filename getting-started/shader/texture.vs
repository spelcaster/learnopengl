#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 texture_coord;

out vec3 color_vs;
out vec2 texture_coord_vs;

void main ()
{
    gl_Position = vec4(position, 1.0f);
    color_vs = color;

    // little hack to fix image y-axis
    texture_coord_vs = vec2(texture_coord.x, 1.0f - texture_coord.y);
}

