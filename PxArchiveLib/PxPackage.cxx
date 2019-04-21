
#include "PxPackage.hpp"
#include <cassert>

PxPackage::PxPackage(DWORD inNumEntries)
	: mNumEntries(inNumEntries)
	, mLanguage(0)
	, mVersion(1)
	, mMetaDataChunkOffset(0)
	, mMetaDataPool(nullptr)
{
	mHashTable.Files = nullptr;
	mHashTable.NumEntries = 0;
	mHashTable.NumMetaData = 0;
	mFormats.NumFormats = 0;

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

	if (!mSerializer.Read(mFormats))
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
	mSerializer.Write(mFormats);
	mSerializer.Write(mFormats, mHashTable);
}

void PxPackage::Close()
{
	mMappedFile.Close();

	ClearHashTable();
	ClearMetaDataPool();
	ClearFormats();
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

PxPackageFileEntry* PxPackage::Insert(const PxPackageInsertionDescriptor &inDescriptor)
{
	if (mHashTable.Files == nullptr)
	{
		AllocateHashMap(mHashTable, mNumEntries);
	}

	const auto hash = GetHash(inDescriptor.FileName);
	const auto index = hash % (mNumEntries - 1);

	PxPackageFileEntry *node = mHashTable.Files[index];

	while (node && node->Next)
	{
		node = node->Next;
	}

	int fileFormat = RegisterFormat(inDescriptor.FileFormat);

	auto newNode = new PxPackageFileEntry;
	newNode->Next = nullptr;
	newNode->Compressed = inDescriptor.IsCompressed;
	newNode->Language = inDescriptor.Language;
	newNode->Type = fileFormat;
	newNode->NumMetaData = 0;
	newNode->Offset = 0;
	
	newNode->AddressInRAM = inDescriptor.Address;
	newNode->Size = inDescriptor.NumBytes;

	newNode->Name = new char[strlen(inDescriptor.FileName)];
	strcpy(newNode->Name, inDescriptor.FileName);

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
		auto address = mMappedFile.GetVirtualAddressAs<BYTE>();
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

void PxPackage::ClearFormats()
{
	if (mFormats.Formats)
	{
		for (size_t i = 0; i < _countof(mFormats.Formats); ++i)
		{
			delete[] mFormats.Formats[i];
			mFormats.Formats[i] = nullptr;
		}
	}

	mFormats.NumFormats = 0;
}

std::vector<PxPackageFileEntry*> PxPackage::GetAllFileInfos() const
{
	std::vector<PxPackageFileEntry*> entries;

	if (mHashTable.Files)
	{
		for (DWORD i = 0; i < mNumEntries; ++i)
		{
			PxPackageFileEntry *node = mHashTable.Files[i];

			while (node)
			{
				entries.push_back(node);
				node = node->Next;
			}
		}
	}

	return entries;
}

const char* PxPackage::GetFileExtension(DWORD inType) const
{
	if (mFormats.NumFormats < inType)
	{
		return "";
	}

	return mFormats.Formats[inType];
}

DWORD PxPackage::RegisterFormat(const char *inFormat)
{
	int fileFormat = -1;

	for (DWORD i = 0; i < mFormats.NumFormats; ++i)
	{
		if (strcmp(mFormats.Formats[i], inFormat) == 0)
		{
			fileFormat = i;
		}
	}

	int index = fileFormat;

	if (fileFormat == -1)
	{
		index = mFormats.NumFormats;
		mFormats.NumFormats += 1;
	}

	mFormats.Formats[index] = new char[strlen(inFormat)];
	strcpy(mFormats.Formats[index], inFormat);
	
	return static_cast<DWORD>(index);
}