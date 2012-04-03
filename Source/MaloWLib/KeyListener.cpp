#include "KeyListener.h"

using namespace MaloW;

KeyListener::KeyListener(MaloW::Process* notifier)
{
	this->notifier = notifier;
	for(int i = 0; i < 256; i++)
		this->keys[i] = false;
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