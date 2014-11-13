#include "GameMap.h"
#include "Floor.h"
#include <stdlib.h>
#include <time.h>
#include "Game.h"
#include <math.h>

GameMap::GameMap(int size, MeshManager* mManager, Renderer* renderer)
{
	m_size = size;
	m_gameMap.clear();
	m_gameMap.resize(m_size);
	m_isBlocked.resize(m_size);
	float tileWidth;
	float tileDepth;

	if(!mManager->Load(renderer->GetDevice(), "TileModel.x"))
		return; // probably destroy this object

	Floor* f = new Floor(mManager->GetMesh("TileModel.x"));
	tileDepth = f->GetDepth();
	tileWidth = f->GetWidth();

	m_tileDepth = f->GetDepth();
	m_tileWidth = f->GetWidth();
	m_tileHeight = f->GetHeight();

	f->Release(mManager);
	f = NULL;

	for(int row = 0; row < m_size; row++)
	{
		m_gameMap[row].resize(m_size);
		m_isBlocked[row].resize(m_size);
		for(int col = 0; col < m_size; col++)
		{
			D3DXVECTOR3 floorPos = D3DXVECTOR3(m_pos.x + col*tileWidth, 0, m_pos.z + row*tileDepth);
			Floor* f = new Floor(mManager->GetMesh("TileModel.x"));
			f->SetPos(floorPos);
			m_gameMap[row][col] = f;
			m_isBlocked[row][col] = false;
			f->SetOccupied(false);
		}
	}
}

GameMap::GameMap(const char* mapFileName, MeshManager* mManager, Renderer* renderer, Game* gHandler)
{
	m_gameHandler = gHandler;
	m_gameMap.clear();
	m_isBlocked.clear();
	if(!mManager->Load(renderer->GetDevice(), "TileModel.x"))
		return; // probably destroy this object

	Floor* f = new Floor(mManager->GetMesh("TileModel.x"));

	m_tileDepth = f->GetDepth();
	m_tileWidth = f->GetWidth();
	m_tileHeight = f->GetHeight();

	bool ok = CreateMapFromFile(mapFileName, mManager);
	if(!ok)
		GameMap(10, mManager, renderer);
}
/*
	This function reads and creates a game map from file. 
	It reads row by row and checks each character on every row of the file.
	X is used to identify a wall and the '¨.' character is used to identify a floor tile
	It goes through the file and creates floor tiles and walls for the gamemap object
*/
bool GameMap::CreateMapFromFile(const char* filename, MeshManager* meshManager)
{
	std::ifstream inputFile;
	inputFile.open( filename );
	std::string rowString;
	if(inputFile) // check if file opening was ok
	{
		
		// count the number of lines in the file
		int numRows = 0;
		while(std::getline(inputFile, rowString))
			numRows++;

		inputFile.close();
		m_gameMap.resize(numRows);
		m_isBlocked.resize(numRows);

		inputFile.open( filename);

		int row = 0;

		while (!inputFile.eof()) // read until the end of file
		{
			std::getline(inputFile, rowString);
			// resize the gamemap and isBlocked matrices
			m_gameMap[row].resize(rowString.size());
			m_isBlocked[row].resize(rowString.size());

			// go through each character of the input string
			for(size_t  col = 0; col < rowString.size(); col++)
			{
				
				// calc position
				D3DXVECTOR3 position = D3DXVECTOR3(m_pos.x + col*m_tileWidth, 0, m_pos.z + row*m_tileDepth);
				// parse the current character
				MapObject mapObject = GetMapObject(rowString[col]);
				GameObject* object;
				switch (mapObject)
				{
				case FLOOR:
					//Floor* f = new Floor(meshManager->GetMesh("TileModel.x"));
					object = new Floor(meshManager->GetMesh("TileModel.x"));
					object->SetPos(position);
					//object->SetOccupied(false);

					// add the floor tile to the map and mark its position as non blocked
					m_gameMap[row][col] = object;
					m_isBlocked[row][col] = false;
					break;
				case WALL:
					object = new Wall(meshManager->GetMesh("TileModel.x"), position);
					
					// add the wall to the map and mark its position as blocked
					m_gameMap[row][col] = object;
					m_isBlocked[row][col] = true;

					// add the wall collider to the world
					if(m_gameHandler)
						m_gameHandler->AddCollider(GAME_STATE, (Wall*)object);

					break;
				case CHEST:
					
					break;
				case STARTPOS:
					break;
				default:
					break;
				}
				
			}
			row++;
		}
		inputFile.close();
		return true;
	}

	return false;
}
void GameMap::Update(float timestep)
{
	
}

