#include "pch.h"
#include "RenderSystem.h"
#include "Application.h"
#include "ResourceManager.h"
#include "SceneManager.h"
#include "RScene.h"
#include "RGameObject.h"
#include "Material.h"
#include "Mesh.h"
#include "Texture.h"
#include "GLProgram.h"
#include "CCamera.h"
#include "CTransform.h"
#include "CLight.h"
#include "CRender.h"
#include "CSkybox.h"
#include "CPostProcess.h"
#include "util.h"
#include "SCLightCameraCollecter.h"

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
void RenderSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {

}

//组件从实体移除时调用
void RenderSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {

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
	std::list<CLight*> lightList = SCLightCameraCollecter::Instance()->getLightList();
	for (auto light : lightList) {
		if (light->isActiveAndEnabled() && light->isCastShadow()) {
			LIGHTTYPE lightType = light->getLightType();
			switch (lightType)
			{
			case LIGHTTYPE::SPOT:
			case LIGHTTYPE::DIRECTION:
			{
				_resetTexAlloc();
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

						for (auto submesh : rend->m_meshs) {
							glBindVertexArray(submesh->getVAO());

							//glDrawArrays(GL_TRIANGLES, 0, rend->m_mesh->getVertexCount());
							glDrawElements(GL_TRIANGLES, submesh->getFaceCount() * 3, GL_UNSIGNED_INT, 0);
						}
					}
				}
				glBindFramebuffer(GL_FRAMEBUFFER, 0);
				break;
			}
			case LIGHTTYPE::LPOINT:
			{
				_resetTexAlloc();
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

						for (auto submesh : rend->m_meshs) {
							glBindVertexArray(submesh->getVAO());
							glDrawElements(GL_TRIANGLES, submesh->getFaceCount() * 3, GL_UNSIGNED_INT, 0);
						}
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
	glEnable(GL_DEPTH_TEST);
	glViewport(0, 0, 1024, 1024);
	//两个颜色附件
	RUInt attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(1, attachments);
	glClearColor(camera->backgroundColor.r, camera->backgroundColor.g, camera->backgroundColor.b, camera->backgroundColor.a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
	if (camera->getBloom() >= 1.0f) {
		glDrawBuffers(1, attachments + 1);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glDrawBuffers(2, attachments);
	}

	std::list<CLight*> lightList = SCLightCameraCollecter::Instance()->getLightList();
	_resetTexAlloc();
	for (auto it : pSce->getGameObjectList()) {
		if ((it->getLayer() & camera->getCullMask()) == 0) continue;
		CTransform* trans = it->transform;
		CRender* rend = (CRender*)it->getComponent(RENDER);
		if (rend != nullptr) {
			for (std::vector<Mesh*>::size_type i = 0; i < rend->m_meshs.size(); i++) {
				Mesh* submesh = rend->m_meshs[i];
				Material* submat = rend->m_mats[i];
				//材质中添加灯光define
				if (submat->isUseLight()) {
					if (SCLightCameraCollecter::Instance()->getLightNum(LIGHTTYPE::DIRECTION) > 0)
						submat->addDefine("DIR_LIGHT_NUM", util::num2str(SCLightCameraCollecter::Instance()->getLightNum(LIGHTTYPE::DIRECTION)));
					if (SCLightCameraCollecter::Instance()->getLightNum(LIGHTTYPE::LPOINT) > 0)
						submat->addDefine("POI_LIGHT_NUM", util::num2str(SCLightCameraCollecter::Instance()->getLightNum(LIGHTTYPE::LPOINT)));
					if (SCLightCameraCollecter::Instance()->getLightNum(LIGHTTYPE::SPOT) > 0)
						submat->addDefine("SPO_LIGHT_NUM", util::num2str(SCLightCameraCollecter::Instance()->getLightNum(LIGHTTYPE::SPOT)));
				}
				if (camera->getBloom() >= 1.0f) {
					submat->addDefine("BLOOM", "1");
				}
				submat->getShader()->use();
				if (submat->isUseLight()) {
					_updateLightsUniforms(submat->getShader(), lightList);
				}
				_updateUniforms(rend, camera, trans, submat);

				glBindVertexArray(submesh->getVAO());
				glDrawElements(GL_TRIANGLES, submesh->getFaceCount() * 3, GL_UNSIGNED_INT, 0);
			}
		}
	}
	//检查是否有天空盒组件,有则渲染天空盒
	
	CSkybox* skybox = (CSkybox*)camera->gameObject->getComponent(COMPTYPE::SKYBOX);
	glDepthFunc(GL_LEQUAL);
	if (nullptr != skybox) {
		GLProgram* shader = skybox->getShader();
		shader->use();
		shader->setMat4("uView", glm::mat4(glm::mat3(camera->getViewMatrix())));
		shader->setMat4("uProjection", camera->getProjMatrix());

		RUInt ind = _allocTexture();
		shader->setInt("uSkybox", ind);
		glActiveTexture(0x84C0 + ind);
		glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getBoxTexture());

		glBindVertexArray(skybox->getBoxVAO());

		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	}
	glDepthFunc(GL_LESS);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
//后渲染
void RenderSystem::_postRender(CCamera* camera) {
	_resetTexAlloc();
	RFramebuffer* pFramebuffer[2];
	RBool horizontal = true;
	if (camera->getBloom() >= 1.0f) {
		//开启bloom时，用高斯模糊器处理高光区域
		pFramebuffer[0] = _popFramebuffer();
		pFramebuffer[1] = _popFramebuffer();
		RBool first_iteration = true;
		RUInt amout = 10;
		GLProgram* shader0 = Application::Instance()->resourceMng->getShader("gaussianBlur");
		shader0->use();
		for (RUInt i = 0; i < amout; i++) {
			//交替模糊十次
			glBindFramebuffer(GL_FRAMEBUFFER, pFramebuffer[horizontal]->fbo);
			glViewport(0, 0, 1024, 1024);
			shader0->setBool("uHorizontal", horizontal);
			RUInt ind = _allocTexture();
			shader0->setInt("uTexture", ind);
			glActiveTexture(0x84C0 + ind);
			glBindTexture(GL_TEXTURE_2D, first_iteration ? camera->getColorTex(1) : pFramebuffer[!horizontal]->colorTex);
			glBindVertexArray(m_rectVAO);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

			horizontal = !horizontal;
			first_iteration = false;
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		_pushFramebuffer(pFramebuffer[0]);
		_pushFramebuffer(pFramebuffer[1]);
	}

	//渲染到屏幕buffer上
	_resetTexAlloc();
	Viewport viewport = camera->getViewport();
	glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
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
	shader->setFloat("uExposure", camera->getExposure());
	shader->setFloat("uBloomValue", camera->getBloom());
	//绑定主渲染得到的texture
	RUInt ind = _allocTexture();
	shader->setInt("uTexture", ind);
	glActiveTexture(0x84C0 + ind);
	glBindTexture(GL_TEXTURE_2D, camera->getColorTex(0));

	ind = _allocTexture();
	shader->setInt("uBloomMap", ind);
	glActiveTexture(0x84C0 + ind);
	glBindTexture(GL_TEXTURE_2D, 
		camera->getBloom() >= 1.0f ? pFramebuffer[!horizontal]->colorTex : camera->getColorTex(1));
	//glBindTexture(GL_TEXTURE_2D, camera->getColorTex(1));
	glBindVertexArray(m_rectVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

}

//延迟渲染
void RenderSystem::_defferedRender(CCamera* camera, RScene* pSce) {
	//几何信息获取阶段
	_resetTexAlloc();
	glBindFramebuffer(GL_FRAMEBUFFER, camera->getGBuffer());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glViewport(0, 0, 1024, 1024);
	glEnable(GL_DEPTH_TEST);
	GLProgram* shaderGeo = Application::Instance()->resourceMng->getShader("deferGeom");
	shaderGeo->use();
	//shaderGeo->setMat4("uView", camera->getViewMatrix());
	//shaderGeo->setMat4("uProjection", camera->getProjMatrix());
	for (auto it : pSce->getGameObjectList()) {
		if ((it->getLayer() & camera->getCullMask()) == 0) continue;
		CTransform* trans = it->transform;
		CRender* rend = (CRender*)it->getComponent(RENDER);
		if (rend != nullptr) {
			//shaderGeo->setMat4("uModel", trans->getModelMatrix());
			//shaderGeo->setMat4("uNModel", glm::inverse(glm::transpose(trans->getModelMatrix())));
			for (std::vector<Mesh*>::size_type i = 0; i < rend->m_meshs.size(); i++) {
				Mesh* submesh = rend->m_meshs[i];
				Material* submat = rend->m_mats[i];

				_updateUniforms(rend, camera, trans, submat, shaderGeo);

				glBindVertexArray(submesh->getVAO());
				glDrawElements(GL_TRIANGLES, submesh->getFaceCount() * 3, GL_UNSIGNED_INT, 0);
			}
		}
	}
	//光照计算阶段
	_resetTexAlloc();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	Viewport viewport = camera->getViewport();
	glViewport(viewport.x, viewport.y, viewport.w, viewport.h);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // also clear the depth buffer now!
	GLProgram* shaderLight = Application::Instance()->resourceMng->getShader("deferLight");

	//使用后处理shader
	shaderLight->use();
	std::list<CLight*> lightList = SCLightCameraCollecter::Instance()->getLightList();
	_updateLightsUniforms(shaderLight, lightList);
	shaderLight->setFloat("uExposure", camera->getExposure());
	shaderLight->setVec3("uViewPos", camera->gameObject->transform->getPosition());
	//绑定主渲染得到的texture
	RUInt ind = _allocTexture();
	shaderLight->setInt("uPosition", ind);
	glActiveTexture(0x84C0 + ind);
	glBindTexture(GL_TEXTURE_2D, camera->getGTexture(0));
	ind = _allocTexture();
	shaderLight->setInt("uNormal", ind);
	glActiveTexture(0x84C0 + ind);
	glBindTexture(GL_TEXTURE_2D, camera->getGTexture(1));
	ind = _allocTexture();
	shaderLight->setInt("uAlbedoSpec", ind);
	glActiveTexture(0x84C0 + ind);
	glBindTexture(GL_TEXTURE_2D, camera->getGTexture(2));
	glBindVertexArray(m_rectVAO);
	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

//核心渲染函数
void RenderSystem::_render() {
	Application* app = Application::Instance();
	RScene* pSce = app->sceneMng->getAllScene().front();
	std::list<CCamera*> cameraList = SCLightCameraCollecter::Instance()->getCameraList();

	for (auto camera : cameraList) {
		if (RENDERMODEL::FORWARD == camera->getRenderModel()) {
			_preRender(camera, pSce);
			_mainRender(camera, pSce);
			_postRender(camera);
		}
		else {
			_preRender(camera, pSce);
			_defferedRender(camera, pSce);
		}
	}
}

//更新uniform变量
void RenderSystem::_updateUniforms(CRender* pRender, CCamera* camera, CTransform* tran, Material* pMat, GLProgram* shader) {
	if (nullptr == shader) {
		shader = pMat->getShader();
	}
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
		if (sName == "uProjection") {
			shader->setMat4(sName, camera->getProjMatrix());
		}
		else if(sName == "uView")
		{
			shader->setMat4(sName, camera->getViewMatrix());
		}
		else if (sName == "uModel") {
			shader->setMat4(sName, tran->getModelMatrix());
		}
		else if (sName == "uNModel") {
			shader->setMat4(sName, glm::inverse(glm::transpose(tran->getModelMatrix())));
		}
		else if (sName == "uColor") {
			shader->setVec3(sName, pMat->getColor());
		}
		else if (sName == "uHasTex") {
			int has = pMat->getTexNum();
			shader->setInt(sName, has);
		}
		else if (sName == "uTexture0") {
			Texture* tex = pMat->getTexture(0);
			if (tex != nullptr) {
				RUInt ind = _allocTexture();
				tex->use(ind);
				shader->setInt(sName, ind);
			}
		}
		else if (sName == "uTexture1") {
			Texture* tex = pMat->getTexture(1);
			if (tex != nullptr) {
				RUInt ind = _allocTexture();
				tex->use(ind);
				shader->setInt(sName, ind);
			}
		}
		else if (sName == "uTexture2") {
			Texture* tex = pMat->getTexture(2);
			if (tex != nullptr) {
				RUInt ind = _allocTexture();
				tex->use(ind);
				shader->setInt(sName, ind);
			}
		}
		else if (sName == "uTexture3") {
			Texture* tex = pMat->getTexture(3);
			if (tex != nullptr) {
				RUInt ind = _allocTexture();
				tex->use(ind);
				shader->setInt(sName, ind);
			}
		}
		else if (sName == "uNormalMap") {
			Texture* tex = pMat->getNormalMap();
			if (tex != nullptr) {
				RUInt ind = _allocTexture();
				glActiveTexture(0x84C0 + ind);
				glBindTexture(GL_TEXTURE_2D, tex->getTexture());
				shader->setInt(sName, ind);
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
		else if (sName == "uMetallic") {
			shader->setFloat(sName, pMat->getMetallic());
		}
		else if (sName == "uRoughness") {
			shader->setFloat(sName, pMat->getRoughness());
		}
		else if (sName == "uAo") {
			shader->setFloat(sName, pMat->getAo());
		}
		else if (sName == "uBloomValue") {
			shader->setFloat(sName, camera->getBloom());
		}
		else if (sName == "uSkybox") {
			CSkybox* skybox = (CSkybox*)camera->gameObject->getComponent(COMPTYPE::SKYBOX);
			if (skybox != nullptr) {
				RUInt ind = _allocTexture();
				shader->setInt(sName, ind);
				glActiveTexture(0x84C0 + ind);
				glBindTexture(GL_TEXTURE_CUBE_MAP, skybox->getBoxTexture());
			}
		}
	}

	delete[] uniformName;
}

//更新光源相关的uniform变量
void RenderSystem::_updateLightsUniforms(GLProgram* shader, std::list<CLight*> lights) {
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

//取出一个framebuffer
RFramebuffer* RenderSystem::_popFramebuffer() {
	RFramebuffer* temp;
	if (m_framebufferPool.size() > 0) {
		temp = m_framebufferPool.back();
		m_framebufferPool.pop_back();
	}
	else {
		temp = new RFramebuffer();
	}
	return temp;
}

//用完的framebuffer重新放入buffer池
void RenderSystem::_pushFramebuffer(RFramebuffer* pf) {
	m_framebufferPool.push_back(pf);
}
