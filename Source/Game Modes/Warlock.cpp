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
		
	for(int i = 0; i<7;i++)
	{
		SAFE_DELETE(this->mProgressBars[i]);
	}
	SAFE_DELETE_ARRAY(this->mProgressBars);
	if(this->mPe)
	{
		#if FixedTimeStep
		/*
			for(int i = 0;i<this->mNumberOfPlayers; i++)
				this->mPe->RemoveBody(this->mBalls[i]);
			this->mPe->RemoveMap(this->mPlatform);*/
			SAFE_DELETE(this->mPe);
			this->mPe = NULL;
		#endif
	}
		
	while(this->SpellIcons.size() > 0)
		mGe->DeleteImage(this->SpellIcons.getAndRemove(0));
}

void Warlock::Initialize()
{
	
	D3DXVECTOR3 centerPlatform = D3DXVECTOR3(0,4,0);
	mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 70, -10)); //D3DXVECTOR3(0, 25, -10)
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
	this->mHud[0] = mGe->CreateText("",D3DXVECTOR2(20,400),0.5f,"Media/Fonts/1");
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
	

		
	this->mProgressBars = new ProgressBar*[7];
	float percentX = 0.02f;
	float percentY = 0.88f;
	for(int i = 0; i<5;i++)
	{
		D3DXVECTOR2 temp = D3DXVECTOR2(percentX,percentY);
		this->mProgressBars[i] = new ProgressBar("Media/LoadingScreen/Green.png",
													"Media/LoadingScreen/Red.png",
													"Media/LoadingScreen/BlueOcean.png",
													"Media/LoadingScreen/Black.png",
													temp,0.10f,0.02,0.01f,0.05f);
		this->mProgressBars[i]->SetPercentOfProgressBarColor1(0.0f);
		this->mProgressBars[i]->SetPercentOfProgressBarColor2(0.0f);
		this->mProgressBars[i]->SetPercentOfProgressBarColor3(0.0f);
		percentX = percentX + 0.16f;
			
	}
	this->mProgressBars[5] = new ProgressBar();

	/* test bar 4 malow. there is now no percentage when you set the position of the progressbar. */
	this->mProgressBars[6] = new ProgressBar(30, 500,250, 50);
	this->mProcX = 50;
	this->mProgressBars[6]->SetPercentOfProgressBarColor1(this->mProcX);
	
	
	
	this->mTimeElapsedText = this->mGe->CreateText(	"", D3DXVECTOR2(15.0f, 10.0f), 1.0f, "Media/Fonts/1");



	// Add spell Icons
	// Rework ScreenWidth to 4:3
	const float UISCALE = 0.5f;
	int screenHeight = mGe->GetEngineParameters().windowHeight;
	int screenWidth = (screenHeight * 4) / 3;
	int distX = (mGe->GetEngineParameters().windowWidth - screenWidth) / 2;
	distX /= UISCALE;

	D3DXVECTOR2 imgDim = D3DXVECTOR2(screenWidth / 5, screenWidth / 5) * UISCALE;
	Image* charge = mGe->CreateImage(D3DXVECTOR2(distX, screenHeight - imgDim.y), imgDim, "Media/WarlockUI/ChargeIcon.png");
	Image* sprint = mGe->CreateImage(D3DXVECTOR2(distX + imgDim.x, screenHeight - imgDim.y), imgDim, "Media/WarlockUI/SprintIcon.png");
	Image* harden = mGe->CreateImage(D3DXVECTOR2(distX + imgDim.x * 2, screenHeight - imgDim.y), imgDim, "Media/WarlockUI/HardenIcon.png");
	Image* invis = mGe->CreateImage(D3DXVECTOR2(distX + imgDim.x * 3, screenHeight - imgDim.y), imgDim, "Media/WarlockUI/InvisIcon.png");
	Image* jump = mGe->CreateImage(D3DXVECTOR2(distX + imgDim.x * 4, screenHeight - imgDim.y), imgDim, "Media/WarlockUI/JumpIcon.png");

	this->SpellIcons.add(charge);
	this->SpellIcons.add(sprint);
	this->SpellIcons.add(harden);
	this->SpellIcons.add(invis);
	this->SpellIcons.add(jump);
}

