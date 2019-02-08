#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uTexture;

void main () {
	FragColor = vec4(vec3(1.0f - texture(uTexture, TexCoord)), 1.0f);
}