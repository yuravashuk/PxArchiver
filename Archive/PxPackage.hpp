
#pragma once

#ifndef _PX_PACKAGE_HPP_
#define _PX_PACKAGE_HPP_

#include <functional>

#include "PxPackageStructure.hpp"
#include "PxPackageSerializer.hpp"
#include "PxMappedFile.hpp"

using THashFunction = std::function<DWORD(const char *)>;

const auto kPxJenkinsHashFunction = [](const char *inName) {
	size_t i = 0;
	size_t length = strlen(inName);

	DWORD hash = 0;
	
	while (i != length)
	{
		hash += inName[i++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
};


class PxPackage final 
	: public PxPackageSerializerDelegate
{
public:
	PxPackage(DWORD inNumEntries = 256);

	bool Open(const std::string &inFileName);
	void Save(const std::string &inFileName);
	void Close();

	void SetVersion(DWORD inVersion);
	void SetLanguage(DWORD inLanguage);
	void SetHashFunction(THashFunction inFunction);

	DWORD GetVersion() const;
	DWORD GetLanguage() const;
	DWORD GetNumEntries() const;

	PxPackageFileEntry* Insert(const char *inFileName, void *inAddress, size_t inNumBytes, DWORD inLanguage, DWORD inIsCompressed, DWORD inFileType);

	PxPackageFileEntry* GetFileInfo(const char *inFileName) const;

	PxPackageFileMetaData* AllocateParameters(const char *inFileName, size_t inNumMetaData);

	void* GetFileVirtualAddress(const char *inFileName);

	template<typename T>
	T* GetFileVirtualAddressAs(const char *inFileName)
	{
		return (T*)GetFileVirtualAddress(inFileName);
	}

	template<typename T>
	T* GetFileVirtualAddressAs()
	{
		return mMappedFile.GetVirtualAddressAs<T>();
	}

	void PreallocateMetaDataChunks(DWORD inNumMetaData) override;
	void AllocateHashMap(PxPackageFileTable &outTable, DWORD inNumEntries) override;
	PxPackageFileMetaData* GetAllocatedMetaDataChunk() override;
	PxPackageFileMetaData* GetAllocatedMetaDataChunks(DWORD inNumMeta) override;

private:
	DWORD GetHash(const char *inName) const;
	void ClearHashTable();

private:
	THashFunction mHashFunction;

	PxPackageFileMetaData *mMetaDataPool;
	PxPackageSerializer mSerializer;
	PxMappedFile mMappedFile;

	PxPackageHeader mHeader;
	PxPackageFileTable mHashTable;

	DWORD mLanguage;
	DWORD mVersion;

	DWORD mNumEntries;

	DWORD mRelativeOffset;
	DWORD mMetaDataChunkOffset;
};

#endif //_PX_PACKAGE_HPP_