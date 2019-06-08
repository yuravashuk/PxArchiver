
#pragma once

#ifndef _PX_PACKAGE_SERIALIZER_HPP_
#define _PX_PACKAGE_SERIALIZER_HPP_

#include "PxPackageStructure.hpp"

/*!
 * Forward declaration of mapped file wrapper class
 * 
 */
class PxMappedFile;

/*!
 * Serialization delegate, used for memory management functionality
 * 
 */
struct PxPackageSerializerDelegate
{
	
	virtual ~PxPackageSerializerDelegate() = default;

	/*!
	 * Preallocate metadata chunks
	 * 
	 * \param inNumMetaData number of items
	 */
	virtual void PreallocateMetaDataChunks(DWORD inNumMetaData) = 0;

	/*!
	 * Allocate hash table for file entries
	 * 
	 * \param outTable hash table structure
	 * \param inNumEntries num entries for allocation
	 */
	virtual void AllocateHashMap(PxPackageFileTable &outTable, DWORD inNumEntries) = 0;

	/*!
	 * Get next free allocated meta data chunk
	 * 
	 * \return pointer to allocated chunk, if failure return nullptr
	 */
	virtual PxPackageFileMetaData* GetAllocatedMetaDataChunk() = 0;

	/*!
	 * Get pointer to allocated meta data chunks array
	 * 
	 * \param inNumMeta num of meta data
	 * \return pointer to allocated array, if failure return nullptr
	 */
	virtual PxPackageFileMetaData* GetAllocatedMetaDataChunks(DWORD inNumMeta) = 0;
};

/*!
 * Package Serializer, used to read and write data from file in appropriate order.
 * 
 */
class PxPackageSerializer
{
public:
	/*!
	 * Default constructor
	 * 
	 */
	PxPackageSerializer();

	/*!
	 * Set pointer to mapped file
	 * 
	 * \param inMappedFile mapped file pointer
	 */
	void SetMappedFile(PxMappedFile *inMappedFile);

	/*!
	 * Set delegate
	 * 
	 * \param inDelegate pointer to serializer delegate
	 */
	void SetDelegate(PxPackageSerializerDelegate *inDelegate);

	/*!
	 * Write package header to file
	 * 
	 * \param inHeader package header
	 * \return 
	 */
	bool Write(const PxPackageHeader &inHeader);

	/*!
	 * Write list of file formats
	 * 
	 * \param inFormats list of file extensions
	 * \return 
	 */
	bool Write(const PxPackageFileFormats &inFormats);

	/*!
	 * Write files hash table and files data
	 * 
	 * \param inFormats file formats used to calculate relative file offsets
	 * \param inTable files hash table
	 * \return 
	 */
	bool Write(const PxPackageFileFormats &inFormats,
		const PxPackageFileTable &inTable);

	/*!
	 * Read header from mapped file
	 * 
	 * \param outHeader
	 * \return 
	 */
	bool Read(PxPackageHeader &outHeader);

	/*!
	 * Read list of file extensions from mapped file
	 * 
	 * \param outFormats
	 * \return 
	 */
	bool Read(PxPackageFileFormats &outFormats);

	/*!
	 * Read files hash table from mapped file
	 * 
	 * \param outTable
	 * \return 
	 */
	bool Read(PxPackageFileTable &outTable);

	/*!
	 * Calculate package info offset
	 * 
	 * \param inFormats list of files extensions
	 * \param inTable files hash table
	 * \return 
	 */
	DWORD CalculateDataOffset(const PxPackageFileFormats &inFormats, 
		const PxPackageFileTable &inTable) const;

private:
	/*!
	 * Write meta data to mapped file
	 * 
	 * \param inMetaData
	 */
	void WriteMetaData(PxPackageFileMetaData *inMetaData);

	/*!
	 * Read meta data from mapped file
	 * 
	 * \param outMetaData
	 */
	void ReadMetaData(PxPackageFileMetaData *outMetaData);

private:
	PxMappedFile *mMappedFile;
	PxPackageSerializerDelegate *mDelegate;
};

#endif //_PX_PACKAGE_SERIALIZER_HPP_