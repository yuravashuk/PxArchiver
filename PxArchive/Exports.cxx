
#include "Exports.hpp"
#include "PxPackageArchiver.hpp"

#include <cassert>

static PxPackageArchiver *g_Archiver = nullptr;
static PxPackage *g_Package = nullptr;

PXARCHIVE_API void CreateArchiver(int inVersion)
{
	assert(g_Archiver == nullptr);

	g_Archiver = new PxPackageArchiver();
	g_Archiver->GetPackage()->SetVersion(inVersion);
	g_Archiver->SetHashFunction(kPxJenkinsHashFunction);
}

PXARCHIVE_API void DestroyArchiver()
{
	assert(g_Archiver);

	delete g_Archiver;
	g_Archiver = nullptr;

	if (g_Package)
	{
		delete g_Package;
		g_Package = nullptr;
	}
}

PXARCHIVE_API void AddFile(const char * inFileName, const char * inName, const char *inFileFormat)
{
	g_Archiver->AddFile(inName, inFileName, inFileFormat);
}

PXARCHIVE_API void Save(const char * inFileName)
{
	g_Archiver->Save(inFileName);
}

PXARCHIVE_API void SetCompressed(const char * inFileName, int inCompressed)
{
	auto package = g_Archiver->GetPackage();
	auto info = package->GetFileInfo(inFileName);

	if (info)
	{
		info->Compressed = inCompressed;
	}
}

PXARCHIVE_API void SetLanguage(const char * inFileName, int inLanguage)
{
	auto package = g_Archiver->GetPackage();
	auto info = package->GetFileInfo(inFileName);

	if (info)
	{
		info->Language = inLanguage;
	}
}

PXARCHIVE_API void SetFileType(const char * inFileName, int inType)
{
	auto package = g_Archiver->GetPackage();
	auto info = package->GetFileInfo(inFileName);

	if (info)
	{
		info->Type = inType;
	}
}

extern "C" PXARCHIVE_API int GetTypeFile(const char *inFileName)
{
	auto package = g_Archiver->GetPackage();
	auto info = package->GetFileInfo(inFileName);

	if (info)
	{
		return info->Type;
	}

	return -1;
}

extern "C" PXARCHIVE_API void Open(const char *inFileName)
{
	if (g_Package == nullptr)
	{
		g_Package = new PxPackage();
		g_Package->SetHashFunction(kPxJenkinsHashFunction);
	}

	g_Package->Open(inFileName);
}

extern "C" PXARCHIVE_API void Export(const char *inOutputDir)
{
	if (g_Package)
	{
		std::vector<PxPackageFileEntry*> fileInfos = g_Package->GetAllFileInfos();
		PxMappedFile outputFile;

		for (const auto file : fileInfos)
		{
			const auto extension = g_Package->GetFileExtension(file->Type);
			const auto path = std::string(inOutputDir) + "\\" + std::string(file->Name) + std::string(extension);

			if (outputFile.Open(path, PxMappedFileMode::CreateUpdate, false))
			{
				auto filePtr = g_Package->GetFileVirtualAddressAs<BYTE>(file->Name);

				outputFile.SeekTo(0);
				outputFile.Write(filePtr, sizeof(BYTE), file->Size);
				outputFile.Close();
			}
		}
	}
}