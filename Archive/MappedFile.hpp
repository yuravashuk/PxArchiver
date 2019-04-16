
#pragma once

#ifndef _MAPPED_FILE_HPP_
#define _MAPPED_FILE_HPP_

#include <Windows.h>
#include <string>

class MappedFile final
{
public:
	MappedFile()
		: mFileHandle(INVALID_HANDLE_VALUE)
		, mMappingHandle(INVALID_HANDLE_VALUE)
		, mOverlapped{0}
	{ }

	bool Open(const std::string &inFileName)
	{
		LPCSTR name = inFileName.c_str();
		mFileHandle = ::CreateFileA(name,
			GENERIC_READ | GENERIC_WRITE,
			FILE_SHARE_READ,
			NULL, 
			OPEN_EXISTING, 
			FILE_ATTRIBUTE_NORMAL,
			NULL);

		if (mFileHandle == INVALID_HANDLE_VALUE || mFileHandle == NULL)
		{
			return false;
		}

		mMappingHandle = ::CreateFileMappingA(
			mFileHandle,
			NULL,
			PAGE_READWRITE,
			0,
			0,
			name
		);

		if (mMappingHandle == INVALID_HANDLE_VALUE || mMappingHandle == NULL)
		{
			return false;
		}

		return true;
	}

	void Close()
	{
		if (mMappingHandle != INVALID_HANDLE_VALUE || mMappingHandle != NULL)
		{
			::UnmapViewOfFile(mMappingHandle);
			mMappingHandle = INVALID_HANDLE_VALUE;
		}

		if (mFileHandle != INVALID_HANDLE_VALUE)
		{
			::CloseHandle(mFileHandle);
			mFileHandle = INVALID_HANDLE_VALUE;
		}
	}

	bool Read(BYTE *outBuffer, size_t inBufferSize)
	{
		BOOL result = ::ReadFile(mFileHandle, outBuffer, inBufferSize, NULL, &mOverlapped);
		DWORD error = ::GetLastError();

		mOverlapped.Offset += inBufferSize;

		if (result && (error == ERROR_IO_PENDING))
		{
			::WaitForSingleObject(mFileHandle, INFINITE);
			result = TRUE;
		}

		return result == TRUE;
	}

	LPVOID GetMappedAddress()
	{
		if (mMappingHandle != INVALID_HANDLE_VALUE || mMappingHandle != NULL)
		{
			return ::MapViewOfFile(mMappingHandle,
				FILE_MAP_READ | FILE_MAP_WRITE,
				0,
				0,
				0);
		}

		return NULL;
	}

	template<typename T>
	T* GetMappedAddressAs()
	{
		return (T*)GetMappedAddress();
	}

private:
	OVERLAPPED mOverlapped;
	HANDLE mFileHandle;
	HANDLE mMappingHandle;
};

#endif //_MAPPED_FILE_HPP_