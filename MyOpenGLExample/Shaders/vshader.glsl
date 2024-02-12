#version 330 core

in vec3 inColor;
in vec3 inVertex;
out vec3 vcolor;
uniform mat4 proy;
uniform vec4 rot;
uniform mat4 view;

vec3 qtransform( in vec4 q, in vec3 v )
{
    return v + 2.0*cross(cross(v, q.xyz) + q.w*v, q.xyz);
}

void main()
{
     vcolor = inColor;
     gl_Position= proy * view * vec4(qtransform(rot,inVertex).xyz,1);
}
