
#pragma once

#ifndef _HASH_TABLE_HPP_
#define _HASH_TABLE_HPP_

#include <iostream>

template<typename K, typename V>
class HashTable final
{
	struct Node
	{
		K Key;
		V Value;
		Node *Next;
	};
public:
	HashTable(size_t inCapacity)
		: mCapacity(inCapacity)
		, mNodes(nullptr)
	{
		mNodes = new Node*[inCapacity];
		for (size_t i = 0; i < inCapacity; ++i)
		{
			mNodes[i] = nullptr;
		}
	}

	void RemoveAll()
	{
		for (size_t i = 0; i < mCapacity; ++i)
		{
			if (mNodes[i])
			{
				Node * node = mNodes[i];
				while (node)
				{
					node = node->Next;
					delete node;
				}
			}
		}
		delete[] mNodes;
	}

	void Deconstruct(DWORD *inHash, DWORD *inElements, size_t numElements)
	{

	}

	void Insert(const K &inKey, const V &inValue)
	{
		const auto hash = Hash(inKey);
		const auto index = hash % (mCapacity - 1);

		Node *node = mNodes[index];

		while (node && node->Next)
		{
			node = node->Next;
		}

		Node *newNode = new Node;
		newNode->Next = nullptr;
		newNode->Key = inKey;
		newNode->Value = inValue;

		const auto info = (node == nullptr) ? " as root" : " as next node";
		std::cout << "insert at index " << index << info << "\n";

		if (node)
		{
			node->Next = newNode;
		}
		else
		{
			mNodes[index] = newNode;
		}
	}

	bool GetValue(const K &inKey, V &outValue)
	{
		const auto hash = Hash(inKey);
		const auto index = hash % (mCapacity - 1);

		Node *node = mNodes[index];

		while (node)
		{
			if (node->Key == inKey)
			{
				outValue = node->Value;
				return true;
			}

			node = node->Next;
		}

		return false;
	}

private:
	int Hash(K inKey)
	{
		return ((int)inKey) % ((int)mCapacity);
	}

private:
	Node** mNodes;
	size_t mCapacity;
};

#endif //_HASH_TABLE_HPP_