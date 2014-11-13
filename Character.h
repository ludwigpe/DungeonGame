#ifndef CHARACTER_H
#define CHARACTER_H
#include "GameObject.h"
#include "GameCollider.h"
#include "BattleAttack.h"

class Character : public GameObject, public GameCollider
{
protected:
	int m_maxHealth;	// the max health of the character
	int m_health;		// the health of the character
	int m_dodgeChance; //  chance to dodge between 0 - 100
	D3DXVECTOR3 m_lastPos;	// last position of the character. used when colliding to reposition character
	typedef std::map<UINT, BattleAttack*>::iterator AttackMapIterator;
	typedef std::vector<std::pair<UINT, BattleAttack*>> AttackVector;
//	std::map<UINT, BattleAttack*> m_attackMap;
	AttackVector m_attackMap;
	const char* m_modelName;
public:
	Character();
	Character(Mesh* mesh);
	~Character(){}
	virtual void Update(float timestep) = 0;
	virtual void Draw(Renderer* renderer);

	// collider interface
	virtual void OnCollision(GameObject* other) = 0;	// a character must ovveride the OnCollision method since it should do something
	virtual D3DXVECTOR3 GetMaxBound();			// gets the max bound of the bounding box in world space
	virtual D3DXVECTOR3 GetMinBound();			// gets the min bound of the bounding box in world space
	virtual bool SetUpCollider();
	virtual GameObject* GetGameObject() { return this;}
	virtual void DealDamage(int dmg);			// a way to deal damage
	int GetCurrentHealth() { return m_health; }
	void AddAttack(UINT keyCode, BattleAttack* attack);
	int GetDodgeChance(){return m_dodgeChance; }
	void GiveHealth(int hp);
	const char* GetModelName(){ return m_modelName; }
	AttackVector GetAttacks(){ return m_attackMap; }
};
#endif // !CHARACTER_H
