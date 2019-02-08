#version 330 core
out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube uSkybox;

void main() {
	FragColor = texture(uSkybox, TexCoords);
	//FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}