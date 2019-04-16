#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uSpirit;

void main () {
	vec3 result = vec3(texture(uSpirit, TexCoord));
	result = vec3(1.0f) - exp(-result);
	FragColor = vec4(result, 1.0f);
}