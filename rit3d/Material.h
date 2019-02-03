#pragma once
#include "RCommon.h"
#include "GLProgram.h"
#include "Texture.h"

class RUniform {
public:
	GLint location;
	GLenum type;
	RString name;
};

class Material {
public:
	static Material* Create(RString _name);
	static void Destroy(Material* _mat);
private:
	Material(RString _name);
	~Material();

	RString m_name;//材质名称
	glm::vec3 m_color{ 1.0f,1.0f,1.0f };//材质主颜色
	glm::vec3 m_ambient{ 1.0f,1.0f,1.0f };//材质环境光颜色
	glm::vec3 m_diffuse{ 1.0f,1.0f,1.0f };//材质漫反射颜色
	glm::vec3 m_specular{ 1.0f,1.0f,1.0f };//材质镜面反射颜色
	RFloat m_shininess{ 32.0 };//材质镜面反射高光系数
	std::vector<Texture*> m_texList;//纹理列表
	RBool m_useLight{ true };//是否使用灯光效果

	RString m_shaderName{ "" };//shader的名称
	GLProgram* m_shader{ nullptr };//shader对象

	std::map<RString, RString> m_defines;//shader中用到的宏定义

	RBool m_shaderDirty{ true };//shader标脏

	void _destroy();

public:
	//shader相关操作
	GLProgram* getShader();
	RString getShaderName() const;
	void setShader(RString _shader);

	//增加define
	void addDefine(RString _key, RString _value);

	glm::vec3 getColor() const;
	void setColor(RFloat r, RFloat g, RFloat b);

	glm::vec3 getAmbient() const;
	void setAmbient(RFloat r, RFloat g, RFloat b);

	glm::vec3 getDiffuse() const;
	void setDiffuse(RFloat r, RFloat g, RFloat b);

	glm::vec3 getSpecular() const;
	void setSpecular(RFloat r, RFloat g, RFloat b);

	RFloat getShininess() const;
	void setShininess(RFloat s);

	void addTexture(Texture* _tex);
	void removeTexture(Texture* _tex);
	void clearTexture();
	Texture* getTexture(RUInt i);
	RUInt getTexNum() const;

	RBool isUseLight() const;
	void useLight();
	void closeLight();
};