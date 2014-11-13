#ifndef BATTLEHANDLER_H
#define BATTLEHANDLER_H
#include "BattleAttack.h"
class GameObject;
class Enemy;
class Player;
class GameHandler;
class Renderer;


#define ATTACKTIME 10.0f

class BattleHandler
{
private:
	Enemy* m_enemy;
	Player* m_player;
	GameHandler* m_gameHandler;
	GameObject* m_objectTurn;
	float m_timeLeft;

public:
	BattleHandler(GameHandler* gHandler);
	~BattleHandler(){}
	void SetTimeLeft(float timeleft) { m_timeLeft = timeleft; }
	void Update(float timestep);
	void AddBattlePlayer(Player* p) { m_player = p; }
	void AddBattleEnemy(Enemy* e) { m_enemy = e; }
	void InitBattle(Player* p, Enemy* e);
	void Attack(GameObject* attacker, BattleAttack* bAttack);
	void Draw(Renderer* renderer);
	GameObject* GetObjectTurn(){ return m_objectTurn; }

};

#endif // !BATTLEHANDLER_H
