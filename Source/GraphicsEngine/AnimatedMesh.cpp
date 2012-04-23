#include "AnimatedMesh.h"

AnimatedMesh::AnimatedMesh(D3DXVECTOR3 pos) : Mesh(pos)
{
	this->keyframes = new MaloW::Array<KeyFrame*>();
}

AnimatedMesh::~AnimatedMesh()
{
	if(this->keyframes)
	{
		while(this->keyframes->size() > 0)
			delete this->keyframes->getAndRemove(0);
		
		delete this->keyframes;
	}
}

void AnimatedMesh::GetCurrentKeyFrames(KeyFrame* one, KeyFrame* two, float* t, float time)
{
	this->timer = time;

}

void AnimatedMesh::LoadFromFile(string file)
{
	// if substr of the last 4 = .obj do this:    - else load other format / print error
	ObjLoader oj;

	
	// Get the directory correct
	string tempFilename = file;
	string pathfolder = "";
	size_t slashpos = tempFilename.find("/");
	while(slashpos != string::npos)
	{
		slashpos = tempFilename.find("/");
		pathfolder += tempFilename.substr(0, slashpos+1);
		tempFilename = tempFilename.substr(slashpos + 1);
	}

	

	ifstream anifile;
	anifile.open(file);
	{
		string line = "";
		getline(anifile, line);
		int nrOfKeyframes = atoi(line.c_str());

		for(int a = 0; a < nrOfKeyframes; a++)
		{
			int time = 0;
			string path = "";
			getline(anifile, line);
			time = atoi(line.c_str());
			getline(anifile, path);

			KeyFrame* frame = new KeyFrame();
			frame->time = time;

			{
				ObjData* od = oj.LoadObjFile(pathfolder + path);
				MaloW::Array<MaterialData>* mats = od->mats;
				for(int q = 0; q < mats->size(); q++)
				{
					bool hasFace = false;
					MeshStrip* strip = new MeshStrip();
		

					int nrOfVerts = 0;
		
					Vertex* tempverts = new Vertex[od->faces->size()*3];
		
					for(int i = 0;  i < od->faces->size(); i++)
					{
						if(od->faces->get(i).material == mats->get(q).name)
						{
							int vertpos = od->faces->get(i).data[0][0] - 1;
							int textcoord = od->faces->get(i).data[0][1] - 1;
							int norm = od->faces->get(i).data[0][2] - 1;
							tempverts[nrOfVerts] = Vertex(od->vertspos->get(vertpos), od->textcoords->get(textcoord), od->vertsnorms->get(norm));
							nrOfVerts++;

							vertpos = od->faces->get(i).data[2][0] - 1;
							textcoord = od->faces->get(i).data[2][1] - 1;
							norm = od->faces->get(i).data[2][2] - 1;
							tempverts[nrOfVerts] = Vertex(od->vertspos->get(vertpos), od->textcoords->get(textcoord), od->vertsnorms->get(norm));
							nrOfVerts++;

							vertpos = od->faces->get(i).data[1][0] - 1;
							textcoord = od->faces->get(i).data[1][1] - 1;
							norm = od->faces->get(i).data[1][2] - 1;
							tempverts[nrOfVerts] = Vertex(od->vertspos->get(vertpos), od->textcoords->get(textcoord), od->vertsnorms->get(norm));
							nrOfVerts++;



							hasFace = true;
						}
					}

					strip->setNrOfVerts(nrOfVerts);
					Vertex* verts = new Vertex[nrOfVerts];
					for(int z = 0; z < nrOfVerts; z++)
					{
						verts[z] = tempverts[z];
					}
					delete tempverts;
					strip->SetVerts(verts);
			
					strip->SetTexturePath(od->mats->get(q).texture);

					Material* mat = new Material();
					mat->AmbientColor = od->mats->get(q).ka;
					if(mat->AmbientColor == D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//////////// MaloW Fix, otherwise completely black with most objs
						mat->AmbientColor += D3DXVECTOR3(0.2f, 0.2f, 0.2f);			//////////// MaloW Fix, otherwise completely black with most objs

					mat->DiffuseColor = od->mats->get(q).kd;
					if(mat->DiffuseColor == D3DXVECTOR3(0.0f, 0.0f, 0.0f))				//////////// MaloW Fix, otherwise completely black with most objs
						mat->DiffuseColor += D3DXVECTOR3(0.6f, 0.6f, 0.6f);			//////////// MaloW Fix, otherwise completely black with most objs
			
					mat->SpecularColor = od->mats->get(q).ks;
					strip->SetMaterial(mat);

					if(hasFace)
						frame->strips->add(strip);
					else
						delete strip;
				}
				this->topology = D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
				delete od;

			}
			this->keyframes->add(frame);
		}
	}
}

MaloW::Array<MeshStrip*>* AnimatedMesh::GetStrips()
{
	/*
	if(this->strips)
	{
		while(this->strips->size() > 0)
		{
			delete this->strips->GetAndRemoev(0);
		}
		delete this->strips;
	}

	this->strips = InterpolatedStrips(this->timer);


	*/

	return this->strips;
}