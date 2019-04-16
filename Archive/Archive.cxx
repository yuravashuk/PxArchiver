
#include "pch.h"
#include "Archive.hpp"

Archive::Archive()
{
	memset(&mData, 0, sizeof(ArchiveData));
}

bool Archive::Deserialize(const std::string &inFileName, DWORD inActualVersion)
{
	if (!mMappedFile.Open(inFileName))
	{
		return false;
	}

	auto dataPtr = mMappedFile.GetMappedAddressAs<char>();

	size_t offset = 0;

	memcpy(&mData.Header.Version, dataPtr, sizeof(DWORD));
	offset += sizeof(DWORD);

	memcpy(mData.Header.Type, dataPtr + offset, sizeof(char) * 4);
	offset += sizeof(char) * 4;

	if (mData.Header.Type[0] != 'd' ||
		mData.Header.Type[1] != 'a' ||
		mData.Header.Type[2] != 't' ||
		mData.Header.Type[3] != 'a' ||
		mData.Header.Version != inActualVersion)
	{
		UnloadContent();
		return false;
	}

	memcpy(&mData.FileTable.NumEntries, dataPtr + offset, sizeof(DWORD));
	offset += sizeof(DWORD);

	if (mData.FileTable.NumEntries <= 0)
	{
		Unload();
		return false;
	}

	// preallocate hash table
	const auto numEntries = mData.FileTable.NumEntries;
	mData.FileTable.HashKeys = new DWORD[numEntries];
	mData.FileTable.FileOffsets = new DWORD[numEntries];
	mData.FileTable.FileSizes = new DWORD[numEntries];

	memcpy(mData.FileTable.HashKeys, dataPtr + offset, sizeof(DWORD) * numEntries);
	offset += sizeof(DWORD) * numEntries;

	memcpy(mData.FileTable.FileOffsets, dataPtr + offset, sizeof(DWORD) * numEntries);
	offset += sizeof(DWORD) * numEntries;

	memcpy(mData.FileTable.FileSizes, dataPtr + offset, sizeof(DWORD) * numEntries);
	offset += sizeof(DWORD) * numEntries;

	return true;
}

void Archive::Serialize(const std::string &inFileName)
{
	FILE *file = fopen("example.bin", "wb");

	if (file == NULL)
	{
		return;
	}

	ArchiveData data;

	data.Header.Version = 2;
	data.Header.Type[0] = 'd';
	data.Header.Type[1] = 'a';
	data.Header.Type[2] = 't';
	data.Header.Type[3] = 'a';

	for (size_t i = 0; i < mCapacity; ++i)
	{
		FileEntryNode *node = mHashTable[i];
		while (node && node->Next)
		{
			const auto hash = GetHash(node->Key);
			
		}
	}

	const size_t numFiles = 4;
	data.FileTable.NumEntries = numFiles;
	data.FileTable.FileSizes = new DWORD[numFiles];
	data.FileTable.HashKeys = new DWORD[numFiles];
	data.FileTable.FileOffsets = new DWORD[numFiles];

	for (DWORD i = 0; i < static_cast<DWORD>(numFiles); ++i)
	{
		data.FileTable.HashKeys[i] = i;
		data.FileTable.FileSizes[i] = i + 2;
		data.FileTable.FileOffsets[i] = i + 4;
	}

	fwrite(&data.Header.Version, sizeof(DWORD), 1, file);
	fwrite(&data.Header.Type, sizeof(char), 4, file);

	fwrite(&data.FileTable.NumEntries, sizeof(DWORD), 1, file);
	fwrite(&data.FileTable.HashKeys, sizeof(DWORD), numFiles, file);
	fwrite(&data.FileTable.FileOffsets, sizeof(DWORD), numFiles, file);
	fwrite(&data.FileTable.FileSizes, sizeof(DWORD), numFiles, file);

	fclose(file);

	delete[] data.FileTable.FileOffsets;
	delete[] data.FileTable.FileSizes;
	delete[] data.FileTable.HashKeys;
}

void Archive::UnloadContent()
{
	mMappedFile.Close();

	if (mData.FileTable.HashKeys != nullptr)
	{
		delete[] mData.FileTable.HashKeys;
	}
	if (mData.FileTable.FileOffsets != nullptr)
	{
		delete[] mData.FileTable.FileOffsets;
	}
	if (mData.FileTable.FileSizes != nullptr)
	{
		delete[] mData.FileTable.FileSizes;
	}

	memset(&mData, 0, sizeof(ArchiveData));
}

void Archive::Insert(const char *inKey, const char *inData, size_t inDataLength)
{

}

void Archive::Insert(const char *inKey, const unsigned char *inData, size_t inDataLength)
{

}

DWORD Archive::GetHash(const char *inKey)
{
	// Jenkins hash function
	size_t i = 0;
	size_t length = sizeof(inKey);
	DWORD hash = 0;

	while (i != length) 
	{
		hash += inKey[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
}