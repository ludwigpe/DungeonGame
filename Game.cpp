

#include "Game.h"
#include <time.h>
#include <sstream>
#include "Character.h"
#include "Player.h"
#include "Enemy.h"
#include "Chest.h"

#include "GameMap.h"
#include "GameComponent.h"

#include "Projectile.h"
#include "BattleAttack.h"
#include "FireballAttack.h"
#include "AIComponent.h"

#define BOARD_SIZE 5
GameStates Game::currentState;

Game::Game(Renderer* renderer, InputController* inputController)
{
	//Assign the renderer and input controller to the memeber variables
	m_renderer = renderer;
	m_inputController = inputController;

	//Create the texture and mesh managers
	m_textureManager = new TextureManager();
	m_meshManager = new MeshManager(m_textureManager);
	m_attackTimeLeft = ATTACKTIME;
	srand ( (unsigned int)time(NULL) );
	m_characterTurn = NULL;
	//Create the state machine and register the states to their functions
	m_stateMachine = new StateMachine<GameStates, Game>(this, EMPTY_STATE);
	m_stateMachine->RegisterState(MENU_STATE, &Game::Menu_Enter, &Game::Menu_Update,
		&Game::Menu_Draw, &Game::Menu_Exit);
	m_stateMachine->RegisterState(GAME_STATE, &Game::Game_Enter, &Game::Game_Update,
		&Game::Game_Draw, &Game::Game_Exit);
	m_stateMachine->RegisterState(BATTLE_STATE, &Game::Battle_Enter, &Game::Battle_Update,
		&Game::Battle_Draw, &Game::Battle_Exit);
	m_stateMachine->RegisterState(GAMEOVER_STATE, &Game::GameOver_Enter, &Game::GameOver_Update,
		&Game::GameOver_Draw, &Game::GameOver_Exit);


}

Game::~Game()
{
	//Delete all of our game objects

	//Delete our camera
	delete m_mainCamera;
	if(m_aiHandler)
	{
		delete m_aiHandler;
		m_aiHandler = NULL;
	}
	if(m_guiHandler)
	{
		delete m_guiHandler;
		m_guiHandler = NULL;
	}

	if(m_objectHandler)
	{
		m_objectHandler->Release();
		delete m_objectHandler;
		m_objectHandler = NULL;
	}

	if(m_collisionHandler)
	{
		delete m_collisionHandler;
		m_collisionHandler = NULL;
	}
	
	//If there is a mesh manager, delete it
	if(m_meshManager)
	{
		delete m_meshManager;
		m_meshManager = NULL;
	}

	//If there is a texture manager, delete it
	if(m_textureManager)
	{
		delete m_textureManager;
		m_textureManager = NULL;
	}
	
	//If there is a state machine, delete it
	if(m_stateMachine)
	{
		delete m_stateMachine;
		m_stateMachine = NULL;
	}
}

