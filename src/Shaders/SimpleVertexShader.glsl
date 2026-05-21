#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 textcoords;

out vec2 TexCoord;
out float depth;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    TexCoord = aPos.xy * 0.5 + 0.5;
    TexCoord = TexCoord / 0.2;
    vec4 viewPos = view * model * vec4(aPos, 1.0);
    depth = -viewPos.z;
    gl_Position = projection * viewPos;
}
