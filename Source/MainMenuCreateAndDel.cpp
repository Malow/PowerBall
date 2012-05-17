#include "MainMenu.h"
/**
* All menus are created here.
**/
void MainMenu::CreateCircleMenu()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;
	
	/* Create the first menu*/
	Element* tempElement = new GUIPicture((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/mainmenu.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f);
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/buttonplay.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(MAINMENU_PLAY), 
		"Media/MainMenu_Circle/clickplay.png", "Media/MainMenu_Circle/mouseoverplay.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/buttoncredits.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(CREDIT), 
		"Media/MainMenu_Circle/clickcredits.png", "Media/MainMenu_Circle/mouseovercredits.png");
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement	 = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/buttonexit.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(EXIT), 
		"Media/MainMenu_Circle/clickexit.png", "Media/MainMenu_Circle/mouseoverexit.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/buttonoptions.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS), 
		"Media/MainMenu_Circle/clickoptions.png", "Media/MainMenu_Circle/mouseoveroptions.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	/*Hotseat Lan Online etc*/

	tempElement = new GUIPicture((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/mainmenu.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f);
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);
	
	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/buttononline.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_ONLINE), 
		"Media/MainMenu_Circle/clickonline.png", "Media/MainMenu_Circle/mouseoveronline.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/buttonlan.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_LAN), 
		"Media/MainMenu_Circle/clicklan.png", "Media/MainMenu_Circle/mouseoverlan.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);
	
	tempElement	 = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/buttonbacktomenucircle.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(MAINMENU), 
		"Media/MainMenu_Circle/clickbacktomenucircle.png", "Media/MainMenu_Circle/mouseoverbacktomenucircle.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, 
		"Media/MainMenu_Circle/buttonhotseat.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_HOTSEAT), 
		"Media/MainMenu_Circle/clickhotseat.png", "Media/MainMenu_Circle/mouseoverhotseat.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);
	
	tempElement = NULL;
}
void MainMenu::CreateOptionsMenu()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;
	
	/* Adding the buttons for the options menu*/
	Element* tempElement = new GUIPicture(offSet, 0, 1, "Media/OptionsMenu/optionsmenu.png", dx, windowHeight);
	this->mSets[OPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonbacktomenu.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), 
		"Media/OptionsMenu/clickbacktomenu.png" , "Media/OptionsMenu/mouseoverbacktomenu.png", dx * (18.0f / 1200)+offSet, windowHeight * (847.0f / 900),
		dx * (325.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttongraphics.png", dx, windowHeight, new ChangeSubSetEvent(OPTIONS_GRAPHICS, NOMENU), 
		"Media/OptionsMenu/clickgraphics.png", "Media/OptionsMenu/mouseovergraphics.png", dx * (42.0f / 1200) + offSet, windowHeight * (198.0f / 900), 
		dx * (197.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonsound.png", dx, windowHeight, new ChangeSubSetEvent(OPTIONS_SOUND, NOMENU), 
		"Media/OptionsMenu/clicksound.png", "Media/OptionsMenu/mouseoversound.png", dx * (42.0f / 1200) + offSet, windowHeight * (248.0f / 900), 
		dx * (130.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS].AddElement(tempElement);
	
	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonbasic.png", dx, windowHeight, new NoEvent(),
		"Media/OptionsMenu/clickbasic.png", "Media/OptionsMenu/mouseoverbasic.png", dx * (370.0f / 1200) + offSet, windowHeight * (26.0f / 900), 
		dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS].AddElement(tempElement);
	
	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonapply.png", dx, windowHeight, new ChangeOptionEvent("Apply", "true"),
		"Media/OptionsMenu/clickapply.png", "Media/OptionsMenu/mouseoverapply.png", dx * (370.0f / 1200) + offSet, windowHeight * (848.0f / 900), dx * (155.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS].AddElement(tempElement);
	

	/*
	* Subset code start here
	*/
	/*Sound*/
	tempElement = new GUIPicture(dx * (375.0f / 1200.0f) + offSet, windowHeight * (100.0f / 900.0f), 1, "Media/Menus/Sound.png",
		dx * (155.0f / 1200.0f), windowHeight * (30.0f / 900.0f));
	this->mSets[OPTIONS_SOUND].AddElement(tempElement);

	tempElement = new CheckBox(dx * (530.0f / 1200.0f) + offSet, windowHeight * (100.0f / 900.0f), 1, "Media/Menus/CheckBoxFrame.png",dx * (30.0f / 1200.0f), windowHeight * (30.0f / 900.0f),
		"Media/Menus/CheckBoxChecked.png", true, new ChangeOptionEvent("Sound", "true"), "Sound");
	this->mSets[OPTIONS_SOUND].AddElement(tempElement);

	/*Graphic*/

	tempElement = new GUIPicture(offSet,0,0, "Media/OptionsMenu/optionsresolution.png", dx, windowHeight);
	this->mSets[OPTIONS_GRAPHICS].AddElement(tempElement);

	float dropX = dx * (385.0f / 1200.0f) + offSet, dropY = windowHeight * (125.0f / 900.0f);
	tempElement = new DropDownList(dropX, dropY,1.0f,"Media/Menus/DropDownMenu.png", 300.0f, 25.0f, "GameMode");
	DropDownList* dropdownlist = (DropDownList*)tempElement;
	
	if(windowWidth == 1920.0f && windowHeight == 1080.0f)
	{
		dropdownlist->AddButton("Media/OptionsMenu/1920x1080.png", new ChangeResEvent(1920, 1080),
			"", "Media/Menus/EmptyMenu.png");
		dropdownlist->AddButton("Media/OptionsMenu/1680x1050.png", new ChangeResEvent(1680, 1050),
			"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
		dropdownlist->AddButton("Media/OptionsMenu/800x600.png", new ChangeResEvent(800, 600),
			"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	}
	else if(windowWidth == 1680.0f && windowHeight == 1050.0f)
	{
		dropdownlist->AddButton("Media/OptionsMenu/1680x1050.png", new ChangeResEvent(1680, 1050),
			"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
		dropdownlist->AddButton("Media/OptionsMenu/1920x1080.png", new ChangeResEvent(1920, 1080),
			"", "Media/Menus/EmptyMenu.png");
		dropdownlist->AddButton("Media/OptionsMenu/800x600.png", new ChangeResEvent(800, 600),
			"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	}
	else if(windowWidth == 800.0f && windowHeight == 600.0f)
	{
		dropdownlist->AddButton("Media/OptionsMenu/800x600.png", new ChangeResEvent(800, 600),
			"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
		dropdownlist->AddButton("Media/OptionsMenu/1680x1050.png", new ChangeResEvent(1680, 1050),
			"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
		dropdownlist->AddButton("Media/OptionsMenu/1920x1080.png", new ChangeResEvent(1920, 1080),
			"", "Media/Menus/EmptyMenu.png");
	}
	else
	{
		dropdownlist->AddButton("Media/OptionsMenu/1920x1080.png", new ChangeResEvent(1920, 1080),
			"", "Media/Menus/EmptyMenu.png");
		dropdownlist->AddButton("Media/OptionsMenu/1680x1050.png", new ChangeResEvent(1680, 1050),
			"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
		dropdownlist->AddButton("Media/OptionsMenu/800x600.png", new ChangeResEvent(800, 600),
			"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	}

	this->mSets[OPTIONS_GRAPHICS].AddElement(tempElement);

	tempElement = new GUIPicture(offSet,0,0, "Media/OptionsMenu/optionsfxaa.png", dx, windowHeight);
	this->mSets[OPTIONS_GRAPHICS].AddElement(tempElement);

	tempElement = new GUIPicture(offSet,0,0, "Media/OptionsMenu/optionshadow.png", dx, windowHeight);
	this->mSets[OPTIONS_GRAPHICS].AddElement(tempElement);

	GraphicsEngineParams gep = GetGraphicsEngine()->GetEngineParameters();
	string startValue = MaloW::convertNrToString(gep.FXAAQuality);

	tempElement = new TextBox(offSet + dx * (525.0f / 1200.0f), windowHeight * (280.0f / 900.0f), 1, "Media/Menus/TextBox30x30.png",
		dx * (31.0f / 1200.0f), windowHeight * (30.0f / 900.0f), startValue, "FXAA", 0.75f, 1, NR, 0, 4);
	this->mSets[OPTIONS_GRAPHICS].AddElement(tempElement);

	tempElement = new GUIPicture(offSet + dx * (560.0f / 1200.0f), windowHeight * (285.0f / 900.0f),0, "Media/OptionsMenu/0-4.png", 
		dx * (53.0f / 1200.0f), windowHeight * (22.0f / 900.0f));
	this->mSets[OPTIONS_GRAPHICS].AddElement(tempElement);

	startValue = MaloW::convertNrToString(gep.ShadowMapSettings);

	tempElement = new TextBox(offSet + dx * (625.0f / 1200.0f), windowHeight * (195.0f / 900.0f), 1, "Media/Menus/TextBox30x30.png",
		dx * (31.0f / 1200.0f), windowHeight * (30.0f / 900.0f), startValue, "SHADOW", 0.75f, 1, NR, 0, 6);
	this->mSets[OPTIONS_GRAPHICS].AddElement(tempElement);

	tempElement = new GUIPicture(offSet + dx * (660.0f / 1200.0f), windowHeight * (200.0f / 900.0f),0, "Media/OptionsMenu/0-6.png", 
		dx * (53.0f / 1200.0f), windowHeight * (22.0f / 900.0f));
	this->mSets[OPTIONS_GRAPHICS].AddElement(tempElement);

	tempElement = NULL;
}
void MainMenu::CreateHotseatMenu()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	Element* tempElement = new GUIPicture(offSet, 0, 1, "Media/HotseatMenu/gamesetupmenu.png", dx, windowHeight);
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new GUIPicture(dx * (50.0f / 1200.0f) + offSet, windowHeight * (50.0f / 900.0f), 1, "Media/Menus/Lifes.png",
		dx * (135.0f / 1200.0f), windowHeight * (25.0f / 900.0f));
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new TextBox(offSet + dx * (185.0f / 1200.0f), windowHeight * (48.0f / 900.0f), 1, "Media/Menus/TextBox50x30.png",
		dx * (50.0f / 1200.0f), windowHeight * (32.0f / 900.0f), "3", "Lifes", 0.80, 2, NR);
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new GUIPicture(dx * (50.0f / 1200.0f) + offSet, windowHeight * (100.0f / 900.0f), 1, "Media/Menus/Rounds.png",
		dx * (190.0f / 1200.0f), windowHeight * (25.0f / 900.0f));
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new TextBox(offSet + dx * (240.0f / 1200.0f), windowHeight * (98.0f / 900.0f), 1, "Media/Menus/TextBox50x30.png",
		dx * (50.0f / 1200.0f), windowHeight * (32.0f / 900.0f), "5", "Rounds", 0.80, 2, NR);
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/HotseatMenu/buttonbacktomenuhotseat.png", dx, windowHeight, new ChangeSetEvent(MAINMENU_PLAY),
		"Media/HotseatMenu/clickbacktomenuhotseat.png" , "Media/HotseatMenu/mouseoverbacktomenuhotseat.png",
		dx * (605.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (330.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/HotseatMenu/buttonstartgame.png", dx, windowHeight, new ChangeSetEvent(PLAY_HOTSEAT),
		"Media/HotseatMenu/clickstartgame.png" , "Media/HotseatMenu/mouseoverstartgame.png", 
		dx * (280.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (300.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

}
void MainMenu::CreateOnlineAndLanMenu()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	/*ONLINE menu*/

	Element* tempElement = new GUIPicture(offSet, 0, 1, "Media/OnlineAndLanMenu/onlinelanmenu.png", dx, windowHeight);
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonbacktomenuonline.png", dx, windowHeight, new ChangeSetEvent(MAINMENU_PLAY),
		"Media/OnlineAndLanMenu/clickbacktomenuonline.png" , "Media/OnlineAndLanMenu/mouseoverbacktomenuonline.png", 
		dx * (605.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (330.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonstartgameonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_ONLINE),
		"Media/OnlineAndLanMenu/clickstartgameonline.png" , "Media/OnlineAndLanMenu/mouseoverstartgameonline.png", 
		dx * (280.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (300.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonsendonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_ONLINE),
		"Media/OnlineAndLanMenu/clicksendonline.png" , "Media/OnlineAndLanMenu/mouseoversendonline.png", 
		dx * (1006.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	/*LAN menu*/

	tempElement = new GUIPicture(offSet, 0, 1, "Media/OnlineAndLanMenu/onlinelanmenu.png", dx, windowHeight);
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonbacktomenuonline.png", dx, windowHeight, new ChangeSetEvent(MAINMENU_PLAY),
		"Media/OnlineAndLanMenu/clickbacktomenuonline.png" , "Media/OnlineAndLanMenu/mouseoverbacktomenuonline.png", 
		dx * (605.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (330.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonstartgameonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_LAN),
		"Media/OnlineAndLanMenu/clickstartgameonline.png" , "Media/OnlineAndLanMenu/mouseoverstartgameonline.png", 
		dx * (280.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (300.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonsendonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_LAN),
		"Media/OnlineAndLanMenu/clicksendonline.png" , "Media/OnlineAndLanMenu/mouseoversendonline.png", 
		dx * (1006.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_LAN].AddElement(tempElement);
	
	float dropX = dx * (50.0f / 1200.0f) + offSet, dropY = windowHeight * (140.0f / 900.0f);
	tempElement = new DropDownList(dropX, dropY,1.0f,"Media/Menus/DropDownMenu.png", 300.0f, 25.0f, "GameMode");
	DropDownList* dropdownlist = (DropDownList*)tempElement;
	
	dropdownlist->AddButton("Media/Menus/CaptureTheFlag_DropDown.png", new ChangeSubSetEvent(OPTIONS_LAN_SUBCTF, CTF),
		"", "Media/Menus/EmptyMenu.png");
	dropdownlist->AddButton("Media/Menus/KingOfTheHill_DropDown.png", new ChangeSubSetEvent(OPTIONS_LAN_SUBKOTH, KOTH),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	dropdownlist->AddButton("Media/Menus/Warlock_DropDown.png", new ChangeSubSetEvent(OPTIONS_LAN_SUBWARLOCK, WARLOCK),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	/*
	* SUB MENUS START HERE
	*
	*/

	/* Sub menu for CTF LAN*/
	tempElement = new GUIPicture(dx * (50.0f / 1200.0f) + offSet, windowHeight * (80.0f / 900.0f), 1, "Media/Menus/ServerName.png",
		dx * (345.0f / 1200.0f), windowHeight * (30.0f / 900.0f));
	this->mSets[OPTIONS_LAN_SUBCTF].AddElement(tempElement);

	tempElement = new TextBox(offSet + dx * (395.0f / 1200.0f), windowHeight * (80.0f / 900.0f), 1, "Media/Menus/TextBox300x30.png",
		dx * (310.0f / 1200.0f), windowHeight * (30.0f / 900.0f), "CTF Server", "ServerName", 0.75f, 8, NORMALCHAR_NR);
	this->mSets[OPTIONS_LAN_SUBCTF].AddElement(tempElement);

	tempElement = new GUIPicture(dx * (705.0f / 1200.0f) + offSet, windowHeight * (80.0f / 900.0f), 1, "Media/Menus/Flags.png",
		dx * (150.0f / 1200.0f), windowHeight * (30.0f / 900.0f));
	this->mSets[OPTIONS_LAN_SUBCTF].AddElement(tempElement);

	tempElement = new TextBox(offSet + dx * (855.0f / 1200.0f), windowHeight * (80.0f / 900.0f), 1, "Media/Menus/TextBox50x30.png",
		dx * (50.0f / 1200.0f), windowHeight * (32.0f / 900.0f), "5", "Flags", 0.80f, 2, NR);
	this->mSets[OPTIONS_LAN_SUBCTF].AddElement(tempElement);
	
	/* Sub menu for KOTH LAN*/
	tempElement = new GUIPicture(dx * (50.0f / 1200.0f) + offSet, windowHeight * (80.0f / 900.0f), 1, "Media/Menus/ServerName.png",
		dx * (345.0f / 1200.0f), windowHeight * (30.0f / 900.0f));
	this->mSets[OPTIONS_LAN_SUBKOTH].AddElement(tempElement);

	tempElement = new TextBox(offSet + dx * (395.0f / 1200.0f), windowHeight * (80.0f / 900.0f), 1, "Media/Menus/TextBox300x30.png",
		dx * (310.0f / 1200.0f), windowHeight * (30.0f / 900.0f), "KOTH Server", "ServerName", 0.75f, 8, NORMALCHAR_NR);
	this->mSets[OPTIONS_LAN_SUBKOTH].AddElement(tempElement);


	/*tempElement = new DropDownList(dropX, dropY,1.0f,"Media/Menus/DropDownMenu.png", 300.0f, 25.0f, "GameMode");
	dropdownlist = (DropDownList*)tempElement;

	dropdownlist->AddButton("Media/Menus/KingOfTheHill_DropDown.png", new ChangeSubSetEvent(OPTIONS_LAN_SUBKOTH, KOTH),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	dropdownlist->AddButton("Media/Menus/CaptureTheFlag_DropDown.png", new ChangeSubSetEvent(OPTIONS_LAN_SUBCTF, CTF),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	dropdownlist->AddButton("Media/Menus/Warlock_DropDown.png", new ChangeSubSetEvent(OPTIONS_LAN_SUBWARLOCK, WARLOCK),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
		
	this->mSets[OPTIONS_LAN_SUBKOTH].AddElement(tempElement);*/
	
	tempElement = new GUIPicture(offSet, 0, 1, "Media/OnlineAndLanMenu/timermenu.png",
		dx, windowHeight);
	this->mSets[OPTIONS_LAN_SUBKOTH].AddElement(tempElement);

	tempElement = new TextBox(dx * (1000.0f / 1200.0f) + offSet, windowHeight * (140.0f / 900.0f), 1, "Media/Menus/TextBox50x30.png",
		dx * (50.0f / 1200.0f), windowHeight * (32.0f / 900.0f), "20", "Continuously", 0.80f, 2, NR);
	this->mSets[OPTIONS_LAN_SUBKOTH].AddElement(tempElement);

	tempElement = new TextBox(dx * (1015.0f / 1200.0f) + offSet, windowHeight * (197.0f / 900.0f), 1, "Media/Menus/TextBox50x30.png",
		dx * (50.0f / 1200.0f), windowHeight * (32.0f / 900.0f), "10", "Accumulated", 0.80f, 2, NR);
	this->mSets[OPTIONS_LAN_SUBKOTH].AddElement(tempElement);

	tempElement = new GUIPicture(dx * (50.0f / 1200.0f) + offSet, windowHeight * (200.0f / 900.0f), 1, "Media/Menus/Rounds.png",
		dx * (190.0f / 1200.0f), windowHeight * (30.0f / 900.0f));
	this->mSets[OPTIONS_LAN_SUBKOTH].AddElement(tempElement);

	tempElement = new TextBox(dx * (240.0f / 1200.0f) + offSet, windowHeight * (199.0f / 900.0f), 1, "Media/Menus/TextBox50x30.png",
		dx * (50.0f / 1200.0f), windowHeight * (32.0f / 900.0f), "5", "Rounds", 0.80f, 2, NR);
	this->mSets[OPTIONS_LAN_SUBKOTH].AddElement(tempElement);

	/* Sub menu for WARLOCK LAN*/
	tempElement = new GUIPicture(dx * (50.0f / 1200.0f) + offSet, windowHeight * (80.0f / 900.0f), 1, "Media/Menus/ServerName.png",
		dx * (345.0f / 1200.0f), windowHeight * (30.0f / 900.0f));
	this->mSets[OPTIONS_LAN_SUBWARLOCK].AddElement(tempElement);

	tempElement = new TextBox(offSet + dx * (395.0f / 1200.0f), windowHeight * (80.0f / 900.0f), 1, "Media/Menus/TextBox300x30.png",
		dx * (310.0f / 1200.0f), windowHeight * (30.0f / 900.0f), "WARLOCK Server", "ServerName", 0.75f, 8, NORMALCHAR_NR);
	this->mSets[OPTIONS_LAN_SUBWARLOCK].AddElement(tempElement);

	/*tempElement = new DropDownList(dropX, dropY,1.0f,"Media/Menus/DropDownMenu.png", 300.0f, 25.0f, "GameMode");
	dropdownlist = (DropDownList*)tempElement;

	dropdownlist->AddButton("Media/Menus/Warlock_DropDown.png", new ChangeSubSetEvent(OPTIONS_LAN_SUBWARLOCK, WARLOCK),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	dropdownlist->AddButton("Media/Menus/KingOfTheHill_DropDown.png", new ChangeSubSetEvent(OPTIONS_LAN_SUBKOTH, KOTH),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");
	dropdownlist->AddButton("Media/Menus/CaptureTheFlag_DropDown.png", new ChangeSubSetEvent(OPTIONS_LAN_SUBCTF, CTF),
		"Media/Menus/EmptyMenu.png", "Media/Menus/EmptyMenu.png");

	this->mSets[OPTIONS_LAN_SUBWARLOCK].AddElement(tempElement);*/
}

void MainMenu::CreateScene()
{
	/*this->mGe->GetCamera()->setPosition(D3DXVECTOR3(0, 15, -15.6));
	this->mGe->GetCamera()->LookAt(D3DXVECTOR3(30, 10, 10));*/

	this->mCamRec = new CamRecording(2000, true);
	this->mCamRec->Init(this->mGe->GetCamera());

	this->mCamRec->CircleAround(true, 50, 1000, 0, D3DXVECTOR3(30, 60, 30), D3DXVECTOR3(0,-5,0));

	if(!this->mCamRec->IsPlaying())
		this->mCamRec->Play();
	this->mCamRec->Update(1);

	this->mBall1 = this->mGe->CreateStaticMesh("Media/Ball.obj", D3DXVECTOR3(0,14.71f,5));
	this->mBall2 = this->mGe->CreateStaticMesh("Media/Ball.obj", D3DXVECTOR3(0,14.71f,-5));

	this->mPlatform = this->mGe->CreateStaticMesh("Media/Cylinder.obj", D3DXVECTOR3(0,10,0));
	this->mKoth = this->mGe->CreateStaticMesh("Media/KOTHMap1.obj", D3DXVECTOR3(40,5,0));
	this->mCtf = this->mGe->CreateStaticMesh("Media/CTFMap1.obj", D3DXVECTOR3(-40,5,0));


	this->mLight1 = this->mGe->CreateLight(D3DXVECTOR3(0, 50.0f, -20));
	this->mLight1->SetIntensity(50.0f);
	this->mLight2 = this->mGe->CreateLight(D3DXVECTOR3(0, 50.0f, 20));
	this->mLight2->SetIntensity(50.0f);

}
void MainMenu::DeleteScene()
{

	this->mGe->DeleteStaticMesh(this->mBall1);
	this->mBall1 = NULL;
	this->mGe->DeleteStaticMesh(this->mBall2);
	this->mBall2 = NULL;
	
	this->mGe->DeleteLight(this->mLight1);
	this->mLight1 = NULL;
	this->mGe->DeleteLight(this->mLight2);
	this->mLight2 = NULL;

	this->mGe->DeleteStaticMesh(this->mPlatform);
	this->mPlatform = NULL;
	this->mGe->DeleteStaticMesh(this->mKoth);
	this->mKoth = NULL;
	this->mGe->DeleteStaticMesh(this->mCtf);
	this->mCtf = NULL;

	SAFE_DELETE(this->mCamRec);
}