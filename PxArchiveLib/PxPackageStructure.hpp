
#pragma once

#ifndef _PX_PACKAGE_STRUCTURE_HPP_
#define _PX_PACKAGE_STRUCTURE_HPP_

#include <Windows.h>
#include <functional>

/*!
 * Package header structure
 * 
 */
struct PxPackageHeader
{
	DWORD Version;	// file version
	DWORD Size;		// file size
	DWORD Type;		// file type
	DWORD CRC32;	// file hash sum
};

/*!
 * Metadata entry structure
 * 
 */
struct PxPackageFileMetaData
{
	enum
	{
		Double = 0, 
		Float = 1, 
		Int = 2,
		Boolean = 3
	} Type;

	union
	{
		double AsDouble;
		float AsFloat;
		int AsInt;
		bool AsBoolean;
	} Value;

	char *Name;
};

/*!
 * File info structure
 * 
 */
struct PxPackageFileEntry
{
	DWORD NumMetaData;					// num meta data in array
	PxPackageFileMetaData *MetaData;	// array of meta data

	DWORD Offset;						// offset in file
	DWORD Size;							// size of file
	DWORD Language;						// file language
	DWORD Type;							// file type
	DWORD Compressed;					// file compression flag
	char *Name;							// file name
	PxPackageFileEntry *Next;			// ptr to next file

	void* AddressInRAM;					// temporary file address, used only in archivation mode
};

/*!
 * File formats table structure
 * 
 */
struct PxPackageFileFormats
{
	DWORD NumFormats;					// num formats
	char* Formats[256];					// formats
};

/*!
 * File hash table structure
 * 
 */
struct PxPackageFileTable
{
	DWORD NumEntries;					// num entries in hash table (a.k.a hash table size)
	DWORD NumMetaData;					// num metadata, used to preallocate linear memory chunk for all properties
	PxPackageFileEntry **Files;			// hash table
};

/*!
 * Meta data utils
 * 
 */
namespace PxMetaUtils
{
	template<typename T>
	static inline PxPackageFileMetaData* AllocMeta(const char *inName, int inType, const T &inValue)
	{
		PxPackageFileMetaData *meta = new PxPackageFileMetaData();

		switch (inType)
		{
		case PxPackageFileMetaData::Boolean:
			meta->Type = PxPackageFileMetaData::Boolean;
			meta->Value.AsBoolean = inValue;
			break;
		case PxPackageFileMetaData::Double:
			meta->Type = PxPackageFileMetaData::Double;
			meta->Value.AsDouble = inValue;
			break;
		case PxPackageFileMetaData::Float:
			meta->Type = PxPackageFileMetaData::Float;
			meta->Value.AsFloat = inValue;
			break;
		case PxPackageFileMetaData::Int:
			meta->Type = PxPackageFileMetaData::Int;
			meta->Value.AsInt = inValue;
			break;
		}

		meta->Name = new char[strlen(inName)];
		strcpy(meta->Name, inName);

		return meta;
	}

	template<typename T>
	static inline void MakeMeta(PxPackageFileMetaData *inMeta, const char *inName, T inValue)
	{
		inMeta->Name = new char[strlen(inName)];
		strcpy(inMeta->Name, inName);
	}

	template<>
	static inline void MakeMeta<float>(PxPackageFileMetaData *inMeta, const char *inName, float inValue)
	{
		inMeta->Type = PxPackageFileMetaData::Float;
		inMeta->Value.AsFloat = inValue;
		inMeta->Name = new char[strlen(inName)];
		strcpy(inMeta->Name, inName);
	}

	template<>
	static inline void MakeMeta<double>(PxPackageFileMetaData *inMeta, const char *inName, double inValue)
	{
		inMeta->Type = PxPackageFileMetaData::Double;
		inMeta->Value.AsDouble = inValue;
		inMeta->Name = new char[strlen(inName)];
		strcpy(inMeta->Name, inName);
	}

	template<>
	static inline void MakeMeta<int>(PxPackageFileMetaData *inMeta, const char *inName, int inValue)
	{
		inMeta->Type = PxPackageFileMetaData::Int;
		inMeta->Value.AsInt = inValue;
		inMeta->Name = new char[strlen(inName)];
		strcpy(inMeta->Name, inName);
	}

	template<>
	static inline void MakeMeta<bool>(PxPackageFileMetaData *inMeta, const char *inName, bool inValue)
	{
		inMeta->Type = PxPackageFileMetaData::Boolean;
		inMeta->Value.AsBoolean = inValue;
		inMeta->Name = new char[strlen(inName)];
		strcpy(inMeta->Name, inName);
	}
}

using THashFunction = std::function<DWORD(const char *)>;

const auto kPxJenkinsHashFunction = [](const char *inName) {
	size_t index = 0;
	size_t length = strlen(inName);

	DWORD hash = 0;

	while (index != length)
	{
		hash += inName[index++];
		hash += hash << 10;
		hash ^= hash >> 6;
	}

	hash += hash << 3;
	hash ^= hash >> 11;
	hash += hash << 15;

	return hash;
};

#endif //_PX_PACKAGE_STRUCTURE_HPP_