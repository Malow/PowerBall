#include "Warlock.h"

Warlock::Warlock()
{
		this->mGe = NULL;
		this->mIGM = NULL;
		this->mBalls = NULL;
		this->mPlatform = NULL;
		this->mNumberOfPlayers = -1;
		this->mNumberOfRounds = -1;
		this->mGameMode = -1;
		this->mTimeElapsed = 0.0f;

}

Warlock::Warlock(GraphicsEngine* ge, GameNetwork* net, ServerInfo server)
{
		this->mGe = ge;
		this->mNumberOfPlayers = 0;
		this->mNumberOfRounds = 3;
		this->mGameMode = server.GetGameMode();
		this->mNet = net;
		this->mServerInfo = server;
		this->mTimeElapsed = 0.0f;
		
	
}

Warlock::~Warlock()
{
		for(int i = 0; i < 5; i++)
		{
			this->mGe->DeleteLight(this->mLights[i]);
		}
		SAFE_DELETE(this->mIGM);

		for(int i = 0;i<16; i++)
			mGe->DeleteText(this->mHud[i]);
		SAFE_DELETE(this->mChooseTeamMenu);

}

void Warlock::Initialize()
{
		D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,4,0);
		mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 25, -10));
		mGe->GetCamera()->LookAt(centerPlatform);	
		this->mLights[0] = mGe->CreateLight(D3DXVECTOR3(0, 50, 0));
		this->mLights[1] = mGe->CreateLight(D3DXVECTOR3(0, 50, -20)); 
		this->mLights[2] = mGe->CreateLight(D3DXVECTOR3(0, 50, 20));
		this->mLights[3] = mGe->CreateLight(D3DXVECTOR3(10, 50, 0));
		this->mLights[4] = mGe->CreateLight(D3DXVECTOR3(-10, 50, 0));
		for(int i = 0; i < 5; i++)
			this->mLights[i]->SetIntensity(30.0f);
		
		D3DXVECTOR3 startPositions[4];
		startPositions[0] = D3DXVECTOR3(0,30.0f,-10);
		startPositions[1] = D3DXVECTOR3(0,30.0f,10);
		startPositions[2] = D3DXVECTOR3(-10,30.0f,0);
		startPositions[3] = D3DXVECTOR3(10,30.0f,0); 
		this->mNet->SetStartPosistions(startPositions, 4);

		D3DXVECTOR3 forwardVectors[4];
		D3DXVECTOR3 center = D3DXVECTOR3(0,0,0);
		for(int i = 0; i<4;i++)
		{
			D3DXVECTOR3 startPosXZ = D3DXVECTOR3(startPositions[i].x, 0, startPositions[i].z);
			D3DXVECTOR3 temp = center - startPosXZ;
			D3DXVECTOR3 tempN;
			D3DXVec3Normalize(&tempN, &temp);
			forwardVectors[i] = tempN;
		}
		this->mNet->SetStartForwardVectors(forwardVectors, 4);
		this->mPlatform		= new Map("Media/WarlockMap.obj", centerPlatform);
		
		this->mBalls		= new PowerBall*[this->mNumberOfPlayers];
		this->mPlatform->SetShrinkValue(0.0f);
		/*
		if(mGe->GetEngineParameters().CamType == TRD)
			((TRDCamera*)mGe->GetCamera())->setPowerBallToFollow(this->mBalls[0]);
			*/
		this->mIGM	= new InGameMenu(this->mGe);
		this->mChooseTeamMenu = new ChooseTeamMenu(this->mGe);
		WARLOCKInfo* gmi = (WARLOCKInfo*)this->mServerInfo.GetGameModeInfo();
		this->mNumberOfRounds = gmi->GetNumRounds();

		/* Set hud */
		this->mHud[0] = mGe->CreateText("",D3DXVECTOR2(20,20),2.0f,"Media/Fonts/1");
		this->mHud[1] = mGe->CreateText("",D3DXVECTOR2(20,90),1.0f,"Media/Fonts/1");
		this->mHud[2] = mGe->CreateText("",D3DXVECTOR2(20,140),1.0f,"Media/Fonts/1");
		this->mHud[3] = mGe->CreateText("",D3DXVECTOR2(20,190),1.0f,"Media/Fonts/1");
		this->mHud[4] = mGe->CreateText("",D3DXVECTOR2(20,240),1.0f,"Media/Fonts/1");
		this->mHud[5] = mGe->CreateText("",D3DXVECTOR2(20,290),1.0f,"Media/Fonts/1");
		this->mHud[6] = mGe->CreateText("",D3DXVECTOR2(20,800),1.0f,"Media/Fonts/1");
		this->mHud[7] = mGe->CreateText("",D3DXVECTOR2(260,800),1.0f,"Media/Fonts/1");
		this->mHud[8] = mGe->CreateText("",D3DXVECTOR2(500,800),1.0f,"Media/Fonts/1");
		this->mHud[9] = mGe->CreateText("",D3DXVECTOR2(740,800),1.0f,"Media/Fonts/1");
		this->mHud[10] = mGe->CreateText("",D3DXVECTOR2(980,800),1.0f,"Media/Fonts/1");

		this->mHud[11] = mGe->CreateText("",D3DXVECTOR2(70,740),1.0f,"Media/Fonts/1");
		this->mHud[12] = mGe->CreateText("",D3DXVECTOR2(310,740),1.0f,"Media/Fonts/1");
		this->mHud[13] = mGe->CreateText("",D3DXVECTOR2(550,740),1.0f,"Media/Fonts/1");
		this->mHud[14] = mGe->CreateText("",D3DXVECTOR2(790,740),1.0f,"Media/Fonts/1");
		this->mHud[15] = mGe->CreateText("",D3DXVECTOR2(1030,740),1.0f,"Media/Fonts/1");
}

