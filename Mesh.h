#ifndef MESH_H_
#define MESH_H_

#include <d3dx9.h>

#include "TextureManager.h"

class Mesh
{
private:
	int				m_referenceCount;
	LPD3DXBUFFER	m_materialBuffer;
	LPD3DXMESH		m_mesh;
	DWORD			m_numberOfMaterials;
	D3DMATERIAL9*	m_materials;
	Texture**		m_meshTextures;
	const char*		m_filename;

	void Release(TextureManager* textureManager);

protected:
	Mesh();
	~Mesh() {};

	void AddRef() { m_referenceCount++; }
	void RemoveRef() { m_referenceCount--; }
	int GetRefCount() { return m_referenceCount; }

public:
	bool Load(LPDIRECT3DDEVICE9 device, const char* filename, TextureManager* textureManager);

	const char* GetFileName() { return m_filename; }
	LPD3DXMESH GetD3DMesh() { return m_mesh; }
	D3DMATERIAL9* GetMeshMaterial(int index) { return &m_materials[index]; }
	Texture* GetMeshTexture(int index) { return m_meshTextures[index]; }

	void Render(LPDIRECT3DDEVICE9 d3dDevice);

	friend class MeshManager;
};

#endif