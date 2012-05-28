#include "Warlock.h"
#include "..\Physics\PhysicsEngine.h"
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
		this->mProgressBars = NULL;
		

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
		this->mProgressBars = NULL;
}
Warlock::~Warlock()
{
		for(int i = 0; i < 5; i++)
		{
			this->mGe->DeleteLight(this->mLights[i]);
		}
		SAFE_DELETE(this->mIGM);

		for(int i = 0;i<17; i++)
			mGe->DeleteText(this->mHud[i]);
		SAFE_DELETE(this->mChooseTeamMenu);
		
		for(int i = 0; i<6;i++)
		{
			SAFE_DELETE(this->mProgressBars[i]);
		}
		SAFE_DELETE_ARRAY(this->mProgressBars);
		if(this->mPe)
		{
			#if FixedTimeStep
				for(int i = 0;i<this->mNumberOfPlayers; i++)
					this->mPe->RemoveBody(this->mBalls[i]);
				this->mPe->RemoveMap(this->mPlatform);
				SAFE_DELETE(this->mPe);
			#endif
		}
		
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
		WARLOCKInfo* gmi = (WARLOCKInfo*)this->mServerInfo.GetGameModeInfo();
		/* check if team is chooosen from serverinfo. */
		//this->mChooseTeamMenu = new ChooseTeamMenu(this->mGe);
		this->mNumberOfRounds = gmi->GetNumRounds();

		float width = GetGraphicsEngine()->GetEngineParameters().windowWidth;
		float height = GetGraphicsEngine()->GetEngineParameters().windowHeight;

		
		/* Set hud */
		this->mHud[0] = mGe->CreateText("",D3DXVECTOR2(20,20),2.0f,"Media/Fonts/1");
		this->mHud[1] = mGe->CreateText("",D3DXVECTOR2(20,90),1.0f,"Media/Fonts/1");
		this->mHud[2] = mGe->CreateText("",D3DXVECTOR2(20,140),1.0f,"Media/Fonts/1");
		this->mHud[3] = mGe->CreateText("",D3DXVECTOR2(20,190),1.0f,"Media/Fonts/1");
		this->mHud[4] = mGe->CreateText("",D3DXVECTOR2(20,240),1.0f,"Media/Fonts/1");
		this->mHud[5] = mGe->CreateText("",D3DXVECTOR2(20,290),1.0f,"Media/Fonts/1");

		this->mHud[6] = mGe->CreateText("",D3DXVECTOR2(width*0.03f,height*0.90f),1.0f,"Media/Fonts/1");
		this->mHud[7] = mGe->CreateText("",D3DXVECTOR2(width*(0.03f+ 0.16f),height*0.90f),1.0f,"Media/Fonts/1");
		this->mHud[8] = mGe->CreateText("",D3DXVECTOR2(width*(0.03f+ 0.32f),height*0.90f),1.0f,"Media/Fonts/1");
		this->mHud[9] = mGe->CreateText("",D3DXVECTOR2(width*(0.03f+ 0.48f),height*0.90f),1.0f,"Media/Fonts/1");
		this->mHud[10] = mGe->CreateText("",D3DXVECTOR2(width*(0.03f+ 0.64f),height*0.90f),1.0f,"Media/Fonts/1");

		this->mHud[11] = mGe->CreateText("",D3DXVECTOR2(width*0.07f,height*0.82f),1.0f,"Media/Fonts/1");
		this->mHud[12] = mGe->CreateText("",D3DXVECTOR2(width*(0.05f+ 0.16f),height*0.82f),1.0f,"Media/Fonts/1");
		this->mHud[13] = mGe->CreateText("",D3DXVECTOR2(width*(0.05f+ 0.32f),height*0.82f),1.0f,"Media/Fonts/1");
		this->mHud[14] = mGe->CreateText("",D3DXVECTOR2(width*(0.05f+ 0.48f),height*0.82f),1.0f,"Media/Fonts/1");
		this->mHud[15] = mGe->CreateText("",D3DXVECTOR2(width*(0.05f+ 0.64f),height*0.82f),1.0f,"Media/Fonts/1");
		this->mHud[16] = mGe->CreateText("",D3DXVECTOR2(width*0.45f,height*0.15f),1.0f,"Media/Fonts/1");
	

		
		this->mProgressBars = new ProgressBar*[6];
		float percentX = 0.02f;
		float percentY = 0.88f;
		for(int i = 0; i<5;i++)
		{
			D3DXVECTOR2 temp = D3DXVECTOR2(percentX,percentY);
			this->mProgressBars[i] = new ProgressBar("Media/LoadingScreen/Green.png",
													 "Media/LoadingScreen/Red.png",
													 "Media/LoadingScreen/Black.png",
													 temp,0.10f,0.02,0.01f,0.05f);
			this->mProgressBars[i]->SetPercentOfProgressBarColor1(0.0f);
			this->mProgressBars[i]->SetPercentOfProgressBarColor2(0.0f);
			percentX = percentX + 0.16f;
			
		}
		this->mProgressBars[5] = new ProgressBar();
	
		this->mTimeElapsedText = this->mGe->CreateText(	"", D3DXVECTOR2(15.0f, 10.0f), 1.0f, "Media/Fonts/1");
}

