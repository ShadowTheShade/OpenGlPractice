#version 330 core
layout (location = 0) in vec3 aPos;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform vec3 normal;

out vec3 Normal;
out vec3 FragPos;

void main()
{
FragPos	= vec3(model * vec4(aPos, 1.0));
Normal = normal;

gl_Position = projection * view * vec4(FragPos, 1.0);
}