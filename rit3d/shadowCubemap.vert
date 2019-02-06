#version 330 core
layout (location = 0) in vec3 position;

uniform mat4 uModel;

void main() {
	gl_Position = uModel * vec4(position, 1.0f);
}