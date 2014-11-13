#ifndef BATTLEATTACK_H
#define BATTLEATTACK_H
/*
	This class describes an attack that can have min and max damage
	a type and a name. THe game will calculate what the final damage for
	the attack will be and set it as the final damage
*/
enum AttackType
{
	KNIGHT_SWORDATTACK,
	KNIGHT_POWERATTACK,
	MAGE_FIREBALL,
	MAGE_POWERATTACK,
	RANGER_ARROW,
	RANGER_TRIARROW,
	FIREBALL,
	CERBERUS_STUNN,
	BOSS_POWERATTACK,
};
class GameObject;
class Player;
class Enemy;


class BattleAttack
{
protected:
	AttackType m_attackType;	// type of attack
	const char* m_attackName;	// name of the attack
	int m_minDamage;			// minimum possible damage
	int m_maxDamage;			// maximum possible damage
	float m_successrate;		// not used
	int m_finalDmg;				// the final damage to be dealt upon impact
	const char* m_modelName;	// name of the model to use when casting this battle attack
	float m_attackVelocity;		// the velocity that the attack will travel when attacked to a proejctile object
public:
	BattleAttack(const char* name, AttackType type, int minDamage, int maxDamage);
	BattleAttack(const char* name, AttackType type);
	~BattleAttack(){};
	virtual void OnHit(GameObject* other);	// this function can be overriden by other objects that inherit from this one.
	int GetMinDamage() { return m_minDamage; }
	int GetMaxDamage() { return m_maxDamage; }
	void SetFinalDmg(int fDmg) { m_finalDmg = fDmg; }
	int GetFinalDmg(){ return m_finalDmg; }
	AttackType GetAttackType(){ return m_attackType; }
	const char* GetModelName() { return m_modelName; }
	const char* GetAttackName() { return m_attackName; }
	float GetVelocity() { return m_attackVelocity; }

};

#endif // !BATTLEATTACK_H
