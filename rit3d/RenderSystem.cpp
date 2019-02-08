#include "pch.h"
#include "RenderSystem.h"
#include "Application.h"

RenderSystem::RenderSystem(RInt od) : ISystem(od) {
	m_type = RENDERSYSTEM;
}
RenderSystem::~RenderSystem() {

}

RenderSystem* RenderSystem::CreateInstance(RInt od) {
	return new RenderSystem(od);
}

//系统初始化时调用
void RenderSystem::onAwake() {
	RFloat vertices[] = {
		-1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
		1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
		1.0f, 1.0f, 0.0f, 1.0f, 1.0f
	};
	RUInt indices[] = {
		0, 1, 2,
		0, 2, 3
	};
	//创建attay buffer
	RUInt vbo, ebo;
	glGenVertexArrays(1, &m_rectVAO);
	glGenBuffers(1, &vbo);
	glGenBuffers(1, &ebo);

	glBindVertexArray(m_rectVAO);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(float), vertices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 6 * sizeof(unsigned int), indices, GL_STATIC_DRAW);
	//attribute
	//position
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	//uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

//系统被激活时调用
void RenderSystem::onEnable() {

}

//系统开始运行时调用
void RenderSystem::onStart() {

}

//实体被添加到系统时调用
void RenderSystem::onAddGameObject() {

}

//组件添加到实体时调用
void RenderSystem::onAddComponent() {

}

//组件从实体移除时调用
void RenderSystem::onRemoveComponent() {

}

//实体从系统中移除时调用
void RenderSystem::onRemoveGameObject() {

}

//系统更新时调用
void RenderSystem::onUpdate() {
	_render();
}

//系统更新时onUpdate之后调用
void RenderSystem::onLateUpdate() {

}

//系统被禁用时调用
void RenderSystem::onDisable() {

}

//系统被注销时调用
void RenderSystem::onDestroy() {

}

