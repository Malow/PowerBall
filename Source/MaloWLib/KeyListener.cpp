#include "KeyListener.h"

using namespace MaloW;

KeyListener::KeyListener(HWND handle, MaloW::Process* notifier)
{
	this->hwnd = handle;
	this->notifier = notifier;
	for(int i = 0; i < 256; i++)
		this->keys[i] = false;
	for(int i = 0; i < 50; i++)
		this->mouse[i] = false;
}

void KeyListener::KeyDown(WPARAM param)
{
	if(!this->keys[param])
	{
		this->keys[param] = true;
		if(this->notifier)
		{
			KeyPressedEvent* kpe = new KeyPressedEvent(param, true);
			this->notifier->PutEvent(kpe);
		}
	}
}

void KeyListener::KeyUp(WPARAM param)
{
	this->keys[param] = false;
	if(this->notifier)
	{
		KeyPressedEvent* kpe = new KeyPressedEvent(param, false);
		this->notifier->PutEvent(kpe);
	}
}

bool KeyListener::IsPressed(char key)
{
	return this->keys[key];
}

/*
bool KeyListener::HasBeenPressedSinceLast(char key)
{
	bool retval = this->keys[key];

	this->keys[key] = false;

	return retval;
}*/

D3DXVECTOR2 KeyListener::GetMousePosition() const
{
	D3DXVECTOR2 mp;
	POINT p;
	if(GetCursorPos(&p))
	{
		if(ScreenToClient(this->hwnd, &p))
		{
			mp.x = (float)p.x;
			mp.y = (float)p.y;
		}
	}
	return mp;
}

void KeyListener::SetMousePosition(D3DXVECTOR2 mousePos)
{
	POINT np;
	np.x = (long)mousePos.x;
	np.y = (long)mousePos.y;
	if(ClientToScreen(this->hwnd, &np))
	{
		SetCursorPos(np.x, np.y);
	}
}

void KeyListener::MouseDown(int button)
{
	if(!this->mouse[button])
	{
		this->mouse[button] = true;
		if(this->notifier)
		{
			MouseClickEvent* mce = new MouseClickEvent(button, true);
			this->notifier->PutEvent(mce);
		}
	}
}

void KeyListener::MouseUp(int button)
{
	this->mouse[button] = false;
	if(this->notifier)
	{
		MouseClickEvent* mce = new MouseClickEvent(button, false);
		this->notifier->PutEvent(mce);
	}
}

bool KeyListener::IsClicked(int button)
{
	return this->mouse[button];
}