void Warlock::Intro()
{
		Text*	intro = mGe->CreateText("Warlock",D3DXVECTOR2(400,500),2.0f,"Media/Fonts/1");
		mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png");	// Changed by MaloW
		intro->SetText("");
		mGe->DeleteText(intro);
}

bool Warlock::PlaySpecific()
{	
	return	GameMode::PlayLan();
}

void Warlock::ShowStats()
{

}

bool Warlock::checkWinConditions(float dt)
{
		float newdt = dt/1000.0f;
		Vector3 pos;
		Vector3 posLav;
		float distance = -1;
		for(int i = 0; i<this->mNumberOfPlayers; i++)
		{
			pos = this->mBalls[i]->GetPositionVector3();
			posLav = Vector3(pos.x,this->mGe->GetLavaHeightAt(pos.x, pos.z), pos.z);
			Vector3 distanceVector = (pos - posLav);
			distance = distanceVector.GetLength();
			if(distance < this->mBalls[i]->GetRadius() ||  distanceVector.y < 0.0f )
				this->mBalls[i]->SetHealth(this->mBalls[i]->GetHealth() - 4.0f*newdt); // minus 4 hp per sec.
		}

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
		float percentage = 0;
		if(spells[0]->InUse())
		{
			s = "Using";
			percentage = ((spells[0]->GetMaxTimeUse() - spells[0]->GetTimerCounterInUse())/spells[0]->GetMaxTimeUse())*100.0f;
			//this->mProgressBars[0]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[0]->SetPercentOfProgressBarColor1(percentage);
			
		}
		else if(spells[0]->IsCharging())
		{
			s = "Charging";
			
		}
		else if(spells[0]->NeedCoolDown())
		{
			s = "ColdDown";
			percentage = (spells[0]->GetTimerCounterCoolDown()/spells[0]->GetTimeNeededToCoolDown())*100.0f;
			//this->mProgressBars[0]->SetPercentOfProgressBarColor1(0);
			this->mProgressBars[0]->SetPercentOfProgressBarColor2(percentage);
			
		}
		else
		{
			s = "Ready";
			percentage = ((spells[0]->GetMaxTimeUse() - spells[0]->GetTimerCounterInUse())/spells[0]->GetMaxTimeUse())*100.0f;
			//this->mProgressBars[0]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[0]->SetPercentOfProgressBarColor1(percentage);
			
		}
		this->mHud[6]->SetText(s);
		if(spells[1]->InUse())
		{
			s = "Using";
			percentage = ((spells[1]->GetMaxTimeUse() - spells[1]->GetTimerCounterInUse())/spells[1]->GetMaxTimeUse())*100.0f;
			//this->mProgressBars[1]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[1]->SetPercentOfProgressBarColor1(percentage);
			
			
		}
		else if(spells[1]->IsCharging())
			s = "Charging";
		else if(spells[1]->NeedCoolDown())
		{
			s = "ColdDown";
			percentage = (spells[1]->GetTimerCounterCoolDown()/spells[1]->GetTimeNeededToCoolDown())*100.0f;
			
			this->mProgressBars[1]->SetPercentOfProgressBarColor2(percentage);
			//this->mProgressBars[1]->SetPercentOfProgressBarColor1(0);
		}
		else 
		{
			s = "Ready";
			percentage = ((spells[1]->GetMaxTimeUse() - spells[1]->GetTimerCounterInUse())/spells[1]->GetMaxTimeUse())*100.0f;
			
			//this->mProgressBars[1]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[1]->SetPercentOfProgressBarColor1(percentage);
		}
		this->mHud[7]->SetText(s);
		if(spells[2]->InUse())
		{
			s = "Using";
			percentage = ((spells[2]->GetMaxTimeUse() - spells[2]->GetTimerCounterInUse())/spells[2]->GetMaxTimeUse())*100.0f;
			//this->mProgressBars[2]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[2]->SetPercentOfProgressBarColor1(percentage);
			

		}
		else if(spells[2]->IsCharging())
			s = "Charging";
		else if(spells[2]->NeedCoolDown())
		{
			s = "ColdDown";
			percentage = (spells[2]->GetTimerCounterCoolDown()/spells[2]->GetTimeNeededToCoolDown())*100.0f;
			
			this->mProgressBars[2]->SetPercentOfProgressBarColor2(percentage);
			//this->mProgressBars[2]->SetPercentOfProgressBarColor1(0);
		}
		else 
		{
			s = "Ready";
			percentage = ((spells[2]->GetMaxTimeUse() - spells[2]->GetTimerCounterInUse())/spells[2]->GetMaxTimeUse())*100.0f;
			
			//this->mProgressBars[2]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[2]->SetPercentOfProgressBarColor1(percentage);
		}
		this->mHud[8]->SetText(s);
		if(spells[3]->InUse())
		{
			s = "Using";
			percentage = ((spells[3]->GetMaxTimeUse() - spells[3]->GetTimerCounterInUse())/spells[3]->GetMaxTimeUse())*100.0f;
			//this->mProgressBars[3]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[3]->SetPercentOfProgressBarColor1(percentage);
			
		}
		else if(spells[3]->IsCharging())
			s = "Charging";
		else if(spells[3]->NeedCoolDown())
		{
			s = "ColdDown";
			percentage = (spells[3]->GetTimerCounterCoolDown()/spells[3]->GetTimeNeededToCoolDown())*100.0f;
			//this->mProgressBars[3]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[3]->SetPercentOfProgressBarColor2(percentage);
		}
		else 
		{
			s = "Ready";
			percentage = ((spells[3]->GetMaxTimeUse() - spells[3]->GetTimerCounterInUse())/spells[3]->GetMaxTimeUse())*100.0f;
			//this->mProgressBars[3]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[3]->SetPercentOfProgressBarColor1(percentage);
			
		}
		this->mHud[9]->SetText(s);
		if(spells[4]->InUse())
		{
			s = "Using";
			percentage = ((spells[4]->GetMaxTimeUse() - spells[4]->GetTimerCounterInUse())/spells[4]->GetMaxTimeUse())*100.0f;
			//this->mProgressBars[4]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[4]->SetPercentOfProgressBarColor1(percentage);
			
		}
		else if(spells[4]->IsCharging())
			s = "Charging";
		else if(spells[4]->NeedCoolDown())
		{
			s = "ColdDown";
			percentage = (spells[4]->GetTimerCounterCoolDown()/spells[2]->GetTimeNeededToCoolDown())*100.0f;
			//this->mProgressBars[4]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[4]->SetPercentOfProgressBarColor2(percentage);
		}
		else 
		{
			s = "Ready";
			percentage = ((spells[4]->GetMaxTimeUse() - spells[4]->GetTimerCounterInUse())/spells[4]->GetMaxTimeUse())*100.0f;
			
			//this->mProgressBars[4]->SetPercentOfProgressBarColor2(0);
			this->mProgressBars[4]->SetPercentOfProgressBarColor1(percentage);
		}
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
		s = "Life: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetHealth())/10.0f);
		
		this->mProgressBars[5]->SetPercentOfProgressBarColor1(this->mBalls[this->mNet->GetIndex()]->GetHealth());
		Vector3 n = this->mBalls[this->mNet->GetIndex()]->GetPositionVector3();
		string t = " Position: x: " + MaloW::convertNrToString(floor(10*n.x)/10.0f) + " y: " + MaloW::convertNrToString(floor(10*n.y)/10.0) + " z: " + MaloW::convertNrToString(floor(10*n.z)/10.0);
		s = s + t;
		this->mHud[16]->SetText(s);
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
			if(i < 4)
				temp[i] = new PowerBall("Media/Ball"+ MaloW::convertNrToString(i+1) + ".obj", this->mNet->GetBall(i)->GetStartPos(), GAMEMODE::WARLOCK);
			else
				temp[i] = new PowerBall("Media/Ball.obj", this->mNet->GetBall(i)->GetStartPos(), GAMEMODE::WARLOCK);
			temp[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
			temp[i]->SetStartForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
			temp[i]->AddSpell(new ChargeSpell(this->mGe->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/Spell_Charge.mp3")));
			temp[i]->AddSpell(new SprintSpell());
			temp[i]->AddSpell(new HardenSpell(this->mGe->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/Spell_Harden2FIXED.wav")));
			temp[i]->AddSpell(new InvisibilitySpell(this->mGe->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/Spell_Invisibility.mp3")));
			temp[i]->AddSpell(new JumpSpell());
			temp[i]->SetWarlockMode(true);
			temp[i]->SetSound(true);
			#if FixedTimeStep
				this->mPe->AddBody(temp[i]);
			#endif
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