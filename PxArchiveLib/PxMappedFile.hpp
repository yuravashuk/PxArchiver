
#pragma once

#ifndef _PX_MAPPED_FILE_HPP_
#define _PX_MAPPED_FILE_HPP_

#include <Windows.h>
#include <string>

/*!
 * File open mode
 * 
 */
enum class PxMappedFileMode
{
	CreateNew,
	CreateUpdate
};

/*!
 * Mapped file wrapper class
 * 
 */
class PxMappedFile final
{
public:
	/*!
	 * Default ctor
	 * 
	 */
	PxMappedFile();

	/*!
	 * Open for reading or writing file on disk
	 * 
	 * \param inFileName path to file
	 * \param inMode open mode
	 * \param inOpenForMapping create file mapping flag
	 * \return 
	 */
	bool Open(const std::string &inFileName, 
		PxMappedFileMode inMode = PxMappedFileMode::CreateUpdate,
		bool inOpenForMapping = true);

	/*!
	 * Close file and mapping
	 * 
	 */
	void Close();

	/*!
	 * Check if file is oppened
	 * 
	 * \return 
	 */
	bool IsOpened() const;

	/*!
	 * Seek cursor to specific position
	 * 
	 * \param inNumBytes
	 */
	void SeekTo(size_t inNumBytes);

	/*!
	 * Read bytes to generic buffer
	 * 
	 * \param outBuffer generic buffer
	 * \param inNumBytes buffer length
	 * \return true if successed
	 */
	bool Read(void *outBuffer, size_t inNumBytes);
	
	/*!
	 * Read bytes to generic buffer from specific cursor position
	 * 
	 * \param outBuffer generic buffer
	 * \param inNumBytes buffer length
	 * \param inOffset cursor position
	 * \return true if successed
	 */
	bool ReadWithOffset(void *outBuffer, size_t inNumBytes, size_t inOffset);

	/*!
	 * Read bytes to generic buffer
	 * 
	 * \param outBuffer generic buffer
	 * \param inElementSize size of element
	 * \param inNumElements num of elements
	 * \return true if successed
	 */
	bool Read(void *outBuffer, size_t inElementSize, size_t inNumElements);
	
	/*!
	 * Write from generic buffer to file
	 * 
	 * \param inBuffer generic buffer
	 * \param inBufferLength buffer length in bytes
	 * \return true if successed
	 */
	bool Write(const void *inBuffer, size_t inBufferLength);

	/*!
	 * Write from generic buffer to file at specific location
	 * 
	 * \param inBuffer generic buffer
	 * \param inBufferLength buffer length in bytes
	 * \param inOffset cursor position
	 * \return true if successed
	 */
	bool WriteWithOffset(const void *inBuffer, size_t inBufferLength, size_t inOffset);
	
	/*!
	 * Write from generic buffer to file
	 * 
	 * \param inBuffer generic buffer
	 * \param inElementSize size of element
	 * \param inNumElements num of elements
	 * \return true if successed
	 */
	bool Write(const void *inBuffer, size_t inElementSize, size_t inNumElements);

	/*!
	 * Get file length
	 * 
	 * \return length of opened file
	 */
	size_t GetFileSize();

	/*!
	 * Get base pointer to mapped file
	 * 
	 * \param inBufferLen length of mapping buffer (default unspecified)
	 * \return pointer to mapped file
	 */
	LPVOID GetVirtualAddress(size_t inBufferLen = 0);

	/*!
	 * Template version of GetVirtualAddress function
	 * 
	 * \param inBufferLen
	 * \return 
	 */
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