bool Game::Initialise()
{
	//Load content!
	//Use the texture and mesh managers to do this
	//Load our model

	m_objectHandler = new ObjectHandler(m_meshManager, m_textureManager);
	m_collisionHandler = new CollisionHandler();
	m_collisionHandler->SetGameHandler(this);
	m_guiHandler = new GUIHandler(m_renderer);
	
	if(!InitialiseGameObjects())
		return false;

	//Create a fixed camera

	//Find the counter frequency (the number of counts per second)
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	m_frequency = freq.QuadPart;

	//Initialise the current and previous counts with the current count of the counter
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	m_currentCount = count.QuadPart;
	m_prevCount = count.QuadPart;

	//Change state to the starting state (game)
	ChangeState(MENU_STATE);


	return true;
}
bool Game::InitialiseGameObjects()
{
	
	m_map = new GameMap("dungeonMap.txt", m_meshManager, m_renderer, this);
	m_map->SetTag("Map");
	std::vector<D3DXVECTOR3> positionsToFree;
	D3DXVECTOR3 objectPos;
	if(!AddGameObject(GAME_STATE, m_map))
		return false;
	m_aiHandler = new AIHandler(m_map);

	// create cerberus enemies
	
	for(int i = 0; i < 3; i++)
	{
		Enemy* enemy = new Enemy(ENEMY_CERBERUS);
		objectPos = m_map->GetRandomFreePosition();
		positionsToFree.push_back(objectPos);
		enemy->SetPos(objectPos);
		enemy->SetTag("Enemy");
		AIComponent* aiComp = new AIComponent(enemy, m_map, true);
		enemy->AddComponent(aiComp);

		if(!AddGameObject(GAME_STATE, enemy, enemy->GetModelName()))
			return false;
	}
		for(int i = 0; i < 1; i++)
	{
		Enemy* enemy = new Enemy(ENEMY_BOSS);
		objectPos = m_map->GetRandomFreePosition();
		positionsToFree.push_back(objectPos);
		enemy->SetPos(objectPos);
		enemy->SetTag("Enemy");
		AIComponent* aiComp = new AIComponent(enemy, m_map, true);
		enemy->AddComponent(aiComp);

		if(!AddGameObject(GAME_STATE, enemy, enemy->GetModelName()))
			return false;
	}
	

	// create chests
	for(int i = 0; i < 5; i++)
	{
		Chest* chest = new Chest(20);
		chest->SetPos(m_map->GetRandomFreePosition());
		chest->SetTag("Chest");

		if(!AddGameObject(GAME_STATE, chest, "TreasureModel.x"))
			return false;
	}

	for(size_t  i = 0; i < positionsToFree.size(); i++)
	{
		m_map->SetTileFree(&positionsToFree[i]);
	}
	positionsToFree.clear();

	m_mainCamera = new Camera(0, 50, -50, 0, 0, 0, 0, 1, 0,
		m_renderer->GetWidth() / (float)m_renderer->GetHeight(), D3DX_PI / 2, 1.0f, 5000);

	m_renderer->GetDevice()->SetRenderState(D3DRS_LIGHTING,true);
	D3DLIGHT9 light;
	ZeroMemory( &light, sizeof(D3DLIGHT9) );
	light.Type       = D3DLIGHT_SPOT;
	light.Diffuse.r  = 1.0f;
	light.Diffuse.g  = 1.0f;
	light.Diffuse.b  = 1.0f;
	light.Diffuse.a  = 1.0f;
	
	// Spot lights have direction and a position
	D3DXVECTOR3 spotPos(m_mainCamera->GetPosX(), m_mainCamera->GetPosY(), m_mainCamera->GetPosZ());
	D3DXVECTOR3 spotDir(m_mainCamera->GetLookAtX(),m_mainCamera->GetLookAtY(), m_mainCamera->GetLookAtZ());
	
	// Tell the device about the light and turn it on
	light.Range = 200.0f;
	light.Theta= D3DX_PI/10;
	light.Phi= D3DX_PI/2;
	light.Falloff=1.0f;
	light.Attenuation0= 1.0f;

	m_renderer->GetDevice()->SetLight( 1, &light );
	m_renderer->GetDevice()->LightEnable( 1, TRUE );

	//m_mainCamera->SetFocusedObject(player);
	return true;
}