void GameMap::Draw(Renderer* renderer)
{
	// loop thorough all floor-tiles and walls and draw them
	for(size_t row = 0; row < m_gameMap.size(); row++)
	{
		for(size_t  col = 0; col < m_gameMap[row].size(); col++)
		{
			 m_gameMap[row][col]->Draw(renderer);

		}
	}

}
/*
	This function check if a certain tile is blocked or not by a wall.
*/
bool GameMap::IsBlocked(int row, int col)
{
	// check so index not out of bound
	if(row < 0 || row >= m_isBlocked.size())
		return true;
	// check so index not out of bound
	if(col < 0 || col >= m_isBlocked[row].size())
		return true;

	return m_isBlocked[row][col];
}
/*
	This function returns a random position on the map that is not aleady blocked by a wall or
	something.
*/
D3DXVECTOR3 GameMap::GetRandomPosition()
{
	srand(time(NULL));
	bool found  = false;
	D3DXVECTOR3 position(-10, 0, -10);
	int iter = 0;
	// teriible loop! but works for now
	while(!found && iter < 1000)	
	{
		int randRow = rand() % m_gameMap.size();
		int randCol = rand() % m_gameMap[randRow].size();

		if(!m_isBlocked[randRow][randCol])
		{
			position = m_gameMap[randRow][randCol]->GetPos();
			found = true;
		}
		iter++;
	}

	return position;
	
}
/*
	This function does the same as above but will set the found free position
	found to blocked.
*/
D3DXVECTOR3 GameMap::GetRandomFreePosition()
{
	srand(time(NULL));
	bool found  = false;
	D3DXVECTOR3 position(-10, 0, -10);
	int iter = 0;
	while(!found && iter < 1000)	// could be infinite loop if map is full of items
	{
		int randRow = rand() % m_gameMap.size();
		int randCol = rand() % m_gameMap[randRow].size();

		if(!m_isBlocked[randRow][randCol])
		{
			position = m_gameMap[randRow][randCol]->GetPos();
			m_isBlocked[randRow][randCol] = true;
			found = true;
		}
		iter++;
	}

	return position;
	
}
/*
	Set a tile free from being blocked. Mainly used by chests or other items that can dissapear

*/
void GameMap::SetTileFree(const D3DXVECTOR3* pos)
{
	std::pair<int, int> tileIndex = GetTileIndex(pos);
	int row = tileIndex.first;
	int col = tileIndex.second;

	if(row >= 0 && col >= 0)
	{
		m_isBlocked[row][col] = false;
	}
		

}

void GameMap::Release(MeshManager* mManager)
{

	if(!m_gameMap.empty())
	{
		for(size_t  row = 0; row < m_gameMap.size(); row++)
		{
			for(size_t  col = 0; col < m_gameMap[row].size(); col++)
			{
				GameObject* object = m_gameMap[row][col];
				if(object != NULL)
				{
					object->Release(mManager);
					delete m_gameMap[row][col];
				}
					
			}
		}
	}
	m_gameMap.clear();
	m_isBlocked.clear();
}
/*
	Retrieves a D3DXVECTOR3 world position of a specific tile in the game map
*/
D3DXVECTOR3 GameMap::GetTilePosition(int row, int col)
{
	
	return m_gameMap[row][col]->GetPos();
	
}
/*
	retrieves the local tile index of a position in the world
*/
std::pair<int,int> GameMap::GetTileIndex(const D3DXVECTOR3* pos)
{
	int rowIndex;	// z-component
	int colIndex;	// x- component

	// get the xpos and zpos
	double xPos = pos->x;		
	double zPos = pos->z;

	//solve the equations xPos = colIndex * tileWidth => colIndex = xPos/tileWidth 
	xPos = xPos/m_tileWidth;
	xPos = Round(xPos);
	xPos += 0.5;
	colIndex = static_cast<int> (xPos);
	//solve the equations zPos = rowIndex * tiledDepth => rowIndex = zPos/tileDepth 
	zPos = zPos/m_tileDepth;
	zPos = Round(zPos);
	zPos += 0.5;
	rowIndex = static_cast<int>(zPos);

	std::pair<int,int> indexes;
	indexes.first = rowIndex;
	indexes.second = colIndex;

	return indexes;

	
}
/*
*/
double GameMap::Round(double n)
{
	return (n < 0.0f)? ceil(n-0.5) : floor(n + 0.5);
}
/*
	This function just parses a character to a predefined ENum type for easeier creation
	of map objects during map creation
*/
MapObject GameMap::GetMapObject(char inputChar)
{
	// parse the inputChar to a MapObject type
	if(inputChar == '#')
		return WALL;
	
	else if(inputChar == '.')
		return FLOOR;
	
	else if(inputChar == 'C')
		return CHEST;

	// default value if unable to parse inputChar
	return FLOOR;
}

GameMap::~GameMap()
{
	
}

