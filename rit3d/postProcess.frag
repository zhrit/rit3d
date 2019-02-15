#version 330 core
out vec4 FragColor;

in vec2 TexCoord;

uniform float uExposure;
uniform sampler2D uTexture;
uniform sampler2D uBloomMap;
uniform float uBloomValue;

void main () {
	vec3 result = vec3(texture(uTexture, TexCoord));
	if(uBloomValue >= 1.0f) {
		vec3 bloomColor = vec3(texture(uBloomMap, TexCoord));
		result += bloomColor;
	}
	result = vec3(1.0f) - exp(-result * uExposure);
	FragColor = vec4(result, 1.0f);
}