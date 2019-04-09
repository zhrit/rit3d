#pragma once
#include "ISingletonComponent.h"
#include "CLight.h"
#include "CCamera.h"

/**
 *灯光相机收集组件
 */
class SCLightCameraCollecter : public ISingletonComponent {
private:
	SCLightCameraCollecter();
	virtual ~SCLightCameraCollecter();
	static SCLightCameraCollecter *m_instance;

	std::list<CLight*> m_lightList;
	std::list<CCamera*> m_cameraList;

	RUInt m_dirLightNum{ 0 };//平行光数量
	RUInt m_poiLightNum{ 0 };//点光源数量
	RUInt m_spoLightNum{ 0 };//聚光灯数量

public:
	static SCLightCameraCollecter* Instance();

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
};

