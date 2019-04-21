
#include "PxPackageSerializer.hpp"
#include "PxMappedFile.hpp"

#include <cassert>

PxPackageSerializer::PxPackageSerializer()
	: mMappedFile(nullptr)
	, mDelegate(nullptr)
{ }

void PxPackageSerializer::SetMappedFile(PxMappedFile * inMappedFile)
{
	assert(inMappedFile);
	mMappedFile = inMappedFile;
}

void PxPackageSerializer::SetDelegate(PxPackageSerializerDelegate * inDelegate)
{
	assert(inDelegate);
	mDelegate = inDelegate;
}

bool PxPackageSerializer::Write(const PxPackageHeader & inHeader)
{
	mMappedFile->SeekTo(0);

	mMappedFile->Write(&inHeader.Version, sizeof(DWORD));
	mMappedFile->Write(&inHeader.Size, sizeof(DWORD));
	mMappedFile->Write(&inHeader.Type, sizeof(DWORD));
	mMappedFile->Write(&inHeader.CRC32, sizeof(DWORD));

	return true;
}

bool PxPackageSerializer::Read(PxPackageHeader & outHeader)
{
	mMappedFile->SeekTo(0);

	mMappedFile->Read(&outHeader.Version, sizeof(DWORD));
	mMappedFile->Read(&outHeader.Size, sizeof(DWORD));
	mMappedFile->Read(&outHeader.Type, sizeof(DWORD));
	mMappedFile->Read(&outHeader.CRC32, sizeof(DWORD));

	return true;
}

bool PxPackageSerializer::Write(const PxPackageFileFormats & inFormats)
{
	mMappedFile->Write(&inFormats.NumFormats, sizeof(DWORD));
	
	for (DWORD i = 0; i < inFormats.NumFormats; ++i)
	{
		DWORD formatLength = static_cast<DWORD>(strlen(inFormats.Formats[i]));
		mMappedFile->Write(&formatLength, sizeof(DWORD));
		mMappedFile->Write(inFormats.Formats[i], sizeof(char), formatLength);
	}

	return true;
}

bool PxPackageSerializer::Read(PxPackageFileFormats & outFormats)
{
	mMappedFile->Read(&outFormats.NumFormats, sizeof(DWORD));

	DWORD nameLength = 0;
	char name[256];
	
	for (DWORD i = 0; i < outFormats.NumFormats; ++i)
	{
		memset(name, 0, 256);

		mMappedFile->Read(&nameLength, sizeof(DWORD));
		mMappedFile->Read(name, sizeof(char), nameLength);

		outFormats.Formats[i] = new char[nameLength];
		strcpy(outFormats.Formats[i], name);
	}

	return true;
}

bool PxPackageSerializer::Write(const PxPackageFileFormats &inFormats, const PxPackageFileTable & inTable)
{
	// write num entries and num of all meta data
	mMappedFile->Write(&inTable.NumEntries, sizeof(DWORD));
	mMappedFile->Write(&inTable.NumMetaData, sizeof(DWORD));

	DWORD dataOffset = CalculateDataOffset(inFormats, inTable);

	for (DWORD i = 0; i < inTable.NumEntries; ++i)
	{
		PxPackageFileEntry *entry = inTable.Files[i];
		DWORD numFiles = 0;

		while (entry)
		{
			numFiles += 1;
			entry = entry->Next;
		}

		mMappedFile->Write(&numFiles, sizeof(DWORD));

		entry = inTable.Files[i];

		while (entry)
		{
			// meta data
			mMappedFile->Write(&entry->NumMetaData, sizeof(DWORD));
			
			for (DWORD j = 0; j < entry->NumMetaData; ++j)
			{
				WriteMetaData(&entry->MetaData[j]);
			}

			// common info
			mMappedFile->Write(&dataOffset, sizeof(DWORD));
			mMappedFile->Write(&entry->Size, sizeof(DWORD));
			mMappedFile->Write(&entry->Language, sizeof(DWORD));
			mMappedFile->Write(&entry->Type, sizeof(DWORD));
			mMappedFile->Write(&entry->Compressed, sizeof(DWORD));

			// name
			DWORD nameLength = strlen(entry->Name);
			mMappedFile->Write(&nameLength, sizeof(DWORD));
			mMappedFile->Write(entry->Name, sizeof(char), nameLength);

			if (entry->AddressInRAM != nullptr)
			{
				dataOffset += entry->Size;
			}
			
			entry = entry->Next;
		}
	}

	for (DWORD i = 0; i < inTable.NumEntries; ++i)
	{
		PxPackageFileEntry *entry = inTable.Files[i];

		while (entry)
		{
			void *bufferPtr = entry->AddressInRAM;

			if (bufferPtr)
			{
				mMappedFile->Write(bufferPtr, sizeof(BYTE), entry->Size);
			}
			
			entry = entry->Next;
		}
	}

	return true;
}

