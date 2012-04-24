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
	Element* tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/mainmenu.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new NoEvent(), " ", " ");
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttonplay.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(MAINMENU_PLAY), "Media/clickplay.png", "Media/mouseoverplay.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttoncredits.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(CREDIT), "Media/clickcredits.png", "Media/mouseovercredits.png");
	this->mSets[MAINMENU].AddElement(tempElement);
	
	tempElement	 = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttonexit.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(EXIT), "Media/clickexit.png", "Media/mouseoverexit.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttonoptions.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_GAMEPLAY), "Media/clickoptions.png", "Media/mouseoveroptions.png");
	this->mSets[MAINMENU].AddElement(tempElement);

	/*Hotseat Lan Online etc*/

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/mainmenu.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new NoEvent(), " ", " ");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);
	
	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttononline.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_ONLINE), "Media/clickonline.png", "Media/mouseoveronline.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttonlan.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_LAN), "Media/clicklan.png", "Media/mouseoverlan.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);
	
	tempElement	 = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttonbacktomenucircle.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(MAINMENU), "Media/clickbacktomenucircle.png", "Media/mouseoverbacktomenucircle.png");
	this->mSets[MAINMENU_PLAY].AddElement(tempElement);

	tempElement = new GUIArchButton((dx * (317.0f / 1440))+(offSet), windowHeight * 0.125f, 1, "Media/buttonhotseat.png", (windowWidth * 0.56f) - offSet, windowHeight * 0.745f, new ChangeSetEvent(OPTIONS_HOTSEAT), "Media/clickhotseat.png", "Media/mouseoverhotseat.png");
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
	Element* tempElement = new GUIArchButton(offSet, 0, 1, "Media/optionsmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonbacktomenu.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), "Media/clickbacktomenu.png" , "Media/mouseoverbacktomenu.png", dx * (18.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (325.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttongraphics.png", dx, windowHeight, new NoEvent(), "Media/clickgraphics.png", "Media/mouseovergraphics.png", dx * (42.0f / 1200) + offSet, windowHeight * (198.0f / 900), dx * (197.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	
	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonbasic.png", dx, windowHeight, new NoEvent(), "Media/clickbasic.png", "Media/mouseoverbasic.png", dx * (370.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	/*
	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonadvanced.png", dx, windowHeight, new NoEvent(), "Media/clickadvanced.png", "Media/mouseoveradvanced.png", dx * (496.0f / 1200) + offSet, windowHeight * (26.0f / 900), dx * (222.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	*/

	//Ugly drop down list to start with
	float dropX = dx * (380.0f / 1200.0f) + offSet, dropY = windowHeight * (80.0f / 900.0f);
	tempElement = new DropDownList(dropX, dropY,1.0f,"Media/DropDownMenu.png", 300.0f, 60.0f);
	DropDownList* dropdownlist = (DropDownList*)tempElement;

	dropdownlist->AddButton((dropX+73.0f), (dropY+27.0f), 1.0f, "Media/Options1920x1080.png", 200.0f, 30.0f, new ChangeResEvent(1920, 1080), "Media/PressedOptions1920x1080.png", "Media/MouseOverOptions1920x1080.png", (dropX+73.0f), (dropY+27.0f), 200.0f, 30.0f);
	dropdownlist->AddButton((dropX+73.0f), (dropY+27.0f)+31.0f, 1.0f, "Media/Options1680x1050.png", 200.0f, 30.0f, new ChangeResEvent(1680, 1050), "Media/PressedOptions1680x1050.png", "Media/MouseOverOptions1680x1050.png", (dropX+73.0f), (dropY+27.0f)+31.0f, 200.0f, 30.0f);

	this->mSets[OPTIONS_GAMEPLAY].AddElement(tempElement);
	tempElement = NULL;
}
void MainMenu::CreateHotseatMenu()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	Element* tempElement = new GUIArchButton(offSet, 0, 1, "Media/gamesetupmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonbacktomenuhotseat.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), "Media/clickbacktomenuhotseat.png" , "Media/mouseoverbacktomenuhotseat.png", dx * (605.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (330.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonstartgame.png", dx, windowHeight, new ChangeSetEvent(PLAY_HOTSEAT), "Media/clickstartgame.png" , "Media/mouseoverstartgame.png", dx * (280.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (300.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_HOTSEAT].AddElement(tempElement);

}
void MainMenu::CreateOnlineAndLanMenu()
{
	float windowWidth = (float)this->mGe->GetEngineParameters().windowWidth;
	float windowHeight = (float)this->mGe->GetEngineParameters().windowHeight;
	float dx = (windowHeight * 4.0f) / 3.0f;
	float offSet = (windowWidth - dx) / 2.0f;

	/*ONLINE menu*/

	Element* tempElement = new GUIArchButton(offSet, 0, 1, "Media/onlinelanmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonbacktomenuonline.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), "Media/clickbacktomenuonline.png" , "Media/mouseoverbacktomenuonline.png", dx * (605.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (330.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonstartgameonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_ONLINE), "Media/clickstartgameonline.png" , "Media/mouseoverstartgameonline.png", dx * (280.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (300.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonsendonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_ONLINE), "Media/clicksendonline.png" , "Media/mouseoversendonline.png", dx * (1006.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_ONLINE].AddElement(tempElement);

	/*LAN menu*/

	tempElement = new GUIArchButton(offSet, 0, 1, "Media/onlinelanmenu.png", dx, windowHeight, new NoEvent(), " ", " ");
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonbacktomenuonline.png", dx, windowHeight, new ChangeSetEvent(MAINMENU), "Media/clickbacktomenuonline.png" , "Media/mouseoverbacktomenuonline.png", dx * (605.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (330.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonstartgameonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_LAN), "Media/clickstartgameonline.png" , "Media/mouseoverstartgameonline.png", dx * (280.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (300.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_LAN].AddElement(tempElement);

	tempElement = new SimpleButton(offSet, 0, 1, "Media/buttonsendonline.png", dx, windowHeight, new ChangeSetEvent(PLAY_LAN), "Media/clicksendonline.png" , "Media/mouseoversendonline.png", dx * (1006.0f / 1200)+offSet, windowHeight * (847.0f / 900), dx * (118.0f / 1200), windowHeight * (30.0f / 900));
	this->mSets[OPTIONS_LAN].AddElement(tempElement);
}