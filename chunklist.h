#pragma once

class ChunkNode;

class ChunkListEntry
{
public:
	ChunkListEntry(ChunkNode* node)
		: prev(nullptr)
		, next(nullptr)
		, chunk(node)
	{
	}

	ChunkListEntry* prev;
	ChunkListEntry* next;

	ChunkNode* chunk;	//!< TODO: shared pointer
};

//! double linked list of chunks.
//! does not own entries!
class ChunkList
{
public:
	ChunkList();

	int trueCount() const;
	int count() const { return mCount; }

	ChunkListEntry* first() const { return mHead; }
	ChunkListEntry* last() const { return mTail; }
	bool isEmpty() const;

	void insertEntry(ChunkListEntry* entry, ChunkListEntry* next);

	void takeEntry(ChunkListEntry* entry);

	ChunkListEntry *takeFirst();

	ChunkListEntry *takeLast();

	void insertFirst(ChunkListEntry* entry);
	
private:
	ChunkListEntry *mHead;
	ChunkListEntry *mTail;
	int mCount;
};
