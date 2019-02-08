#version 330 core
layout (location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 uProjection;
uniform mat4 uView;

void main() {
	TexCoords = aPos;
	vec4 pos = uProjection * uView * vec4(aPos, 1.0f);
	gl_Position = pos.xyww;
}