void Game::Update()
{
	//Calculate the timestep
	LARGE_INTEGER count;
	QueryPerformanceCounter(&count);
	m_currentCount = count.QuadPart;
	float timestep = (m_currentCount - m_prevCount) / (float)m_frequency;
	
	//Update input
	m_inputController->BeginUpdate();
	D3DLIGHT9 light;
	m_renderer->GetDevice()->GetLight(1, &light);
	light.Position = m_mainCamera->GetCameraPos();
	light.Direction = m_mainCamera->GetCameraDirection();
	m_renderer->GetDevice()->SetLight(1, &light);
	if(m_currentState != Game::currentState)
	{
		// someone has signaled the game to change state
		m_currentState = currentState;
		ChangeState(m_currentState);

	}
	//Update the current state
	m_stateMachine->Update(timestep);

	//Finalise the timestep
	m_prevCount = m_currentCount;
	//Finalise the input
	m_inputController->EndUpdate();
}
void Game::Draw()
{
	//Clear the scene
	//Begin the scene
	m_renderer->BeginScene(m_mainCamera);

	//Draw the current state
	m_stateMachine->Draw();

	//End the scene
	//Present the scene
	m_renderer->EndScene();
}
/*
	This function is responsible for adding new game objects to the game
	it is responsible for checking with the mesh manager if it can load a specific mesh
*/
bool Game::AddGameObject(GameStates state, GameObject* object, const char* meshName)
{
	if(meshName != NULL)
	{
		if(!m_meshManager->Load(m_renderer->GetDevice(), meshName))
			return false;
		// set the mesh to the object
		object->SetMesh(m_meshManager->GetMesh(meshName));
	}

	// call the other add game object function
	return AddGameObject(state, object);
}
/*
	This function is responsible for actually adding new game objects
	it checks if the object has a GameCollider, since we then also want to add that
	to the collision handler
*/
bool Game::AddGameObject(GameStates state, GameObject* object)
{

	//If the object implements the GameCollider interface we want to register
	// its collider with the collision handler
	if(GameCollider* collider =  dynamic_cast<GameCollider*>(object))
	{
		AddCollider(state, collider);
	}
	// Give the object a handle to the game
	object->SetGameHandler(this);	
	
	// add the game object to object handler
	m_objectHandler->AddGameObject(state, object);
	return true;
}

// adds colliders to a specific state
void Game::AddCollider(GameStates state, GameCollider* gCollider)
{
	// if there is no bounding box set up, try to do it
	if(!gCollider->GetBoundingBoxLocal())
	{
		gCollider->SetUpCollider();
	}
	// register the collider with the collision handler
	m_collisionHandler->RegisterCollider(state, gCollider);
}

// function to add new GUI objects to a state
void Game::AddGUIObject(GameStates state, GUIObject* guiObject)
{
	m_guiHandler->AddGUIObject(state, guiObject);
}

void Game::RemoveObject(GameObject* theObject)
{
	// removes the object from Game and battle state
	RemoveObjectFromState(GAME_STATE, theObject);
	RemoveObjectFromState(BATTLE_STATE, theObject);

	// release the object
	theObject->Release(m_meshManager);
	delete theObject;
}
void Game::RemoveObjectFromState(GameStates state, GameObject* theObject)
{
	// check if the object implements the GameCollider class, then its collider should be removed 
	// from the collider handler.
	if(GameCollider* theCollider = dynamic_cast<GameCollider*>(theObject))
		m_collisionHandler->RemoveColliderFromState(state, theCollider);
	// remove the object 
	m_objectHandler->RemoveGameObjectFromState(state, theObject->GetObjectId());

}
void Game::RemoveObjectFromMap(GameObject* theObject)
{
	m_map->SetTileFree(&(theObject->GetPos()));
}

void Game::Attack(GameObject* attacker, BattleAttack* bAttack)
{
	if(Game::currentState != BATTLE_STATE)
		return;
	bool dodged;
	// calculate the damage to be done by the attack, somewhere between min and max damage
	int damage = bAttack->GetMinDamage();
	int diff = abs(bAttack->GetMaxDamage() - damage);
	damage += rand() % diff;
	// set the final damage of the attack
	bAttack->SetFinalDmg(damage);

	if(attacker == m_battlePlayer)
	{
		
		dodged = ((rand() % 100) <= m_battleEnemy->GetDodgeChance())?  true : false; // see if enemy dodged the attack
		if(dodged)
			bAttack->SetFinalDmg(0);
		// the player has done his attack, now its the enemy's turn
		m_characterTurn = m_battleEnemy;

		// reset the attack time for the enemy
		m_attackTimeLeft = ATTACKTIME;

	}
	else if(attacker == m_battleEnemy)
	{

		dodged = ((rand() % 100) <= m_battlePlayer->GetDodgeChance())?  true : false; // see if enemy dodged the attack
		if(dodged)
			bAttack->SetFinalDmg(0);

		m_characterTurn = m_battlePlayer;
		m_attackTimeLeft = ATTACKTIME;

	}
		

}
/*
	this function is responsible for changing the state of all handlers and the game
*/
void Game::ChangeState(GameStates nextState)
{
	m_currentState = nextState;
	currentState = nextState;
	m_stateMachine->ChangeState(nextState);
	m_objectHandler->ChangeState(nextState);
	m_collisionHandler->ChangeState(nextState);
	m_guiHandler->ChangeState(nextState);
}

