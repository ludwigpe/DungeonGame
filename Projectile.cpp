#include "Projectile.h"
#include "Game.h"
#include <sstream>
Projectile::Projectile(float speed, D3DXVECTOR3 pos, D3DXVECTOR3 direction) : GameObject(), GameCollider()
{
	m_speed = speed;
	m_pos = pos;
	m_direction = direction;
	m_timetoLive = DEFAULT_TTL;
	m_isVisible = true;
	m_tag = "Projectile";
}
Projectile::Projectile(float speed, D3DXVECTOR3 pos, D3DXVECTOR3 direction, Mesh* mesh) : GameObject(mesh)
{
	m_speed = speed;
	m_pos = pos;
	m_direction = direction;
	m_timetoLive = DEFAULT_TTL;
	m_isVisible = true;
	m_tag = "Projectile";
	SetUpCollider();
}
void Projectile::Update(float timestep)
{
	if(!m_isVisible)
		return;
	m_timetoLive -= timestep;
	/*if(m_timetoLive <= 0)
	{
		m_isVisible = false;
	}
	*/
	m_pos += m_direction * m_speed * timestep;
}
void Projectile::Draw(Renderer* renderer)
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
void Projectile::OnCollision(GameObject* other)
{
	// projectiles should not collide with eachother, this is not bulletproof check but works for now
	if(other->GetTag() == "Projectile")
		return; 

	if(m_bAttack)
	{
		// call the onHit method on the the battleAttack object to let it deal with the 'other' object
		m_bAttack->OnHit(other);
		if(m_bAttack->GetFinalDmg() == 0)
		{

			// a miss occured
			// add a GUI object with the text "miss" in yellow
			GUIObject* gui = new GUIObject("MISS", 1.0f, GUI_WORLD, m_pos);
			gui->SetFontColor(255, 255, 0);	// yellow collor
			m_gameHandler->AddGUIObject(Game::currentState, gui);
		}
		else
		{
			// add a GUI object with the text "HIT" and damage number in red
			std::string text;
			text.append("HIT -");
			std::ostringstream ss;
			ss << m_bAttack->GetFinalDmg();
			text.append(ss.str());
			GUIObject* gui = new GUIObject(text, 1.0f, GUI_WORLD, m_pos);
			gui->SetFontColor(255, 0, 0); // red colors
			m_gameHandler->AddGUIObject(Game::currentState, gui);
		}
	}
	m_isVisible = false;
}
D3DXVECTOR3 Projectile::GetMaxBound()
{
	/*	returns the maxBound of the bounding box around the object in world space
		this is used by the collision handler to check if two boxes collide
	*/
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
D3DXVECTOR3 Projectile::GetMinBound()
{
	/*	returns the minBound of the bounding box around the object in world space
		this is used by the collision handler to check if two boxes collide
	*/

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
bool Projectile::SetUpCollider()
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
	m_boundingBox = new BoundingBox(D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1, 1, 1));
	return true;
}