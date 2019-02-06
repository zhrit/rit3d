#version 330 core
in vec4 fragPos;

uniform vec3 uLightPos;
uniform float uFarPlane;

void main() {
	float lightDistance = length(fragPos.xyz - uLightPos);
	lightDistance = lightDistance / uFarPlane;

	gl_FragDepth = lightDistance;
}