void Warlock::Intro()
{
	float x = this->mGe->GetEngineParameters().windowWidth * 0.5f - this->mGe->GetEngineParameters().windowWidth * 0.2125f;
	float y = this->mGe->GetEngineParameters().windowHeight * 0.4f;
	Text* intro = mGe->CreateText("Warlock", D3DXVECTOR2(x, y), 2.0f, "Media/Fonts/1");
	mGe->LoadingScreen("Media/LoadingScreen/LoadingScreenBG.png", "Media/LoadingScreen/LoadingScreenPB.png", 1.0f, 1.0f, 1.0f, 1.0f);	// Changed by MaloW
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
	if(mGe->GetKeyListener()->IsPressed('M'))
	{
		this->mProcX += 5.0f*newdt; // add 5 % every sec button is pressed.
		this->mProgressBars[6]->SetPercentOfProgressBarColor1(this->mProcX);
	}
	if(mGe->GetKeyListener()->IsPressed('N'))
	{
		this->mProcX -= 5.0f*newdt; // remove 5 % every sec button is pressed.
		this->mProgressBars[6]->SetPercentOfProgressBarColor1(this->mProcX);
	}
	
	this->mTimeElapsed += newdt;
	Vector3 pos;
	Vector3 posLav;
	float distance = -1;
	int numberBallsAlive = 0;
	for(int i = 0; i<this->mNumberOfPlayers; i++)
	{
		pos = this->mBalls[i]->GetPositionVector3();
		posLav = Vector3(pos.x,this->mGe->GetLavaHeightAt(pos.x, pos.z), pos.z);
		Vector3 distanceVector = (pos - posLav);
		distance = distanceVector.GetLength();
		if(distance < this->mBalls[i]->GetRadius() ||  distanceVector.y < 0.0f )
		{
			/* here the ball take damage. */
			float radiusMapInside = 55.0f;
			float moreDamageDistance = pos.GetLength() - radiusMapInside;
			/* minus 4 hp per sec and minus distance in lava per sec */
			this->mBalls[i]->SetHealth(this->mBalls[i]->GetHealth() - 4.0f*newdt - moreDamageDistance*newdt);
		}
	}
	/*
	if(this->mTimeElapsed > 600.0f)
		return true;
	return false;*/
	int ballIndex = 0;
	int* arrayIndexs = new int[this->mNumberOfPlayers];
	bool returnValue = false;
	
	/* checking which balls that are alive. */
	for(int i = 0; i<this->mNumberOfPlayers; i++)
	{
		if(this->mBalls[i]->GetHealth() > 0)
		{
			/* saving the index of the balls that are alive and how many they are. */
			arrayIndexs[numberBallsAlive] = i;
			numberBallsAlive++;
		}
	}

	/* checking so they belong to the same team. */
	int numberRed = 0;
	int numberBlue = 0;
	int numberNone = 0;
	
	for(int i = 0; i<numberBallsAlive; i++)
	{
		ballIndex = arrayIndexs[i];
		if(this->mBalls[ballIndex]->GetTeamColor() == TEAM::BLUETEAM)
			numberBlue++;
		if(this->mBalls[ballIndex]->GetTeamColor() == TEAM::REDTEAM)
			numberRed++;
		if(this->mBalls[ballIndex]->GetTeamColor() == TEAM::NOTEAM) // this is for free 4 all
			numberNone++;
	}
	/* if all balls belong to the same team and they are more that zero alive on the map or one alive in free 4 all. */
	if((numberBallsAlive >0) && (numberBallsAlive == numberBlue || numberBallsAlive == numberRed || numberNone == 1 ))  
	{
		/* here the round is over. */
		/* to do. add winning information to teams or to player in free 4 all. */
		returnValue = true;
	}	
	else
	{
		returnValue = false;
	}
	delete arrayIndexs;

	
	/* when someone starts a game there will only be one alive on the map and he will be the winner by algoritm
	** but for free 4 all it has to be at least 2 players for the win condition to be able to be true.
	** and at least one player for each team in team mode of warlock for the win condition to be able to be true.
	** 
	*/

	/* checking which ball belong to which team and how many they are and how many that are no team aka free 4 all. */
	numberRed = 0;
	numberBlue = 0;
	numberNone = 0;
	
	for(int i = 0; i<this->mNumberOfPlayers; i++)
	{
		if(this->mBalls[i]->GetTeamColor() == TEAM::BLUETEAM)
			numberBlue++;
		if(this->mBalls[i]->GetTeamColor() == TEAM::REDTEAM)
			numberRed++;
		if(this->mBalls[i]->GetTeamColor() == TEAM::NOTEAM) // this is for free 4 all
			numberNone++;
	}
	
	if(returnValue && ( (numberRed > 0 && numberBlue > 0) || numberNone > 1))
	{
		for(int i = 0; i<this->mNumberOfPlayers; i++)
		{
			this->mBalls[i]->ResetTime();
			this->mBalls[i]->SetToStartPosition();
			this->mBalls[i]->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
			this->mNet->GetBall(i)->SetPos(this->mBalls[i]->GetPosition());
			Vector3 vel = Vector3(0,0,0);
			this->mNet->GetBall(i)->SetVel(::D3DXVECTOR3(vel.x, vel.y, vel.z));
			this->mNet->GetBall(i)->SetForwardVector(this->mNet->GetBall(i)->GetStartForwardVector());
		}
		returnValue = true;
	}
	else
		returnValue = false;
	return returnValue;
	
}