D3DXVECTOR3 Game::GetRandomPosOnMap()
{
	return m_map->GetRandomPosition();
}


void Game::Menu_Enter()
{
	m_menuChoices.clear();

	GUIObject* knightGui = new GUIObject("Knight", 100, GUI_STATIC, D3DXVECTOR3(0, 0, 0));
	GUIObject* mageGui = new GUIObject("Mage", 100, GUI_STATIC, D3DXVECTOR3(0, 0, 0));
	GUIObject* rangerGui = new GUIObject("Ranger", 100, GUI_STATIC, D3DXVECTOR3(0, 0, 0));
	GUIObject* pointer = new GUIObject("Choose", 100, GUI_STATIC, D3DXVECTOR3(0, 0, 0));
	
	
	std::pair<GUIObject*, PlayerTypes> choice(knightGui, PLAYER_KNIGHT);
	m_menuChoices.push_back(choice);

	choice = std::make_pair(mageGui, PLAYER_MAGE);
	m_menuChoices.push_back(choice);
	
	choice = std::make_pair(rangerGui, PLAYER_RANGER);
	m_menuChoices.push_back(choice);

	// calculate the size of the menu rectangle that will contain all the player choices.
	float menuHeight = 0;
	float menuWidth = 0;
	for(size_t  i = 0; i < m_menuChoices.size(); i++)
	{
		GUIObject* gui = m_menuChoices[i].first;
		RECT rct;
		rct = m_guiHandler->CalcRect(gui);
		gui->SetRect(rct);
		long h = abs(rct.bottom-rct.top);
		long w = abs(rct.left - rct.right);
		menuHeight += h;
		menuWidth += w;
	}
	// calc rect for pointe object
	RECT r = m_guiHandler->CalcRect(pointer);
	menuWidth += abs(r.left - r.right);
	
	

	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;

	m_renderer->GetDevice()->GetCreationParameters(&cparams);		// get creation parameters to get the screen height and width
	GetWindowRect(cparams.hFocusWindow, &rect);			// get the rectangle for the game window
		
	// calc screen height and width
	long screenWidth = rect.right - rect.left;
	long screenHeight = rect.bottom - rect.top;

	// calc the center of the screen
	long centerX = screenWidth/2;
	long centerY = screenHeight/2;

	// calc the top left corner of the menu-rectangle
	rect.top = long(centerY - (menuHeight/2));
	rect.left = long(centerX - (menuWidth/2));

	// calculate the position of the pointer.
	long pWidth = abs(r.left - r.right);
	r.top = rect.top;
	r.left = long(centerX + (menuWidth/2) -pWidth);
	pointer->SetPos(D3DXVECTOR3((float)r.left, (float)r.top, 0));
	m_menuPointer = pointer;


	float dy = menuHeight/m_menuChoices.size();

	// calculate the new positions of the GUIObjects within the menu rectangle
	for(size_t  i = 0; i < m_menuChoices.size(); i++)
	{
		GUIObject* gui = m_menuChoices[i].first;
		D3DXVECTOR3 newPos(0,0,0);
		RECT rct;
		newPos.x = (float)rect.left;
		newPos.y = float(rect.top + i*dy);
		rct.left = rect.left;
		rct.top = (long)newPos.y;
		gui->SetRect(rct);
		gui->SetPos(newPos);

		m_guiHandler->AddGUIObject(MENU_STATE, gui);
	}
	
	m_guiHandler->AddGUIObject(MENU_STATE, m_menuPointer);
}
void Game::Menu_Update(float timestep)
{
	D3DXVECTOR3 pointerPos = m_menuPointer->GetPos();
	
	// check if user pressed up or down and change the pointer accordingly
	if(m_inputController->GetKeyUp(VK_DOWN))
	{
		m_menuChoice++;
	}
	else if(m_inputController->GetKeyUp(VK_UP))
	{
		m_menuChoice--;
	}
	else if(m_inputController->GetKeyUp(VK_SPACE))
	{
		// the player has chosen a character
		PlayerTypes type = m_menuChoices[m_menuChoice].second;
		Player* player = new Player(m_inputController, type);
		D3DXVECTOR3 startPos = m_map->GetRandomFreePosition();
		player->SetPos(startPos);
		
		if(!AddGameObject(GAME_STATE, player, player->GetModelName()))
			PostQuitMessage(0);
		m_mainCamera->SetFocusedObject(player);
		ChangeState(GAME_STATE);
	}
	m_menuChoice = m_menuChoice % m_menuChoices.size();
	pointerPos.y = m_menuChoices[m_menuChoice].first->GetPos().y;
	m_menuPointer->SetPos(pointerPos);

	m_guiHandler->Update(timestep);
}
void Game::Menu_Draw()
{

	m_guiHandler->Draw();
}
void Game::Menu_Exit()
{

}

