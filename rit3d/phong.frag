layout (location = 0) out vec4 FragColor;
#ifdef BLOOM
layout (location = 1) out vec4 BrightColor;
#endif

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

#ifdef BLOOM
uniform float uBloomValue;
#endif

#ifdef NORMALMAP
uniform sampler2D uNormalMap;
in mat3 TBN;
#endif

#ifdef DIR_LIGHT_NUM
uniform DirLight uDirLights[DIR_LIGHT_NUM];
uniform sampler2D uDirShadowMap[DIR_LIGHT_NUM];
in vec4 positionFromLightDir[DIR_LIGHT_NUM];
#endif
#ifdef POI_LIGHT_NUM
uniform PoiLight uPoiLights[POI_LIGHT_NUM];
uniform samplerCube uPoiShadowMap[POI_LIGHT_NUM];
#endif
#ifdef SPO_LIGHT_NUM
uniform SpoLight uSpoLights[SPO_LIGHT_NUM];
uniform sampler2D uSpoShadowMap[SPO_LIGHT_NUM];
in vec4 positionFromLightSpo[SPO_LIGHT_NUM];
#endif
uniform vec3 uColor;
uniform int uHasTex;
uniform sampler2D uTexture0;
uniform vec3 uViewPos;
uniform float uShininess;
uniform bool uRecieveShadow;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, sampler2D shadowMap, vec4 pos4light);
vec3 CalcPoiLight(PoiLight light, vec3 normal, vec3 viewDir, vec3 fragPos, samplerCube shadowMap);
vec3 CalcSpoLight(SpoLight light, vec3 normal, vec3 viewDir, vec3 fragPos, sampler2D shadowMap, vec4 pos4light);

void main() {
	vec3 norm = normalize(fragNormal);
	#ifdef NORMALMAP
	norm = texture(uNormalMap, TexCoord).rgb;
	norm = normalize(norm * 2.0f - 1.0f);
	norm = normalize(TBN * norm);
	#endif
	vec3 viewDir = normalize(uViewPos - fragPos);

	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	#ifdef DIR_LIGHT_NUM
	for(int i = 0; i < DIR_LIGHT_NUM; i++) {
		result += CalcDirLight(uDirLights[i], norm, viewDir, uDirShadowMap[i], positionFromLightDir[i]);
	}
	#endif
	#ifdef POI_LIGHT_NUM
	for(int i = 0; i < POI_LIGHT_NUM; i++) {
		result += CalcPoiLight(uPoiLights[i], norm, viewDir, fragPos, uPoiShadowMap[i]);
	}
	#endif
	#ifdef SPO_LIGHT_NUM
	for(int i = 0; i < SPO_LIGHT_NUM; i++) {
		result += CalcSpoLight(uSpoLights[i], norm, viewDir, fragPos, uSpoShadowMap[i], positionFromLightSpo[i]);
	}
	#endif
    FragColor = vec4(result, 1.0f);

	#ifdef BLOOM
	float brightness = dot(FragColor.rgb, vec3(0.0, 0.0, 0.0));
	BrightColor = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	if (brightness == 200000.0f) {
		BrightColor = vec4(FragColor.rgb, 1.0f);
	}
	#endif
	//FragColor = texture(texture0, TexCoord) * hasTex + vec4(color, 1.0f) * (1 - hasTex);
}

