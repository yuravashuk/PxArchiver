
#include "pch.h"

#include <stdio.h>
#include <iostream>
#include <cassert>

#include "PxPackageArchiver.hpp"

const std::string kRootDir = "E:\\Images\\";
const std::string kFileNames[] =
{
	"0be091e482e0a3724f2fcaac5729aadf",
	"0cf8bd5d840d3dfdce19a63cf7574e53",
	"0f7c763a50cad9706fed969c29846d5e",
	"anna-soldatova-ton-line-soldatova-anna",
	"4117",
	"09a8d0270862a2c84daa750500c2435d",
	"184fc0724d463092ccdbc767256d0e35",
	"247c21b8a7da12eca2da68a5a01bf29c"
};

void TestPack()
{
	PxPackageArchiver archiver;
	archiver.SetHashFunction(kPxJenkinsHashFunction);

	for (size_t i = 0; i < _countof(kFileNames); ++i)
	{
		const auto fileName = kFileNames[i];
		archiver.AddFile(fileName.c_str(), std::string(kRootDir + fileName + ".jpg").c_str());
	} 

	auto package = archiver.GetPackage();
	auto meta = package->AllocateParameters(kFileNames[0].c_str(), 2);
	if (meta)
	{
		MakeMeta(&meta[0], "param0", 11.0f);
		MakeMeta(&meta[1], "param2", 5);
	}

	archiver.Save("output.bin");
}

void TestOpen()
{
	PxPackage package;

	package.SetHashFunction(kPxJenkinsHashFunction);

	if (!package.Open("output.bin"))
	{
		return;
	}

	std::vector<PxPackageFileEntry*> fileInfos;

	for (size_t i = 0; i < _countof(kFileNames); ++i)
	{
		const auto fileName = kFileNames[i];
		fileInfos.push_back
		(
			package.GetFileInfo(fileName.c_str()) 
		);
	}

	PxMappedFile outputFile;
	const std::string outputDir = "E:\\E\\";

	for (const auto &file : fileInfos)
	{
		for (DWORD i = 0; i < file->NumMetaData; ++i)
		{
			std::cout << file->MetaData[i].Name << " " << file->MetaData[i].Value.AsFloat << "\n";
		}

		const auto path = outputDir + file->Name + ".jpg";

		if (outputFile.Open(path))
		{	
			auto filePtr = package.GetFileVirtualAddressAs<unsigned char>() + file->Offset;
			outputFile.SeekTo(0);
			outputFile.Write(filePtr, sizeof(unsigned char), file->Size);
			outputFile.Close();
		}
	}
}

int main()
{
	if (true)
		TestOpen();
	else
		TestPack();

	return 0;
}