void Warlock::Intro()
{
		Text*	intro = mGe->CreateText("Warlock",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);
}

void Warlock::PlaySpecific()
{	
		GameMode::PlayLan();
}

void Warlock::ShowStats()
{

}

bool Warlock::checkWinConditions(float dt)
{
		float newdt = dt/1000.0f;
		/* will be implemented when we have the rules, for now just play around in 600 seconds then gameover */
		this->mTimeElapsed += newdt;
		if(this->mTimeElapsed > 600.0f)
			return true;
		return false;
}

void Warlock::ShowHud()
{
		string s;
		float tmp = (600.0f - this->mTimeElapsed);
		tmp = floor(tmp * 10.0f) / 10.0f;
		s = "Timer: " + MaloW::convertNrToString(tmp);
		this->mHud[0]->SetText(s);
		s = "Speed: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[0]->GetVelocity().GetLength())/10.0f) + " m/s";
		this->mHud[1]->SetText(s);
		float y = floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetVelocity().y)/10.0f;
		if(y > -0.2f && y <0.2f)
			y = 0.0f;
		s = "Vector: ";
		this->mHud[2]->SetText(s);
		s = "x: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetVelocity().x)/10.0f);
		this->mHud[3]->SetText(s);
		s = "y: " + MaloW::convertNrToString(y);
		this->mHud[4]->SetText(s);
		s = "z: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetVelocity().z)/10.0f);
		this->mHud[5]->SetText(s);
		Spell** spells = this->mBalls[this->mNet->GetIndex()]->GetSpells();
		if(spells[0]->InUse())
			s = "Using";
		else if(spells[0]->IsCharging())
			s = "Charging";
		else if(spells[0]->NeedCoolDown())
			s = "ColdDown";
		else
			s = "Ready";
		this->mHud[6]->SetText(s);
					if(spells[1]->InUse())
			s = "Using";
		else if(spells[1]->IsCharging())
			s = "Charging";
		else if(spells[1]->NeedCoolDown())
			s = "ColdDown";
		else 
			s = "Ready";
		this->mHud[7]->SetText(s);
		if(spells[2]->InUse())
			s = "Using";
		else if(spells[2]->IsCharging())
			s = "Charging";
		else if(spells[2]->NeedCoolDown())
			s = "ColdDown";
		else 
			s = "Ready";
		this->mHud[8]->SetText(s);
		if(spells[3]->InUse())
			s = "Using";
		else if(spells[3]->IsCharging())
			s = "Charging";
		else if(spells[3]->NeedCoolDown())
			s = "ColdDown";
		else 
			s = "Ready";
		this->mHud[9]->SetText(s);
		if(spells[4]->InUse())
			s = "Using";
		else if(spells[4]->IsCharging())
			s = "Charging";
		else if(spells[4]->NeedCoolDown())
			s = "ColdDown";
		else 
			s = "Ready";
		this->mHud[10]->SetText(s);
		if(spells[0]->InUse())
			s = "";
		else if(spells[0]->IsCharging())
			s = "";
		else if(spells[0]->NeedCoolDown())
			s = MaloW::convertNrToString(floor(10.0f*spells[0]->GetCoolDownTimeLeft())/10.0f);
		else
			s = "";
		this->mHud[11]->SetText(s);
		if(spells[1]->InUse())
			s = "";
		else if(spells[1]->IsCharging())
			s = "";
		else if(spells[1]->NeedCoolDown())
			s = MaloW::convertNrToString(floor(10.0f*spells[1]->GetCoolDownTimeLeft())/10.0f);
		else
			s = "";
		this->mHud[12]->SetText(s);
		if(spells[2]->InUse())
			s = "";
		else if(spells[2]->IsCharging())
			s = "";
		else if(spells[2]->NeedCoolDown())
			s = MaloW::convertNrToString(floor(10.0f*spells[2]->GetCoolDownTimeLeft())/10.0f);
		else
			s = "";
		this->mHud[13]->SetText(s);
		if(spells[3]->InUse())
			s = "";
		else if(spells[3]->IsCharging())
			s = "";
		else if(spells[3]->NeedCoolDown())
			s = MaloW::convertNrToString(floor(10.0f*spells[3]->GetCoolDownTimeLeft())/10.0f);
		else
			s = "";
		this->mHud[14]->SetText(s);
		if(spells[4]->InUse())
			s = "";
		else if(spells[4]->IsCharging())
			s = "";
		else if(spells[4]->NeedCoolDown())
			s = MaloW::convertNrToString(floor(10.0f*spells[4]->GetCoolDownTimeLeft())/10.0f);
		else 
			s = "";
		this->mHud[15]->SetText(s);
}

