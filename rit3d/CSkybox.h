#pragma once
#include "RCommon.h"
#include "IComponent.h"
#include "GLProgram.h"

class CSkybox : public IComponent {
private:
	CSkybox();
	~CSkybox();

	RUInt m_boxVAO;//天空盒的立方体vao对象
	RUInt m_vbo;
	RUInt m_ebo;

	std::vector<RString> m_texName;//右左上下前后

	RUInt m_cubeTexture;//立方体贴图纹理

	GLProgram* m_shader;//渲染天空盒用的着色器

public:
	//设置天空盒纹理，右左上下前后
	void setTextures(std::vector<RString> _path);

	//获取天空盒vao
	RUInt getBoxVAO() const;

	//获取i立方体贴图纹理
	RUInt getBoxTexture() const;

	void setShader(const RString& _texName);

	GLProgram* getShader() const;

	static CSkybox* CreateInstance();
};