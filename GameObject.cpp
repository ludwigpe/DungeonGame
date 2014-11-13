#include "GameObject.h"
#include "GameComponent.h"
#include "GameHandler.h"

int GameObject::IDCounter = 0;
GameObject::GameObject()
{
	m_mesh = NULL;

	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rotation = D3DXVECTOR3(0, 0, 0);
	m_scale = D3DXVECTOR3(1, 1, 1);
	m_direction = D3DXVECTOR3(0, 0, 1);
	m_isVisible = true;
	m_components.clear();
	m_objectId = IDCounter;
	IDCounter++;

}

GameObject::GameObject(Mesh* mesh)
{
	m_mesh = mesh;
	
	m_pos = D3DXVECTOR3(0, 0, 0);
	m_rotation = D3DXVECTOR3(0, 0, 0);
	m_scale = D3DXVECTOR3(1, 1, 1);
	m_direction = D3DXVECTOR3(0, 0, 1);
	m_isVisible = true;

	m_components.clear();
	m_objectId = IDCounter;
	IDCounter++;

}

GameObject::GameObject(Mesh* mesh, float posX, float posY, float posZ, float rotX, float rotY, float rotZ,
		float scaleX, float scaleY, float scaleZ)
{
	m_mesh = mesh;
	
	m_pos = D3DXVECTOR3(posX, posY, posZ);
	m_rotation = D3DXVECTOR3(rotX, rotY, rotZ);
	m_scale = D3DXVECTOR3(scaleX, scaleY, scaleZ);
	m_direction = D3DXVECTOR3(0, 0, 1);
	m_isVisible = true;

	m_components.clear();
	m_objectId = IDCounter;
	IDCounter++;
}


void GameObject::Release(MeshManager* meshManager)
{
	if(m_mesh)
	{
		meshManager->ReleaseMesh(m_mesh);
		m_mesh = NULL;
	}

	for(int i = 0; i < m_components.size(); i++)
	{
		delete m_components[i];
		m_components[i] = NULL;
	}
	IDCounter++;
}
void GameObject::Update(float timestep)
{
	for(int i = 0; i < m_components.size(); i++)
	{
		m_components[i]->Update(timestep);
	}
	
}
void GameObject::Draw(Renderer* renderer)
{
	if(!m_isVisible)
		return;
	//We need to create a world matrix for our game object
	D3DXMATRIX world;
	D3DXMATRIX scale;
	D3DXMATRIX rotation;
	D3DXMATRIX translation;

	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rotation, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&translation, m_pos.x, m_pos.y, m_pos.z);

	//Scale then rotation (yaw pitch roll) then translation
	world = scale * rotation * translation;
	//Tell the renderer to draw the mesh according to this matrix
	renderer->Draw(m_mesh, world);

	
}
void GameObject::AddComponent(ComponentType type, GameComponent* component)
{
	
	m_componentsMap[type] = component;
	m_components.push_back(component);
	
	
}
void GameObject::AddComponent(GameComponent* component)
{
	m_components.push_back(component);
}
/*
template<typename T>
void GameObject::AddComponent(T* component)
{
	if(!GameComponent* gComp = dynamic_cast<GameComponent*>(component))
		return;	// the component is not a game component and should not be added
	if(!GetComponent<T>())
	{
		// there is no component of type T in the components list
		// add it to the list of components
		m_components.push_back(component);
	}

}
*/


void GameObject::SetDirection(D3DXVECTOR3 direction)
{
	//D3DXVECTOR3 forward = direction;
	if(m_direction = D3DXVECTOR3(0, 0, 0))
	{
		D3DXMATRIX rotationY;
		D3DXMatrixRotationY(&rotationY, m_rotation.y);
		D3DXVec3TransformCoord(&m_direction, &D3DXVECTOR3(0, 0, 1), &rotationY);
	}
	// rotate player
	
	float phi = acos(D3DXVec3Dot(&direction, &m_direction));
	m_rotation.y += (m_direction.x < 0) ? phi : -phi;

	m_direction = direction;
}
