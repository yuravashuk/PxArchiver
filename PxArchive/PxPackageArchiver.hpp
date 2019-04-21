
#pragma once

#ifndef _PX_PACKAGE_ARCHIVER_HPP_
#define _PX_PACKAGE_ARCHIVER_HPP_

#include "PxPackage.hpp"
#include <vector>

class PxPackageArchiver
{
	struct FileInfo
	{
		void *Data;
		size_t Size;
	};
public:
	void SetHashFunction(THashFunction inHashFunction);
	void AddFile(const char *inFileName, const char *inFileDirection, const char *inFileFormat);
	void Save(const char *inFileName);

	PxPackage* GetPackage();

private:
	std::vector<FileInfo> mFiles;
	PxPackage mPackage;
	PxMappedFile mMappedFile;
};

#endif //_PX_PACKAGE_ARCHIVER_HPP_