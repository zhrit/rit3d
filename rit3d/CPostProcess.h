#pragma once
#include "RCommon.h"
#include "IComponent.h"
#include "GLProgram.h"

//只有加到有相机组件的对象下才有用
//需要用户自己写shader并设置shader路径
class CPostProcess : public IComponent {
private:
	CPostProcess();
	virtual ~CPostProcess();

	RString m_shaderPath{ "postProcess" };//没有.frag后缀

	GLProgram* m_shader;

public:
	//设置shader路径
	RString getShaderPath() const;
	void setShaderPath(const RString& _path);

	GLProgram* getShader() const;

	static CPostProcess* CreateInstance();
};