bool PxPackageSerializer::Read(PxPackageFileTable & outHeader)
{
	DWORD numEntries = 0;
	DWORD numMetaData = 0;

	mMappedFile->Read(&numEntries, sizeof(DWORD));
	mMappedFile->Read(&numMetaData, sizeof(DWORD));

	if (mDelegate)
	{
		mDelegate->AllocateHashMap(outHeader, numEntries);
		mDelegate->PreallocateMetaDataChunks(numMetaData);
	}

	for (DWORD i = 0; i < numEntries; ++i)
	{
		char name[256];
		DWORD numFiles = 0;

		mMappedFile->Read(&numFiles, sizeof(DWORD));

		if (numFiles > 0)
		{
			for (DWORD j = 0; j < numFiles; ++j)
			{
				PxPackageFileEntry *entry = new PxPackageFileEntry;
				PxPackageFileEntry *prev = nullptr;

				DWORD nameLength = 0;
				memset(name, 0, 256);

				mMappedFile->Read(&entry->NumMetaData, sizeof(DWORD));

				if (entry->NumMetaData > 0)
				{
					entry->MetaData = mDelegate->GetAllocatedMetaDataChunks(entry->NumMetaData);

					for (DWORD j = 0; j < entry->NumMetaData; ++j)
					{
						ReadMetaData(&entry->MetaData[j]);
					}
				}

				mMappedFile->Read(&entry->Offset, sizeof(DWORD));
				mMappedFile->Read(&entry->Size, sizeof(DWORD));
				mMappedFile->Read(&entry->Language, sizeof(DWORD));
				mMappedFile->Read(&entry->Type, sizeof(DWORD));
				mMappedFile->Read(&entry->Compressed, sizeof(DWORD));
				mMappedFile->Read(&nameLength, sizeof(DWORD));
				mMappedFile->Read(name, sizeof(char), nameLength);

				entry->Name = new char[nameLength];
				strcpy(entry->Name, name);
				
				entry->Next = nullptr;

				if (outHeader.Files[i] == nullptr)
				{
					outHeader.Files[i] = entry;
				}
				else
				{
					if (prev != nullptr)
					{
						prev->Next = entry;
					}
				}

				prev = entry;
			}
		}
	}

	return true;
}

DWORD PxPackageSerializer::CalculateDataOffset(const PxPackageFileFormats &inFormats, const PxPackageFileTable &inTable) const
{
	DWORD offset = 0;

	// header
	offset += sizeof(DWORD) * 4;					// header
	offset += sizeof(DWORD);						// num entries
	offset += sizeof(DWORD);						// num metadata

	// file formats
	offset += sizeof(DWORD);						// num formats
	for (size_t i = 0; i < inFormats.NumFormats; ++i)
	{
		offset += sizeof(DWORD);
		offset += strlen(inFormats.Formats[i]) * sizeof(char);
	}

	// file entries
	for (size_t i = 0; i < inTable.NumEntries; ++i)
	{
		offset += sizeof(DWORD);					// num files

		PxPackageFileEntry *entry = inTable.Files[i];

		while (entry)
		{
			offset += sizeof(DWORD); // num metadata

			for (size_t j = 0; j < entry->NumMetaData; ++j)
			{
				offset += sizeof(DWORD);	// meta type

				switch (entry->MetaData[j].Type) // meta value
				{
				case PxPackageFileMetaData::Boolean: offset += sizeof(bool); break;
				case PxPackageFileMetaData::Double: offset += sizeof(double); break;
				case PxPackageFileMetaData::Float: offset += sizeof(float); break;
				case PxPackageFileMetaData::Int: offset += sizeof(int); break;
				}

				offset += sizeof(DWORD); // name len
				offset += sizeof(char) * strlen(entry->MetaData[j].Name); // name
			}

			offset += sizeof(DWORD);				// offset
			offset += sizeof(DWORD);				// size
			offset += sizeof(DWORD);				// language
			offset += sizeof(DWORD);				// type
			offset += sizeof(DWORD);				// compressed
			offset += sizeof(DWORD);				// name length
			offset += sizeof(char) * strlen(entry->Name);	// name

			entry = entry->Next;
		}
	}

	return offset;
}

void PxPackageSerializer::WriteMetaData(PxPackageFileMetaData * inMetaData)
{
	mMappedFile->Write(&inMetaData->Type, sizeof(DWORD));

	switch (inMetaData->Type)
	{
	case PxPackageFileMetaData::Boolean:
		mMappedFile->Write(&inMetaData->Value.AsBoolean, sizeof(bool));
		break;
	case PxPackageFileMetaData::Double:
		mMappedFile->Write(&inMetaData->Value.AsDouble, sizeof(double));
		break;
	case PxPackageFileMetaData::Float:
		mMappedFile->Write(&inMetaData->Value.AsFloat, sizeof(float));
		break;
	case PxPackageFileMetaData::Int:
		mMappedFile->Write(&inMetaData->Value.AsInt, sizeof(int));
		break;
	}

	size_t nameLength = strlen(inMetaData->Name);
	mMappedFile->Write(&nameLength, sizeof(DWORD));
	mMappedFile->Write(inMetaData->Name, sizeof(char), nameLength);
}

void PxPackageSerializer::ReadMetaData(PxPackageFileMetaData * outMetaData)
{
	mMappedFile->Read(&outMetaData->Type, sizeof(DWORD));

	switch (outMetaData->Type)
	{
	case PxPackageFileMetaData::Boolean:
		mMappedFile->Read(&outMetaData->Value.AsBoolean, sizeof(bool));
		break;
	case PxPackageFileMetaData::Double:
		mMappedFile->Read(&outMetaData->Value.AsDouble, sizeof(double));
		break;
	case PxPackageFileMetaData::Float:
		mMappedFile->Read(&outMetaData->Value.AsFloat, sizeof(float));
		break;
	case PxPackageFileMetaData::Int:
		mMappedFile->Read(&outMetaData->Value.AsInt, sizeof(int));
		break;
	}

	DWORD nameLength = 0;

	char name[256];
	memset(name, 0, 256);

	mMappedFile->Read(&nameLength, sizeof(DWORD));
	mMappedFile->Read(name, sizeof(char), nameLength);

	outMetaData->Name = new char[nameLength];
	strcpy(outMetaData->Name, name);
}