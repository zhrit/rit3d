#include "pch.h"
#include "HairSystem.h"
#include "SCLightCameraCollecter.h"
#include "RGameObject.h"
#include "CTransform.h"

HairSystem::HairSystem(RInt od) : ISystem(od) {
	m_type = HAIRSYSTEM;
}

HairSystem::~HairSystem() {

}

void HairSystem::addHair(CHair* _b) {
	std::vector<CHair*>::iterator iter;
	iter = std::find(m_hairPool.begin(), m_hairPool.end(), _b);
	if (iter == m_hairPool.end()) {
		m_hairPool.push_back(_b);
	}
}

void HairSystem::removeHair(CHair* _b) {
	std::vector<CHair*>::iterator iter;
	iter = std::find(m_hairPool.begin(), m_hairPool.end(), _b);
	if (iter != m_hairPool.end()) {
		m_hairPool.erase(iter);
	}
}

HairSystem* HairSystem::CreateInstance(RInt od) {
	return new HairSystem(od);
}

//系统初始化时调用
void HairSystem::onAwake() {

}

//系统被激活时调用
void HairSystem::onEnable() {

}

//系统开始运行时调用
void HairSystem::onStart() {

}

//实体被添加到系统时调用
void HairSystem::onAddGameObject() {

}

//组件添加到实体时调用
void HairSystem::onAddComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::HAIR) {
		addHair((CHair*)pComp);
		_transform((CHair*)pComp);//组件加入系统组件池时，进行坐标变化，此过程不可逆，也就是说即使之后再移除组件，坐标也不会还原
		_barycentric((CHair*)pComp);//创建插值重心坐标
	}
}

//组件从实体移除时调用
void HairSystem::onRemoveComponent(COMPTYPE type, IComponent* pComp) {
	if (type == COMPTYPE::HAIR) {
		removeHair((CHair*)pComp);
	}
}

//实体从系统中移除时调用
void HairSystem::onRemoveGameObject() {

}

//系统更新时调用
void HairSystem::onUpdate(DWORD deltaT) {
	_updateHair(deltaT);
	_drawHair();
}

void HairSystem::_updateHair(DWORD deltaT) {
	RFloat dt = static_cast<RFloat>(deltaT) / 1000.0f;
	//cout << dt << endl;
	if (dt > 1.0f / 60.0f) {
		dt = 1.0f / 60.0f;
	}
	for (auto hair : m_hairPool) {
		glm::mat4 mMat = hair->gameObject->transform->getModelMatrix();
		//verlet数值积分获取新的位置
		for (auto& node : hair->m_nodes) {
			glm::vec3 p2 = node.p1 + hair->d * (node.p1 - node.p0) + hair->a * dt * dt;
			//新状态取代旧状态
			node.p0 = node.p1;
			node.p1 = p2;
		}
		//对每束发丝松弛法进行约束求解
		for (auto strand : hair->m_strands) {
			//松弛法迭代
			for (int i = 0; i < 10; i++) {
				//对每个节点的处理
				for (int index = strand.nodeStart; index < strand.nodeEnd; index++) {
					HairNode& na = hair->m_nodes[index];
					HairNode& nb = hair->m_nodes[index + 1];
					//碰撞检测和决议
					_collideHair(hair, nb.p1);
					//长度约束
					glm::vec3 a2b = nb.p1 - na.p1;
					float l = glm::length(a2b);
					float factor = (l - nb.maxLength) / (2.0f * l);
					na.p1 = na.p1 + a2b * factor;
					nb.p1 = nb.p1 - a2b * factor;
				}
				hair->m_nodes[strand.nodeStart].p1 = glm::vec3(mMat * glm::vec4(strand.rootP, 1.0f));
			}
		}
		//更新绘制数据，主要是3次Bezier曲线拟合
		_bezierCurve(hair);
		_interpolate(hair);
		_calcTangent(hair);
	}
}
void HairSystem::_drawHair() {
	std::list<CCamera*> cameraList = SCLightCameraCollecter::Instance()->getCameraList();
	std::list<CLight*> lightList = SCLightCameraCollecter::Instance()->getLightList();
	CLight* light = lightList.front();
	for (auto camera : cameraList) {
		for (auto hair : m_hairPool) {
			int num = (hair->nodeInStrand - 1) * (hair->interN + 1) + 1;
			glBindVertexArray(hair->m_VAO);
			glBindBuffer(GL_ARRAY_BUFFER, hair->m_VBO);
			glBufferData(GL_ARRAY_BUFFER, hair->m_dataSize * sizeof(float), &(hair->m_drawData[0]), GL_STREAM_DRAW);
			hair->shader->use();
			//hair->shader->setMat4("uModel", hair->gameObject->transform->getModelMatrix());
			hair->shader->setMat4("uView", camera->getViewMatrix());
			hair->shader->setMat4("uProjection", camera->getProjMatrix());
			hair->shader->setVec3("uLight.direction", light->gameObject->transform->getLocalFrontDir());
			hair->shader->setVec3("uLight.color", light->getColor());
			hair->shader->setFloat("uLight.ambInt", light->getAmbInt());
			hair->shader->setFloat("uLight.difInt", light->getDifInt());
			hair->shader->setFloat("uLight.speInt", light->getSpeInt());
			hair->shader->setFloat("uLight.castShadow", light->isCastShadow());
			hair->shader->setVec3("uViewPos", camera->gameObject->transform->getPosition());
			int count_strand = static_cast<int>(hair->m_dataSize / 6 / num);
			//int count_strand = static_cast<int>(hair->m_strands.size());
			for (int i = 0; i < count_strand; i++) {
				glDrawArrays(GL_LINE_STRIP, i * num, num);
			}
		}
		glBindVertexArray(0);
	}
}

