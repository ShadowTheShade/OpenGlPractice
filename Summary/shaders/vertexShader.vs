#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aCol;
layout(location = 2) in vec2 aTex;

out vec3 outCol;
out vec2 outTex;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;


void main()
{
   gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
   outCol = aCol;
   outTex = aTex;
}