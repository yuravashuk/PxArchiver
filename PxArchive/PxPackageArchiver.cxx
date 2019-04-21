
#include "PxPackageArchiver.hpp"
#include "PxPackageStructure.hpp"

void PxPackageArchiver::SetHashFunction(THashFunction inHashFunction)
{
	mPackage.SetHashFunction(inHashFunction);
}

void PxPackageArchiver::AddFile(const char * inFileName, const char * inFileDirection, const char * inFileFormat)
{
	if (mMappedFile.Open(inFileDirection))
	{
		size_t fileSize = mMappedFile.GetFileSize();

		FileInfo info;
		info.Data = malloc(fileSize);
		info.Size = fileSize;
		mFiles.push_back(info);

		memset(info.Data, 0, fileSize);

		mMappedFile.SeekTo(0);
		mMappedFile.Read(info.Data, info.Size);

		PxPackageInsertionDescriptor descriptor;
		descriptor.Address = info.Data;
		descriptor.FileFormat = inFileFormat;
		descriptor.FileName = inFileName;
		descriptor.IsCompressed = 0;
		descriptor.Language = 0;
		descriptor.NumBytes = static_cast<DWORD>(info.Size);

		mPackage.Insert(descriptor);
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