#include "Wall.h"
#include "Floor.h"
Wall::Wall(Mesh* mesh, D3DXVECTOR3 position) : GameObject(mesh)
{
	m_pos = position;
	m_sides.clear();
	float depth;
	float height;
	float width;

	D3DXVECTOR3 sidePos;
	D3DXVECTOR3 sideRot;
	std::pair<D3DXVECTOR3, D3DXVECTOR3> side;
	Floor* temp = new Floor(mesh);
	depth = temp->GetDepth();
	height = temp->GetHeight();
	width = temp->GetWidth();
	sidePos = m_pos;
	sideRot = m_rotation;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);
	delete temp;
	temp = NULL;


	// the top side
	sidePos = m_pos;
	sidePos.y += width;
	sideRot = m_rotation;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);

	// the left side
	sidePos = m_pos;
	sideRot = m_rotation;
	sideRot.z = D3DX_PI/2;
	sidePos.x -= width/2;
	sidePos.y += width/2;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);

	// right side
	sidePos = m_pos;
	sideRot = m_rotation;
	sideRot.z = D3DX_PI/2;
	sidePos.x += width/2;
	sidePos.y += width/2;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);

	// front side
	sidePos = m_pos;
	sideRot = m_rotation;
	sideRot.x = -D3DX_PI/2;
	sidePos.z -= depth/2;
	sidePos.y += width/2;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);


	// back side
	sidePos = m_pos;
	sideRot = m_rotation;
	sideRot.x = D3DX_PI/2;
	sidePos.z += depth/2;
	sidePos.y += width/2;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);

	m_wallMaxBound = m_pos;
	m_wallMaxBound.y += height;
	m_wallMaxBound.x += width/2;
	m_wallMaxBound.z += depth/2;

	m_wallMinBound = m_pos;
	m_wallMinBound.x -= width/2;
	m_wallMinBound.z -= depth/2;


	/*
	Floor* leftSide = new Floor(mManager->GetMesh(meshModel));
	D3DXVECTOR3 rot = m_rotation;
	pos = m_pos;
	rot.z = D3DX_PI/2;
	pos.x -= width/2;
	pos.y += width/2;
	leftSide->SetPos(pos);
	leftSide->SetRotation(rot);
	m_sides.push_back(leftSide);
	
	// create the right side of the cube wall
	Floor* rightSide = new Floor(mManager->GetMesh(meshModel));
	rot = m_rotation;
	pos = m_pos;
	rot.z = D3DX_PI/2;
	// position the right side of the cube wall
	pos.x += width/2;
	pos.y += width/2;
	rightSide->SetPos(pos);
	rightSide->SetRotation(rot);
	m_sides.push_back(rightSide);

	//creat the front side
	Floor* front = new Floor(mManager->GetMesh(meshModel));
	rot = m_rotation;
	pos = m_pos;
	rot.x = D3DX_PI/2;
	// position the front side of the cube wall
	pos.y += width/2;
	pos.z += depth/2;
	front->SetPos(pos);
	front->SetRotation(rot);
	m_sides.push_back(front);

	//creat the back side
	Floor* back = new Floor(mManager->GetMesh(meshModel));
	rot = m_rotation;
	pos = m_pos;
	rot.x = -D3DX_PI/2;
	// position the back side of the cube wall
	pos.y += width/2;
	pos.z -= depth/2;
	back->SetPos(pos);
	back->SetRotation(rot);
	m_sides.push_back(back);
	//SetUpCollider();
	//m_pos.y += m_boundingBox->GetWidth()/2;
	*/

}


