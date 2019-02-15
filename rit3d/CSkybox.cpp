#include "pch.h"
#include "CSkybox.h"
#include "GLProgram.h"
#include "Application.h"
#include "ResourceManager.h"

CSkybox::CSkybox() {
	//创建cubetex的vao
	RFloat vertices[] = {
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,

		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,

		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f
	};
	RUInt indices[] = {
		0,2,1,0,3,2,4,6,5,4,7,6,8,10,9,8,11,10,12,14,13,12,15,14,16,18,17,16,19,18,20,22,21,20,23,22
	};

	glGenVertexArrays(1, &m_boxVAO);
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_ebo);

	glBindVertexArray(m_boxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 72 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 36 * sizeof(unsigned int), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//创建cubetex的纹理

	glGenTextures(1, &m_cubeTexture);

	m_shader = Application::Instance()->resourceMng->getShader("skybox");
}
CSkybox::~CSkybox() {
	glDeleteVertexArrays(1, &m_boxVAO);
	glDeleteBuffers(1, &m_vbo);
	glDeleteBuffers(1, &m_ebo);

	glDeleteTextures(1, &m_cubeTexture);
}

CSkybox* CSkybox::CreateInstance() {
	return new CSkybox();
}

//设置天空盒纹理，右左上下前后
void CSkybox::setTextures(std::vector<RString> _path) {
	if (_path.size() < 6) {
		cout << "skybox::图片不够六张！" << endl;
	}
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_cubeTexture);
	int width, height, nrChannels;
	unsigned char *data;
	for (RUInt i = 0; i < 6; i++) {
		data = stbi_load(_path[i].c_str(), &width, &height, &nrChannels, 0);
		if (data) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height,
				0, GL_RGB, GL_UNSIGNED_BYTE, data);
		}
		else {
			cout << "skybox::图片加载失败！" << endl;
			return;
		}
		stbi_image_free(data);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}

//获取天空盒vao
RUInt CSkybox::getBoxVAO() const {
	return m_boxVAO;
}

//获取i立方体贴图纹理
RUInt CSkybox::getBoxTexture() const {
	return m_cubeTexture;
}

void CSkybox::setShader(const RString& _texName) {
	GLProgram* shader = Application::Instance()->resourceMng->getShader(_texName);
	if (nullptr == shader) {
		cout << "skybox::shader不存在！" << endl;
		return;
	}
	m_shader = shader;
}

GLProgram* CSkybox::getShader() const {
	return m_shader;
}