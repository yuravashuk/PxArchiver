
#pragma once

#ifndef _PX_PACKAGE_SERIALIZER_HPP_
#define _PX_PACKAGE_SERIALIZER_HPP_

#include "PxPackageStructure.hpp"

class PxMappedFile;

struct PxPackageSerializerDelegate
{
	virtual ~PxPackageSerializerDelegate() = default;

	virtual void PreallocateMetaDataChunks(DWORD inNumMetaData) = 0;
	virtual void AllocateHashMap(PxPackageFileTable &outTable, DWORD inNumEntries) = 0;

	virtual PxPackageFileMetaData* GetAllocatedMetaDataChunk() = 0;
	virtual PxPackageFileMetaData* GetAllocatedMetaDataChunks(DWORD inNumMeta) = 0;
};

class PxPackageSerializer
{
public:
	PxPackageSerializer();

	void SetMappedFile(PxMappedFile *inMappedFile);
	void SetDelegate(PxPackageSerializerDelegate *inDelegate);

	bool Write(const PxPackageHeader &inHeader);
	bool Write(const PxPackageFileFormats &inFormats);
	bool Write(const PxPackageFileFormats &inFormats,
		const PxPackageFileTable &inTable);

	bool Read(PxPackageHeader &outHeader);
	bool Read(PxPackageFileFormats &outFormats);
	bool Read(PxPackageFileTable &outTable);

	DWORD CalculateDataOffset(const PxPackageFileFormats &inFormats, 
		const PxPackageFileTable &inTable) const;

private:
	void WriteMetaData(PxPackageFileMetaData *inMetaData);
	void ReadMetaData(PxPackageFileMetaData *outMetaData);

private:
	PxMappedFile *mMappedFile;
	PxPackageSerializerDelegate *mDelegate;
};

#endif //_PX_PACKAGE_SERIALIZER_HPP_