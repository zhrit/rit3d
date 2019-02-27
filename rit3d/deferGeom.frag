#version 330 core
layout(location = 0) out vec4 gPosition;
layout(location = 1) out vec4 gNormal;
layout(location = 2) out vec4 gAlbedoSpec;

in vec3 fragPos;
in vec3 fragNormal;
in vec2 TexCoord;

uniform vec3 uColor;
uniform int uHasTex;
uniform sampler2D uTexture0;

void main() {
	gPosition = vec4(fragPos, 1.0f);
	gNormal = vec4(fragNormal, 1.0f);
	vec3 matColor = vec3(texture(uTexture0, TexCoord) * uHasTex + vec4(uColor, 1.0f) * (1 - uHasTex));
	gAlbedoSpec = vec4(matColor, 1.0f);
}