//预渲染
void RenderSystem::_preRender(CCamera* camera, RScene* pSce) {
	//shadow map
	glEnable(GL_DEPTH_TEST);
	std::list<CLight*> lightList = pSce->getLightList();
	for (auto light : lightList) {
		if (light->isActiveAndEnabled() && light->isCastShadow()) {
			LIGHTTYPE lightType = light->getLightType();
			switch (lightType)
			{
			case LIGHTTYPE::SPOT:
			case LIGHTTYPE::DIRECTION:
			{
				glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				glBindFramebuffer(GL_FRAMEBUFFER, light->getFramebuffer());
				glClear(GL_DEPTH_BUFFER_BIT);
				//渲染
				GLProgram* shader = Application::Instance()->resourceMng->getShader("shadowMap");
				shader->use();
				for (auto it : pSce->getGameObjectList()) {
					if ((it->getLayer() & camera->getCullMask()) == 0) continue;
					CTransform* trans = it->transform;
					CRender* rend = (CRender*)it->getComponent(RENDER);
					if (rend != nullptr && rend->isCastShadow()) {
						shader->setMat4("uModel", trans->getModelMatrix());
						shader->setMat4("uLightSpaceMatrix", light->getLightSpaceMatrix());

						glBindVertexArray(rend->m_mesh->getVAO());

						//glDrawArrays(GL_TRIANGLES, 0, rend->m_mesh->getVertexCount());
						glDrawElements(GL_TRIANGLES, rend->m_mesh->getFaceCount() * 3, GL_UNSIGNED_INT, 0);
					}
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				break;
			}
			case LIGHTTYPE::LPOINT:
			{
				glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
				glBindFramebuffer(GL_FRAMEBUFFER, light->getFramebuffer());
				glClear(GL_DEPTH_BUFFER_BIT);
				//渲染
				GLProgram* shader = Application::Instance()->resourceMng->getShader("shadowCubemap");
				shader->use();
				for (auto it : pSce->getGameObjectList()) {
					if ((it->getLayer() & camera->getCullMask()) == 0) continue;
					CTransform* trans = it->transform;
					CRender* rend = (CRender*)it->getComponent(RENDER);
					if (rend != nullptr && rend->isCastShadow()) {
						//传uniform
						shader->setMat4("uModel", trans->getModelMatrix());
						shader->setVec3("uLightPos", light->gameObject->transform->getLocalPosition());
						shader->setFloat("uFarPlane", light->getFar());
						std::vector<glm::mat4> lightSpace = light->getLightSpaceMatrixs();
						for (int i = 0; i < 6; i++) {
							shader->setMat4("uShadowMatrices[" + util::num2str(i) + "]", lightSpace[i]);
						}
						glBindVertexArray(rend->m_mesh->getVAO());

						glDrawElements(GL_TRIANGLES, rend->m_mesh->getFaceCount() * 3, GL_UNSIGNED_INT, 0);
					}
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				break;
			}
			default:
				break;
			}
		}
	}
}
//主渲染
void RenderSystem::_mainRender(CCamera* camera, RScene* pSce) {
	//绑定相机的帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, camera->getFramebuffer());
	std::list<CLight*> lightList = pSce->getLightList();
	glClearColor(camera->backgroundColor.r, camera->backgroundColor.g, camera->backgroundColor.b, camera->backgroundColor.a);
	glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
	glEnable(GL_DEPTH_TEST);
	// create transformations
	//glm::mat4 view = camera->getViewMatrix(); // make sure to initialize matrix to identity matrix first
	//glm::mat4 projection = camera->getProjMatrix();
	for (auto it : pSce->getGameObjectList()) {
		if ((it->getLayer() & camera->getCullMask()) == 0) continue;
		CTransform* trans = it->transform;
		CRender* rend = (CRender*)it->getComponent(RENDER);
		if (rend != nullptr) {
			//材质中添加灯光define
			if (rend->m_mat->isUseLight()) {
				if (pSce->getLightNum(LIGHTTYPE::DIRECTION) > 0)
					rend->m_mat->addDefine("DIR_LIGHT_NUM", util::num2str(pSce->getLightNum(LIGHTTYPE::DIRECTION)));
				if (pSce->getLightNum(LIGHTTYPE::LPOINT) > 0)
					rend->m_mat->addDefine("POI_LIGHT_NUM", util::num2str(pSce->getLightNum(LIGHTTYPE::LPOINT)));
				if (pSce->getLightNum(LIGHTTYPE::SPOT) > 0)
					rend->m_mat->addDefine("SPO_LIGHT_NUM", util::num2str(pSce->getLightNum(LIGHTTYPE::SPOT)));
				_updateLightsUniforms(rend->m_mat, lightList);
			}
			//获取shader
			rend->m_mat->getShader()->use();
			_updateUniforms(rend, camera, trans, lightList);
			glBindVertexArray(rend->m_mesh->getVAO());

			//glDrawArrays(GL_TRIANGLES, 0, rend->m_mesh->getVertexCount());
			glDrawElements(GL_TRIANGLES, rend->m_mesh->getFaceCount() * 3, GL_UNSIGNED_INT, 0);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//后渲染
void RenderSystem::_postRender(CCamera* camera) {
	glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
	glDisable(GL_DEPTH_TEST);
	
	CPostProcess* cpost = (CPostProcess*)camera->gameObject->getComponent(COMPTYPE::POSTPROCESS);
	GLProgram* shader;
	if (cpost == nullptr) {
		shader = Application::Instance()->resourceMng->getShader("postProcess");
	}
	else {
		shader = cpost->getShader();
	}
	
	//使用后处理shader
	shader->use();
	//绑定主渲染得到的texture
	RUInt ind = _allocTexture();
	shader->setInt("uTexture", ind);
	glActiveTexture(0x84C0 + ind);
	glBindTexture(GL_TEXTURE_2D, camera->getColorTex());

	glBindVertexArray(m_rectVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

//核心渲染函数
void RenderSystem::_render() {
	Application* app = Application::Instance();
	RScene* pSce = app->sceneMng->getAllScene().front();
	std::list<CCamera*> cameraList = pSce->getCameraList();
	//std::list<CLight*> lightList = pSce->getLightList();
	//_preRender(pSce);
	for (auto camera : cameraList) {
		_resetTexAlloc();
		_preRender(camera, pSce);
		_mainRender(camera, pSce);
		_postRender(camera);
	}
}

//更新uniform变量
void RenderSystem::_updateUniforms(CRender* pRender, CCamera* camera, CTransform* tran, std::list<CLight*> lights) {
	Material* pMat = pRender->m_mat;
	GLProgram* shader = pMat->getShader();
	RInt maxUniformLen;//uniform名称最大长度
	RInt numUniforms;//保存uniform的数量

	RChar* uniformName;
	//获取uniform的数量
	glGetProgramiv(shader->ID, GL_ACTIVE_UNIFORMS, &numUniforms);
	//获取uniform名称最大长度
	glGetProgramiv(shader->ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxUniformLen);

	uniformName = new RChar[maxUniformLen];

	for (int i = 0; i < numUniforms; i++) {
		RInt size;
		GLenum type;

		glGetActiveUniform(shader->ID, i, maxUniformLen, NULL, &size, &type, uniformName);

		RString sName = (RString)uniformName;
		if (sName == "projection") {
			shader->setMat4(sName, camera->getProjMatrix());
		}
		else if(sName == "view")
		{
			shader->setMat4(sName, camera->getViewMatrix());
		}
		else if (sName == "model") {
			shader->setMat4(sName, tran->getModelMatrix());
		}
		else if (sName == "nmodel") {
			shader->setMat4(sName, glm::inverse(glm::transpose(tran->getModelMatrix())));
		}
		else if (sName == "uColor") {
			shader->setVec3(sName, pMat->getColor());
		}
		else if (sName == "uHasTex") {
			int has = pMat->getTexNum() > 0 ? 1 : 0;
			shader->setInt(sName, has);
		}
		else if (sName == "uTexture0") {
			Texture* tex = pMat->getTexture(0);
			if (tex != nullptr) {
				RUInt ind = _allocTexture();
				tex->use(ind);
				shader->setInt(sName, ind);
			}
			//glActiveTexture(0x84C0);
			//glBindTexture(GL_TEXTURE_2D, lights.front()->getDepthMap());
		}
		else if (sName == "uTexture1") {
			Texture* tex = pMat->getTexture(1);
			if (tex != nullptr) {
				tex->use(1);
				shader->setInt(sName, 1);
			}
		}
		else if (sName == "uViewPos") {
			shader->setVec3(sName, camera->gameObject->transform->getPosition());//可能有问题
		}
		else if (sName == "uShininess") {
			shader->setFloat(sName, pMat->getShininess());
		}
		else if (sName == "uRecieveShadow") {
			shader->setBool(sName, pRender->isRecieveShadow());
		}
	}

	delete[] uniformName;
}

//更新光源相关的uniform变量
void RenderSystem::_updateLightsUniforms(Material* pMat, std::list<CLight*> lights) {
	GLProgram* shader = pMat->getShader();

	RUInt i = 0, j = 0, k = 0;

	for (auto light : lights) {
		if (!light->isEnabled()) continue;
		LIGHTTYPE type = light->getLightType();
		switch (type)
		{
		case LIGHTTYPE::NONE:
			cout << "请手动设置灯光类型！" << endl;
			break;
		case LIGHTTYPE::DIRECTION :
		{
			shader->setVec3("uDirLights[" + util::num2str(i) + "].direction", light->gameObject->transform->getLocalFrontDir());
			shader->setVec3("uDirLights[" + util::num2str(i) + "].color", light->getColor());
			shader->setFloat("uDirLights[" + util::num2str(i) + "].ambInt", light->getAmbInt());
			shader->setFloat("uDirLights[" + util::num2str(i) + "].difInt", light->getDifInt());
			shader->setFloat("uDirLights[" + util::num2str(i) + "].speInt", light->getSpeInt());
			shader->setFloat("uDirLights[" + util::num2str(i) + "].castShadow", light->isCastShadow());

			shader->setMat4("uLightSpaceMatrixDir[" + util::num2str(i) + "]", light->getLightSpaceMatrix());

			RUInt ind = _allocTexture();
			shader->setInt("uDirShadowMap[" + util::num2str(i) + "]", ind);
			glActiveTexture(0x84C0 + ind);
			glBindTexture(GL_TEXTURE_2D, light->getDepthMap());
			i++;
			break;
		}
		case LIGHTTYPE::LPOINT:
		{
			shader->setVec3("uPoiLights[" + util::num2str(j) + "].position", light->gameObject->transform->getLocalPosition());
			shader->setVec3("uPoiLights[" + util::num2str(j) + "].color", light->getColor());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].ambInt", light->getAmbInt());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].difInt", light->getDifInt());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].speInt", light->getSpeInt());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].kc", light->getKc());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].kl", light->getKl());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].kq", light->getKq());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].castShadow", light->isCastShadow());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].far", light->getFar());

			RUInt ind = _allocTexture();
			shader->setInt("uPoiShadowMap[" + util::num2str(j) + "]", ind);
			glActiveTexture(0x84C0 + ind);
			glBindTexture(GL_TEXTURE_CUBE_MAP, light->getDepthMap());
			j++;
			break;
		}
		case LIGHTTYPE::SPOT:
		{
			shader->setVec3("uSpoLights[" + util::num2str(k) + "].position", light->gameObject->transform->getLocalPosition());
			shader->setVec3("uSpoLights[" + util::num2str(k) + "].direction", light->gameObject->transform->getLocalFrontDir());
			shader->setVec3("uSpoLights[" + util::num2str(k) + "].color", light->getColor());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].ambInt", light->getAmbInt());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].difInt", light->getDifInt());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].speInt", light->getSpeInt());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].inner", light->getCutInner());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].outer", light->getCutOuter());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].castShadow", light->isCastShadow());

			shader->setMat4("uLightSpaceMatrixSpo[" + util::num2str(k) + "]", light->getLightSpaceMatrix());

			RUInt ind = _allocTexture();
			shader->setInt("uSpoShadowMap[" + util::num2str(k) + "]", ind);
			glActiveTexture(0x84C0 + ind);
			glBindTexture(GL_TEXTURE_2D, light->getDepthMap());
			k++;
			break;
		}
		default:
			break;
		}
	}
}

//texture分配器
RUInt RenderSystem::_allocTexture() {
	RUInt a = m_texCounter;
	m_texCounter++;
	return a;
}
//texture分配器状态重置
void RenderSystem::_resetTexAlloc() {
	m_texCounter = 0;
}
