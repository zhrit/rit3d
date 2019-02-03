#pragma once

#include "RCommon.h"
#include "RGameObject.h"
#include "CCamera.h"
#include "CLight.h"

class RScene {
public:
	//创建场景
	static RScene* CreateScene(RString name = DEFAULT_SCENE_NAME);

private:
	RScene(RString name);
	virtual ~RScene();

	std::list<RGameObject*> m_GameObjectList;//场景中对象列表
	std::list<CCamera*> m_cameraList;//相机列表
	std::list<CLight*> m_lightList;//光源列表
	RUInt m_dirLightNum{ 0 };//平行光数量
	RUInt m_poiLightNum{ 0 };//点光源数量
	RUInt m_spoLightNum{ 0 };//聚光灯数量
	RBool m_active{ true };//场景是否激活 
	RString m_name;//场景名称

	RUInt m_layer{ LAYER::Default }; //图层

public:
	//场景销毁
	void destroy();
	
	//场景名称操作
	RString getName() const;
	void setName(RString name);

	//场景状态操作
	RBool isActive() const;
	void setActive(RBool value);

	//图层操作
	RUInt getLayer() const;
	void setLayer(RUInt l);

	//创建并添加游戏对象到场景中
	RGameObject* addGameObject(RString n = "UnName", RString t = "UnTag", LAYER l = LAYER::Default);
	//删除游戏对象
	void removeGameObject(RGameObject* pgb);
	//查找游戏对象
	RGameObject* findGameObjectWithName(RString name);
	RGameObject* findGameObjectWithTag(RString tag);
	std::list<RGameObject*> findGameObjectsWithTag(RString tag);
	std::list<RGameObject*> getAllGameObjects();
	//场景是否包含游戏对象
	RBool hasGameObject(RGameObject* pgb);
	//场景中游戏对象数量
	RUInt getGameObjectCount();
	//获取游戏对象列表
	std::list<RGameObject*> getGameObjectList() const;

	//添加相机组件
	void addCamera(CCamera* camera);
	//移除相机组件
	void removeCamera(CCamera* camera);
	//获取相机列表
	std::list<CCamera*> getCameraList() const;

	//添加灯光组件
	void addLight(CLight* light);
	//移除灯光组件
	void removeLight(CLight* light);
	//获取灯光列表
	std::list<CLight*> getLightList() const;
	//获取光源数量
	RUInt getLightNum(LIGHTTYPE _type) const;
	//修正光源数量
	void resetLightNum(LIGHTTYPE _type, RInt delta);

private:
	//递归删除游戏对象
	void _removeGameObject(RGameObject* pgb);
};