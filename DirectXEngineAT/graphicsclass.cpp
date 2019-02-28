
#include "graphicsclass.h"

GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;
	m_LightShader = 0;
	m_Light = 0;
	assimp = 0;

}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;


	width = 1;
	height = 1;
	depth = 1;

	building_scale = D3DXVECTOR3(width, height, depth);

	

	int   g_Angle = 0;
	float g_Scale = 1;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if (!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);


	TwInit(TW_DIRECT3D11, m_D3D->GetDevice()); // for Direct3D 11


	TwWindowSize(screenWidth, screenHeight);

	// Create a tweak bar
	TwBar *bar = TwNewBar("TweakBar");
	TwDefine(" GLOBAL help='This example shows how to integrate AntTweakBar into a DirectX11 application.' "); // Message added to the help bar.
	int barSize[2] = { 224, 320 };
	TwSetParam(bar, NULL, "size", TW_PARAM_INT32, 2, barSize);


	

	//TwAddVarRW(bar, "TestFloat", TW_TYPE_FLOAT, &test, " ");

	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if (!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 0.0f, -30.0f);





	// Create the light shader object.
	m_LightShader = new LightShaderClass;
	if (!m_LightShader)
	{
		return false;
	}

	// Initialize the light shader object.
	result = m_LightShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the light shader object.", L"Error", MB_OK);
		return false;
	}

	// Create the light object.
	m_Light = new LightClass;
	if (!m_Light)
	{
		return false;
	}

	// Initialize the light object.
	m_Light->SetAmbientColor(1.15f, 1.15f, 1.15f, 1.0f);
	m_Light->SetDiffuseColor(1.0f, 1.0f, 1.0f, 1.0f);
	m_Light->SetDirection(1.0f, 0.0f, 0.0f);

	exporter = new Exporter;

	TwAddVarRW(bar, "Width", TW_TYPE_INT8, &width, "");
	TwAddVarRW(bar, "Height", TW_TYPE_INT8, &height, "");
	TwAddVarRW(bar, "Depth", TW_TYPE_INT8, &depth, "");


	assimp = new AssimpTool();

	assimp->Initialize(m_D3D->GetDevice(), "./data/DoomSlayer/DoomSlayer.fbx", L"./data/brick-wall.jpg");

	assimp->SetRotation(0.0f, 0.0f, 90.0f);

	//assimp->Initialize(m_D3D->GetDevice(), "./data/cube.obj", L"./data/brick-wall.jpg");
	return true;
}




