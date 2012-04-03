#ifndef KEYLISTENER_H
#define KEYLISTENER_H

#include <windows.h>
#include "stdafx.h"
#include "Process.h"

/*
This class can either be set a notifier to which it will send keypresses / releases as events.
Or it can be inherited and implemeneted with the virtual functions 
virtual void KeyPressed(char key) { };
virtual void KeyReleased(char key) { };
In that case it needs to be set as the keylistener in the MaloWEngineParams
*/

namespace MaloW
{
	class KeyPressedEvent : public MaloW::ProcessEvent
	{
	private:
		WPARAM key;
		bool pressed;

	public:
		KeyPressedEvent(WPARAM key, bool pressed) { this->key = key; this->pressed = pressed; }
		virtual ~KeyPressedEvent() { }
		WPARAM GetKey() const { return this->key; }
		bool GetPressed() const { return this->pressed; }
	};



	class KeyListener
	{
	private:
		bool keys[256];
		MaloW::Process* notifier;
		HWND hwnd;

	public:
		KeyListener(HWND handle, MaloW::Process* notifier = NULL);
		virtual ~KeyListener() { }

		void KeyDown(WPARAM param);
		void KeyUp(WPARAM param);

		bool IsPressed(char key);
		//bool HasBeenPressedSinceLast(char NotYetImplemented) { }

		/*! Returns the mouse-position relative to the window (excluding borders) */
		D3DXVECTOR2 GetMousePosition() const;

		/*! Sets the mouse-position relative to the window (excluding borders) */
		void SetMousePosition(D3DXVECTOR2 mousePos);
	};
}

#endif