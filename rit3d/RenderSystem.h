#pragma once
#include "RCommon.h"
#include "ISystem.h"

class CCamera;
class CRender;
class CLight;
class CTransform;
class Material;
class RScene;
class RFramebuffer {
public:
	RFramebuffer() {
		glGenFramebuffers(1, &fbo);
		glGenTextures(1, &colorTex);
		glBindFramebuffer(GL_FRAMEBUFFER, fbo);
		glBindTexture(GL_TEXTURE_2D, colorTex);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, DEFAULT_WIDTH, DEFAULT_HEIGHT, 0, GL_RGB, GL_FLOAT, NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorTex, 0);
	}
	~RFramebuffer() {
		glDeleteFramebuffers(1, &fbo);
		glDeleteTextures(1, &colorTex);
	}
	RUInt fbo;
	RUInt colorTex;
};

class RenderSystem : public ISystem {
private:
	RenderSystem(RInt od);
	virtual ~RenderSystem();
public:
	static RenderSystem* CreateInstance(RInt);

	//系统初始化时调用
	virtual void onAwake();

	//系统被激活时调用
	virtual void onEnable();

	//系统开始运行时调用
	virtual void onStart();

	//实体被添加到系统时调用
	virtual void onAddGameObject();

	//组件添加到实体时调用
	virtual void onAddComponent();

	//组件从实体移除时调用
	virtual void onRemoveComponent();

	//实体从系统中移除时调用
	virtual void onRemoveGameObject();

	//系统更新时调用
	virtual void onUpdate();

	//系统更新时onUpdate之后调用
	virtual void onLateUpdate();

	//系统被禁用时调用
	virtual void onDisable();

	//系统被注销时调用
	virtual void onDestroy();

private:
	//预渲染
	void _preRender(CCamera* camera, RScene* pSce);
	//主渲染
	void _mainRender(CCamera* camera, RScene* pSce);
	//后渲染
	void _postRender(CCamera* camera);
	//核心渲染函数
	void _render();

	//更新uniform变量
	void _updateUniforms(CRender* pRender, CCamera* camera, CTransform* tran, std::list<CLight*> lights);

	//更新光源相关的uniform变量
	void _updateLightsUniforms(Material* pMat, std::list<CLight*> lights);

	//vao对象供后处理使用
	RUInt m_rectVAO;

	//texture计数器
	RUInt m_texCounter{ 0 };
	//texture分配器
	RUInt _allocTexture();
	//texture分配器状态重置
	void _resetTexAlloc();

	//framebuffer池
	std::vector<RFramebuffer*> m_framebufferPool;
	//取出一个framebuffer
	RFramebuffer* _popFramebuffer();
	//用完的framebuffer重新放入buffer池
	void _pushFramebuffer(RFramebuffer* pf);
};