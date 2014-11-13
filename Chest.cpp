#include "Chest.h"
#include "Game.h"
#include "Character.h"
Chest::Chest(int reward) : GameObject(), GameCollider()
{
	m_hpReward = reward;
}
void Chest::OnCollision(GameObject* other)
{
	// give health or item to character
	if(Character* cOther = dynamic_cast<Character*>(other))
		cOther->GiveHealth(m_hpReward);

	m_isVisible = false;
	m_gameHandler->RemoveObjectFromMap(this);

}
bool Chest::SetUpCollider()
{
	/*
		This method sets up a bounding box arround the projectile
		if it does not have mesh it creates a small bounding box

	*/
	if(m_mesh != NULL)
	{
		m_boundingBox = new BoundingBox(m_mesh);
		return true;
	}

	return false;
}
D3DXVECTOR3 Chest::GetMaxBound()
{
if(m_boundingBox != NULL)
	{
		D3DXVECTOR3 maxBound = m_boundingBox->GetMaxBound();
		m_colliderPosition = m_pos;
		maxBound.x = m_colliderPosition.x + m_boundingBox->GetWidth()/2;
		maxBound.y = m_colliderPosition.y + m_boundingBox->GetHeight();
		maxBound.z = m_colliderPosition.z + m_boundingBox->GetDepth()/2;

	return maxBound;
	}
	return D3DXVECTOR3(-100, -100, -100);
}
D3DXVECTOR3 Chest::GetMinBound()
{
if(m_boundingBox != NULL)
		{
			// calculate the minbound in world space
			D3DXVECTOR3 minBound = m_boundingBox->GetMinBound();
			m_colliderPosition = m_pos;
			minBound.x = m_colliderPosition.x - m_boundingBox->GetWidth()/2;
			minBound.y = m_colliderPosition.y;
			minBound.z = m_colliderPosition.z - m_boundingBox->GetDepth()/2;

			return minBound;
		}
	return D3DXVECTOR3(-100, -100, -100);
}

void Chest::Draw(Renderer* renderer)
{
		// only draw the object if it is visible and has a mesh
	if(!m_isVisible)
		return;
	
	if(m_mesh)
	{
		GameObject::Draw(renderer);
		if(m_boundingBox)
			GameCollider::DrawBounds(renderer);
	}
}

void Chest::Update(float timestep)
{
	
}