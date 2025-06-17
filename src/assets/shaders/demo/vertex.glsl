#version 410 core


layout(location=0) in vec3 position;
layout(location=1) in vec3 color;

uniform mat4 u_model;
uniform mat4 u_projection;

out vec3 v_vcolor;
out vec2 pos;

void main()
{
    v_vcolor = color;
    gl_Position = u_projection * u_model * vec4(position, 1.0f);
    pos = position.xy;
}; 