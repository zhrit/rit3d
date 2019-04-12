#pragma once
#include <cmath>
#include <algorithm>
#include <cassert>
#include <string>
#include <vector>
#include <list>
#include <deque>
#include <queue>
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