//系统更新时onUpdate之后调用
void HairSystem::onLateUpdate() {

}

//系统被禁用时调用
void HairSystem::onDisable() {

}

void HairSystem::onDestroy() {

}

//头发的碰撞检测
void HairSystem::_collideHair(CHair* ch, glm::vec3& point) {
	glm::vec3 dir = point - ch->center;
	float l = glm::length(dir);
	if (l < ch->raduis) {
		point = ch->center + ch->raduis / l * dir;
	}
}

//节点数据从局部坐标系转换到世界坐标系
void HairSystem::_transform(CHair* ch) {
	glm::mat4 mMat = ch->gameObject->transform->getModelMatrix();
	int l = ch->m_nodes.size();
	for (int i = 0; i < l; i++) {
		ch->m_nodes[i].p0 = ch->m_nodes[i].p1 = glm::vec3(mMat * glm::vec4(ch->m_nodes[i].p1, 1.0f));
	}
	ch->center = glm::vec3(mMat * glm::vec4(ch->center, 1.0f));
}

//3次Bezier曲线拟合
void HairSystem::_bezierCurve(CHair* ch) {
	ch->m_pointer4Data = 0;
	RFloat a = 0.25f, b = 0.25f;//计算控制点用到的系数
	int n = ch->interN;//插值点个数
	float dt = 1.0f / (n + 1);
	std::vector<HairStrand>& ms = ch->m_strands;
	//对每条发束进行bezier插值
	for (auto s : ms) {
		std::vector<glm::vec3> cpa;//控制点A
		std::vector<glm::vec3> cpb;//控制点B
		//反推控制点
		cpa.push_back(ch->m_nodes[s.nodeStart].p1 + a * (ch->m_nodes[s.nodeStart + 1].p1 - ch->m_nodes[s.nodeStart].p1));
		for (int i = s.nodeStart + 1; i < s.nodeEnd; i++) {
			glm::vec3 tan = ch->m_nodes[i + 1].p1 - ch->m_nodes[i - 1].p1;
			cpa.push_back(ch->m_nodes[i].p1 + a * tan);
			cpb.push_back(ch->m_nodes[i].p1 - b * tan);
		}
		cpb.push_back(ch->m_nodes[s.nodeEnd].p1 - b * (ch->m_nodes[s.nodeEnd].p1 - ch->m_nodes[s.nodeEnd - 1].p1));
		//插值
		for (int i = s.nodeStart; i < s.nodeEnd; i++) {
			for (int j = 0; j <= n; j++) {
				float t = dt * j;
				glm::vec3 p = ch->m_nodes[i].p1 * (1.0f - t) * (1.0f - t) * (1.0f - t) +
					cpa[i - s.nodeStart] * 3.0f * t * (1 - t) * (1 - t) +
					cpb[i - s.nodeStart] * 3.0f * t * t * (1 - t) +
					ch->m_nodes[i + 1].p1 * t * t * t;
				ch->m_drawData[ch->m_pointer4Data++] = p.x;
				ch->m_drawData[ch->m_pointer4Data++] = p.y;
				ch->m_drawData[ch->m_pointer4Data++] = p.z;
				//空出切线的位置
				ch->m_drawData[ch->m_pointer4Data++] = 0.0f;
				ch->m_drawData[ch->m_pointer4Data++] = 0.0f;
				ch->m_drawData[ch->m_pointer4Data++] = 0.0f;
			}
		}
		ch->m_drawData[ch->m_pointer4Data++] = ch->m_nodes[s.nodeEnd].p1.x;
		ch->m_drawData[ch->m_pointer4Data++] = ch->m_nodes[s.nodeEnd].p1.y;
		ch->m_drawData[ch->m_pointer4Data++] = ch->m_nodes[s.nodeEnd].p1.z;
		//空出切线的位置
		ch->m_drawData[ch->m_pointer4Data++] = 0.0f;
		ch->m_drawData[ch->m_pointer4Data++] = 0.0f;
		ch->m_drawData[ch->m_pointer4Data++] = 0.0f;
	}
}

