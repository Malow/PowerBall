#ifndef ARRAY_H
#define ARRAY_H

#include <cstdlib>
#include <iostream>

using namespace std;

namespace MaloW
{
	template <class T>
	class Array
	{
	private:
		T** items;
		int nrOfItems;
		int itemsCapacity;
		void expand();

	public:
		Array();
		virtual ~Array();

		T& operator[](int pos);

		void add(const T& item);
		T& get(int pos);
		T& getAndRemove(int pos);
		bool isEmpty();
		bool hasItem(const T& item);
		bool remove(const T& item);
		bool remove(int pos);
		int search(const T& item) const;
		void sort();
		int size() const { return this->nrOfItems; }

	};

	template <typename T>
	Array<T>::Array()
	{
		this->nrOfItems = 0;
		this->itemsCapacity = 10;
		this->items = new T*[this->itemsCapacity];
	}

	template <typename T>
	Array<T>::~Array()
	{
		for(int i = 0; i < this->nrOfItems; i++)
			if(this->items[i])
			{
				delete this->items[i];
			}

		delete this->items;
	}

	template <typename T>
	void Array<T>::expand()
	{
		T** tempItems = new T*[this->itemsCapacity*2];
		int i = 0;
		while(i < this->itemsCapacity)
		{
			tempItems[i] = this->items[i];
			i++;
		}
		while(i < this->itemsCapacity*2)
		{
			tempItems[i] = NULL;
			i++;
		}
		delete this->items;
		this->items = tempItems;
		tempItems = NULL;
		this->itemsCapacity = this->itemsCapacity*2;
	}

	template <typename T>
	T& Array<T>::operator[](int pos)
	{
		return this->get(pos);
	}

	template <typename T>
	void Array<T>::add(const T& item)
	{
		this->items[this->nrOfItems] = new T(item);
		this->nrOfItems++;
		if(this->nrOfItems == this->itemsCapacity)
			this->expand();
	}

	template <typename T>
	T& Array<T>::get(int pos)
	{
		return *this->items[pos];
	}

	template <typename T>
	T& Array<T>::getAndRemove(int pos)
	{
		T retVal;
		retVal = *this->items[pos];
		this->remove(pos);
		return retVal;
	}

	template <typename T>
	bool Array<T>::isEmpty()
	{
		bool ret = true;
		if(this->nrOfItems > 0)
			ret = false;

		return ret;
	}

	template <typename T>
	bool Array<T>::hasItem(const T& item)
	{
		bool ret = false;
		if(this->search(item) != -1)
			ret = true;

		return ret;
	}

	template <typename T>
	bool Array<T>::remove(const T& item)
	{
		return this->remove(this->search(item));
	}

	template <typename T>
	bool Array<T>::remove(int pos)
	{
		bool succeed = false;
		if(pos > -1 || pos < this->nrOfItems)
			if(this->items[pos])
			{
				delete this->items[pos];
				this->nrOfItems--;
				this->items[pos] = this->items[this->nrOfItems];
				this->items[this->nrOfItems] = NULL;
				succeed = true;
			}
		return succeed;
	}

	template <typename T>
	int Array<T>::search(const T& item) const
	{
		int pos = -1;
		for(int i = 0; i < this->nrOfItems; i++)
			if(*this->items[i] == item)
			{
				pos = i;
				i = this->nrOfItems;
			}
		return pos;
	}

	template <typename T>
	void Array<T>::sort()
	{
		T temp;
		int counter = 0;
		for(int i = 1; i < this->nrOfItems; i++)
		{
			temp = this->items[i];
			counter = i-1;
			while(this->items[counter] > temp && counter >= 0)
			{
				this->items[counter+1] = this->items[counter];
				counter--;
			}
			this->items[counter+1] = temp;
		}
	}
}
#endif


