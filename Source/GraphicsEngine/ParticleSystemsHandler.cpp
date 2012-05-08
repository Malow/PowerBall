#include "ParticleSystemHandler.h"


ParticleSystemHandler::ParticleSystemHandler()
{
	this->gDevice = NULL;
	this->gDeviceContext = NULL;
	//this->gCamera = NULL;

	//particle shaders
	this->mNrOfShaders = 0;
	this->mShaderCapacity = 10;
	this->mShaders = new ParticleShader*[this->mShaderCapacity];
	this->mShaderNames = new string*[this->mShaderCapacity];
	for(int i = 0; i < this->mNrOfShaders; i++)
	{
		this->mShaders[i] = NULL;
		this->mShaderNames[i] = NULL;
	}
	//particle systems
	this->mNrOfSystems = 0;
	this->mSystemCapacity = 10;
	this->mSystems = new ParticleSystem*[this->mSystemCapacity];
	this->mSystemNames = new string*[this->mSystemCapacity];
	for(int i = 0; i < this->mSystemCapacity; i++)
	{
		this->mSystems[i] = NULL;
		this->mSystemNames[i] = NULL;
	}
}

ParticleSystemHandler::~ParticleSystemHandler()
{
	//particle shaders
	if(this->mShaders)
	{
		for(int i = 0; i < this->mNrOfShaders; i++)
		{
			delete this->mShaders[i];
			delete this->mShaderNames[i];
		}
		delete [] this->mShaders;
		delete [] this->mShaderNames;
		this->mNrOfShaders = 0;
		this->mShaders = NULL;
		this->mShaderNames = NULL;
	}
	//particle systems
	if(this->mSystems)
	{
		for(int i = 0; i < this->mNrOfSystems; i++)
		{
			delete this->mSystems[i];
			delete this->mSystemNames[i];
		}
		delete [] this->mSystems;
		delete [] this->mSystemNames;
		this->mNrOfSystems = 0;
		this->mSystems = NULL;
		this->mSystemNames = NULL;
	}
}

void ParticleSystemHandler::Init(ID3D11Device* device, ID3D11DeviceContext* deviceContext)
{
	this->gDevice = device;
	this->gDeviceContext = deviceContext;
	//this->gCamera = camera;
}
int ParticleSystemHandler::GetNrOfParticleShaders() const
{
	return this->mNrOfShaders;
}
int ParticleSystemHandler::GetNrOfParticleSystems() const
{
	return this->mNrOfSystems;
}
const string* ParticleSystemHandler::GetParticleShaderName(int index) const
{
	return this->mShaderNames[index];
}
const string* ParticleSystemHandler::GetParticleSystemName(int index) const
{
	return this->mSystemNames[index];
}

