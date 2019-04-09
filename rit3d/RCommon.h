#pragma once
#include <cmath>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <map>
#include <functional>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include "stb_image.h"
using std::cout;
using std::endl;
using std::string;

//类型定义
typedef char           RChar;
typedef short          RShort;
typedef float          RFloat;
typedef double         RDouble;
typedef int            RInt;
typedef bool           RBool;
typedef long           RLong;
typedef std::string    RString;
typedef std::wstring   RWString;
typedef unsigned char  RUChar;
typedef unsigned short RUShort;
typedef unsigned int   RUInt;
typedef RUInt          VAO;
typedef RUInt          VBO;
typedef RUInt          EBO;
typedef RUInt          GLTexture;

//默认值
const RInt DEFAULT_WIDTH = 800;
const RInt DEFAULT_HEIGHT = 600;
const RString DEFAULT_NAMW = "Rit3D";
const RString DEFAULT_SCENE_NAME = "SCENE";
const RString DEFAULT_GAMEOBJECT_NAME = "GameObject";

//数字常量
#define PI 3.141592654f
#define PI2 (PI * 2.0f)
#define PI_DIV_2 (PI * 0.5f)
#define PI_DIV_4 (PI * 0.25f)
#define PI_INV 0.318309886f

//计算精度
#define EPSILON_E6 ((RFloat)(1E-6))

//常用数学函数
#define Degree2Radian(deg) (deg * 0.017453f)
#define Radian2Degree(rad) (rad * 57.29578f)

#define Abs(a) ((a) >= 0 ? (a) : -(a))

//安全删除
#define SafeDelete(ptr) if(ptr){delete ptr; ptr = nullptr;}
#define SafeDeleteArray(ptr) if(ptr){delete[] ptr; ptr = nullptr;}

//输出
#define COUT(a) std::cout << #a << ":" << (a) << std::endl;

//shadow map size
#define SHADOW_WIDTH 1024
#define SHADOW_HEIGHT 1024

//系统类型
typedef enum {
	STARTSYSTEM = 0,
	BEHAVIORSYSTEM,
	COLLIDESYSTEM,
	RENDERSYSTEM,
	ENDSYSTEM,
} SYSTEMTYPE;

//组件类型
typedef enum {
	TRANFORM = 0,
	MESH,
	RENDER,
	COLLIDER,
	BEHAVIOR,
	CAMERA,
	LIGHT,
	POSTPROCESS,
	SKYBOX,
} COMPTYPE;

//游戏对象所在的图层
typedef enum {
	CullingMaskNothing = 0x0000,
	CullingMaskEverything = 0xFFFF,

	BuiltinLayer0 = 0x0001,
	BuiltinLayer1 = 0x0002,
	BuiltinLayer2 = 0x0004,
	BuiltinLayer3 = 0x0008,
	BuiltinLayer4 = 0x0010,
	BuiltinLayer5 = 0x0020,
	BuiltinLayer6 = 0x0040,
	BuiltinLayer7 = 0x0080,

	UserLayer0 = 0x0100,
	UserLayer1 = 0x0200,
	UserLayer2 = 0x0400,
	UserLayer3 = 0x0800,
	UserLayer4 = 0x1000,
	UserLayer5 = 0x2000,
	UserLayer6 = 0x4000,
	UserLayer7 = 0x8000,

	Default = BuiltinLayer0,
} LAYER;


//相机类型
typedef enum {
	ORTHO = 0,
	PERSPECTIVE = 1,
} CAMERATYPE;

//光照类型
typedef enum {
	NONE = 0,
	DIRECTION,
	LPOINT,
	SPOT,
} LIGHTTYPE;

namespace util {
	RString num2str(RUInt num);
	RString num2str(RInt num);
};