bool Wall::SetUpCollider()
{
	if(m_mesh != NULL)
	{
		m_boundingBox = new BoundingBox(m_mesh);
		return true;
	}
	return false;

}
D3DXVECTOR3 Wall::GetMaxBound()
{ 
/*	D3DXMATRIX world;
	D3DXMATRIX rotation;
	D3DXMATRIX scale;
	D3DXMATRIX translate;


	D3DXVECTOR3 localMaxBound = m_boundingBox->GetMaxBound();
	D3DXVECTOR3 worldMaxBound;

	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rotation, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);

	world = scale * rotation * translate;
	D3DXVec3TransformCoord(&worldMaxBound, &localMaxBound, &world);

	return worldMaxBound;
	*/
	return m_wallMaxBound;
}
D3DXVECTOR3 Wall::GetMinBound()
{
	/*D3DXMATRIX world;
	D3DXMATRIX rotation;
	D3DXMATRIX scale;
	D3DXMATRIX translate;


	D3DXVECTOR3 localMinBound = m_boundingBox->GetMinBound();
	D3DXVECTOR3 worldMinBound;

	D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
	D3DXMatrixRotationYawPitchRoll(&rotation, m_rotation.y, m_rotation.x, m_rotation.z);
	D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);

	world = scale * rotation * translate;
	D3DXVec3TransformCoord(&worldMinBound, &localMinBound, &world);

	return worldMinBound;
	*/
	return m_wallMinBound;
}

void Wall::Draw(Renderer* renderer)
{	
	if(!m_isVisible)
		return;

	for(int i = 0; i < m_sides.size(); i++)
	{
	
		//We need to create a world matrix for our game object
		D3DXMATRIX world;
		D3DXMATRIX scale;
		D3DXMATRIX rotation;
		D3DXMATRIX translation;
		D3DXVECTOR3 pos = m_sides[i].first;
		D3DXVECTOR3 rot = m_sides[i].second;
	
		D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixRotationYawPitchRoll(&rotation, rot.y, rot.x, rot.z);
		D3DXMatrixTranslation(&translation, pos.x, pos.y, pos.z);

		//Scale then rotation (yaw pitch roll) then translation
		world = scale * rotation * translation;
		//Tell the renderer to draw the mesh according to this matrix
		renderer->Draw(m_mesh, world);

	}
	
}
void Wall::SetPos(D3DXVECTOR3 newPos)
{
	m_pos = newPos;
	m_sides.clear();
	float depth;
	float height;
	float width;

	D3DXVECTOR3 sidePos;
	D3DXVECTOR3 sideRot;
	std::pair<D3DXVECTOR3, D3DXVECTOR3> side;
	Floor* temp = new Floor(m_mesh);
	depth = temp->GetDepth();
	height = temp->GetHeight();
	width = temp->GetWidth();
	sidePos = m_pos;
	sideRot = m_rotation;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);
	delete temp;
	temp = NULL;


	// the top side
	sidePos = m_pos;
	sidePos.y += width;
	sideRot = m_rotation;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);

	// the left side
	sidePos = m_pos;
	sideRot = m_rotation;
	sideRot.z = D3DX_PI/2;
	sidePos.x -= width/2;
	sidePos.y += width/2;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);

	// right side
	sidePos = m_pos;
	sideRot = m_rotation;
	sideRot.z = D3DX_PI/2;
	sidePos.x += width/2;
	sidePos.y += width/2;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);

	// front side
	sidePos = m_pos;
	sideRot = m_rotation;
	sideRot.x = -D3DX_PI/2;
	sidePos.z -= depth/2;
	sidePos.y += width/2;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);


	// back side
	sidePos = m_pos;
	sideRot = m_rotation;
	sideRot.x = D3DX_PI/2;
	sidePos.z += depth/2;
	sidePos.y += width/2;
	side.first = sidePos;
	side.second = sideRot;
	m_sides.push_back(side);

	m_wallMaxBound = m_pos;
	m_wallMaxBound.y += height;
	m_wallMaxBound.x += width/2;
	m_wallMaxBound.z += depth/2;

	m_wallMinBound = m_pos;
	m_wallMinBound.x -= width/2;
	m_wallMinBound.z -= depth/2;
}
Wall::~Wall()
{
	
	if(!m_sides.empty())
	{
		m_sides.clear();
	}
}
