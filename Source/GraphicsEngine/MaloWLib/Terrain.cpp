#include "Terrain.h"

Terrain::Terrain(D3DXVECTOR3 pos, D3DXVECTOR3 dimension, string texture, string heightmap, int vertexSize) : StaticMesh(pos)
{
	//this->scale = dimension;

	// cant make a quad smaller than 2x2
	if(vertexSize < 2)
		vertexSize = 2;

	this->SIZE = vertexSize;
	this->dimensions = dimension;

	MeshStrip* strip = new MeshStrip();
	Vertex* verts = new Vertex[this->SIZE * this->SIZE];
	strip->SetVerts(verts);

	int tilingFactor = 1;
	
	for(int i = 0; i < this->SIZE; i++)
	{
		for(int u = 0; u < this->SIZE; u++)
		{
			verts[i*this->SIZE + u] = Vertex(
				D3DXVECTOR3(((float)i/(this->SIZE-1))*this->dimensions.x, this->dimensions.y, ((float)u/(this->SIZE-1))*this->dimensions.z), 
				D3DXVECTOR2((float)i/((this->SIZE-1)/tilingFactor), (float)u/((this->SIZE-1)/tilingFactor)), 
				D3DXVECTOR3(0, 1, 0),
				D3DXVECTOR3(0, 0, 0));
		}
	}
	strip->setNrOfVerts(this->SIZE*this->SIZE);

	int nrOfIndicies = (this->SIZE-1) * 2 * (this->SIZE-1) * 3;
	strip->setNrOfIndicies(nrOfIndicies);
	int* inds = new int[nrOfIndicies];
	strip->SetIndicies(inds);

	int offset = 0; 
	for(int i = 0; i < this->SIZE-1; i++)
	{
		for(int u = 0; u < this->SIZE-1; u++)
		{
			inds[offset] = i * this->SIZE + u;
			offset++;
			inds[offset] = (i+1) * this->SIZE + u + 1; 
			offset++;
			inds[offset] = (i+1) * this->SIZE + u; 
			offset++;

			inds[offset] = i * this->SIZE + u;
			offset++;
			inds[offset] = i * this->SIZE + u + 1; 
			offset++;
			inds[offset] = (i+1) * this->SIZE + u + 1; 
			offset++;
		}
	}
	
	strip->SetTexturePath(texture);
	this->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	strip->SetMaterial(new Material(LAMBERT));
	strip->GetMaterial()->SpecularColor = D3DXVECTOR3(0.05f, 0.05f, 0.05f);

	this->strips->add(strip);

	this->LoadAndApplyHeightMap(heightmap);
	this->filter(1);
	this->calculateNormals();
}

Terrain::~Terrain()
{
}

bool Terrain::LoadAndApplyHeightMap(string fileName)
{
	if(fileName != "")
	{
		ifstream fin;
		fin.open(fileName.c_str(), ios_base::binary);
		if(!fin)
		{
			MaloW::Debug("Heightmap for terrain could not be found");
			return false;
		}

		vector<unsigned char> vertexHeights(this->SIZE*this->SIZE);

		fin.read((char *)&vertexHeights[0], (streamsize)vertexHeights.size());
		fin.close();

		Vertex* verts = this->strips->get(0)->getVerts();
		for(int i = 0; i < (int)vertexHeights.size(); i++)
		{
			verts[i].pos.y *= (float)vertexHeights[i]/10;
		}
	}
	return true;
}

float Terrain::getYPositionAt(float x, float z)
{
	x -= this->pos.x;
	z -= this->pos.z;

	float ex = x;
	ex /= this->dimensions.x;
	float ez = z;
	ez /= this->dimensions.z;

	
	if(ex <= 1.0f && ez <= 1.0f && ex > 0.0f && ez > 0.0f)
	{
		ex *= this->SIZE;
		ez *= this->SIZE;

		int i = (int)ex;
		int u = (int)ez;

		int a = i * this->SIZE + u;
		int b = (i+1) * this->SIZE + u;
		int c = i * this->SIZE + u+1;
		int d = (i+1) * this->SIZE + u+1;

		Vertex* verts = this->strips->get(0)->getVerts();

		float posya = verts[i * this->SIZE + u].pos.y;
		float posyb = posya;
		float posyc = posya;
		float posyd = posya;
		
		if(a < this->SIZE * this->SIZE)
			posya = verts[a].pos.y;

		if(b < this->SIZE * this->SIZE)
			posyb = verts[b].pos.y;

		if(c < this->SIZE * this->SIZE)
			posyc = verts[c].pos.y;

		if(d < this->SIZE * this->SIZE)
			posyd = verts[d].pos.y;
		
		float amem = ((1.0f - ((float)ex - i)) * (1.0f - ((float)ez - u)));
		float bmem = (((float)ex - i) * (1.0f - ((float)ez - u)));
		float cmem = ((1.0f - ((float)ex - i)) * ((float)ez - u));
		float dmem = (((float)ex - i) * ((float)ez - u));
		
		return posya*amem + posyb*bmem + posyc*cmem + posyd*dmem;
	}
	else
		return 10.0f;
}

