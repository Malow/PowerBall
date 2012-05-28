#include "AnimatedMesh.h"


AnimatedMesh::AnimatedMesh(D3DXVECTOR3 pos) : Mesh(pos)
{
	this->mNrOfTimesLooped = 0;
	this->mLoopNormal = false;
	this->mLoopSeamless = false;
	this->mCurrentTime = 0.0f;
	this->mKeyFrames = new MaloW::Array<KeyFrame*>();
}

AnimatedMesh::~AnimatedMesh()
{
	if(this->mKeyFrames)
	{
		while(this->mKeyFrames->size() > 0)
			delete this->mKeyFrames->getAndRemove(0);

		delete this->mKeyFrames;
	}
}


void AnimatedMesh::GetCurrentKeyFrames(KeyFrame** one, KeyFrame** two, float& t)
{
	if(this->mKeyFrames->size() > 1) //if 2 or more keyframes
	{
		if(this->IsLooping() || !this->mNrOfTimesLooped) //is looping or still haven't reach 0 on the loop count
		{
			if(this->mLoopSeamless) //if looping seamlessly, add first keyframe as last keyframe & return the 2 current keyframes
			{
				int diff = this->mKeyFrames->get(this->mKeyFrames->size() - 1)->time - this->mKeyFrames->get(this->mKeyFrames->size() - 2)->time;
				int newEndTime = this->mKeyFrames->get(this->mKeyFrames->size() - 1)->time + diff;

				this->mNrOfTimesLooped = (unsigned int)((int)this->mCurrentTime / newEndTime);
			
				//compute the indices for the keyframes to interpolate
				int currentPlayTimeMillis = (int)this->mCurrentTime % newEndTime;
				int firstIndex = 0;
				int lastIndex = 1;
				bool foundIndex = false;
				while(!foundIndex)
				{
					if(firstIndex == this->mKeyFrames->size() - 1)
					{
						lastIndex = 0;
						foundIndex = true;
					}
					else if(this->mKeyFrames->get(firstIndex + 1)->time > currentPlayTimeMillis)
					{
						foundIndex = true;
					}
					else
					{
						firstIndex++;
					}
				}

				//get previous and next keyframes
				*one = this->mKeyFrames->get(firstIndex);
				if(!lastIndex)
				{
					*two = this->mKeyFrames->get(lastIndex);
				}
				else
				{
					*two = this->mKeyFrames->get(firstIndex + 1);
				}

				//compute interpolation value t
				int newTimeTwo = (*two)->time - (*one)->time; //can also be seen as the time between keyframe one & two. (new time for keyframe one is 0.)
				//if this time is negative, then we're at the end of the looping, and therefore have to compute the time
				//between the first and second keyframes. (newTimeTwo = time of two = too long, t won't become [0,1])
				if(newTimeTwo < 0)
				{
					newTimeTwo = this->mKeyFrames->get(1)->time;
				}
				int newCurrentTimeMillis = currentPlayTimeMillis - (*one)->time;
				//convert to range [0,1]
				t = ((float)newCurrentTimeMillis / (float)newTimeTwo);
			}
			else //if not (normal looping), return the 2 current keyframes
			{
				int endTime = this->mKeyFrames->get(this->mKeyFrames->size() - 1)->time;
				this->mNrOfTimesLooped = (unsigned int)((int)this->mCurrentTime / endTime);

				//compute the indices for the keyframes to interpolate
				int currentPlayTimeMillis = (int)this->mCurrentTime % endTime;
				int firstIndex = 0; 
				bool foundIndex = false;
				while(!foundIndex)
				{
					if(this->mKeyFrames->get(firstIndex + 1)->time > currentPlayTimeMillis)
					{
						foundIndex = true;
					}
					else
					{
						firstIndex++;
					}
				}

				//get previous and next keyframes
				*one = this->mKeyFrames->get(firstIndex);
				*two = this->mKeyFrames->get(firstIndex + 1);

				//compute interpolation value t
				int newTimeTwo = (*two)->time - (*one)->time; //can also be seen as the time between keyframe one & two. (new time for keyframe one is 0.)
				int newCurrentTimeMillis = currentPlayTimeMillis - (*one)->time;
				//convert to range [0,1]
				t = ((float)newCurrentTimeMillis / (float)newTimeTwo);
			}
		}
		else //if no looping, return last key frame for both
		{
			*one = this->mKeyFrames->get(this->mKeyFrames->size() - 1);
			*two = *one;
			t = 0.0f;
		}
	}
	else if(this->mKeyFrames->size() == 1) //if only 1 keyframe, return it for both
	{
		*one = this->mKeyFrames->get(this->mKeyFrames->size() - 1);
		*two = *one;
		t = 0.0f;
	}
	else //if no keyframes, return null
	{
		*one = *two = NULL;
		t = 0.0f;
	}
}
MaloW::Array<MeshStrip*>* AnimatedMesh::GetStrips()
{
	KeyFrame* one; 
	KeyFrame* two;
	//KeyFrame* interpolated = new KeyFrame();
	float t = 0.0f; 
	this->GetCurrentKeyFrames(&one, &two, t);
	/* interpolation on CPU:
	for(int i = 0; i < one->strips->size(); i++)
	{
		Vertex* oneVerts = one->strips->get(i)->getVerts(); //** samma vertiser **
		Vertex* twoVerts = two->strips->get(i)->getVerts(); //** samma vertiser **
		Vertex* interpolatedVerts = new Vertex[one->strips->get(i)->getNrOfVerts()];
		
		for(int j = 0; j < one->strips->get(i)->getNrOfVerts(); j++)
		{
			D3DXVECTOR3 posMorph;
			D3DXVec3Lerp(&posMorph, &oneVerts[j].pos, &twoVerts[j].pos, t);
			D3DXVECTOR2 texMorph;
			D3DXVec2Lerp(&texMorph, &oneVerts[j].texCoord, &twoVerts[j].texCoord, t);
			D3DXVECTOR3 normalMorph;
			D3DXVec3Lerp(&normalMorph, &oneVerts[j].normal, &twoVerts[j].normal, t);
			D3DXVECTOR3 colorMorph;
			D3DXVec3Lerp(&colorMorph, &oneVerts[j].color, &twoVerts[j].color, t);
			
			interpolatedVerts[j].pos = posMorph;
			interpolatedVerts[j].texCoord = texMorph;
			interpolatedVerts[j].normal = normalMorph;
			interpolatedVerts[j].color = colorMorph;
		}
		
		//MeshStrip strip = new MeshStrip();
		//strip.SetVerts(interpolatedVerts);
		//interpolated->strips->add(&strip);
		interpolated->strips->add(one->strips->get(i));
		interpolated->strips->get(i)->SetVerts(interpolatedVerts);
	}
	
	return interpolated->strips;
	*/

	return two->strips;
}

void AnimatedMesh::SetCurrentTime(float currentTime)
{
	this->mCurrentTime = currentTime;
}


void AnimatedMesh::NoLooping()
{
	this->mLoopNormal = false;
	this->mLoopSeamless = false;
}
void AnimatedMesh::LoopNormal()
{
	this->mLoopNormal = true;
	this->mLoopSeamless = false;
}
void AnimatedMesh::LoopSeamless()
{
	this->mLoopNormal = false;
	this->mLoopSeamless = true;
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
	if(anifile)
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
			this->mKeyFrames->add(frame);
			
		}
	}
	else
		MaloW::Debug("Failed to open AnimatedMesh: " + file);
}