#include "Player.h"
#include "Game.h"
#include "Projectile.h"
#include "FireballAttack.h"
Player::Player(InputController* inputController, Mesh* mesh) : Character(mesh)
{
	m_inputController = inputController;
	m_isVisible = true;
	m_tag = "Player";
	//Collider* collider = new Collider(this, &OnCollision2);
	m_dodgeChance = 30;
	m_attackMap.clear();
	m_spotLight = NULL;
}
Player::Player(InputController* inputController, PlayerTypes playerType) : Character()
{
	m_inputController = inputController;
	m_isVisible = true;
	m_tag = "Player";
	m_dodgeChance = 30;
	m_attackMap.clear();
	m_spotLight = NULL;
	m_playerType = playerType;
	BattleAttack* attack;
	UINT keyCode;

	switch (m_playerType)
	{
	case PLAYER_KNIGHT:
		m_modelName = "KnightModel.x";
		attack = new BattleAttack("Sword Attack", KNIGHT_SWORDATTACK);
		keyCode = 0x31;
		AddAttack(keyCode, attack);

		attack = new BattleAttack("Sword Powerattack", KNIGHT_POWERATTACK);
		keyCode = 0x32;
		AddAttack(keyCode, attack);
		break;
	case PLAYER_MAGE:
		m_modelName = "MageModel.x";
		attack = new BattleAttack("Cast Fireball", MAGE_FIREBALL);
		keyCode = 0x31; //num 1-key
		AddAttack(keyCode, attack);

		attack = new BattleAttack("Mage Powerattack", MAGE_POWERATTACK);
		keyCode = 0x32;
		AddAttack(keyCode, attack);
		break;
	case PLAYER_RANGER:
		m_modelName = "RangerModel.x";
		
		attack = new BattleAttack("Shoot Arrow", RANGER_ARROW);
		keyCode = 0x31;
		AddAttack(keyCode, attack);

		attack = new BattleAttack("Tripple Arrow attack", RANGER_TRIARROW);
		keyCode = 0x32;
		AddAttack(keyCode, attack);
		break;
	default:
		m_modelName = "KnightModel.x";
		attack = new BattleAttack("Sword Attack", KNIGHT_SWORDATTACK);
		keyCode = 0x31;
		AddAttack(keyCode, attack);

		attack = new BattleAttack("Sword Powerattack", KNIGHT_POWERATTACK);
		keyCode = 0x32;
		AddAttack(keyCode, attack);
		break;
	}
}

void Player::Update(float timestep)
{
	GameStates currState = Game::currentState;
	m_lastPos = m_pos;
	if(!m_isVisible)
		return;

	switch (currState)
	{
	case EMPTY_STATE:
		break;
	case MENU_STATE:
		break;
	case GAME_STATE:
		{
			m_lastPos = m_pos;
				//If left or right are pressed increase or decrease the y rotation



			if(m_inputController->GetKeyDown(VK_LEFT))
			{
				m_rotation.y -= ROT_SPEED * timestep;

			}
			if(m_inputController->GetKeyDown(VK_RIGHT))
			{
				m_rotation.y += ROT_SPEED * timestep;
			}

			//Use the y rotation value to modify the concept of "forward"
			D3DXVECTOR3 pos(m_pos.x, m_pos.y, m_pos.z);
			D3DXVECTOR3 forward(0, 0, 1);
			D3DXMATRIX rotationY;
			D3DXMatrixRotationY(&rotationY, m_rotation.y);
			D3DXVec3TransformCoord(&forward, &forward, &rotationY);
			m_direction = forward;
			//Add or subtract our new "forward" direction to our position
			if(m_inputController->GetKeyDown(VK_UP))
			{
				pos += forward * MOVE_SPEED * timestep;
			}
			if(m_inputController->GetKeyDown(VK_DOWN))
			{
				pos -= forward * MOVE_SPEED * timestep;
			}

			m_pos = pos;

			// update spotlight
		/*	if(m_spotLight)
			{
				D3DXVECTOR3 spotPos = m_pos;
				spotPos.z += m_direction.z * m_boundingBox->GetDepth/2)
				m_spotLight->Position = m_pos;
			}
			*/
		}
		break;
	case BATTLE_STATE:
		{
			m_lastPos = m_pos;
			if(!(m_gameHandler->GetCharacterTurn() == this))
				return;
			for(size_t  i = 0; i < m_attackMap.size(); i++)
			{
				int keyCode = m_attackMap[i].first;
				if(m_inputController->GetKeyUp(keyCode))
				{
					Projectile* projectile = new Projectile(50, m_pos, m_direction);
					BattleAttack* pAttack =  m_attackMap[i].second;
					projectile->SetBattleAttack(pAttack);
					D3DXVECTOR3 projectilePos = m_pos;
					projectilePos.z += m_direction.z * (GetMaxBound().z +10);
	
					//projectilePos.y += GetMaxBound().y - (m_boundingBox->GetHeight()/2);
					projectilePos.y = 10;

					projectile->SetPos(projectilePos);
					projectile->SetRotation(m_rotation);
					projectile->SetDirection(m_direction);

					m_gameHandler->AddGameObject(BATTLE_STATE, projectile, pAttack->GetModelName());
					m_gameHandler->Attack(this, pAttack);

					break;
				}
			}

	
		}
		break;
	case GAMEOVER_STATE:
		break;
	default:
		break;
	}
	
	GameObject::Update(timestep);
}




void Player::OnCollision(GameObject* other)
{

	m_pos = m_lastPos;
}

