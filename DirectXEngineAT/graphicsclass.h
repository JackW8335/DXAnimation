
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


#include <DirectXMath.h>
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"
#include "lightshaderclass.h"
#include "lightclass.h"
#include "Exporter.h"
#include <AntTweakBar.h>
#include "AssimpTool.h"
using namespace DirectX;


const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;



class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(float posX, float posY, float posZ, float rotX, float rotY, float rotZ);

private:
	bool Render(float);

private:
	int width, height, depth;
	D3DXVECTOR3 building_scale;
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	std::vector <ModelClass*> m_Models;
	LightShaderClass* m_LightShader;
	AssimpTool* assimp;
	LightClass* m_Light;
	Exporter* exporter;
	bool addModel(float, float, float, float, std::string name, std::string objectName);
	void createRoom(float, float, float, float);
	void deleteRooms();
	void createBuilding();
	float rotation = 0.0f;

};

#endif