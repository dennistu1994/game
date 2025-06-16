#version 410 core
in vec3 v_vcolor;
out vec4 f_color;
void main()
{
  f_color = vec4(v_vcolor.r, v_vcolor.g, v_vcolor.b, 1.0f);
};