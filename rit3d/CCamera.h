#pragma once
#include "RCommon.h"
#include "IComponent.h"

class CCamera : public IComponent
{
private:
	CCamera();
	virtual ~CCamera();

	RFloat m_near{ 0.1f };
	RFloat m_far{ 100.0f };
	RFloat m_asp{ 800.0f / 600.0f };
	RFloat m_fov{ 45.0f };
	RFloat m_size{ 100.0f };

	CAMERATYPE m_cameraType{ CAMERATYPE::PERSPECTIVE };

	glm::mat4 m_viewMatrix{ 1.0f };
	glm::mat4 m_projMatrix{ 1.0f };

	RBool m_projDirty{ true };

	RUInt m_cullMask{ LAYER::Default };

	RUInt m_framebuffer;//帧缓冲，为后处理提供接口
	RUInt m_colorTex;//绑定在帧缓冲上的纹理

public:
	RUInt getCullMask() const;
	void setCullMask(RUInt _mask);

	RBool clearColor{ true };//清除深度缓冲
	RBool clearDepth{ true };//清除颜色缓冲

	glm::vec4 backgroundColor{ 0.2f, 0.3f, 0.3f, 1.0f };//背景色

	RUInt order;

	CAMERATYPE getCameraType() const;
	void setCameraType(CAMERATYPE type);

	RFloat getNear() const;
	void setNear(RFloat _n);

	RFloat getFar() const;
	void setFar(RFloat _f);

	RFloat getAsp() const;
	void setAsp(RFloat _a);

	RFloat getFov() const;
	void setFov(RFloat _f);

	RFloat getSize() const;
	void setSize(RFloat _s);

	//设置视锥体
	void setOrthoFrustum(RFloat _fov, RFloat _asp, RFloat _near, RFloat _far);
	void setPerspFrustum(RFloat _size, RFloat _asp, RFloat _near, RFloat _far);

	//获取投影矩阵
	glm::mat4 getProjMatrix();
	//获取视图矩阵
	glm::mat4 getViewMatrix();

	//获取帧缓冲对象
	RUInt getFramebuffer();
	//获取帧缓冲上的颜色纹理对象
	RUInt getColorTex();

	static CCamera* CreateInstance();
};

