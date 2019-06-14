
#pragma once

#ifndef _PX_PACKAGE_WRITER_HPP_
#define _PX_PACKAGE_WRITER_HPP_

#include <string>
#include <vector>

#include "PxPackageAllocator.hpp"
#include "PxPackageStructure.hpp"
#include "PxPackageSerializer.hpp"

struct PxPackageInsertionDescriptor
{
	const char *FileName;
	const char *FileFormat;

	void *Address;
	DWORD NumBytes;

	DWORD Language;
	DWORD IsCompressed;
};

class PxPackageWriter final
{
public:
	PxPackageWriter(DWORD inNumEntries = 256);
	~PxPackageWriter();

	bool Open(const std::string &inFileName);
	void Save(const std::string &inFileName);

	void SetVersion(DWORD inVersion);
	void SetLanguage(DWORD inLanguage);
	void SetHashFunction(THashFunction inFunction);

	PxPackageFileEntry* Insert(const PxPackageInsertionDescriptor &inDescriptor);
	PxPackageFileEntry* GetFileInfo(const char *inFileName) const;
	PxPackageFileMetaData* AllocateParameters(const char *inFileName, size_t inNumMetaData);

	std::vector<PxPackageFileEntry*> GetAllFileInfos() const;
	const char* GetFileExtension(DWORD inType) const;

private:

};

#endif //_PX_PACKAGE_WRITTER_HPP_