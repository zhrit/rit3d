out vec4 FragColor;

in vec2 TexCoord;//uv坐标
in vec3 fragNormal;//世界坐标系法向量
in vec3 fragPos;//世界坐标系位置

//平行光结构体
struct DirLight {
	vec3 direction;
	vec3 color;
	float ambInt;
	float difInt;
	float speInt;
	bool castShadow;
};

//点光源结构体
struct PoiLight {
	vec3 position;
	vec3 color;
	float ambInt;
	float difInt;
	float speInt;
	float kc;
	float kl;
	float kq;
	bool castShadow;
	float far;//计算阴影时用
};

//聚光灯结构体
struct SpoLight {
	vec3 position;
	vec3 direction;
	vec3 color;
	float ambInt;
	float difInt;
	float speInt;
	float inner;
	float outer;
	bool castShadow;
};

#ifdef DIR_LIGHT_NUM
uniform DirLight uDirLights[DIR_LIGHT_NUM];
//uniform sampler2D uDirShadowMap[DIR_LIGHT_NUM];
//in vec4 positionFromLightDir[DIR_LIGHT_NUM];
#endif
#ifdef POI_LIGHT_NUM
uniform PoiLight uPoiLights[POI_LIGHT_NUM];
//uniform samplerCube uPoiShadowMap[POI_LIGHT_NUM];
#endif
#ifdef SPO_LIGHT_NUM
uniform SpoLight uSpoLights[SPO_LIGHT_NUM];
//uniform sampler2D uSpoShadowMap[SPO_LIGHT_NUM];
//in vec4 positionFromLightSpo[SPO_LIGHT_NUM];
#endif

uniform vec3 uColor;
uniform int uHasTex;
uniform sampler2D uTexture0;
uniform sampler2D uTexture1;
uniform sampler2D uTexture2;
uniform sampler2D uTexture3;

uniform vec3 uViewPos;
//uniform float uShininess;
//uniform bool uRecieveShadow;
uniform float uMetallic;
uniform float uRoughness;
//uniform float uAo;
const float PI = 3.14159265359f;

//分布函数
float DistributionGGX(vec3 N, vec3 H, float roughness) {
	//float a = roughness*roughness;
	float alpha2 = roughness * roughness;
	float NdotH = max(dot(N, H), 0.0f);

	float NdotH2 = NdotH * NdotH;

	float nom = alpha2;
	float denom = NdotH2 * (alpha2 - 1.0f) + 1.0f;
	denom = PI * denom * denom;

	return nom / max(denom, 0.0f);
}

//几何遮蔽函数
float GeometrySchlickGGX(float NdotV, float roughness) {
	float k = (roughness + 1.0f) * (roughness + 1.0f) / 8.0f;

	float nom = NdotV;
	float denom = NdotV * (1.0f - k) + k;

	return nom / denom;
}
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N, V), 0.0f);
	float NdotL = max(dot(N, L), 0.0f);
	float ggx2 = GeometrySchlickGGX(NdotV, roughness);
	float ggx1 = GeometrySchlickGGX(NdotL, roughness);

	return ggx1 * ggx2;
}

//菲涅尔方程
vec3 FresnelSchlick(float cosTheta, vec3 F0) {
	return F0 + (1.0f - F0) * pow(1.0f - cosTheta, 5.0f);
}

//vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);

//点光源pbr
vec3 CalcPoiLight(PoiLight light, vec3 matColor, vec3 normal, vec3 viewDir, vec3 fragPos, float roughness, float metallic, vec3 F0) {
	vec3 L = normalize(light.position - fragPos);
	vec3 H = normalize(viewDir + L);
	float dis = length(light.position - fragPos);
	float attenuation = 1.0f / (dis * dis);
	vec3 radiance = light.color * attenuation;

	//Cook-Torrance BRDF
	float NDF = DistributionGGX(normal, H, roughness);
	float G = GeometrySmith(normal, viewDir, L, roughness);
	vec3 F = FresnelSchlick(clamp(dot(normal, viewDir), 0.0f, 1.0f), F0);

	vec3 nominator = NDF * G * F;
	float denominator = 4 * max(dot(normal, viewDir), 0.1f) * max(dot(normal, L), 0.0f);
	vec3 specular = nominator / max(denominator, 0.001f);

	vec3 ks = F;
	vec3 kd = vec3(1.0f) - ks;
	kd *= 1.0 - metallic;

	float NdotL = max(dot(normal, L), 0.0f);

	return (kd * matColor / PI + specular) * radiance * NdotL;
	//return vec3(300.0f * attenuation, 300.0f * attenuation, 300.0f * attenuation);

	//return light.color;
}
//vec3 CalcSpoLight(SpoLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main() {
	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(uViewPos - fragPos);

	//从纹理中选择颜色
	vec3 matColor = pow(vec3(texture(uTexture0, TexCoord)), vec3(2.2)) * uHasTex + uColor * (1 - uHasTex);
	//从纹理中选择粗糙度
	float roughness = texture(uTexture2, TexCoord).r * uHasTex + uRoughness * (1 - uHasTex);
	//从纹理中选择金属度
	float metallic = texture(uTexture1, TexCoord).r * uHasTex + uMetallic * (1 - uHasTex);
	vec3 F0 = vec3(0.04f);
	F0 = mix(F0, matColor, metallic);

	vec3 result = vec3(0.0f, 0.0f, 0.0f);
//	#ifdef DIR_LIGHT_NUM
//	for(int i = 0; i < DIR_LIGHT_NUM; i++) {
//		result += CalcDirLight(uDirLights[i], norm, viewDir, uDirShadowMap[i], positionFromLightDir[i]);
//	}
//	#endif
	#ifdef POI_LIGHT_NUM
	for(int i = 0; i < POI_LIGHT_NUM; i++) {
		result += CalcPoiLight(uPoiLights[i], matColor, norm, viewDir, fragPos, roughness, metallic, F0);
	}
	#endif
//	#ifdef SPO_LIGHT_NUM
//	for(int i = 0; i < SPO_LIGHT_NUM; i++) {
//		result += CalcSpoLight(uSpoLights[i], norm, viewDir, fragPos, uSpoShadowMap[i], positionFromLightSpo[i]);
//	}
//	#endif

	//HDR tonemapping
	result = result / (result + vec3(1.0f));
	//gamma correct,本机不需要gamma correct
	//result = pow(result, vec3(1.0f / 2.2f));
    FragColor = vec4(result, 1.0f);
}