void Terrain::filter(int smootheness)
{
	for(int smoothe = 0; smoothe < smootheness; smoothe++)
	{
		Vertex** grid = new Vertex*[this->SIZE];
		for(int i = 0; i < this->SIZE; i++)
			grid[i] = new Vertex[this->SIZE];

		Vertex* verts = this->strips->get(0)->getVerts();

		for(int i = 0; i < this->SIZE; i++)
			for(int u = 0; u < this->SIZE; u++)
				grid[i][u] = verts[i * this->SIZE + u];


		for(int q = 0; q < this->SIZE; q++)
		{
			for(int u = 0; u < this->SIZE; u++)
			{
				int a = (q-1)*this->SIZE + u-1;
				int b = (q-1)*this->SIZE + u;
				int c = (q-1)*this->SIZE + u+1;
				int d = q*this->SIZE + u-1;
				int e = q*this->SIZE + u;		// Urself
				int f = q*this->SIZE + u+1;
				int g = (q+1)*this->SIZE + u-1;
				int h = (q+1)*this->SIZE + u;
				int i = (q+1)*this->SIZE + u+1;
				if(q == 0 && u == 0)
				{
					grid[q][u].pos.y = (verts[e].pos.y + verts[f].pos.y + verts[h].pos.y + verts[i].pos.y) / 4;
				}
				else if(q == 0 && u == this->SIZE-1)
				{
					grid[q][u].pos.y = (verts[d].pos.y + verts[e].pos.y + verts[g].pos.y + verts[h].pos.y) / 4;
				}
				else if(q == this->SIZE-1 && u == 0)
				{
					grid[q][u].pos.y = (verts[b].pos.y + verts[c].pos.y + verts[e].pos.y + verts[f].pos.y) / 4;
				}
				else if(q == this->SIZE-1 && u == this->SIZE-1)
				{
					grid[q][u].pos.y = (verts[a].pos.y + verts[b].pos.y + verts[d].pos.y + verts[e].pos.y) / 4;
				}
				else if(q == 0)
				{
					grid[q][u].pos.y = (verts[d].pos.y + verts[e].pos.y + verts[f].pos.y + verts[g].pos.y + verts[h].pos.y + verts[i].pos.y) / 6;
				}
				else if(q == this->SIZE-1)
				{
					grid[q][u].pos.y = (verts[a].pos.y + verts[b].pos.y + verts[c].pos.y + verts[d].pos.y + verts[e].pos.y + verts[f].pos.y) / 6;
				}
				else if(u == 0)
				{
					grid[q][u].pos.y = (verts[b].pos.y + verts[c].pos.y + verts[e].pos.y + verts[f].pos.y + verts[i].pos.y + verts[h].pos.y) / 6;
				}
				else if(u == this->SIZE-1)
				{
					grid[q][u].pos.y = (verts[a].pos.y + verts[b].pos.y + verts[d].pos.y + verts[e].pos.y + verts[g].pos.y + verts[h].pos.y) / 6;
				}
				else
				{
					grid[q][u].pos.y = (verts[a].pos.y + verts[b].pos.y + verts[c].pos.y + verts[d].pos.y + verts[e].pos.y + verts[f].pos.y
						+ verts[g].pos.y + verts[h].pos.y + verts[i].pos.y ) / 9;
				}
			}
		}

		for(int i = 0; i < this->SIZE; i++)
			for(int u = 0; u < this->SIZE; u++)
				verts[i * this->SIZE + u] = grid[i][u];
	
		for(int i = 0; i < this->SIZE; i++)
			delete [] grid[i];

		delete grid;
	}
}

void Terrain::calculateNormals()
{
	Vertex* verts = this->strips->get(0)->getVerts();

	for(int q = 0; q < this->SIZE; q++)
	{
		for(int u = 0; u < this->SIZE; u++)
		{
			int a = (q-1)*this->SIZE + u;
			int b = q*this->SIZE + u-1;
			int d = q*this->SIZE + u+1;
			int e = (q+1)*this->SIZE + u;

			if(q != 0 && u != 0 && u != this->SIZE-1 && q != this->SIZE-1)
			{
				verts[q * this->SIZE + u].normal = MaloW::Vector3(verts[e].pos - verts[a].pos).GetCrossProduct(MaloW::Vector3(verts[b].pos - verts[d].pos)).GetD3DVec();
			}
		}
	}
}