void Game::Game_Enter()
{
	m_battleEnemy = NULL;
	m_battlePlayer = NULL;


}
void Game::Game_Update(float timestep)
{
	//Loop through all of our game objects and call update

	m_objectHandler->Update(timestep);
	m_collisionHandler->Update(timestep);
	m_guiHandler->Update(timestep);
	m_mainCamera->Update(timestep);


}
void Game::Game_Draw()
{
	//Loop through all of our game objects and draw them
	m_objectHandler->Draw(m_renderer);
	m_guiHandler->Draw();
	//m_gameHandler->Draw(m_renderer);

}
void Game::Game_Exit()
{

}

void Game::Battle_Enter()
{
	srand ( (unsigned int)time(NULL) );
	m_attackTimeLeft = ATTACKTIME;

	// Get the player and enemy who will go to battle from the collision handler
	m_battlePlayer = m_collisionHandler->GetBattlers().first;
	m_battleEnemy = m_collisionHandler->GetBattlers().second;
	
	m_playerOldPos = m_battlePlayer->GetPos();
	// create a map for the battle
	GameMap* bMap = new GameMap(5, m_meshManager, m_renderer);
	bMap->SetTag("BattleMap");
	
	// set the player and enemy on opposite sides of the battle map
	D3DXVECTOR3 playerPos = bMap->GetTilePosition(0, 2);
	D3DXVECTOR3 enemyPos = bMap->GetTilePosition(4, 2);

	m_battleEnemy->SetPos(enemyPos);
	m_battlePlayer->SetPos(playerPos);

	// set up the camera for battle
	m_mainCamera->SetFocusedObject(NULL);
	D3DXVECTOR3 camPos = m_map->GetTilePosition(2, 0);
	camPos.y += 50;
	camPos.x -= 10;
	m_mainCamera->SetCameraPosition(camPos);

	// the camera will look at the center of the battle map
	m_mainCamera->SetLookAt(&(m_map->GetTilePosition(2, 2)));

	// set the direction of the characters to face each other
	m_battleEnemy->SetDirection(D3DXVECTOR3(0, 0, -1));
	m_battlePlayer->SetDirection(D3DXVECTOR3(0, 0, 1));

	// add the object to the battle state
	AddGameObject(BATTLE_STATE, m_battlePlayer);
	AddGameObject(BATTLE_STATE, m_battleEnemy);
	AddGameObject(BATTLE_STATE, bMap);
	

	std::string guiText;
	std::vector<std::pair<UINT, BattleAttack*>> attacks = m_battlePlayer->GetAttacks();

	for(size_t i = 0; i < attacks.size(); i++)
	{
		UINT keyCode = attacks[i].first;
		BattleAttack* attack = attacks[i].second;
	//	std::string keyText = GetKeyNameTextA(NULL, &keyText, 10);

	}

	guiText.append("SPACE = fireball attack\n");
	guiText.append("NUMPAD1 = regular attack\n");
	guiText.append("NUMPAD2 = power attack\n");

	D3DDEVICE_CREATION_PARAMETERS cparams;
	RECT rect;

	m_renderer->GetDevice()->GetCreationParameters(&cparams);		// get creation parameters to get the screen height and width
	GetWindowRect(cparams.hFocusWindow, &rect);						// get the rectangle for the game window
		
	// calc screen height and width
	float screenWidth = float(rect.right - rect.left);
	float screenHeight = float(rect.bottom - rect.top);

	D3DXVECTOR3 guiPos;
	guiPos.x = 10;
	guiPos.y = screenHeight - 100;
	guiPos.z = 0;

	GUIObject* gui = new GUIObject(guiText, 100.0f, GUI_STATIC, guiPos);
	AddGUIObject(BATTLE_STATE, gui);

	// the player should start the battle
	m_characterTurn = m_battlePlayer;

}
void Game::Battle_Update(float timestep)
{
	// update and check if player or enemy is dead
		if(m_battleEnemy->GetCurrentHealth() <= 0)
		{
			// the enemy is dead. remove it from the world and go to game state
			RemoveObject(m_battleEnemy);
			ChangeState(GAME_STATE);
			return;
		} 
		else if(m_battlePlayer->GetCurrentHealth() <= 0)
		{
			// the player has died. go to game over
			ChangeState(GAMEOVER_STATE);
			return;
		}

	// switch between whos turn it is
	if(m_attackTimeLeft < 0)
	{
		// reset the timer
		m_attackTimeLeft = ATTACKTIME;
		// switch the current objects turn. If it was player's turn switch to enemy otherwise it is player's turn
		if(m_characterTurn == m_battlePlayer)
			m_characterTurn = m_battleEnemy;
		else
			m_characterTurn = m_battlePlayer;
	}

	// decrease timeleft by timestep
	m_attackTimeLeft -= timestep;

	// update all handlers and camera
	m_objectHandler->Update(timestep);
	m_collisionHandler->Update(timestep);
	m_guiHandler->Update(timestep);
	m_mainCamera->Update(timestep);
}
void Game::Battle_Draw()
{
	// draw all game objects
	m_objectHandler->Draw(m_renderer);
	// draw all gui objects
	m_guiHandler->Draw();

	// draw player and enemy info 
	if(currentState == BATTLE_STATE)
	{
		if(m_battlePlayer != NULL)
			m_guiHandler->DrawPlayerInfo(m_battlePlayer);
		if(m_battleEnemy)
			m_guiHandler->DrawEnemyInfo(m_battleEnemy);
			
	}	

}
void Game::Battle_Exit()
{
	if(m_currentState == GAME_STATE)
	{
		m_characterTurn = NULL;

		// set the player to a random free posiotion in the world map
		m_battlePlayer->SetPos(m_map->GetRandomPosition());
	}
	// clear objects from battle state
	m_objectHandler->ClearObjectsFromState(BATTLE_STATE);

	// clear all colliders from the battle state
	m_collisionHandler->ClearCollidersFromState(BATTLE_STATE);

	// set all gui objects in battle state to invisible
	m_guiHandler->ClearObjectsFromState(BATTLE_STATE);

	// reset the focus to the player
	m_mainCamera->SetFocusedObject(m_battlePlayer);
	m_battlePlayer->SetPos(m_playerOldPos);
	m_battleEnemy = NULL;
	m_battlePlayer = NULL;
}


void Game::GameOver_Enter()
{
	// add the game over text
	GUIObject* gObject = new GUIObject("GAME OVER", 1, GUI_STATIC, D3DXVECTOR3(300, 300, 0));
	m_guiHandler->AddGUIObject(GAMEOVER_STATE, gObject);
}
void Game::GameOver_Update(float timestep)
{
	m_guiHandler->Update(timestep);
}
void Game::GameOver_Draw()
{
	m_guiHandler->Draw();
}
void Game::GameOver_Exit()
{
}

