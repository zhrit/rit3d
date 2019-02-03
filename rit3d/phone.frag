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
};

#ifdef DIR_LIGHT_NUM
uniform DirLight uDirLights[DIR_LIGHT_NUM];
#endif
#ifdef POI_LIGHT_NUM
uniform PoiLight uPoiLights[POI_LIGHT_NUM];
#endif
#ifdef SPO_LIGHT_NUM
uniform SpoLight uSpoLights[SPO_LIGHT_NUM];
#endif

uniform vec3 uColor;
uniform int uHasTex;
uniform sampler2D uTexture0;
uniform vec3 uViewPos;
uniform float uShininess;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPoiLight(PoiLight light, vec3 normal, vec3 viewDir, vec3 fragPos);
vec3 CalcSpoLight(SpoLight light, vec3 normal, vec3 viewDir, vec3 fragPos);

void main() {
	vec3 norm = normalize(fragNormal);
	vec3 viewDir = normalize(uViewPos - fragPos);

	vec3 result = vec3(0.0f, 0.0f, 0.0f);
	#ifdef DIR_LIGHT_NUM
	for(int i = 0; i < DIR_LIGHT_NUM; i++) {
		result += CalcDirLight(uDirLights[i], norm, viewDir);
	}
	#endif
	#ifdef POI_LIGHT_NUM
	for(int i = 0; i < POI_LIGHT_NUM; i++) {
		result += CalcPoiLight(uPoiLights[i], norm, viewDir, fragPos);
	}
	#endif
	#ifdef SPO_LIGHT_NUM
	for(int i = 0; i < SPO_LIGHT_NUM; i++) {
		result += CalcSpoLight(uSpoLights[i], norm, viewDir, fragPos);
	}
	#endif
    FragColor = vec4(result, 1.0f);
	//FragColor = texture(texture0, TexCoord) * hasTex + vec4(color, 1.0f) * (1 - hasTex);
}

//平行光效果计算
vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir) {
	vec3 lightDir = normalize(light.direction);
	float diff = max(dot(-lightDir, normal), 0.0f);
	vec3 reflectDir = normalize(reflect(lightDir, normal));
	float spec =  pow(max(dot(reflectDir, viewDir), 0.0f), uShininess);

	vec3 matColor = vec3(texture(uTexture0, TexCoord) * uHasTex + vec4(uColor, 1.0f) * (1 - uHasTex));
	vec3 ambient = light.ambInt * light.color * matColor;
	vec3 diffuse = light.difInt * diff * light.color * matColor;
	vec3 specular = light.speInt * spec * light.color * matColor;

	return (ambient + diffuse + specular);
}

//点光源效果计算
vec3 CalcPoiLight(PoiLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
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

	return (ambient + diffuse + specular);

}

//聚光灯效果计算
vec3 CalcSpoLight(SpoLight light, vec3 normal, vec3 viewDir, vec3 fragPos) {
	vec3 lightDir = normalize(fragPos - light.position);
	float theta = dot(lightDir, normalize(light.direction));
	float epsilon = light.inner - light.outer;
	float intensity = clamp((theta - light.outer) / epsilon, 0.0f, 1.0f);

	float diff = max(dot(-lightDir, normal), 0.0f);
	vec3 reflectDir = normalize(reflect(lightDir, normal));
	float spec = pow(max(dot(reflectDir, viewDir), 0.0f), uShininess);
	
	vec3 matColor = vec3(texture(uTexture0, TexCoord) * uHasTex + vec4(uColor, 1.0f) * (1 - uHasTex));
	vec3 ambient = light.ambInt * light.color * matColor * intensity;
	vec3 diffuse = light.difInt * diff * light.color * matColor * intensity;
	vec3 specular = light.speInt * spec * light.color * matColor * intensity;

	return (ambient + diffuse + specular);
}
