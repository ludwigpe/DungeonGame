#ifndef GAMEMAP_H
#define GAMEMAP_H

#include "GameObject.h"
#include <iostream>
#include <fstream>
class Floor;
#include "Wall.h"
//class GameObject;

	enum MapObject
	{
		FLOOR,
		WALL,
		CHEST,
		STARTPOS,

	};
class GameMap : public GameObject
{
private:

	float m_width;
	float m_depth;
	float m_tileWidth;
	float m_tileDepth;
	float m_tileHeight;
	int m_size;
	Wall* wall;
	double Round(double n);
	std::vector<std::vector<GameObject*>> m_gameMap;	// the map containing the objects, either floor or wall
	std::vector<std::vector<boolean>> m_isBlocked;		// a matrix storing if a tile position is blocked or not by a wall.
	bool CreateMapFromFile(const char* filename, MeshManager* meshManager);

	MapObject GetMapObject(char inputChar);

public:
	GameMap(int size, MeshManager* mManager, Renderer* renderer);
	GameMap(const char* mapFileName, MeshManager* mManager, Renderer* renderer, Game* gHandler);
	void Release(MeshManager* mManager);
	
	~GameMap();
	D3DXVECTOR3 GetRandomPosition();
	D3DXVECTOR3 GetRandomFreePosition();
	D3DXVECTOR3 GetTilePosition(int row, int col);
	void SetTileFree(const D3DXVECTOR3* pos);
	void Update(float timestep);
	void Draw(Renderer* renderer);
	void OnCollision(GameObject* other);
	std::pair<int,int> GetTileIndex(const D3DXVECTOR3* pos);
	bool IsBlocked(int row, int col);

};
#endif // !GAMEMAP_H
