#pragma once
#include<string>
#include<iostream>
#include<algorithm>
#include"bank.h"

#define DEFAULT_TABLE_SIZE 0xFF
#define INVALID_ARGUMENT_EXEPTION 0x00
#define NULL_FUNC_EXEPTION 0x01

#define ENTER(x) std::cout << "Вставка на " << x << std::endl

template<class K, class V>
struct HashTable {

public:
	typedef unsigned int (*hashF)(K, unsigned int);
	typedef bool (*checkF)(K, V*);

private:
	unsigned int tableSize;
	unsigned int count;
	hashF hashFunc;
	hashF altHashFunc;
	checkF checkFunc;

	K* keys = nullptr;
	

	//Перевыделить память под хэш-таблицу
	void reallocTable();

	//Проинициализировать ячейки таблицы как nullptr
	void initNulls(size_t a, size_t b) {
		for (size_t i = a; i < b; i++) {
			values[i] = nullptr;
		}
	}
public:
	
	V** values = nullptr;

	HashTable();
	HashTable(unsigned int);
	HashTable(hashF);
	HashTable(unsigned int, hashF);
	~HashTable();

	//Функция вставки объекта в хэш-таблицу. Реализует двойное хэширование	
	bool insert(K, V);
	//Функция удаления объекта из хэш-таблицы
	bool remove(K);
	//Функция пересчета хэш-таблицы (нужно при расширении таблицы)
	bool rehash();

	void setHashFunc(hashF f);
	void setAltHashFunc(hashF f);
	void setCheckFunc(checkF f);

	inline unsigned int getCount() { return count; }

	inline unsigned int getTableSize() { return tableSize; }


	unsigned int hash(K);
	unsigned int altHash(K);

	long long indexOf(K);
	V* operator[](K);
};

template<class K, class V>
inline void HashTable<K, V>::reallocTable()
{
	size_t oldSize = tableSize;
	K* newK = new K[tableSize * 2];
	V** newV = new V * [tableSize * 2];
	for (int i = 0; i < tableSize; i++) {
		newK[i] = keys[i];
		newV[i] = values[i];
	}
	delete[] keys;
	delete[] values;
	keys = newK;
	values = newV;
	tableSize += DEFAULT_TABLE_SIZE;
	initNulls(oldSize, tableSize);
}

template<class K, class V>
inline HashTable<K, V>::HashTable()
{
	this->tableSize = DEFAULT_TABLE_SIZE;
	this->keys = new K[tableSize];
	this->values = new V * [tableSize];
	this->count = 0;
	initNulls(0, tableSize);
}

template<class K, class V>
inline HashTable<K, V>::HashTable(unsigned int tableSize)
{
	this->tableSize = tableSize;
	this->keys = new K[tableSize];
	this->values = new V * [tableSize];
	this->count = 0;
	initNulls(0, tableSize);
}

template<class K, class V>
inline HashTable<K, V>::HashTable(hashF f) {
	this->hashFunc = f;
	this->tableSize = DEFAULT_TABLE_SIZE;
	this->keys = new K[tableSize];
	this->values = new V * [tableSize];
	this->count = 0;
	initNulls(0, tableSize);
}

template<class K, class V>
inline HashTable<K, V>::HashTable(unsigned int tableSize, hashF f)
{
	this->tableSize = tableSize;
	this->hashFunc = f;
	keys = new K[tableSize];
	values = new V * [tableSize];
	this->count = 0;
	initNulls(0, tableSize);
}

template<class K, class V>
inline HashTable<K, V>::~HashTable()
{
	if (keys) delete[] keys;
	if (values) delete[] values;
}

template<class K, class V>
bool HashTable<K, V>::insert(K k, V v)
{
	unsigned int index = hash(k); // выполнив хэш-функцию, получаем индекс элемента в массиве

	if (tableSize - count < 8) {
		reallocTable();
	}

	if (values[index] == nullptr) {
		keys[index] = k;
		values[index] = new V(v);
		count++;
		ENTER(index);
		return true;
	}

	index = altHash(k);

	if (values[index] == nullptr) {
		keys[index] = k;
		values[index] = new V(v);
		count++;
		ENTER(index);
		return true;
	}

	size_t step = 1;

	do {
		switch (step % 2)
		{
		case 1:
			index = hash(k) + step;
			break;
		case 0:
			index = altHash(k) + step;
			break;
		}
		++step;
	} while (values[index] != nullptr);

	values[index] = new V(v);

	ENTER(index);
	count = count + 1;
	return true;
}

template<class K, class V>
inline bool HashTable<K, V>::remove(K k)
{
	unsigned int index = hash(k);

	V* v = values[index];

	if (checkFunc(k, v)) {
		delete v;
		values[index] = nullptr;
		return true;
	}

	index = altHashFunc(k, tableSize);
	v = values[index];

	if (checkFunc(k, v)) {
		delete v;
		values[index] = nullptr;
		return true;
	}

	size_t step = 1;

	do {
		if (step > tableSize)
			return false;
		switch (step % 2)
		{
		case 1:
			index = (hash(k) + step) % tableSize;
			v = values[index];
			break;
		case 0:
			index = (altHash(k) + step) % tableSize;
			v = values[index];
			break;
		}

		++step;
	} while (!checkFunc(k, v));

	delete v;
	values[index] = nullptr;
	return true;
}



template<class K, class V>
inline void HashTable<K, V>::setHashFunc(hashF f)
{
	this->hashFunc = f;
}

template<class K, class V>
inline void HashTable<K, V>::setAltHashFunc(hashF f)
{
	this->altHashFunc = f;
}

template<class K, class V>
inline void HashTable<K, V>::setCheckFunc(checkF f)
{
	this->checkFunc = f;
}

template<class K, class V>
inline unsigned int HashTable<K, V>::hash(K k)
{
	if (!hashFunc)
		throw NULL_FUNC_EXEPTION;

	return hashFunc(k, tableSize);
}

template<class K, class V>
inline unsigned int HashTable<K, V>::altHash(K k)
{
	if (!altHashFunc)
		throw NULL_FUNC_EXEPTION;

	return altHashFunc(k, tableSize);
}


template<class K, class V>
 long long HashTable<K, V>::indexOf(K k) {
	unsigned int index = hash(k);

	V* v = values[index];

	if (checkFunc(k, v))
		return index;

	index = altHashFunc(k, tableSize);
	v = values[index];

	if (checkFunc(k, v))
		return index;

	size_t step = 1;

	do {
		if (step > tableSize)
			return -1;
		switch (step % 2)
		{
		case 1:
			index = (hash(k) + step) % tableSize;
			v = values[index];
			break;
		case 0:
			index = (altHash(k) + step) % tableSize;
			v = values[index];
			break;
		}

		++step;
	} while (!checkFunc(k, v));

	return index;

}

template<class K, class V>
V* HashTable<K, V>::operator[](K k)
{
	unsigned int index = hash(k);

	V* v = values[index];
	
	if (checkFunc(k, v))
		return v;

	index = altHashFunc(k, tableSize);
	v = values[index];

	if (checkFunc(k, v))
		return v;

	size_t step = 1;

	do {
		if (step > tableSize)
			return nullptr;
		switch (step % 2)
		{
		case 1:
			index = (hash(k) + step) % tableSize;
			v = values[index];
			break;
		case 0:
			index = (altHash(k) + step) % tableSize;
			v = values[index];
			break;
		}

		++step;
	} while (!checkFunc(k, v));

	return v;
}
