#pragma once
#include "RCommon.h"
#include "IComponent.h"

//光照类型
typedef enum {
	NONE = 0,
	DIRECTION,
	LPOINT,
	SPOT,
} LIGHTTYPE;

class CLight : public IComponent {

private:
	CLight();
	virtual ~CLight();

	//--公共属性
	//颜色
	glm::vec3 m_color{ 1.0f, 1.0f, 1.0f };
	//ambient强度
	RFloat m_ambientInt{ 0.1f };
	//diffuse强度
	RFloat m_diffuseInt{ 0.5f };
	//specular强度
	RFloat m_specularInt{ 1.0f };
	//是否投射阴影
	RBool m_castShadow{ false };
	//光源类型
	LIGHTTYPE m_lightType{ LIGHTTYPE::NONE };

	//--平行光属性

	//--点光源属性
	//作用距离
	RFloat m_distance{ 50.0f };
	//衰减系数
	RFloat m_kc{ 1.0f };   //常数项
	RFloat m_kl{ 0.09f };  //线性项
	RFloat m_kq{ 0.032f }; //二次项
	//--聚光灯属性
	//锥角(弧度)
	RFloat m_cutInner{ cos(15.0f / 57.3f) };//内锥角
	RFloat m_cutOuter{ cos(25.0f / 57.3f) };//外锥角

	//--阴影相关
	RUInt m_depthMapFBO;//shadow map的framebuffer
	RUInt m_depthMap;//存放深度贴图的纹理

	RFloat m_near_s{ 0.1f };
	RFloat m_far_s{ 10.0f };
	RFloat m_asp_s{ 1.0f };
	RFloat m_fov_s{ 90.0f };
	RFloat m_size_s{ 10.0f };

	RBool m_projDirty{ true };

	glm::mat4 m_viewMatrix{ 1.0f };
	glm::mat4 m_projMatrix{ 1.0f };

public:
	//公共方法
	glm::vec3 getColor() const;
	void setColor(RFloat r, RFloat g, RFloat b);
	void setColor(glm::vec3 _color);

	void setIntensity(RFloat amb, RFloat dif, RFloat spe);
	RFloat getAmbInt() const;
	RFloat getDifInt() const;
	RFloat getSpeInt() const;

	RBool isCastShadow() const;
	void castShadow(RBool _cast);

	LIGHTTYPE getLightType() const;
	void setLightType(LIGHTTYPE _type);

	//重写setEnabled
	void setEnabled(RBool value);

	//点光源相关

	void setAttenuation(RFloat dis, RFloat _kc, RFloat _kl, RFloat _kq);
	RFloat getKc() const;
	RFloat getKl() const;
	RFloat getKq() const;

	//聚光灯相关
	void setCutOff(RFloat inner, RFloat outer);
	RFloat getCutInner() const;
	RFloat getCutOuter() const;

	//--阴影
	RUInt getFramebuffer() const;
	RUInt getDepthMap() const;

	RFloat getNear() const;
	void setNear(RFloat _n);

	RFloat getFar() const;
	void setFar(RFloat _f);

	RFloat getAsp() const;

	RFloat getFov() const;

	RFloat getSize() const;
	void setSize(RFloat _s);

	//设置视锥体
	void setOrthoFrustum(RFloat _size, RFloat _near, RFloat _far);
	void setPerspFrustum(RFloat _near, RFloat _far);

	//获取视图投影矩阵
	glm::mat4 getLightSpaceMatrix();
	//获取视图投影矩阵
	std::vector<glm::mat4> getLightSpaceMatrixs();

private:
	//获取投影矩阵
	glm::mat4 _getProjMatrix();
	//获取视图矩阵(平行光)
	glm::mat4 _getViewMatrix();
	//获取视图矩阵(点光源)
	std::vector<glm::mat4> _getViewMatrixs();

public:

	static CLight* CreateInstance();

};

