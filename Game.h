#ifndef GAME_H
#define GAME_H

#include <vector>

//#include "GameObject.h"
#include "Camera.h"
#include "InputController.h"
#include "MeshManager.h"
#include "TextureManager.h"
#include "StateMachine.h"
#include "CollisionHandler.h"
#include "ObjectHandler.h"
#include "GUIHandler.h"
#include "Renderer.h"
#include "BattleAttack.h"
//#include "CollisionHandler.h"
#include "GameHandler.h"
#include "GameMap.h"
#include "AIHandler.h"
class Character;
class Player;
enum PlayerTypes;
class Enemy;
//An enum to represent the states of the game. Used by the state machine object.
enum GameStates
{
	EMPTY_STATE,
	MENU_STATE,
	GAME_STATE,
	BATTLE_STATE,
	GAMEOVER_STATE
};

//#define ATTACKTIME 3.0f

class Game
{
private:
	StateMachine<GameStates, Game>* m_stateMachine;	//A state machine object that calls the correct state functions at the correct time
	TextureManager* m_textureManager;		//A texture manager object used to load the game content
	MeshManager* m_meshManager;				//A mesh manager object used to load the game content
	InputController* m_inputController;		//A pointer to the input controller. Allows the game to access the keyboard and mouse data
	Renderer* m_renderer;					//A pointer to the renderer object. Used to draw the game
	Camera* m_mainCamera;					//A camera object, we will look at this next week
	CollisionHandler* m_collisionHandler;
	GameMap* m_map;
	AIHandler* m_aiHandler;
	//GameHandler* m_gameHandler;
	GUIHandler* m_guiHandler;
	ObjectHandler* m_objectHandler;
	Character* m_characterTurn;
	float m_attackTimeLeft;
	//std::vector<GameObject*> m_gameObjects;	//A list of GameObjects. We will look at this next week
	D3DLIGHT9* m_light;
	Player* m_battlePlayer;
	Enemy* m_battleEnemy;
	std::vector<std::pair<GUIObject*, PlayerTypes>> m_menuChoices;
	GUIObject* m_menuPointer;
	long long m_currentCount;	//The current count of the performance counter. Used to calculate the timestep
	long long m_prevCount;		//The count of the performance counter last frame. Used to calculate the timestep
	long long m_frequency;		//The number of counts per second. Used to calculate the timestep
	GameStates m_currentState;
	int m_menuChoice;
	D3DXVECTOR3 m_playerOldPos;
public:
	//Constructor
	Game(Renderer* renderer, InputController* inputController);

	//Destructor
	~Game();
	static GameStates currentState;
	
	bool Initialise();	//Sets up the game. Load content, initialise timestep etc.
	void Update();		//Updates the game. Calculate timestep, Update Input, Update current state etc.
	void Draw();		//Draws the game. Begin the scene, Draw the current state, end the scene

	bool AddGameObject(GameStates state, GameObject* gameObject, const char* meshName);
	bool AddGameObject(GameStates state, GameObject* gameObject);
	void AddCollider(GameStates state, GameCollider* gCollider);
	void AddGUIObject(GameStates state, GUIObject* guiObject);

	void RemoveObject(GameObject* theObject);
	void RemoveObjectFromState(GameStates state, GameObject* theObject);
	void RemoveObjectFromMap(GameObject* theObject);
	void Attack(GameObject* attacker, BattleAttack* bAttack);
	void ChangeState(GameStates nextState);
	bool InitialiseGameObjects();
	Character* GetCharacterTurn(){ return m_characterTurn; }
	//The menu state functions
	void Menu_Enter();
	void Menu_Update(float timestep);
	void Menu_Draw();
	void Menu_Exit();

	//The game state functions
	void Game_Enter();
	void Game_Update(float timestep);
	void Game_Draw();
	void Game_Exit();

	// The battle state functions
	void Battle_Enter();
	void Battle_Update(float timestep);
	void Battle_Draw();
	void Battle_Exit();

	//The game over state functions
	void GameOver_Enter();
	void GameOver_Update(float timestep);
	void GameOver_Draw();
	void GameOver_Exit();

	D3DXVECTOR3 GetRandomPosOnMap();
};

#endif