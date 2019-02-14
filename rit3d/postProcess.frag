#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D uTexture;

void main () {
	const float exposure = 1.0f;
	vec3 result = vec3(texture(uTexture, TexCoord));
	result = vec3(1.0f) - exp(-result * exposure);
	FragColor = vec4(result, 1.0f);
}