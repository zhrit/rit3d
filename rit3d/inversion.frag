#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float uExposure;
uniform sampler2D uTexture;
uniform sampler2D uBloomMap;
uniform float uBloomValue;

void main () {
	vec3 result = vec3(texture(uTexture, TexCoord));
	result = vec3(1.0f) - exp(-result * uExposure);
	FragColor = vec4(vec3(1.0f - result), 1.0f);
}