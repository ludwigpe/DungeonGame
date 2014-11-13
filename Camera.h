#ifndef CAMERA_H
#define CAMERA_H
#include <d3dx9.h>
/*
	The camera object has a all the basic functionallity of a camera in directx but
	can allso be set to focus on a specific object in the world and follow it
	in a 3rd person mode.
*/

class GameObject;

#define CAM_DIST  40.0f

class Camera
{
private:
	float m_positionX, m_positionY, m_positionZ;
	float m_lookAtX, m_lookAtY, m_lookAtZ;
	float m_upX, m_upY, m_upZ;

	float m_aspectRatio;
	float m_fieldOfView;
	float m_nearClip;
	float m_farClip;



	D3DXVECTOR3* m_focusedPosition;
	D3DXVECTOR3* m_focusedRotation;
	GameObject* m_focusedObject;

public:
	//Constructor
	Camera(float posX, float posY, float posZ, float lookX, float lookY, float lookZ,
		float upX, float upY, float upZ, float aspectRatio, float fov, float nearClip, float farClip);
	
	void SetLookAt(D3DXVECTOR3* lookAt);
	void SetFocusedRotation(D3DXVECTOR3* fRot);
	void SetFocusedObject(GameObject* go);
	void SetCameraPosition(D3DXVECTOR3 newPos);
	float GetPosX() { return m_positionX; }
	float GetPosY() { return m_positionY; }
	float GetPosZ() { return m_positionZ; }

	float GetLookAtX() { return m_lookAtX; }
	float GetLookAtY() { return m_lookAtY; }
	float GetLookAtZ() { return m_lookAtZ; }
	D3DXVECTOR3* GetLookAt() { return m_focusedPosition;}
	float GetUpX() { return m_upX; }
	float GetUpY() { return m_upY; }
	float GetUpZ() { return m_upZ; }

	float GetAspectRatio() { return m_aspectRatio; }
	float GetFieldOfView() { return m_fieldOfView; }
	float GetNearClip() { return m_nearClip; }
	float GetFarClip() { return m_farClip; }
	D3DXVECTOR3 GetCameraPos(){return D3DXVECTOR3(m_positionX, m_positionY, m_positionZ); }
	D3DXVECTOR3 GetCameraDirection();

	virtual void Update(float timestep);

};

#endif