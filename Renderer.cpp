#include "Renderer.h"

Renderer::Renderer()
{
	//Zero or NULL out all the information
	m_D3DObject = NULL;
	m_D3DDevice = NULL;
	m_isFullscreen = false;
	m_width = 0;
	m_height = 0;
}

Renderer::~Renderer()
{
	//If there is a device, release it
	if(m_D3DDevice)
	{
		m_D3DDevice->Release();
		m_D3DDevice = NULL;
	}
	//If there is a Direct 3D Object, release it
	if(m_D3DObject)
	{
		m_D3DObject->Release();
		m_D3DObject = NULL;
	}
}

bool Renderer::Initialise(HWND windowHandle, unsigned int width, unsigned int height, bool fullscreen)
{
	D3DDISPLAYMODE DisplayMode;					//Stores information about the current display mode. width, height, format etc.
	D3DPRESENT_PARAMETERS Present_Parameters;	//Holds a number of settings for determining how the device renders
	D3DCAPS9 D3DCaps;							//Stores information about the capabilities of the hardware

	//Clear out the present parameters
	memset(&Present_Parameters, 0, sizeof(Present_Parameters));

	//Create the Direct 3D object
	m_D3DObject = Direct3DCreate9(D3D_SDK_VERSION);

	//If that fails, stop
	if(m_D3DObject == NULL)
	{
		return false;
	}

	//Attempt to determine the display mode information. If that fails, stop.
	if(FAILED(m_D3DObject->GetAdapterDisplayMode(D3DADAPTER_DEFAULT, &DisplayMode)))
	{
		return false;
	}

	//Attempt to determine the device capabilities. If that fails, stop.
	if(FAILED(m_D3DObject->GetDeviceCaps(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, &D3DCaps)))
	{
		return false;
	}

	//Use the capabilities to determine if we support software or hardware vertex processing
	DWORD VertexProcessing = 0;

	if(D3DCaps.VertexProcessingCaps != 0)
		VertexProcessing |= D3DCREATE_HARDWARE_VERTEXPROCESSING;
	else
		VertexProcessing |= D3DCREATE_SOFTWARE_VERTEXPROCESSING;

	if(fullscreen)
	{
		//If running in fullscreen set the width and height member variables to the information stored in the display mode
		m_width = DisplayMode.Width;
		m_height = DisplayMode.Height;

		//Set the present parameters to support fullscreen
		Present_Parameters.Windowed = FALSE;
		Present_Parameters.BackBufferWidth = m_width;
		Present_Parameters.BackBufferHeight = m_height;
	}
	else
	{
		//If it's not fullscreen set the width and height member variables to the size of the window.
		//Note: The window sized is passed in as parameters to this initialise function.
		m_width = width;
		m_height = height;
		Present_Parameters.Windowed = TRUE;
	}
	
	//Set up further persent parameters.
	Present_Parameters.SwapEffect = D3DSWAPEFFECT_DISCARD;
	Present_Parameters.BackBufferFormat = DisplayMode.Format;
	Present_Parameters.BackBufferCount = 1;
	Present_Parameters.EnableAutoDepthStencil = TRUE;
	Present_Parameters.AutoDepthStencilFormat = D3DFMT_D16;

	//Attempt to create a device object. If this fails, stop
	if(FAILED(m_D3DObject->CreateDevice(D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, windowHandle,
		VertexProcessing,
		&Present_Parameters, &m_D3DDevice)))
	{
		return false;
	}

	//Disable lighting, you will want to change this eventually.
	m_D3DDevice->SetRenderState(D3DRS_LIGHTING, TRUE);
	//Enable the Z buffer.
	m_D3DDevice->SetRenderState(D3DRS_ZENABLE, TRUE);

	return true;
}

void Renderer::BeginScene(Camera* cam)
{
	if(m_D3DDevice)
	{
		//D3DXMATRIX view;
		D3DXVECTOR3 camPos(cam->GetPosX(), cam->GetPosY(), cam->GetPosZ());
		D3DXVECTOR3 camLookAt(cam->GetLookAtX(), cam->GetLookAtY(), cam->GetLookAtZ());
		D3DXVECTOR3 camUp(cam->GetUpX(), cam->GetUpY(), cam->GetUpZ());
		D3DXMatrixLookAtLH(&m_viewMatrix, &camPos, &camLookAt, &camUp);

		m_D3DDevice->SetTransform(D3DTS_VIEW, &m_viewMatrix);

		D3DXMATRIX projection;
		D3DXMatrixPerspectiveFovLH(&m_projectionMatrix, cam->GetFieldOfView(), cam->GetAspectRatio(),
			cam->GetNearClip(), cam->GetFarClip());

		m_D3DDevice->SetTransform(D3DTS_PROJECTION, &m_projectionMatrix);

		//If there is a device object, clear the screen and begin the scene
		m_D3DDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0, 0, 0), 1.0f, 0);
		m_D3DDevice->BeginScene();
	}
}

void Renderer::EndScene()
{
	if(m_D3DDevice)
	{
		//If there is a device object, end the scene and present it.
		m_D3DDevice->EndScene();
		m_D3DDevice->Present(NULL, NULL, NULL, NULL);
	}
}

void Renderer::Draw(Mesh* mesh, D3DXMATRIX world)
{
	if(mesh)
	{
		m_D3DDevice->SetTransform(D3DTS_WORLD, &world);
		mesh->Render(m_D3DDevice);
	}
}
HRESULT Renderer::GetViewPortMatrix(D3DVIEWPORT9* pViewport )
{
	return m_D3DDevice->GetViewport(pViewport);
}
/*
	THis function projects a world coordinate to screen coordinates
*/
D3DXVECTOR3* Renderer::Project3D(D3DXVECTOR3* pOut, D3DXVECTOR3* pSource, D3DXMATRIX* pWorld)
{
	D3DVIEWPORT9 viewport;
	GetViewPortMatrix(&viewport);
	return D3DXVec3Project(pOut, pSource, &viewport, &m_projectionMatrix, &m_viewMatrix, pWorld);
}