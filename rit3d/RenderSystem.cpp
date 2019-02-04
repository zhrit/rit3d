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
void RenderSystem::_preRender(RScene* pSce) {
	//shadow map
	glEnable(GL_DEPTH_TEST);
	std::list<CLight*> lightList = pSce->getLightList();
	for (auto light : lightList) {
		if (light->getLightType() == LIGHTTYPE::DIRECTION && light->isCastShadow()) {
			glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
			glBindFramebuffer(GL_FRAMEBUFFER, light->getFramebuffer());
			glClear(GL_DEPTH_BUFFER_BIT);
			//渲染
			GLProgram* shader = Application::Instance()->resourceMng->getShader("shadowMap");
			shader->use();
			for (auto it : pSce->getGameObjectList()) {
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
		}
	}
}

//主渲染
void RenderSystem::_mainRender(CCamera* camera, RScene* pSce) {
	std::list<CLight*> lightList = pSce->getLightList();
	glClearColor(camera->backgroundColor.r, camera->backgroundColor.g, camera->backgroundColor.b, camera->backgroundColor.a);
	glViewport(0, 0, DEFAULT_WIDTH, DEFAULT_HEIGHT);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
	glEnable(GL_DEPTH_TEST);
	// create transformations
	//glm::mat4 view = camera->getViewMatrix(); // make sure to initialize matrix to identity matrix first
	//glm::mat4 projection = camera->getProjMatrix();
	for (auto it : pSce->getGameObjectList()) {
		CTransform* trans = it->transform;
		CRender* rend = (CRender*)it->getComponent(RENDER);
		if (rend != nullptr) {
			//材质中添加灯光define
			if (rend->m_mat->isUseLight()) {
				if (pSce->getLightNum(LIGHTTYPE::DIRECTION) > 0)
					rend->m_mat->addDefine("DIR_LIGHT_NUM", util::num2str(pSce->getLightNum(LIGHTTYPE::DIRECTION)));
				if (pSce->getLightNum(LIGHTTYPE::POINT) > 0)
					rend->m_mat->addDefine("POI_LIGHT_NUM", util::num2str(pSce->getLightNum(LIGHTTYPE::POINT)));
				if (pSce->getLightNum(LIGHTTYPE::SPOT) > 0)
					rend->m_mat->addDefine("SPO_LIGHT_NUM", util::num2str(pSce->getLightNum(LIGHTTYPE::SPOT)));
				_updateLightsUniforms(rend->m_mat, lightList);
			}
			//获取shader
			rend->m_mat->getShader()->use();
			_updateUniforms(rend->m_mat, camera, trans, lightList);
			glBindVertexArray(rend->m_mesh->getVAO());

			//glDrawArrays(GL_TRIANGLES, 0, rend->m_mesh->getVertexCount());
			glDrawElements(GL_TRIANGLES, rend->m_mesh->getFaceCount() * 3, GL_UNSIGNED_INT, 0);
		}
	}
}
//后渲染
void RenderSystem::_postRender() {

}

//核心渲染函数
void RenderSystem::_render() {
	Application* app = Application::Instance();
	RScene* pSce = app->sceneMng->getAllScene().front();
	std::list<CCamera*> cameraList = pSce->getCameraList();
	//std::list<CLight*> lightList = pSce->getLightList();
	for (auto camera : cameraList) {
		_preRender(pSce);
		_mainRender(camera, pSce);
	}
}

//更新uniform变量
void RenderSystem::_updateUniforms(Material* pMat, CCamera* camera, CTransform* tran, std::list<CLight*> lights) {
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
				tex->use(0);
				shader->setInt(sName, 0);
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
	}

	delete[] uniformName;
}

//更新光源相关的uniform变量
void RenderSystem::_updateLightsUniforms(Material* pMat, std::list<CLight*> lights) {
	GLProgram* shader = pMat->getShader();

	RUInt i = 0, j = 0, k = 0;

	for (auto light : lights) {
		LIGHTTYPE type = light->getLightType();
		switch (type)
		{
		case LIGHTTYPE::DIRECTION :
			shader->setVec3("uDirLights["+ util::num2str(i) +"].direction", light->gameObject->transform->getLocalFrontDir());
			shader->setVec3("uDirLights[" + util::num2str(i) + "].color", light->getColor());
			shader->setFloat("uDirLights[" + util::num2str(i) + "].ambInt", light->getAmbInt());
			shader->setFloat("uDirLights[" + util::num2str(i) + "].difInt", light->getDifInt());
			shader->setFloat("uDirLights[" + util::num2str(i) + "].speInt", light->getSpeInt());

			shader->setMat4("uLightSpaceMatrix[" + util::num2str(i) + "]", light->getLightSpaceMatrix());

			shader->setInt("uDirShadowMap[" + util::num2str(i) + "]", i + 2);
			glActiveTexture(0x84C0 + 2 + i);
			glBindTexture(GL_TEXTURE_2D, light->getDepthMap());
			i++;
			break;
		case LIGHTTYPE::POINT:
			shader->setVec3("uPoiLights[" + util::num2str(j) + "].position", light->gameObject->transform->getLocalPosition());
			shader->setVec3("uPoiLights[" + util::num2str(j) + "].color", light->getColor());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].ambInt", light->getAmbInt());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].difInt", light->getDifInt());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].speInt", light->getSpeInt());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].kc", light->getKc());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].kl", light->getKl());
			shader->setFloat("uPoiLights[" + util::num2str(j) + "].kq", light->getKq());
			j++;
			break;
		case LIGHTTYPE::SPOT:
			shader->setVec3("uSpoLights[" + util::num2str(k) + "].position", light->gameObject->transform->getLocalPosition());
			shader->setVec3("uSpoLights[" + util::num2str(k) + "].direction", light->gameObject->transform->getLocalFrontDir());
			shader->setVec3("uSpoLights[" + util::num2str(k) + "].color", light->getColor());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].ambInt", light->getAmbInt());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].difInt", light->getDifInt());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].speInt", light->getSpeInt());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].inner", light->getCutInner());
			shader->setFloat("uSpoLights[" + util::num2str(k) + "].outer", light->getCutOuter());
			k++;
			break;
		default:
			break;
		}
	}
}
