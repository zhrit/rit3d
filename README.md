# rit3d
rit3d是基于OpenGL API的游戏渲染引擎，主要用来学习和实践计算机图形学和游戏引擎相关知识。

rit3d采用类似Unity3D的组件实体系统架构。

## 已实现的组件
* transform：几何变换
* light：光源
* camera：相机
* render：渲染
* behavior：行为脚本
* post process：后处理
* skybox：天空盒

## 已实现的主要功能
### 光照和阴影
如下图，本示例内的游戏对象(gameObject)采用phong材质，实时阴影采用shadow map技术。示例中包含三种类型的光源：立方体正上方的白色点光源、斜上方的白色平行光和红色的聚光灯。

<img src="doc/demo_image/light_shadow_1.png"  height="300" width="400">

### PBR材质

如下图，本示例采用基于物理的渲染(PBR材质)，六个球体的PBR材质贴图均来自[https://freepbr.com/c/base-metals/](https://freepbr.com/c/base-metals/)。

<img src="doc/demo_image/pbr_1.png"  height="300" width="400">

### HDR和泛光材质

本引擎默认采用高动态范围颜色(HDR)。泛光是为了模拟发光物，下图中白色球和红色球是两个点光源。

<img src="doc/demo_image/bloom_1.png"  height="300" width="400">

### 法线贴图

如下图，左边使用法线贴图，右边没有。

<img src="doc/demo_image/normalmap_1.png"  height="300" width="400">

### 行为脚本

### 后处理

### 天空盒


