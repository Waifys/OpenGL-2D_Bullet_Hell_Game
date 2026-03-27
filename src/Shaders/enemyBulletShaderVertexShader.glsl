#version 330 core
layout (location = 0) in vec2 vertex;
layout (location = 1) in vec2 textCoords;
layout (location = 2) in vec2 instanceOffset;

out vec2 TexCoords;

uniform mat4 projection;

void main()
{
	TexCoords = textCoords;
	vec2 scaledVertex = vertex * 8;
    gl_Position = projection * vec4(scaledVertex + instanceOffset, 0.0, 1.0);
}
