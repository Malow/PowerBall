/*
	Author:			Markus Tillman
	Created:		16/05/2012
	Description:	Action bar for displaying various actions*rofl*
*/

#pragma once

#include "stdafx.h"
#include "Image.h"

class ActionBar
{
	private:
		D3DXVECTOR2				mPosition; //top left corner
		D3DXVECTOR2				mIconSize;
		MaloW::Array<Image*>*	mIcons;

	public:
		ActionBar();
		ActionBar(D3DXVECTOR2 position, D3DXVECTOR2 iconSize);
		virtual ~ActionBar();

		/*! Returns the xy-position of the top left corner of the action bar. */
		D3DXVECTOR2 GetPosition();
		/*! Returns the xy-position of the center of the action bar. */
		D3DXVECTOR2 GetCenterPosition();
		/*! Returns the width of the action bar. */
		float GetWidth();
		/*! Returns the height of the action bar. */
		float GetHeight();

		/*! Set the xy-position of the top left corner of the action bar. */
		void SetPosition(D3DXVECTOR2 position);
		/*! Set the xy-position of the center of the action bar. */
		void SetCenterPosition(D3DXVECTOR2 position);

		/*! Adds an icon to the action bar. Size of the action bar and icon is adjusted by the mIconSize variable. */
		void AddIcon(Image* icon);


};