
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
		__int64 fileSize = mMappedFile.GetFileSize();
		size_t uFileSize = static_cast<size_t>(fileSize);

		if (fileSize > 0)
		{
			FileInfo info;
			info.Data = malloc(uFileSize);
			
			if (info.Data != nullptr)
			{
				info.Size = uFileSize;
				mFiles.push_back(info);

				memset(info.Data, 0, uFileSize);

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
		}
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