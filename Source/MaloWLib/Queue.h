#ifndef QUEUE_H
#define QUEUE_H

#include "List.h"

namespace MaloW
{
	template <class T>
	class Queue
	{
	private:
		List<T>* list;

	public:
		Queue();
		~Queue();

		void Enqueue(T obj);
		T& Dequeue();
		T& Peek();

		int size();
		bool isEmpty();
	};

	template <typename T>
	Queue<T>::Queue()
	{
		this->list = new List<T>;
	}

	template <typename T>
	Queue<T>::~Queue()
	{
		if(this->list)
			delete this->list;
	}

	template <typename T>
	void Queue<T>::Enqueue(T obj)
	{
		this->list->insertLast(obj);
	}

	template <typename T>
	T& Queue<T>::Dequeue()
	{
		return this->list->getAndRemove(0);
	}

	template <typename T>
	T& Queue<T>::Peek()
	{
		return this->list->get(0);
	}

	template <typename T>
	int Queue<T>::size()
	{
		return this->list->size();
	}
	
	template <typename T>
	bool Queue<T>::isEmpty()
	{
		return this->list->isEmpty();
	}
}


#endif