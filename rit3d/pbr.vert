layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aNor;
layout(location = 2) in vec3 aTangent;
layout(location = 3) in vec3 aCol;
layout(location = 4) in vec2 aTexCoord;

out vec2 TexCoord;
out vec3 fragNormal;
out vec3 fragPos;

#ifdef NORMALMAP
out mat3 TBN;
#endif

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProjection;
uniform mat4 uNModel;

//#ifdef DIR_LIGHT_NUM
//uniform mat4 uLightSpaceMatrixDir[DIR_LIGHT_NUM];
//out vec4 positionFromLightDir[DIR_LIGHT_NUM];
//#endif

//#ifdef SPO_LIGHT_NUM
//uniform mat4 uLightSpaceMatrixSpo[SPO_LIGHT_NUM];
//out vec4 positionFromLightSpo[SPO_LIGHT_NUM];
//#endif

void main()
{
	gl_Position = uProjection * uView * uModel * vec4(aPos, 1.0f);
	fragNormal = vec3(uNModel * vec4(aNor, 0.0f));//可能有问题
	fragPos = vec3(uModel * vec4(aPos, 1.0f));
	TexCoord = aTexCoord;

	#ifdef NORMALMAP
	vec3 TBN_T = normalize(vec3(uModel * vec4(aTangent, 0.0f)));
	vec3 TBN_N = fragNormal;
	vec3 TBN_B = cross(TBN_N, TBN_T);
	TBN = mat3(TBN_T, TBN_B, TBN_N);
	#endif
//	#ifdef DIR_LIGHT_NUM
//	for(int i = 0; i < DIR_LIGHT_NUM; i++) {
//		vec4 a = uLightSpaceMatrixDir[i] * vec4(fragPos, 1.0f);
//		positionFromLightDir[i] = a;
//	}
//	#endif

//	#ifdef SPO_LIGHT_NUM
//	for(int i = 0; i < SPO_LIGHT_NUM; i++) {
//		vec4 a = uLightSpaceMatrixSpo[i] * vec4(fragPos, 1.0f);
//		positionFromLightSpo[i] = a;
//	}
//	#endif
}