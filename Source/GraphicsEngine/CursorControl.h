#pragma once

/*
Class that controlls the cursor, initally mainly showing and hiding the cursor.
*/

#include "stdafx.h"

class CursorControl
{
private:
	static bool visable;

public:
	CursorControl() {}
	virtual ~CursorControl() { }
	void SetVisibility(bool vis) 
	{ 
		if(vis)
		{
			if(!this->visable)
			{
				this->visable = true;
				ShowCursor(TRUE);
			}
		}
		else
		{
			if(this->visable)
			{
				this->visable = false;
				ShowCursor(FALSE);
			}
		}
	}
};