void Warlock::AddBall()
{
		int old = this->mNumberOfPlayers;
		this->mNumberOfPlayers = this->mNet->GetNumPlayers();
		PowerBall** temp = new PowerBall*[this->mNumberOfPlayers];
		for(int i = 0; i < old; i++)
		{
			temp[i] = this->mBalls[i];
		}
		for(int i = old; i < this->mNumberOfPlayers; i++)
		{
			temp[i] = new PowerBall("Media/Ball.obj", this->mNet->GetBall(i)->GetStartPos());
			temp[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
			temp[i]->SetStartForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
			temp[i]->AddSpell(new ChargeSpell());
			temp[i]->AddSpell(new SprintSpell());
			temp[i]->AddSpell(new HardenSpell());
			temp[i]->AddSpell(new InvisibilitySpell());
			temp[i]->AddSpell(new JumpSpell());
		}
		delete[] this->mBalls;
		this->mBalls = temp;
	
		if(mGe->GetEngineParameters().CamType == TRD)
				((TRDCamera*)mGe->GetCamera())->setPowerBallToFollow(this->mBalls[this->mNet->GetIndex()]);
		else
		{
			mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 40, this->mNet->GetBall(this->mNet->GetIndex())->GetStartPos().z * 1.5f));
			mGe->GetCamera()->LookAt(D3DXVECTOR3(0,10,0));
		}
}