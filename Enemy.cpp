#include "Enemy.h"
#include "Game.h"
#include "Projectile.h"
#include "FireballAttack.h"
#include <time.h>
Enemy::Enemy(Mesh* mesh, float maxHealt, float health) : Character(mesh)
{
	m_maxHealth = maxHealt;
	m_health = health;
	SetUpCollider();
	m_isVisible = true;
	m_tag = "Enemy";
	m_cooldown = COOLDOWN;
	m_dodgeChance = 10; // 10 percent chance of dodging
}
Enemy::Enemy(float maxHealth) : Character()
{
	m_maxHealth = maxHealth;
	m_health = maxHealth;
	m_isVisible = true;
	m_tag = "Enemy";
	m_cooldown = COOLDOWN;
	m_dodgeChance = 10;
}
Enemy::Enemy(EnemyType type) : Character()
{
	m_maxHealth = 10;
	BattleAttack* attack;
	switch (type)
	{
	case ENEMY_CERBERUS:
		m_modelName = "CerberusModel.x";
		m_maxHealth = 50;
		m_cooldown = 3.0f;
		m_dodgeChance = 5;
		attack = new BattleAttack("Cerberus Stun", CERBERUS_STUNN);
		AddAttack(0x31, attack);
	
		break;
	case ENEMY_BOSS:
		m_modelName = "BossModel.x";
		m_maxHealth = 200;
		m_cooldown = 5.0f;
		m_dodgeChance = 20;

		attack = new BattleAttack("Boss Powerattack", BOSS_POWERATTACK);
		AddAttack(0x31, attack);

		break;
	default:
		m_modelName = "CerberusModel.x";
		m_maxHealth = 50;
		m_cooldown = 3.0f;
		m_dodgeChance = 5;
		attack = new BattleAttack("Cerberus Stun", CERBERUS_STUNN);
		AddAttack(0x31, attack);
		break;
	}
	m_health = m_maxHealth;
	m_isVisible = true;
	m_tag = "Enemy";

}

void Enemy::Update(float timestep)
{
	if(!m_isVisible)
		return;
	m_lastPos = m_pos;
	

	switch (Game::currentState)
	{
	case GAME_STATE:
		GameObject::Update(timestep);
		break;
	case BATTLE_STATE:
		{
			m_lastPos = m_pos;
			if(!(m_gameHandler->GetCharacterTurn() == this))
				return;

			m_cooldown += timestep;

			if(!(m_cooldown > COOLDOWN))
				return;

			srand(time(NULL));
			int attackIndex = rand() % m_attackMap.size();
			
			Projectile* projectile = new Projectile(30, m_pos, m_direction);
			BattleAttack* pAttack = m_attackMap[attackIndex].second;
			projectile->SetBattleAttack(pAttack);
			D3DXVECTOR3 projectilePos = m_pos;
			float z = (m_boundingBox->GetDepth()/2 +10);
			z *= m_direction.z;
			projectilePos.z += z;
			//projectilePos.y += GetMaxBound().y - (m_boundingBox->GetHeight()/2);
			projectilePos.y += 5;
			projectile->SetPos(projectilePos);
			projectile->SetRotation(m_rotation);
			projectile->SetDirection(m_direction);

			m_gameHandler->AddGameObject(BATTLE_STATE, projectile, pAttack->GetModelName());
			m_gameHandler->Attack(this, pAttack);
			m_cooldown = 0;
		}
	default:
		break;
	}


}

D3DXVECTOR3 Enemy::GetRandomTarget()
{
	return m_gameHandler->GetRandomPosOnMap();
}
void Enemy::OnCollision(GameObject* other)
{
	if(other->GetTag() == "Enemy")
	{
		m_pos = m_lastPos;
		AIComponent* ai = GetComponent<AIComponent>();
		ai->OnCollision(other);
	} else {
		m_pos = m_lastPos;
	}
	
}
