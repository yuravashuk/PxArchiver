
#pragma once

#ifndef _PX_PACKAGE_ALLOCATOR_HPP_
#define _PX_PACKAGE_ALLOCATOR_HPP_

#include "PxPackageSerializer.hpp"

class PxPackageAllocator 
	: public PxPackageSerializerDelegate
{
public:
	void SetMetaPool(PxPackageFileMetaData *inMetaPool);
	void SetHeader(PxPackageHeader *inHeader);
	void SetFormats(PxPackageFileFormats *inFormats);
	void SetFileTable(PxPackageFileTable *inFileTable);

	void PreallocateMetaDataChunks(DWORD inNumMetaData) override;

	void AllocateHashMap(PxPackageFileTable &outTable, DWORD inNumEntries) override;

	PxPackageFileMetaData* GetAllocatedMetaDataChunk() override;

	PxPackageFileMetaData* GetAllocatedMetaDataChunks(DWORD inNumMeta) override;

	PxPackageFileMetaData* AllocateParameters(const char *inFileName, size_t inNumMetaData);

private:
	PxPackageFileMetaData *mMetaDataPool;
	PxPackageHeader *mHeader;
	PxPackageFileFormats *mFormats;
	PxPackageFileTable *mHashTable;
};

#endif //_PX_PACKAGE_ALLOCATOR_HPP_