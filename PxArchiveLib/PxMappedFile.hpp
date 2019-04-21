
#pragma once

#ifndef _PX_MAPPED_FILE_HPP_
#define _PX_MAPPED_FILE_HPP_

#include <Windows.h>
#include <string>

enum class PxMappedFileMode
{
	CreateNew,
	CreateUpdate
};

class PxMappedFile final
{
public:
	PxMappedFile();

	bool Open(const std::string &inFileName, 
		PxMappedFileMode inMode = PxMappedFileMode::CreateUpdate,
		bool inOpenForMapping = true);
	void Close();

	bool IsOpened() const;

	void SeekTo(size_t inNumBytes);

	bool Read(void *outBuffer, size_t inNumBytes);
	bool ReadWithOffset(void *outBuffer, size_t inNumBytes, size_t inOffset);
	bool Read(void *outBuffer, size_t inElementSize, size_t inNumElements);
	
	bool Write(const void *inBuffer, size_t inBufferLength);
	bool WriteWithOffset(const void *inBuffer, size_t inBufferLength, size_t inOffset);
	bool Write(const void *inBuffer, size_t inElementSize, size_t inNumElements);

	void ShiftDataFromAddress(void* inStartPosition, size_t inOffset);

	size_t GetFileSize();

	LPVOID GetVirtualAddress(size_t inBufferLen = 0);

	template<typename T>
	T* GetVirtualAddressAs(size_t inBufferLen = 0)
	{
		return (T*)GetVirtualAddress(inBufferLen);
	}

private:
	OVERLAPPED mOverlapped;
	LPVOID mMappedAddress;
	HANDLE mFileHandle;
	HANDLE mMappedFileHandle;
};

#endif //_PX_MAPPED_FILE_HPP_