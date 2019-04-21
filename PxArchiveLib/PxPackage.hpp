
#pragma once

#ifndef _PX_PACKAGE_HPP_
#define _PX_PACKAGE_HPP_

#include <functional>
#include <vector>

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

struct PxPackageInsertionDescriptor
{
	const char *FileName;
	const char *FileFormat;

	void *Address;
	DWORD NumBytes;

	DWORD Language;
	DWORD IsCompressed;
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

	PxPackageFileEntry* Insert(const PxPackageInsertionDescriptor &inDescriptor);

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

	std::vector<PxPackageFileEntry*> GetAllFileInfos() const;

	const char* GetFileExtension(DWORD inType) const;

private:
	DWORD GetHash(const char *inName) const;
	void ClearHashTable();
	void ClearMetaDataPool();
	void ClearFormats();
	DWORD RegisterFormat(const char *inFormat);

private:
	THashFunction mHashFunction;

	PxPackageFileMetaData *mMetaDataPool;
	PxPackageSerializer mSerializer;
	PxMappedFile mMappedFile;

	PxPackageHeader mHeader;
	PxPackageFileFormats mFormats;
	PxPackageFileTable mHashTable;

	DWORD mLanguage;
	DWORD mVersion;
	DWORD mNumEntries;
	DWORD mMetaDataChunkOffset;
};

#endif //_PX_PACKAGE_HPP_