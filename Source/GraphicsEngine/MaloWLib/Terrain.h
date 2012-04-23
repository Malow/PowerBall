#ifndef TERRAIN_H
#define TERRAIN_H

#include "stdafx.h"
#include "Vertex.h"
#include "StaticMesh.h"
#include "Vector.h"


class Terrain : public StaticMesh
{
private:
	int SIZE;
	D3DXVECTOR3 dimensions;

public:
	//static const int SIZE = 256;

	Terrain(D3DXVECTOR3 pos, D3DXVECTOR3 dimension, string texture, string heightmap, int vertexSize);
	virtual ~Terrain();

	bool LoadAndApplyHeightMap(string fileName);

	void filter(int smootheness);
	void calculateNormals();

	float getYPositionAt(float x, float z);
};

#endif