void GraphicsClass::Shutdown()
{
	// Release the light object.
	if (m_Light)
	{
		delete m_Light;
		m_Light = 0;
	}

	// Release the light shader object.
	if (m_LightShader)
	{
		m_LightShader->Shutdown();
		delete m_LightShader;
		m_LightShader = 0;
	}

	if (exporter)
	{
		if (!m_Models.empty())
			//exporter->exportModel(m_Models);

		delete exporter;
		exporter = 0;
	}

	for (int i = 0; i < m_Models.size(); i++)
	{
		// Release the model object.
		if (m_Models[i])
		{
			m_Models[i]->Shutdown();
			m_Models[i] = 0;
		}
	}

	assimp->Shutdown();

	// Release the camera object.
	if (m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if (m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}


	TwTerminate(); // Terminate AntTweakBar
	return;
}


bool GraphicsClass::Frame(float posX, float posY, float posZ, float rotX, float rotY, float rotZ)
{
	bool result;
	float frameTime = 10.0f;
	static float lightAngle = 270.0f;
	float radians;
	static float lightPosX = 9.0f;

	// Set the position of the camera.
	m_Camera->SetPosition(posX, posY, posZ);
	m_Camera->SetRotation(rotX, rotY, rotZ);

	
	rotation += 0.01f;
	// Render the graphics scene.
	result = Render(rotation);
	if (!result)
	{
		return false;
	}


	radians = lightAngle * 0.0174532925f;


	// Set the position and lookat for the light.
	m_Light->SetPosition(0.0f, 0.0f, 0.0f);
	m_Light->SetLookAt(-0.1f, 0.0f, 0.0f);

	if (width != building_scale.x || height != building_scale.y || depth != building_scale.z)
	{
		deleteRooms();
		//createBuilding();
		building_scale = D3DXVECTOR3(width, height, depth);
	}

	

	return true;
}



bool GraphicsClass::Render(float rotation)
{
	D3DXMATRIX worldMatrix, viewMatrix, projectionMatrix, rotationMatrix, translationMatrix, testMatrix;
	
	
	bool result;
	float posX = 0.0f, posY = 0.0f, posZ = 0.0f, rotY;

	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.0f, 0.0f, 1.0f, 1.0f);



	// Generate the view matrix based on the camera's position.
	m_Camera->Render();


	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	//m_D3D->GetWorldMatrix(rotationMatrix);
	m_D3D->GetWorldMatrix(translationMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);




	D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);
	
	

	if (assimp)
	{
		assimp->Render(m_D3D->GetDeviceContext());

		assimp->GetRotationY(rotY);

		result = m_LightShader->Render(m_D3D->GetDeviceContext(), assimp->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
			assimp->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColor());
	}

	
	// Put the model vertex and index buffers on the graphics pipeline to prepare them for drawing.
	for (int i = 0; i < m_Models.size(); i++)
	{

		// Setup the translation matrix for the model.
		m_Models[i]->GetPosition(posX, posY, posZ);
		m_Models[i]->GetRotationY(rotY);


		rotationMatrix._41 = posX;
		rotationMatrix._42 = posY;
		rotationMatrix._43 = posZ;
		D3DXMatrixRotationY(&rotationMatrix, rotY);

		D3DXMatrixTranslation(&worldMatrix, posX, posY, posZ);

		worldMatrix *= worldMatrix * rotationMatrix;

		m_Models[i]->Render(m_D3D->GetDeviceContext());


		// Render the model using the light shader.

		if (!m_Models.empty())
		{

			result = m_LightShader->Render(m_D3D->GetDeviceContext(), m_Models[i]->GetIndexCount(), worldMatrix, viewMatrix, projectionMatrix,
				m_Models[i]->GetTexture(), m_Light->GetDirection(), m_Light->GetAmbientColour(), m_Light->GetDiffuseColor());
		}



		if (!result)
		{
			return false;
		}

		// Reset the world matrix.
		m_D3D->GetWorldMatrix(worldMatrix);
		
	}


	TwDraw();
	// Present the rendered scene to the screen.
	m_D3D->EndScene();


	return true;
}

bool GraphicsClass::addModel(float x, float y, float z, float ry, std::string name, std::string objectName)
{
	bool result;
	std::string modelName = "../DirectXEngineAT/data/" + objectName + ".txt";

	char *save1 = new char[modelName.length() + 1];



	char *saveModel = std::strcpy(save1, modelName.c_str());
	//char *saveTexture = std::strcpy(save2, modelName.c_str());



	// Create the model object.
	ModelClass* m_Model = new ModelClass;
	if (!m_Model)
	{
		return false;
	}
	result = m_Model->Initialize(m_D3D->GetDevice(), saveModel, L"../DirectXEngineAT/data/brick-wall.jpg");



	// Initialize the model object.

	if (!result)
	{
		return false;
	}
	m_Model->setName(name);



	m_Models.push_back(m_Model);

	m_Models[m_Models.size() - 1]->SetPosition(x, y, z);
	m_Models[m_Models.size() - 1]->SetRotation(0.0f, ry, 0.0f);

	delete[] save1;

}

void GraphicsClass::createRoom(float x, float y, float z, float ry)
{
	addModel(0.0f + x, 0.0f + y, 0.0f + z, ry, "wall_1", "wall");
	addModel(0.0f + x, 0.0f + y, 0.93f + z, ry, "wall_2", "wall");
	addModel(0.0f + x, 0.0f + y, 0.0f + z, ry, "wall_3", "wall2");

	addModel(0.0f + x, 0.0f + y, 0.0f + z, ry, "roof_1", "roof");

	addModel(0.0f + x, 0.0f + y, 0.0f + z, ry, "doorway", "doorway");
}

void GraphicsClass::deleteRooms()
{
	for (int i = 0; i < m_Models.size(); i++)
	{
		// Release the model object.
		if (m_Models[i])
		{
			m_Models[i]->Shutdown();
			m_Models[i] = 0;
		}
	}
	m_Models.clear();
}

void GraphicsClass::createBuilding()
{
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			for (int k = 0; k < depth; k++)
			{
				createRoom(i, j, k, 0.0f);
			}
		}

	}
}
