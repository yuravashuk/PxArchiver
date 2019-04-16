
#include "pch.h"
#include "PxPackageArchiver.hpp"
#include "PxPackageStructure.hpp"

void PxPackageArchiver::SetHashFunction(THashFunction inHashFunction)
{
	mPackage.SetHashFunction(inHashFunction);
}

void PxPackageArchiver::AddFile(const char * inFileName, const char * inFileDirection)
{
	if (mMappedFile.Open(inFileDirection))
	{
		size_t fileSize = mMappedFile.GetFileSize() * sizeof(unsigned char);

		FileInfo info;
		info.Data = malloc(fileSize);
		info.Size = fileSize;
		mFiles.push_back(info);

		memset(info.Data, 0, fileSize);

		mMappedFile.SeekTo(0);
		mMappedFile.Read(info.Data, info.Size);

		mPackage.Insert(inFileName, info.Data, info.Size, 1, 2, 3);
	}

	mMappedFile.Close();
}

void PxPackageArchiver::Save(const char *inFileName)
{
	mPackage.Save(inFileName);

	for (const auto &info : mFiles)
	{
		if (info.Data)
		{
			free( info.Data );
		}
	}

	mFiles.clear();
}

PxPackage* PxPackageArchiver::GetPackage()
{
	return &mPackage;
}