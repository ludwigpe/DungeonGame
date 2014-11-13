#include "BattleAttack.h"
#include "Character.h"
BattleAttack::BattleAttack(const char* name, AttackType type, int minDamage, int maxDamage)
{
	m_attackName = name;
	m_attackType = type;
	m_minDamage = minDamage;
	m_maxDamage = maxDamage;
}
BattleAttack::BattleAttack(const char* name, AttackType type)
{
	m_attackName = name;
	m_attackType = type;
	m_attackVelocity = 30;
	switch (type)
	{
	case KNIGHT_SWORDATTACK:
		m_modelName = NULL;
		m_minDamage = 20;
		m_maxDamage = 30;
		m_successrate = 70; //percent
		m_attackVelocity = 70;
		break;
	case KNIGHT_POWERATTACK:
		m_modelName = NULL;
		m_minDamage = 80;
		m_maxDamage = 90;
		m_successrate = 100; //percent
		m_attackVelocity = 100;
		break;
	case MAGE_FIREBALL:
		m_modelName = "BoltModel.x";
		m_minDamage = 50;
		m_maxDamage = 60;
		m_successrate = 70; //percent
		m_attackVelocity = 50;
		break;
	case MAGE_POWERATTACK:
		m_modelName = "BoltModel.x";
		m_minDamage = 85;
		m_maxDamage = 90;
		m_successrate = 88; //percent
		m_attackVelocity = 80;
		break;
	case RANGER_ARROW:
		m_modelName = "ArrowModel.x";
		m_minDamage = 45;
		m_maxDamage = 60;
		m_successrate = 70; //percent
		m_attackVelocity = 70;
		break;
	case RANGER_TRIARROW:
		m_modelName = "ArrowModel.x";
		m_minDamage = 75;
		m_maxDamage = 100;
		m_successrate = 76; //percent
		m_attackVelocity = 70;
		break;
	case FIREBALL:
		m_modelName = "BoltModel.x";
		m_minDamage = 20;
		m_maxDamage = 30;
		m_successrate = 70; //percent
		m_attackVelocity = 50;
		break;
	case CERBERUS_STUNN:
		m_modelName = NULL;
		m_minDamage = 20;
		m_maxDamage = 30;
		m_successrate = 70; //percent
		m_attackVelocity = 100;
		break;
	case BOSS_POWERATTACK:
		m_modelName = "BoltModel.x";
		m_minDamage = 20;
		m_maxDamage = 80;
		m_successrate = 70; //percent
		m_attackVelocity = 20;
	default:
		break;
	}
}
void BattleAttack::OnHit(GameObject* other)
{
	// if this function is not overriden try to deal damage
	if(Character* cOther = dynamic_cast<Character*>(other))
	{
		cOther->DealDamage(m_finalDmg);
	}
}