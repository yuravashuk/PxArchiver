
#include "pch.h"
#include "PxPackage.hpp"
#include <cassert>

PxPackage::PxPackage(DWORD inNumEntries)
	: mNumEntries(inNumEntries)
	, mLanguage(0)
	, mVersion(1)
	, mRelativeOffset(0)
	, mMetaDataChunkOffset(0)
	, mMetaDataPool(nullptr)
{
	mHashTable.Files = nullptr;
	mHashTable.NumEntries = 0;
	mHashTable.NumMetaData = 0;

	mSerializer.SetDelegate(this);
}

bool PxPackage::Open(const std::string & inFileName)
{
	if (!mMappedFile.Open(inFileName))
	{
		return false;
	}

	mSerializer.SetMappedFile(&mMappedFile);

	if (!mSerializer.Read(mHeader))
	{
		return false;
	}

	if (!mSerializer.Read(mHashTable))
	{
		return false;
	}

	return true;
}

void PxPackage::Save(const std::string &inFileName)
{
	if (!mMappedFile.Open(inFileName))
	{
		return;
	}

	mHeader.Version = mVersion;
	mHeader.Type = 2000;
	mHeader.Size = 2000;

	mSerializer.SetMappedFile(&mMappedFile);

	mSerializer.Write(mHeader);
	mSerializer.Write(mHashTable);
}

void PxPackage::Close()
{
	mMappedFile.Close();

	ClearHashTable();
	ClearMetaDataPool();
}

void PxPackage::SetVersion(DWORD inVersion)
{
	mVersion = inVersion;
}

void PxPackage::SetLanguage(DWORD inLanguage)
{
	mLanguage = inLanguage;
}

void PxPackage::SetHashFunction(THashFunction inFunction)
{
	mHashFunction = inFunction;
}

DWORD PxPackage::GetVersion() const
{
	return mVersion;
}

DWORD PxPackage::GetLanguage() const
{
	return mLanguage;
}

DWORD PxPackage::GetNumEntries() const
{
	return mNumEntries;
}

PxPackageFileEntry* PxPackage::Insert(const char * inFileName, void * inAddress, size_t inNumBytes, DWORD inLanguage, DWORD inIsCompressed, DWORD inFileType)
{
	if (mHashTable.Files == nullptr)
	{
		AllocateHashMap(mHashTable, mNumEntries);
	}

	const auto hash = GetHash(inFileName);
	const auto index = hash % (mNumEntries - 1);

	PxPackageFileEntry *node = mHashTable.Files[index];

	while (node && node->Next)
	{
		node = node->Next;
	}

	auto newNode = new PxPackageFileEntry;
	newNode->Next = nullptr;
	newNode->Compressed = inIsCompressed;
	newNode->Language = inLanguage;
	newNode->Type = inFileType;
	newNode->NumMetaData = 0;
	newNode->Offset = mRelativeOffset;
	
	newNode->AddressInRAM = inAddress;
	newNode->Size = static_cast<DWORD>(inNumBytes);

	newNode->Name = new char[strlen(inFileName)];
	strcpy(newNode->Name, inFileName);

	mRelativeOffset += static_cast<DWORD>(inNumBytes);

	if (node)
		node->Next = newNode;
	else
		mHashTable.Files[index] = newNode;

	return newNode;
}

PxPackageFileEntry * PxPackage::GetFileInfo(const char * inFileName) const
{
	const auto hash = GetHash(inFileName);
	const auto index = hash % (mNumEntries - 1);

	PxPackageFileEntry *node = mHashTable.Files[index];

	while (node)
	{
		if (strcmp(node->Name, inFileName) == 0)
		{
			return node;
		}

		node = node->Next;
	}

	return nullptr;
}

PxPackageFileMetaData * PxPackage::AllocateParameters(const char * inFileName, size_t inNumMetaData)
{
	auto info0 = GetFileInfo(inFileName);

	if (info0)
	{
		info0->MetaData = new PxPackageFileMetaData[inNumMetaData];
		info0->NumMetaData = inNumMetaData;

		mHashTable.NumMetaData += inNumMetaData;

		return info0->MetaData;
	}

	return nullptr;
}

void * PxPackage::GetFileVirtualAddress(const char *inFileName)
{
	PxPackageFileEntry *info = GetFileInfo(inFileName);

	if (info)
	{
		auto address = mMappedFile.GetVirtualAddressAs<char>();
		return (void*)(address + info->Offset);
	}

	return nullptr;
}

void PxPackage::PreallocateMetaDataChunks(DWORD inNumMetaData)
{
	if (mMetaDataPool)
	{
		delete[] mMetaDataPool;
	}

	mMetaDataPool = new PxPackageFileMetaData[inNumMetaData];
	memset(mMetaDataPool, 0, sizeof(PxPackageFileMetaData) * inNumMetaData);
}

void PxPackage::AllocateHashMap(PxPackageFileTable & outTable, DWORD inNumEntries)
{
	outTable.NumEntries = inNumEntries;
	outTable.Files = new PxPackageFileEntry*[inNumEntries];

	for (DWORD i = 0; i < inNumEntries; ++i)
	{
		outTable.Files[i] = nullptr;
	}
}

PxPackageFileMetaData * PxPackage::GetAllocatedMetaDataChunk()
{
	auto metadata = &mMetaDataPool[mMetaDataChunkOffset];
	mMetaDataChunkOffset++;
	return metadata;
}

PxPackageFileMetaData * PxPackage::GetAllocatedMetaDataChunks(DWORD inNumMeta)
{
	auto metadata = &mMetaDataPool[mMetaDataChunkOffset];
	mMetaDataChunkOffset += inNumMeta;
	return metadata;
}

DWORD PxPackage::GetHash(const char *inName) const
{
	if (mHashFunction)
	{
		return mHashFunction(inName);
	}

	assert("No hash function found");
	return 0;
}

void PxPackage::ClearHashTable()
{
	if (mHashTable.Files)
	{
		for (DWORD i = 0; i < mNumEntries; ++i)
		{
			PxPackageFileEntry *node = mHashTable.Files[i];

			while (node)
			{
				auto ptr = node;
				node = node->Next;
				delete ptr;
			}

			mHashTable.Files[i] = nullptr;
		}

		delete[] mHashTable.Files;
	}
	
	mHashTable.Files = nullptr;
	mHashTable.Files = 0;
}

void PxPackage::ClearMetaDataPool()
{
	if (mMetaDataPool)
	{
		for (size_t i = 0; i < mMetaDataChunkOffset; ++i)
		{
			if (mMetaDataPool[i].Name)
			{
				delete[] mMetaDataPool[i].Name;
			}
		}

		delete[] mMetaDataPool;
	}

	mMetaDataPool = nullptr;
	mMetaDataChunkOffset = 0;
	mHashTable.NumMetaData = 0;
}