//平行光效果计算
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir, sampler2D shadowMap, vec4 pos4light) {
	vec3 lightDir = normalize(light.direction);
	float diff = max(dot(-lightDir, normal), 0.0f);
	vec3 reflectDir = normalize(reflect(lightDir, normal));
	float spec =  pow(max(dot(reflectDir, viewDir), 0.0f), uShininess);

	vec3 matColor = vec3(texture(uTexture0, TexCoord) * uHasTex + vec4(uColor, 1.0f) * (1 - uHasTex));
	vec3 ambient = light.ambInt * light.color * matColor;
	vec3 diffuse = light.difInt * diff * light.color * matColor;
	vec3 specular = light.speInt * spec * light.color * matColor;

	//阴影
	vec3 shadowCoord = pos4light.xyz / pos4light.w;
	shadowCoord = shadowCoord * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, shadowCoord.xy).r;
	float currentDepth = shadowCoord.z;
	//暂时没考虑阴影失真
	//float bias = max(0.05f * (1.0f - dot(normal, -lightDir)), 0.005f);
	//超出光源视锥体的地方需要特殊处理
	float shadow = (currentDepth > (closestDepth) && currentDepth <= 1.0f && uRecieveShadow && light.castShadow) ? 0.0f : 1.0f;
	//PCF柔化阴影边缘
	//float shadow = 0.0;
	//vec2 texelSize = 1.0 / textureSize(uDirShadowMap[ind], 0);
	//for(int i = -1; i <= 1; ++i) {
	//	for(int j = -1; j <= 1; ++j) {
	//		float pcfDepth = texture(uDirShadowMap[ind], shadowCoord.xy + vec2(i, j) * texelSize).r; 
	//		shadow += (currentDepth > (pcfDepth) && currentDepth <= 1.0f && uRecieveShadow && uDirLights[ind].castShadow) ? 0.0f : 1.0f;
	//	}    
	//}
	//shadow /= 9.0f;

	return (ambient + diffuse * shadow + specular * shadow);
}

//点光源效果计算
vec3 CalcPoiLight(PoiLight light, vec3 normal, vec3 viewDir, vec3 fragPos, samplerCube shadowMap) {
	vec3 lightDir = normalize(fragPos - light.position);
	float distance = length(fragPos - light.position);
	float attenuation = 1.0f / (light.kc + light.kl * distance + light.kq * distance * distance);

	float diff = max(dot(-lightDir, normal), 0.0f);
	vec3 reflectDir = normalize(reflect(lightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), uShininess);

	vec3 matColor = vec3(texture(uTexture0, TexCoord) * uHasTex + vec4(uColor, 1.0f) * (1 - uHasTex));
	vec3 ambient = light.ambInt * light.color * matColor;
	vec3 diffuse = light.difInt * diff * light.color * matColor;
	vec3 specular = light.speInt * spec * light.color * matColor;

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	//阴影
	vec3 lightToFrag = fragPos - light.position;
	float closestDepth = texture(shadowMap, lightToFrag).r;
	closestDepth *= light.far;
	float currentDepth = length(lightToFrag);
	//暂时没考虑阴影失真
	//float bias = max(0.05f * (1.0f - dot(normal, -lightDir)), 0.005f);
	//超出光源视锥体的地方需要特殊处理
	float shadow = (currentDepth > closestDepth && currentDepth <= light.far && uRecieveShadow && light.castShadow) ? 0.0f : 1.0f;

	return (ambient + diffuse * shadow + specular * shadow);

}

//聚光灯效果计算
vec3 CalcSpoLight(SpoLight light, vec3 normal, vec3 viewDir, vec3 fragPos, sampler2D shadowMap, vec4 pos4light) {
	vec3 lightDir = normalize(fragPos - light.position);
	float theta = dot(lightDir, normalize(light.direction));
	float epsilon = light.inner - light.outer;
	float intensity = clamp((theta - light.outer) / epsilon, 0.0f, 1.0f);
	//float intensity = (theta - light.outer) / epsilon;

	float diff = max(dot(-lightDir, normal), 0.0f);
	vec3 reflectDir = normalize(reflect(lightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), uShininess);
	
	vec3 matColor = vec3(texture(uTexture0, TexCoord) * uHasTex + vec4(uColor, 1.0f) * (1 - uHasTex));
	vec3 ambient = light.ambInt * light.color * matColor * intensity;
	vec3 diffuse = light.difInt * diff * light.color * matColor * intensity;
	vec3 specular = light.speInt * spec * light.color * matColor * intensity;

	//阴影
	vec3 shadowCoord = pos4light.xyz / pos4light.w;
	shadowCoord = shadowCoord * 0.5 + 0.5;
	float closestDepth = texture(shadowMap, shadowCoord.xy).r;
	float currentDepth = shadowCoord.z;
	//暂时没考虑阴影失真
	//float bias = max(0.05f * (1.0f - dot(normal, -lightDir)), 0.005f);
	//超出光源视锥体的地方需要特殊处理
	float shadow = (currentDepth > (closestDepth) && currentDepth <= 1.0f && uRecieveShadow && light.castShadow) ? 0.0f : 1.0f;

	return shadow * (ambient + diffuse + specular);
}
