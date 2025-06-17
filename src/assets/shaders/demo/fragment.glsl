#version 410 core
in vec3 v_vcolor;
in vec2 pos;
out vec4 f_color;
void main()
{
  float d = 0.45f-length(pos);
  float trans = smoothstep(0 , 0.05f, -d);
  float alpha = 1 - trans;
  f_color = vec4(d, d, d , alpha);
};