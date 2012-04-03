#ifndef LIST_H
#define LIST_H

#include <cstdlib>
#include <iostream>

using namespace std;

namespace MaloW
{
	template <typename T>
	class List
	{
	private :
		class Node
		{
		public:
			T data;
			Node *next;
			Node(T data){ this->data = data; this->next = NULL;}
			~Node() {}
		};
	
		Node *first;
		int nrOfNodes;
	public:
		List();
		List(const List& list);
		virtual ~List();
		List& operator=(const List& list);
		void insertLast(T data);
		void insertAt(int pos, T data);
		void insertFirst(T data);
		bool isEmpty() const;
		T& elementAt(int pos) const;
		T& getAndRemove(int pos);
		void removeFirst();
		void removeLast();
		void removeAt(int pos);
		int size() const;
	};

	template <typename T>
	List<T>::List()
	{
		this->first = NULL;
		this->nrOfNodes = 0;
	}

	template <typename T>
	List<T>::List(const List& list)
	{	
		this->nrOfNodes = list.nrOfNodes;
		if(list.nrOfNodes > 0)
		{
			Node *temp = list.first;
			this->first = new Node(temp->data);
			Node *tail = this->first;
			while(temp->next != NULL)
			{
				temp = temp->next;
				tail->next = new Node(temp->data);
				tail = tail->next;
			}
		}
		else
		{
			this->first = NULL;
		}
	
	}

	template <typename T>
	List<T>::~List()
	{
		Node *temp = this->first;
		while(temp != NULL)
		{
			this->first = this->first->next;
			delete temp;
			temp = this->first;
		}
	}

	template <typename T>
	List<T>& List<T>::operator=(const List<T>& list)
	{
		if(this->nrOfNodes > 0)
		{
			Node *temp = this->first;
			while(temp != NULL)
			{
				this->first = this->first->next;
				delete temp;
				temp = this->first;
			}
		}

		this->nrOfNodes = 0;
		if(list.nrOfNodes > 0)
		{
			Node *temp = list.first;
			while(temp != NULL)
			{
				insertLast(temp->data);
				temp = temp->next;
			}
		}
		else
			this->first = NULL;

		return *this;
	}

	template <typename T>
	void List<T>::insertLast(T data)
	{
		this->insertAt(this->nrOfNodes+1, data);
	}

	template <typename T>
	void List<T>::insertAt(int pos, T data)
	{
		if(pos > this->nrOfNodes)
			pos = this->nrOfNodes+1;
		if(pos < 2)
			this->insertFirst(data);
		else
		{
			Node *next = this->first;
			Node *prev = next;
			for(int i = 0; i < pos-1; i++)
			{
				prev = next;
				next = next->next;
			}
			Node *e = new Node(data);
			prev->next = e;
			e->next = next;
			this->nrOfNodes++;
		}
	}

	template <typename T>
	void List<T>::insertFirst(T data)
	{
		Node *e = new Node(data);
		e->next = this->first;
		this->first = e;
		this->nrOfNodes++;
	}

	template <typename T>
	bool List<T>::isEmpty() const
	{
		return this->nrOfNodes == 0;
	}

	template <typename T>
	T& List<T>::elementAt(int pos) const
	{
		T data;
		if(pos > this->nrOfNodes)
			pos = this->nrOfNodes;
		if(pos < 1)
			pos = 1;
	
		Node *e = this->first;
		if(e)
		{
			for(int i = 0; i < pos-1; i++)
				e = e->next;
			data = e->data;
		}
		return data;
	}

	template <typename T>
	T& List<T>::getAndRemove(int pos)
	{
		T retval = this->elementAt(pos);
		this->removeAt(pos);
		return retval;
	}

	template <typename T>
	void List<T>::removeFirst()
	{
		if(this->first)
		{
			Node *newFirst = this->first->next;
			delete this->first;
			this->first = newFirst;
			this->nrOfNodes--;
		}
	}

	template <typename T>
	void List<T>::removeLast()
	{
		if(this->nrOfNodes > 1)
			this->removeAt(this->nrOfNodes);
		else
			this->removeFirst();v
	}

	template <typename T>
	void List<T>::removeAt(int pos)
	{
		if(pos < 1)
			this->removeFirst();
		else
		{
			Node *e = this->first;
			for(int i = 0; i < pos-2; i++)
				e = e->next;
			Node* tempNext = e->next->next;
			delete e->next;
			e->next = tempNext;
			this->nrOfNodes--;
		}
	}

	template <typename T>
	int List<T>::size() const
	{
		return this->nrOfNodes;
	}
}



#endif