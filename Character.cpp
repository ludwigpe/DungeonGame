#include "Character.h"

Character::Character() : GameObject(), GameCollider()
{
	m_isVisible =true;
	m_maxHealth = 100;
	m_health = m_maxHealth;
	m_attackMap.clear();
}
Character::Character(Mesh* mesh) : GameObject(mesh)
{
	m_maxHealth = 100;
	m_health = m_maxHealth;
	SetUpCollider();
	m_attackMap.clear();
}
void Character::Draw(Renderer* renderer)
{
	GameObject::Draw(renderer);
	if(m_boundingBox)
		GameCollider::DrawBounds(renderer);
}

/*
	Calculates the Maxbound of the characters bounding box in world space
	and returns it as a vec3
*/ 
D3DXVECTOR3 Character::GetMaxBound()
{
	if(m_boundingBox != NULL)
	{
		
		D3DXVECTOR3 maxBound = m_boundingBox->GetMaxBound();
		m_colliderPosition = m_pos;
		maxBound.x = m_colliderPosition.x + m_boundingBox->GetWidth()/2;
		maxBound.y = m_colliderPosition.y + m_boundingBox->GetHeight();
		maxBound.z = m_colliderPosition.z + m_boundingBox->GetDepth()/2;
		
		return maxBound;
		/*D3DXMATRIX world;
		D3DXMATRIX rotation;
		D3DXMATRIX scale;
		D3DXMATRIX translate;
		m_colliderPosition = m_pos;
		m_colliderPosition.y += m_boundingBox->GetHeight()/2;

		D3DXVECTOR3 localMaxBound = m_boundingBox->GetMaxBound();
		D3DXVECTOR3 worldMaxBound;

		D3DXMatrixScaling(&scale, m_scale.x, m_scale.y, m_scale.z);
		D3DXMatrixRotationYawPitchRoll(&rotation, m_rotation.y, m_rotation.x, m_rotation.z);
		//D3DXMatrixTranslation(&translate, m_pos.x, m_pos.y, m_pos.z);
		D3DXMatrixTranslation(&translate, m_colliderPosition.x, m_colliderPosition.y, m_colliderPosition.z);
		world = scale * rotation * translate;
		D3DXVec3TransformCoord(&worldMaxBound, &localMaxBound, &world);
		
		return worldMaxBound;*/

	}
	return D3DXVECTOR3(-100, -100, -100);
}
/*
	Calculates the minBound of the characters bounding box in world space
	and returns it as a vec3
*/ 
D3DXVECTOR3 Character::GetMinBound()
{
	if(m_boundingBox != NULL)
		{
			
			D3DXVECTOR3 minBound = m_boundingBox->GetMinBound();
			m_colliderPosition = m_pos;

			minBound.x = m_colliderPosition.x - m_boundingBox->GetWidth()/2;
			minBound.y = m_colliderPosition.y;	// bottom of player
			minBound.z = m_colliderPosition.z - m_boundingBox->GetDepth()/2;

			return minBound;
			/*
			D3DXMATRIX world;
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


		}
	return D3DXVECTOR3(-100, -100, -100);
}
bool Character::SetUpCollider()
{
	if(m_mesh != NULL)
	{
		m_boundingBox = new BoundingBox(m_mesh);
		return true;
	}
	return false;
}
void Character::DealDamage(int dmg)
{
	m_health -= dmg;
	if(m_health <= 0)
		m_isVisible = false;
}
void Character::GiveHealth(int hp)
{
	int extra = 0;
	if((m_health + hp) >m_maxHealth)
		extra = (m_health + hp) % m_maxHealth;

	hp -= extra;
	m_health += hp;
}

void Character::AddAttack(UINT keyCode, BattleAttack* attack)
{
		m_attackMap.push_back(std::pair<UINT, BattleAttack*>(keyCode, attack));
}