#version 330 core

in vec3 color_vs;
in vec2 texture_coord_vs;

out vec4 color;

uniform float mix_ratio;

uniform sampler2D texture0;
uniform sampler2D texture1;

void main ()
{
    //color = texture(texture0, texture_coord_vs);
    //color = texture(texture0, texture_coord_vs) * vec4(color_vs, 1.0f);
    color = mix(
        texture(texture0, texture_coord_vs),
        //texture(texture1, vec2(1.0f - texture_coord_vs.x, texture_coord_vs.y)),
        texture(texture1, vec2(texture_coord_vs.x, texture_coord_vs.y)),
        mix_ratio
    );
}