//创建重心坐标
void HairSystem::_barycentric(CHair* ch) {
	ch->m_interCoord.clear();
	for (RUInt i = 0; i < ch->strandN; i++) {
		RFloat x = rand() / (RFloat)RAND_MAX;
		RFloat y = rand() / (RFloat)RAND_MAX;
		if (x + y > 1.0f) {
			if (x > y) {
				x = 1.0f - x;
			}
			else {
				y = 1.0f - y;
			}
		}
		RFloat z = 1.0f - x - y;
		ch->m_interCoord.push_back(glm::vec3(x, y, z));
	}
}

//插值所有的头发
void HairSystem::_interpolate(CHair* ch) {
	int faces = ch->scalp_i.size() / 3;//面的个数
	int nodes = (ch->nodeInStrand - 1) * (ch->interN + 1) + 1;//一条发束上的节点数（bezier插值后）
	for (int i = 0; i < faces; i++) {
		//面上三个发束的索引
		RUInt ip1 = ch->scalp_i[3 * i];
		RUInt ip2 = ch->scalp_i[3 * i + 1];
		RUInt ip3 = ch->scalp_i[3 * i + 2];
		//插入新的发束
		for (auto coord : ch->m_interCoord) {
			for (int j = 0; j < nodes; j++) {
				for (int k = 0; k < 3; k++) {
					RFloat p1k = ch->m_drawData[6 * (ip1 * nodes + j) + k];
					RFloat p2k = ch->m_drawData[6 * (ip2 * nodes + j) + k];
					RFloat p3k = ch->m_drawData[6 * (ip3 * nodes + j) + k];
					ch->m_drawData[ch->m_pointer4Data++] = p1k * coord.x + p2k * coord.y + p3k * coord.z;
				}
				//空出切线的位置
				ch->m_drawData[ch->m_pointer4Data++] = 0.0f;
				ch->m_drawData[ch->m_pointer4Data++] = 0.0f;
				ch->m_drawData[ch->m_pointer4Data++] = 0.0f;
			}
		}
	}
}

//求切线
void HairSystem::_calcTangent(CHair* ch) {
	//在这里不做单位化，在shader里单位化
	int numNodes = (ch->nodeInStrand - 1) * (ch->interN + 1) + 1;//一条发束上的节点数（bezier插值后）
	int numStrand = ch->m_dataSize / 6 / numNodes;
	for (int j = 0; j < numStrand; j++) {
		//发束上第一个点的切线012 345 678
		int p1 = 6 * (j * numNodes);//首地址
		ch->m_drawData[p1 + 3] = ch->m_drawData[p1 + 6] - ch->m_drawData[p1 + 0];
		ch->m_drawData[p1 + 4] = ch->m_drawData[p1 + 7] - ch->m_drawData[p1 + 1];
		ch->m_drawData[p1 + 5] = ch->m_drawData[p1 + 8] - ch->m_drawData[p1 + 2];
		//发束中间点的切线
		for (int i = 1; i < numNodes - 1; i++) {
			int pi = 6 * (j * numNodes + i);
			ch->m_drawData[pi + 3] = ch->m_drawData[pi + 6] - ch->m_drawData[pi - 6];
			ch->m_drawData[pi + 4] = ch->m_drawData[pi + 7] - ch->m_drawData[pi - 5];
			ch->m_drawData[pi + 5] = ch->m_drawData[pi + 8] - ch->m_drawData[pi - 4];
		}
		//发束末尾点的切线
		int pe = 6 * (j * numNodes + numNodes - 1);
		ch->m_drawData[pe + 3] = ch->m_drawData[pe + 0] - ch->m_drawData[pe - 6];
		ch->m_drawData[pe + 4] = ch->m_drawData[pe + 1] - ch->m_drawData[pe - 5];
		ch->m_drawData[pe + 5] = ch->m_drawData[pe + 2] - ch->m_drawData[pe - 4];
	}
}