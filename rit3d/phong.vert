layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNor;
layout(location = 2) in vec3 aCol;
layout(location = 3) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 fragNormal;
out vec3 fragPos;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 nmodel;

#ifdef DIR_LIGHT_NUM
uniform mat4 uLightSpaceMatrixDir[DIR_LIGHT_NUM];
out vec4 positionFromLightDir[DIR_LIGHT_NUM];
#endif

#ifdef SPO_LIGHT_NUM
uniform mat4 uLightSpaceMatrixSpo[SPO_LIGHT_NUM];
out vec4 positionFromLightSpo[SPO_LIGHT_NUM];
#endif

void main()
{
	gl_Position = projection * view * model * vec4(aPos, 1.0f);
	fragNormal = vec3(nmodel * vec4(aNor, 0.0f));//����������
	fragPos = vec3(model * vec4(aPos, 1.0f));
	TexCoord = aTexCoord;

	#ifdef DIR_LIGHT_NUM
	for(int i = 0; i < DIR_LIGHT_NUM; i++) {
		vec4 a = uLightSpaceMatrixDir[i] * vec4(fragPos, 1.0f);
		positionFromLightDir[i] = a;
	}
	#endif

	#ifdef SPO_LIGHT_NUM
	for(int i = 0; i < SPO_LIGHT_NUM; i++) {
		vec4 a = uLightSpaceMatrixSpo[i] * vec4(fragPos, 1.0f);
		positionFromLightSpo[i] = a;
	}
	#endif
}