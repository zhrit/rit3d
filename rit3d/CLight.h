#pragma once
#include "RCommon.h"
#include "IComponent.h"

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
	LIGHTTYPE m_lightType{ LIGHTTYPE::DIRECTION };

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
	RFloat m_cutInner{ 15.0f / 57.3f };//内锥角
	RFloat m_cutOuter{ 25.0f / 57.3f };//外锥角

public:
	glm::vec3 getColor() const;
	void setColor(RFloat r, RFloat g, RFloat b);
	void setColor(glm::vec3 _color);

	void setIntensity(RFloat amb, RFloat dif, RFloat spe);

	RBool isCastShadow() const;
	void castShadow(RBool _cast);

	LIGHTTYPE getLightType() const;
	void setLightType(LIGHTTYPE _type);

	void setAttenuation(RFloat dis, RFloat _kc, RFloat _kl, RFloat _kq);

	void setCutOff(RFloat inner, RFloat outer);

	RFloat getAmbInt() const;
	RFloat getDifInt() const;
	RFloat getSpeInt() const;

	static CLight* CreateInstance();
};

