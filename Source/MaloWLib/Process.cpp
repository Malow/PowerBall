#include "Process.h"

using namespace MaloW;

long Process::nextPID = 0;




void DebugMtxInfo(DWORD res)
{
	if(res != WAIT_OBJECT_0)
	{
		DWORD err = GetLastError();
		if(res == WAIT_ABANDONED)
			MaloW::Debug("MUTEX FAILED: ERROR: WAIT_ABANDONED!");
		else if(res == WAIT_TIMEOUT)
			MaloW::Debug("MUTEX FAILED: ERROR: WAIT_TIMEOUT!");
		else if(res == WAIT_FAILED)
			MaloW::Debug("MUTEX FAILED: ERROR: WAIT_FAILED!");
			
		LPTSTR error;
		
		FormatMessage(
			FORMAT_MESSAGE_ALLOCATE_BUFFER | 
			FORMAT_MESSAGE_FROM_SYSTEM |
			FORMAT_MESSAGE_IGNORE_INSERTS,
			NULL,
			err,
			MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR) &error,
			0, NULL );

		string msg = "Error code: " + *error;

		MaloW::Debug(msg);
	}
}






Process::Process()
{
	this->id = this->nextPID;
	this->nextPID++;
	this->state = NOT_STARTED;
	this->EvQueue = new Queue<ProcessEvent*>;
	this->ProcMtx = CreateMutex(NULL, false, NULL);

	this->debug = false;
	this->stayAlive = true;
}

void Process::Start()
{
	if(this->state == NOT_STARTED)
	{
		this->hThread = ::CreateThread(0, 0, this->threadProc, this, 0, &this->threadID);
		this->state = RUNNING;
	}
}

Process::~Process()
{
	/*
	this->Close();

	if(this->ProcMtx)
	{
		DWORD res = WaitForSingleObject(this->ProcMtx, INFINITE);
		DebugMtxInfo(res);
	}
	else 
		MaloW::Debug("Mutex is broken / hasn't been created / has been closed for proc: " + MaloW::convertNrToString(this->id));

	if(this->debug)
		MaloW::Debug("ERROR: Proc: " + MaloW::convertNrToString(this->id) + " Mutex for PutEvent failed, multiple procs modifying data.");
	this->debug = true;
	*/

	if(this->debug)
		MaloW::Debug("ERROR: Proc: " + MaloW::convertNrToString((float)this->id) + " deleted itself while another process was doing work with it.");
	this->debug = true;

	if(this->EvQueue)
	{
		while(!this->EvQueue->isEmpty())
			delete this->EvQueue->Dequeue();
	
		delete this->EvQueue;
	}
	
	CloseHandle(this->ProcMtx);
}

void Process::Life()
{
	
}

void Process::Suspend()
{
	SuspendThread(this->hThread);
}

void Process::Resume()
{
	ResumeThread(this->hThread);
}

ProcessEvent* Process::WaitEvent()
{
	if(this->ProcMtx)
	{
		//MaloW::Debug("Wiieee, mtx is true");
		DWORD res = WaitForSingleObject(this->ProcMtx, INFINITE);
		DebugMtxInfo(res);
	}
	else 
		MaloW::Debug("Mutex is broken / hasn't been created / has been closed for proc: " + MaloW::convertNrToString((float)this->id));
	
	if(this->debug)
		MaloW::Debug("ERROR: Proc: " + MaloW::convertNrToString((float)this->id) + " Mutex for WaitEvent failed, multiple procs modifying data.");
	this->debug = true;

	bool sleep = this->EvQueue->isEmpty();
	
	if(sleep)
	{
		this->state = WAITING;

		this->debug = false;
		ReleaseMutex(this->ProcMtx);

		this->Suspend();

		if(this->ProcMtx)
		{
			DWORD res = WaitForSingleObject(this->ProcMtx, INFINITE);
			DebugMtxInfo(res);
		}
		else 
			MaloW::Debug("Mutex is broken / hasn't been created / has been closed for proc: " + MaloW::convertNrToString((float)this->id));

		if(this->debug)
			MaloW::Debug("ERROR: Proc: " + MaloW::convertNrToString((float)this->id) + " Mutex for WaitEvent, second,  failed, multiple procs modifying data.");
		this->debug = true;
	}

	ProcessEvent* ev = this->EvQueue->Dequeue();

	this->debug = false;
	ReleaseMutex(this->ProcMtx);
	return ev;
}

ProcessEvent* Process::PeekEvent()
{
	if(this->ProcMtx)
	{
		DWORD res = WaitForSingleObject(this->ProcMtx, INFINITE);
		DebugMtxInfo(res);
	}
	else 
		MaloW::Debug("Mutex is broken / hasn't been created / has been closed for proc: " + MaloW::convertNrToString((float)this->id));

	if(this->debug)
		MaloW::Debug("ERROR: Proc: " + MaloW::convertNrToString((float)this->id) + " Mutex for PeekEvent failed, multiple procs modifying data.");
	this->debug = true;

	ProcessEvent* ev = NULL;
	if(!this->EvQueue->isEmpty())
		ev = this->EvQueue->Dequeue();

	this->debug = false;
	ReleaseMutex(this->ProcMtx);
	return ev;
}

void Process::PutEvent(ProcessEvent* ev, bool important)
{
	bool go = true;
	if(!important)
	{
		if(this->EvQueue->size() > 20)
		{
			go = false;
			delete ev;	//////////// Maybe crash? Added without testing
		}
	}

	if(go)
	{
		if(this->ProcMtx)
		{
			DWORD res = WaitForSingleObject(this->ProcMtx, INFINITE);
			DebugMtxInfo(res);
		}
		else 
			MaloW::Debug("Mutex is broken / hasn't been created / has been closed for proc: " + MaloW::convertNrToString((float)this->id));

		if(this->debug)
			MaloW::Debug("ERROR: Proc: " + MaloW::convertNrToString((float)this->id) + " Mutex for PutEvent failed, multiple procs modifying data.");
		this->debug = true;

		int queueSize = this->EvQueue->size();

		this->EvQueue->Enqueue(ev);

		if(queueSize > 500)
		{
			MaloW::Debug("Warning, EventQueue of process " + MaloW::convertNrToString((float)this->id) + " has " + 
				MaloW::convertNrToString((float)this->EvQueue->size()) + " unread events.");
		}
		if(this->state == WAITING)
		{
			this->state = RUNNING;
			this->Resume();
		}

		this->debug = false;
		ReleaseMutex(this->ProcMtx);
	}
}

void Process::WaitUntillDone()
{
	while(this->state != FINISHED)
		Sleep(1);
}

void Process::Close()
{
	this->stayAlive = false;
	ProcessEvent* ev = new ProcessEvent();
	this->PutEvent(ev);	
	CloseSpecific();
}