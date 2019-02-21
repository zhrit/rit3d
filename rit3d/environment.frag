out vec4 FragColor;

in vec3 fragNormal;
in vec3 fragPos;

uniform vec3 uViewPos;
uniform samplerCube uSkybox;

void main() {
	vec3 I = normalize(fragPos - uViewPos);
	vec3 R = reflect(I, normalize(fragNormal));
	FragColor = vec4(texture(uSkybox, R).rgb, 1.0f);
}