void Warlock::ShowHud()
{
	GameMode::ShowHud();
	string s;
	Vector3 n = this->mBalls[this->mNet->GetIndex()]->GetPositionVector3();
	s = " Position: x: " + MaloW::convertNrToString(floor(10*n.x)/10.0f) + " y: " + MaloW::convertNrToString(floor(10*n.y)/10.0) + " z: " + MaloW::convertNrToString(floor(10*n.z)/10.0);
	s = s + "Rounds: " + MaloW::convertNrToString(this->mNumberOfRounds);
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

	/* spell information showing on display */
	string textInfo;
	string counterInfo;

	/*  Charge Spell */
	if(spells[0]->InUse())
	{
		textInfo = "Using";
		counterInfo = "";
		percentage = ((spells[0]->GetMaxTimeUse() - spells[0]->GetTimerCounterInUse())/spells[0]->GetMaxTimeUse())*100.0f;
		//this->mProgressBars[0]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[0]->SetPercentOfProgressBarColor1(percentage);
	}
	else if(spells[0]->IsCharging())
	{
		textInfo = "Charging";
		counterInfo = "";
		percentage = (((ChargeSpell*)spells[0])->GetTimerCharging()/((ChargeSpell*)spells[0])->GetMaxTimeCharging())*100.0f;
		this->mProgressBars[0]->SetPercentOfProgressBarColor3(percentage);
	}
	else if(spells[0]->NeedCoolDown())
	{
		textInfo = "ColdDown";
		counterInfo = MaloW::convertNrToString(floor(10.0f*spells[0]->GetCoolDownTimeLeft())/10.0f);
		percentage = (spells[0]->GetTimerCounterCoolDown()/spells[0]->GetTimeNeededToCoolDown())*100.0f;
		//this->mProgressBars[0]->SetPercentOfProgressBarColor1(0);
		this->mProgressBars[0]->SetPercentOfProgressBarColor2(percentage);
	}
	else
	{
		textInfo = "Ready";
		counterInfo = "";
		percentage = ((spells[0]->GetMaxTimeUse() - spells[0]->GetTimerCounterInUse())/spells[0]->GetMaxTimeUse())*100.0f;
		//this->mProgressBars[0]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[0]->SetPercentOfProgressBarColor1(percentage);
	}
	/* sets the text, and countertime */
	this->mHud[6]->SetText(textInfo);
	this->mHud[11]->SetText(counterInfo);

		
	/*  Charge Spell */
	if(spells[1]->InUse())
	{
		textInfo = "Using";
		counterInfo = "";
		percentage = ((spells[1]->GetMaxTimeUse() - spells[1]->GetTimerCounterInUse())/spells[1]->GetMaxTimeUse())*100.0f;
		//this->mProgressBars[1]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[1]->SetPercentOfProgressBarColor1(percentage);
			
			
	}
	else if(spells[1]->IsCharging())
	{
		textInfo = "Charging";
		counterInfo = "";
	}
	else if(spells[1]->NeedCoolDown())
	{
		textInfo = "ColdDown";
		counterInfo = MaloW::convertNrToString(floor(10.0f*spells[1]->GetCoolDownTimeLeft())/10.0f);
		percentage = (spells[1]->GetTimerCounterCoolDown()/spells[1]->GetTimeNeededToCoolDown())*100.0f;
			
		this->mProgressBars[1]->SetPercentOfProgressBarColor2(percentage);
		//this->mProgressBars[1]->SetPercentOfProgressBarColor1(0);
	}
	else 
	{
		textInfo = "Ready";
		counterInfo = "";
		percentage = ((spells[1]->GetMaxTimeUse() - spells[1]->GetTimerCounterInUse())/spells[1]->GetMaxTimeUse())*100.0f;
			
		//this->mProgressBars[1]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[1]->SetPercentOfProgressBarColor1(percentage);
	}

	/* sets the text, and countertime */
	this->mHud[7]->SetText(textInfo);
	this->mHud[12]->SetText(counterInfo);

		
	/* Harden Spell */
	if(spells[2]->InUse())
	{
		textInfo = "Using";
		counterInfo = "";
		percentage = ((spells[2]->GetMaxTimeUse() - spells[2]->GetTimerCounterInUse())/spells[2]->GetMaxTimeUse())*100.0f;
		//this->mProgressBars[2]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[2]->SetPercentOfProgressBarColor1(percentage);
			

	}
	else if(spells[2]->IsCharging())
	{
		textInfo = "Charging";
		counterInfo = "";
	}
	else if(spells[2]->NeedCoolDown())
	{
		textInfo = "ColdDown";
		counterInfo = MaloW::convertNrToString(floor(10.0f*spells[2]->GetCoolDownTimeLeft())/10.0f);
		percentage = (spells[2]->GetTimerCounterCoolDown()/spells[2]->GetTimeNeededToCoolDown())*100.0f;
			
		this->mProgressBars[2]->SetPercentOfProgressBarColor2(percentage);
		//this->mProgressBars[2]->SetPercentOfProgressBarColor1(0);
	}
	else 
	{
		textInfo = "Ready";
		counterInfo = "";
		percentage = ((spells[2]->GetMaxTimeUse() - spells[2]->GetTimerCounterInUse())/spells[2]->GetMaxTimeUse())*100.0f;
			
		//this->mProgressBars[2]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[2]->SetPercentOfProgressBarColor1(percentage);
	}

	/* sets the text, and countertime */
	this->mHud[8]->SetText(textInfo);
	this->mHud[13]->SetText(counterInfo);
		

	/* Invisibility Spell */
	if(spells[3]->InUse())
	{
		textInfo = "Using";
		counterInfo = "";
		percentage = ((spells[3]->GetMaxTimeUse() - spells[3]->GetTimerCounterInUse())/spells[3]->GetMaxTimeUse())*100.0f;
		//this->mProgressBars[3]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[3]->SetPercentOfProgressBarColor1(percentage);
			
	}
	else if(spells[3]->IsCharging())
	{
		textInfo = "Charging";
		counterInfo = "";
	}
	else if(spells[3]->NeedCoolDown())
	{
		textInfo = "ColdDown";
		counterInfo = MaloW::convertNrToString(floor(10.0f*spells[3]->GetCoolDownTimeLeft())/10.0f);
		percentage = (spells[3]->GetTimerCounterCoolDown()/spells[3]->GetTimeNeededToCoolDown())*100.0f;
		//this->mProgressBars[3]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[3]->SetPercentOfProgressBarColor2(percentage);
	}
	else 
	{
		textInfo = "Ready";
		counterInfo = "";
		percentage = ((spells[3]->GetMaxTimeUse() - spells[3]->GetTimerCounterInUse())/spells[3]->GetMaxTimeUse())*100.0f;
		//this->mProgressBars[3]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[3]->SetPercentOfProgressBarColor1(percentage);
			
	}

	/* sets the text, and countertime */
	this->mHud[9]->SetText(textInfo);
	this->mHud[14]->SetText(counterInfo);


	/* Jump Spell */
	if(spells[4]->InUse())
	{
		textInfo = "Using";
		counterInfo = "";
		percentage = ((spells[4]->GetMaxTimeUse() - spells[4]->GetTimerCounterInUse())/spells[4]->GetMaxTimeUse())*100.0f;
		//this->mProgressBars[4]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[4]->SetPercentOfProgressBarColor1(percentage);
			
	}
	else if(spells[4]->IsCharging())
	{
		textInfo = "Charging";
		counterInfo = "";
	}
	else if(spells[4]->NeedCoolDown())
	{
		textInfo = "ColdDown";
		counterInfo = MaloW::convertNrToString(floor(10.0f*spells[4]->GetCoolDownTimeLeft())/10.0f);
		percentage = (spells[4]->GetTimerCounterCoolDown()/spells[4]->GetTimeNeededToCoolDown())*100.0f;
		//this->mProgressBars[4]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[4]->SetPercentOfProgressBarColor2(percentage);
	}
	else 
	{
		textInfo = "Ready";
		counterInfo = "";
		percentage = ((spells[4]->GetMaxTimeUse() - spells[4]->GetTimerCounterInUse())/spells[4]->GetMaxTimeUse())*100.0f;
			
		//this->mProgressBars[4]->SetPercentOfProgressBarColor2(0);
		this->mProgressBars[4]->SetPercentOfProgressBarColor1(percentage);
	}

	/* sets the text, and countertime */
	this->mHud[10]->SetText(textInfo);
	this->mHud[15]->SetText(counterInfo);
		
		
	/* Life bar */
	s = "Life: " + MaloW::convertNrToString(floor(10.0f*this->mBalls[this->mNet->GetIndex()]->GetHealth())/10.0f);
	this->mProgressBars[5]->SetPercentOfProgressBarColor1(this->mBalls[this->mNet->GetIndex()]->GetHealth());
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
			temp[i]->AddSpell(new SprintSpell(this->mGe->GetSoundEngine()->LoadSoundEffect("Media/Sounds/SoundEffects/Spell_Sprint2.mp3")));
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

void Warlock::HideHud()
{
	GameMode::HideHud();
	for(int i = 0;i<17;i++)
		this->mHud[i]->SetText("");
	for(int i = 0;i<6;i++)
		this->mProgressBars[i]->HideBar();
	
}