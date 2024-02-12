#version 330 core

in vec3 vcolor;
uniform float transparency;
out vec4 outColor;

void main()
{
    outColor = vec4(vcolor,transparency);
}
