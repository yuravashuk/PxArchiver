
#include "PxMappedFile.hpp"

PxMappedFile::PxMappedFile()
	: mFileHandle(INVALID_HANDLE_VALUE)
	, mMappedAddress(nullptr)
	, mMappedFileHandle(NULL)
	, mOverlapped{0}
{ }

bool PxMappedFile::Open(const std::string &inFileName, PxMappedFileMode inMode, bool inOpenForMapping)
{
	if (IsOpened())
	{
		Close();
		SeekTo(0);
	}

	auto flags = OPEN_ALWAYS;

	switch (inMode)
	{
	case PxMappedFileMode::CreateNew:
		flags = CREATE_NEW;
		break;
	case PxMappedFileMode::CreateUpdate:
		flags = OPEN_ALWAYS;
		break;
	}

	LPCSTR name = inFileName.c_str();
	mFileHandle = ::CreateFileA(name,
		GENERIC_READ | GENERIC_WRITE,
		FILE_SHARE_READ,
		NULL,
		flags,
		FILE_ATTRIBUTE_NORMAL,
		NULL);

	if (mFileHandle == INVALID_HANDLE_VALUE || mFileHandle == NULL)
	{
		return false;
	}

	if (inOpenForMapping)
	{
		mMappedFileHandle = ::CreateFileMappingA(
			mFileHandle,
			NULL,
			PAGE_READWRITE,
			0,
			0,
			"PxMappedResource"
		);

		if (mMappedFileHandle == NULL)
		{
			return false;
		}
	}
	
	return true;
}

void PxMappedFile::Close()
{
	if (IsOpened())
	{
		::UnmapViewOfFile(mMappedFileHandle);
		mMappedFileHandle = NULL;

		::CloseHandle(mFileHandle);
		mFileHandle = INVALID_HANDLE_VALUE;
	}
}

bool PxMappedFile::IsOpened() const
{
	return mFileHandle != INVALID_HANDLE_VALUE &&
		mFileHandle != NULL &&
		mMappedFileHandle != NULL;
}

void PxMappedFile::SeekTo(size_t inNumBytes)
{
	mOverlapped = { 0 };
}

size_t PxMappedFile::GetFileSize()
{
	LARGE_INTEGER fileSize;
	::GetFileSizeEx(mFileHandle, &fileSize);
	return fileSize.QuadPart;
}

bool PxMappedFile::Read(void *outBuffer, size_t inNumBytes)
{
	BOOL result = ::ReadFile(mFileHandle, outBuffer, inNumBytes, NULL, &mOverlapped);
	DWORD error = ::GetLastError();

	mOverlapped.Offset += inNumBytes;

	if (result && (error == ERROR_IO_PENDING))
	{
		::WaitForSingleObject(mFileHandle, INFINITE);
		result = TRUE;
	}

	return result == TRUE;
}

bool PxMappedFile::ReadWithOffset(void *outBuffer, size_t inNumBytes, size_t inOffset)
{
	SeekTo(inOffset);
	return Read(outBuffer, inNumBytes);
}

bool PxMappedFile::Read(void *outBuffer,
	size_t inElementSize, 
	size_t inNumElements)
{
	size_t bufferSize = inElementSize * inNumElements;
	return Read(outBuffer, bufferSize);
}

bool PxMappedFile::Write(const void *inBuffer, size_t inBufferLength)
{
	DWORD writtenBytes = 0;

	while (writtenBytes != static_cast<DWORD>(inBufferLength))
	{
		BOOL result = ::WriteFile(mFileHandle, inBuffer, inBufferLength, &writtenBytes, &mOverlapped);
		DWORD error = ::GetLastError();

		if (error == ERROR_INVALID_USER_BUFFER)
		{
			writtenBytes = static_cast<DWORD>(inBufferLength);
		}

		if (result && (error == ERROR_IO_PENDING))
		{
			::WaitForSingleObject(mFileHandle, INFINITE);
			result = TRUE;
		}
	}
	
	mOverlapped.Offset += writtenBytes;

	return true;
}

bool PxMappedFile::WriteWithOffset(const void *inBuffer, 
	size_t inBufferLength, 
	size_t inOffset)
{
	SeekTo(inOffset);
	return Write(inBuffer, inBufferLength);
}

bool PxMappedFile::Write(const void *inBuffer, size_t inElementSize, size_t inNumElements)
{
	size_t bufferSize = inElementSize * inNumElements;
	return Write(inBuffer, bufferSize);
}

LPVOID PxMappedFile::GetVirtualAddress(size_t inBufferLen)
{
	if (mMappedFileHandle != NULL)
	{
		if (mMappedAddress == nullptr)
		{
			mMappedAddress = ::MapViewOfFile(
				mMappedFileHandle,
				FILE_MAP_READ | FILE_MAP_WRITE,
				0,
				0,
				inBufferLen);
		}
		
		return mMappedAddress;
	}

	return NULL;
}

void PxMappedFile::ShiftDataFromAddress(void* inStartPosition, size_t inOffset)
{
	auto ptr = (char*)inStartPosition;
	
	while (ptr)
	{
		*(ptr - inOffset) = *ptr;
		ptr++;
	}
}