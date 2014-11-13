#include "Camera.h"
#include "GameObject.h"
Camera::Camera(float posX, float posY, float posZ, float lookX, float lookY, float lookZ,
		float upX, float upY, float upZ, float aspectRatio, float fov, float nearClip, float farClip)
{
	m_positionX = posX;
	m_positionY = posY;
	m_positionZ = posZ;

	m_lookAtX = lookX;
	m_lookAtY = lookY;
	m_lookAtZ = lookZ;
	m_focusedPosition = NULL;
	m_focusedRotation = NULL;
	m_focusedObject = NULL;
	m_upX = upX;
	m_upY = upY;
	m_upZ = upZ;

	m_aspectRatio = aspectRatio;
	m_fieldOfView = fov;
	m_nearClip = nearClip;
	m_farClip = farClip;

}

void Camera::SetLookAt(D3DXVECTOR3* lookAt)
{
	m_focusedPosition = lookAt;
	m_lookAtX = m_focusedPosition->x;
	m_lookAtY = m_focusedPosition->y;
	m_lookAtZ = m_focusedPosition->z;
}

void Camera::SetFocusedRotation(D3DXVECTOR3* fRot)
{
	m_focusedRotation = fRot;
}

void Camera::Update(float timestep)
{
	if(m_focusedObject != NULL)
	{
		// if the camera is set to focus on a game object, it will position itself behind and above the object
		// and then look at it.
		m_positionX = m_focusedObject->GetPos().x - cos(m_focusedObject->GetRotation().y - D3DX_PI/2)*CAM_DIST;
		m_positionY = m_focusedObject->GetPos().y + CAM_DIST;
		m_positionZ = m_focusedObject->GetPos().z + sin(m_focusedObject->GetRotation().y - D3DX_PI/2)*CAM_DIST;
		m_lookAtX = m_focusedObject->GetPos().x;
		m_lookAtY = m_focusedObject->GetPos().y;
		m_lookAtZ = m_focusedObject->GetPos().z;

	}


}
void Camera::SetCameraPosition(D3DXVECTOR3 newPos)
{
	m_positionX = newPos.x;
	m_positionY = newPos.y;
	m_positionZ = newPos.z;
}
D3DXVECTOR3 Camera::GetCameraDirection()
{
	D3DXVECTOR3 lookat(m_lookAtX, m_lookAtY, m_lookAtZ);
	D3DXVECTOR3 spotDir = lookat - GetCameraPos();
	D3DXVec3Normalize(&spotDir, &spotDir);
	return spotDir;
}
void Camera::SetFocusedObject(GameObject* gameObject)
{
	m_focusedObject = gameObject;
}