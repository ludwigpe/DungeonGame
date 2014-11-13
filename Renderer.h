#ifndef RENDERER_H
#define RENDERER_H

#include <d3dx9.h>
#include "Mesh.h"
#include "Camera.h"

class Renderer
{
private:
	LPDIRECT3D9 m_D3DObject;		//The Direct 3D Object, used to get device information and to create the device object
	LPDIRECT3DDEVICE9 m_D3DDevice;	//The Device object, used to render the scene
	bool m_isFullscreen;			//A flag that determines if the game is running in fullscreen
	unsigned int m_width;		//The width of the renderable area. Will be the screen width if fullscreen or window width otherwise
	unsigned int m_height;		//The height of the renderable area. Will be the screen height if fullscreen or window height otherwise
	D3DXMATRIX m_viewMatrix;
	D3DXMATRIX m_projectionMatrix;
public:
	//Constructor
	Renderer();

	//Destructor
	~Renderer();

	//Initialise
	bool Initialise(HWND windowHandle, unsigned int width, unsigned int height, bool fullscreen);
	
	//Gets
	LPDIRECT3DDEVICE9 GetDevice() { return m_D3DDevice; }
	unsigned int GetWidth() { return m_width; }
	unsigned int GetHeight() { return m_height; }
	bool GetFullscreen() { return m_isFullscreen; }
	D3DXMATRIX GetProjectionMatrix(){ return m_projectionMatrix; }
	D3DXMATRIX GetViewMatrix(){ return m_viewMatrix; }
	HRESULT GetViewPortMatrix(D3DVIEWPORT9* pViewport );
	void BeginScene(Camera* cam);	//Begins a rendered frame, sets up the device object to draw
	void EndScene();	//Ends a rendered frame, finalises the device object and presents the scene
	D3DXVECTOR3* Project3D(D3DXVECTOR3* pOut, D3DXVECTOR3* pSource, D3DXMATRIX* pWorld);
	void Draw(Mesh* mesh, D3DXMATRIX world);
};

#endif