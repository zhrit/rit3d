#pragma once
#include "RCommon.h"
#include "IComponent.h"

struct Viewport {
	int x;
	int y;
	int w;
	int h;
};

//渲染方式
enum RENDERMODEL {
	FORWARD = 0,
	DEFER,
};

//相机类型
typedef enum {
	ORTHO = 0,
	PERSPECTIVE = 1,
} CAMERATYPE;

class CCamera : public IComponent
{
private:
	CCamera();
	virtual ~CCamera();

	//视锥体
	RFloat m_near{ 0.1f };
	RFloat m_far{ 100.0f };
	RFloat m_asp{ 800.0f / 600.0f };
	RFloat m_fov{ 45.0f };
	RFloat m_size{ 100.0f };

	CAMERATYPE m_cameraType{ CAMERATYPE::PERSPECTIVE };

	glm::mat4 m_viewMatrix{ 1.0f };
	glm::mat4 m_projMatrix{ 1.0f };

	RBool m_projDirty{ true };

	Viewport m_viewport{ 0, 0, 800, 600 };

	//图层
	RUInt m_cullMask{ LAYER::Default };
	//渲染目标
	RUInt m_framebuffer;//帧缓冲，为后处理提供接口
	RUInt m_colorTexs[2];//绑定在帧缓冲上的纹理

	//HDR参数
	RFloat m_exposure{ 1.0f };//曝光参数
	RFloat m_bloom{ 0.0f };//泛光阈值

	//渲染方式，正向渲染或延迟渲染
	RENDERMODEL m_renderModel{ RENDERMODEL::FORWARD };
	RUInt m_gBuffer;//延迟渲染用到的framebuffer
	RUInt m_gTextures[3];//延迟渲染用到的纹理 位置 法线 颜色
	RUInt m_rboDepth;//延迟渲染用到的深度缓冲

public:
	RUInt getCullMask() const;
	void setCullMask(RUInt _mask);

	RBool clearColor{ true };//清除深度缓冲
	RBool clearDepth{ true };//清除颜色缓冲

	glm::vec4 backgroundColor{ 0.0f, 0.0f, 0.0f, 1.0f };//背景色

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

	RFloat getExposure() const;
	void setExposure(RFloat _e);

	RFloat getBloom() const;
	void setBloom(RFloat _b);

	//设置渲染模式
	RENDERMODEL getRenderModel() const;
	void setRenderModel(RENDERMODEL _m);
	//获取gBuffer和gTexture
	RUInt getGBuffer() const;
	RUInt getGTexture(RInt ind) const;

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
	RUInt getColorTex(RUInt ind);

	void setViewport(int _x, int _y, int _w, int _h);
	Viewport getViewport() const;

	static CCamera* CreateInstance();
};