bool ParticleSystemHandler::AddParticleShader(string particleShaderName, string fileName, const D3D11_INPUT_ELEMENT_DESC* inputElementDesc, unsigned int nrOfElements)
{
	//check if already added by looping through the shader names and file names for a match
	for(int i = 0; i < this->mNrOfShaders; i++)
	{
		if(!this->mShaderNames[i]->compare(particleShaderName)) //if the same, then 0. !0 = 1 = true 
		{
			MessageBox(0, "Particle shader with that name already exists.\nParticle shader was not created.", "ParticleSystemHandler", MB_ICONWARNING);
			return false;
		}
		else if(!this->mShaders[i]->GetFileName()->compare(fileName)) //if the same, then 0. !0 = 1 = true 
		{
			MessageBox(0, "A particle shader has already been created with that file.\nParticle shader was not created.", "ParticleSystemHandler", MB_ICONWARNING);
			return false;
		}
	}
	//expand arrays if necessary
	if(this->mNrOfShaders >= this->mShaderCapacity) 
	{
		ParticleSystemHandler::Expand(this->mShaders, this->mShaderCapacity);
		ParticleSystemHandler::Expand(this->mShaderNames, this->mShaderCapacity);
		this->mShaderCapacity += this->mShaderCapacity;
	}
	//add particle shader
	this->mShaderNames[this->mNrOfShaders] = new string(particleShaderName);
	this->mShaders[this->mNrOfShaders] = new ParticleShader();
	this->mShaders[this->mNrOfShaders]->Init(this->gDevice, fileName, inputElementDesc, nrOfElements); 
	this->mNrOfShaders++;

	return true;
}
bool ParticleSystemHandler::AddParticleSystem(string particleShaderName, string particleSystemName, int particleCapacity, const vector<string>& fileNames)
{
	//check if name is taken
	for(int i = 0; i < this->mNrOfSystems; i++)
	{
		if(!this->mSystemNames[i]->compare(particleSystemName)) //if the same, then 0. !0 = 1 = true 
		{
			MessageBox(0, "Particle system with that name already exists.\nParticle system was not created.", "ParticleSystemHandler", MB_ICONWARNING);
			return false;
		}
	}
	//loop through shader names for a match
	ParticleShader* match = NULL;
	for(int i = 0; i < this->mNrOfShaders; i++)
	{
		if(!this->mShaderNames[i]->compare(particleShaderName)) //if the same, then 0. !0 = 1 = true 
		{
			match = this->mShaders[i];
			break; 
		}
	}
	if(!match)
	{
		MessageBox(0, "Specified particle shader not found.\nParticle system was not created.", "ParticleSystemHandler", MB_ICONWARNING);
		return false;
	}
	//expand arrays if necessary
	if(this->mNrOfSystems >= this->mSystemCapacity) 
	{
		ParticleSystemHandler::Expand(this->mSystems, this->mSystemCapacity);
		ParticleSystemHandler::Expand(this->mSystemNames, this->mSystemCapacity);
		this->mSystemCapacity += this->mSystemCapacity; 
	}
	//add particle system
	this->mSystemNames[this->mNrOfSystems] = new string(particleSystemName); 
	this->mSystems[this->mNrOfSystems] = new ParticleSystem(particleCapacity);
	this->mSystems[this->mNrOfSystems]->Init(this->gDevice, this->gDeviceContext, match, fileNames);

	this->mNrOfSystems++;
	return true;
}		
bool ParticleSystemHandler::AddParticleSystem(string particleShaderName, string particleSystemName, int particleCapacity, D3DXVECTOR3 emitterPos, D3DXVECTOR3 emitterDir, const vector<string>& fileNames)
{
	//check if name is taken
	for(int i = 0; i < this->mNrOfSystems; i++)
	{
		if(!this->mSystemNames[i]->compare(particleSystemName)) //if the same, then 0. !0 = 1 = true 
		{
			MessageBox(0, "Particle system with that name already exists.\nParticle system was not created.", "ParticleSystemHandler", MB_ICONWARNING);
			return false;
		}
	}
	
	//loop through shader names for a match
	ParticleShader* match = NULL;
	for(int i = 0; i < this->mNrOfShaders; i++)
	{
		if(!this->mShaderNames[i]->compare(particleShaderName)) //if the same, then 0. !0 = 1 = true 
		{
			match = this->mShaders[i];
			break; 
		}
	}
	if(!match)
	{
		MessageBox(0, "Specified particle shader not found.\nParticle system was not created.", "ParticleSystemHandler", MB_ICONWARNING);
		return false;
	}
	//expand arrays if necessary
	if(this->mNrOfSystems >= this->mSystemCapacity) 
	{
		this->mSystems = ParticleSystemHandler::Expand(this->mSystems, this->mSystemCapacity);
		this->mSystemNames = ParticleSystemHandler::Expand(this->mSystemNames, this->mSystemCapacity);
		this->mSystemCapacity += this->mSystemCapacity; 
	}
	//add particle system
	this->mSystemNames[this->mNrOfSystems] = new string(particleSystemName); 
	this->mSystems[this->mNrOfSystems] = new ParticleSystem(particleCapacity, emitterPos, emitterDir);
	this->mSystems[this->mNrOfSystems]->Init(this->gDevice, this->gDeviceContext, match, fileNames);

	this->mNrOfSystems++;
	return true;
}

void ParticleSystemHandler::MoveParticleSystem(string name, float deltaTime)
{
	bool found = false;
	for(int i = 0; i < this->mNrOfSystems; i++)
	{
		if(!this->mSystemNames[i]->compare(name)) //if the same, then 0. !0 = 1 = true 
		{
			found = true;
			this->mSystems[i]->Move(deltaTime);
			break;
		}
	}
	if(!found)
	{
		MessageBox(0, "Specified particle system not found.", "ParticleSystemHandler", MB_ICONWARNING);
	}
}
void ParticleSystemHandler::MoveParticleSystemTo(string name, D3DXVECTOR3 pos)
{
	bool found = false;
	for(int i = 0; i < this->mNrOfSystems; i++)
	{
		if(!this->mSystemNames[i]->compare(name)) //if the same, then 0. !0 = 1 = true 
		{
			found = true;
			this->mSystems[i]->SetEmitterPos(pos);
			break;
		}
	}
	if(!found)
	{
		MessageBox(0, "Specified particle system not found.", "ParticleSystemHandler", MB_ICONWARNING);
	}
}
void ParticleSystemHandler::Update(float deltaTime, float gameTime, D3DXVECTOR3 camPos)
{
	for(int i = 0; i < this->mNrOfSystems; i++)
	{
		this->mSystems[i]->SetEyePos(camPos);
		this->mSystems[i]->Update(deltaTime, gameTime);
	}
}

void ParticleSystemHandler::PreRender(GraphicsEngineParams engParams, Camera* cam)
{
	for(int i = 0; i < this->mNrOfSystems; i++)
	{
		this->mSystems[i]->PreRender(engParams, cam);
	}
}
void ParticleSystemHandler::PostRender()
{
	for(int i = 0; i < this->mNrOfSystems; i++)
	{
		this->mSystems[i]->PostRender();
	}
}