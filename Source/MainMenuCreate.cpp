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
	Element* tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/mainmenu.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new NoEvent(), " ", " ");
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/buttonplay.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(MAINMENU_PLAY), "Media/MainMenu_Circle/clickplay.png", "Media/MainMenu_Circle/mouseoverplay.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/buttoncredits.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(CREDIT), "Media/MainMenu_Circle/clickcredits.png", "Media/MainMenu_Circle/mouseovercredits.png");
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement	 = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/buttonexit.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(EXIT), "Media/MainMenu_Circle/clickexit.png", "Media/MainMenu_Circle/mouseoverexit.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/buttonoptions.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_GAMEPLAY), "Media/MainMenu_Circle/clickoptions.png", "Media/MainMenu_Circle/mouseoveroptions.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	/*Hotseat Lan Online etc*/

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/mainmenu.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new NoEvent(), " ", " ");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);
	
	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/buttononline.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_ONLINE), "Media/MainMenu_Circle/clickonline.png", "Media/MainMenu_Circle/mouseoveronline.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/buttonlan.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_LAN), "Media/MainMenu_Circle/clicklan.png", "Media/MainMenu_Circle/mouseoverlan.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);
	
	tempElement	 = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/buttonbacktomenucircle.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(MAINMENU), "Media/MainMenu_Circle/clickbacktomenucircle.png", "Media/MainMenu_Circle/mouseoverbacktomenucircle.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/MainMenu_Circle/buttonhotseat.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_HOTSEAT), "Media/MainMenu_Circle/clickhotseat.png", "Media/MainMenu_Circle/mouseoverhotseat.png");
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
	Element* tempElement = new GUIArchButton(offSet, 0, 1, "Media/OptionsMenu/optionsmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonbacktomenu.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), "Media/OptionsMenu/clickbacktomenu.png" , "Media/OptionsMenu/mouseoverbacktomenu.png", dx * (18.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (325.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttongraphics.png", dx, windowHeight, new NoEvent(), "Media/OptionsMenu/clickgraphics.png", "Media/OptionsMenu/mouseovergraphics.png", dx * (42.0f / 1200) + offSet, windowHeight * (198.0f / 900), dx * (197.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	
	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonbasic.png", dx, windowHeight, new NoEvent(), "Media/OptionsMenu/clickbasic.png", "Media/OptionsMenu/mouseoverbasic.png", dx * (370.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	/*
	tempElement = new SimpleButton(offSet, 0, 1, "Media/OptionsMenu/buttonadvanced.png", dx, windowHeight, new NoEvent(), "Media/OptionsMenu/clickadvanced.png", "Media/OptionsMenu/mouseoveradvanced.png", dx * (496.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (222.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	*/
	tempElement = new TextBox(dx * (380.0f / 1200.0f)+offSet, windowHeight*(200.0f / 900.f) ,50 , "Media/Menus/TextBoxBG.png", 600, 100, "Write Text Here", "SampleTest", 1.0f);
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	//Ugly drop down list to start with
	float dropX = dx * (380.0f / 1200.0f) + offSet, dropY = windowHeight * (80.0f / 900.0f);
	tempElement = new DropDownList(dropX, dropY,1.0f,"Media/Menus/DropDownMenu.png", 300.0f, 60.0f);
	DropDownList* dropdownlist = (DropDownList*)tempElement;

	dropdownlist->AddButton((dropX+73.0f), (dropY+27.0f), 1.0f, "Media/Menus/Options1920_1080.png", 200.0f, 30.0f, new ChangeResEvent(1920, 1080), "Media/Menus/PressedOptions1920_1080.png", "Media/Menus/MouseOverOptions1920_1080.png", (dropX+73.0f), (dropY+27.0f), 200.0f, 30.0f);
	dropdownlist->AddButton((dropX+73.0f), (dropY+27.0f)+31.0f, 1.0f, "Media/Menus/Options1920_1080.png", 200.0f, 30.0f, new ChangeResEvent(1680, 1050), "Media/Menus/PressedOptions1920_1080.png", "Media/Menus/MouseOverOptions1920_1080.png", (dropX+73.0f), (dropY+27.0f)+31.0f, 200.0f, 30.0f);

	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(dx * (380.0f / 1200.0f)+offSet, windowHeight*(350.0f / 900.f), 1, "Media/Menus/Square_OK.png", 100, 100, new ChangeSetEvent(100000), "", "", dx * (380.0f / 1200.0f)+offSet, windowHeight*(350.0f / 900.f), 100, 100);
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = NULL;
}
void MainMenu::CreateHotseatMenu()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	Element* tempElement = new GUIArchButton(offSet, 0, 1, "Media/HotseatMenu/gamesetupmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/HotseatMenu/buttonbacktomenuhotseat.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), "Media/HotseatMenu/clickbacktomenuhotseat.png" , "Media/HotseatMenu/mouseoverbacktomenuhotseat.png", dx * (605.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (330.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/HotseatMenu/buttonstartgame.png", dx, windowHeight, new ChangeSetEvent(PLAY_HOTSEAT), "Media/HotseatMenu/clickstartgame.png" , "Media/HotseatMenu/mouseoverstartgame.png", dx * (280.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (300.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

}
void MainMenu::CreateOnlineAndLanMenu()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	/*ONLINE menu*/

	Element* tempElement = new GUIArchButton(offSet, 0, 1, "Media/OnlineAndLanMenu/onlinelanmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonbacktomenuonline.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), "Media/OnlineAndLanMenu/clickbacktomenuonline.png" , "Media/OnlineAndLanMenu/mouseoverbacktomenuonline.png", dx * (605.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (330.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonstartgameonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_ONLINE), "Media/OnlineAndLanMenu/clickstartgameonline.png" , "Media/OnlineAndLanMenu/mouseoverstartgameonline.png", dx * (280.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (300.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonsendonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_ONLINE), "Media/OnlineAndLanMenu/clicksendonline.png" , "Media/OnlineAndLanMenu/mouseoversendonline.png", dx * (1006.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	/*LAN menu*/

	tempElement = new GUIArchButton(offSet, 0, 1, "Media/OnlineAndLanMenu/onlinelanmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonbacktomenuonline.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), "Media/OnlineAndLanMenu/clickbacktomenuonline.png" , "Media/OnlineAndLanMenu/mouseoverbacktomenuonline.png", dx * (605.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (330.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonstartgameonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_LAN), "Media/OnlineAndLanMenu/clickstartgameonline.png" , "Media/OnlineAndLanMenu/mouseoverstartgameonline.png", dx * (280.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (300.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/OnlineAndLanMenu/buttonsendonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_LAN), "Media/OnlineAndLanMenu/clicksendonline.png" , "Media/OnlineAndLanMenu/mouseoversendonline.png", dx * (1006.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_LAN].AddElement(tempElement);
}