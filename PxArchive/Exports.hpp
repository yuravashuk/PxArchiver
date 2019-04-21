
#pragma once

#ifndef _EXPORTS_HPP_
#define _EXPORTS_HPP_

#ifdef PXARCHIVE_EXPORTS
#define PXARCHIVE_API __declspec(dllexport)
#else
#define PXARCHIVE_API __declspec(dllimport)
#endif

extern "C" PXARCHIVE_API void CreateArchiver(int inVersion);
extern "C" PXARCHIVE_API void DestroyArchiver();

extern "C" PXARCHIVE_API void AddFile(const char *inFileName, const char *inName, const char *inFileFormat);
extern "C" PXARCHIVE_API void Save(const char *inFileName);

extern "C" PXARCHIVE_API void SetCompressed(const char *inFileName, int inCompressed);
extern "C" PXARCHIVE_API void SetLanguage(const char *inFileName, int inLanguage);
extern "C" PXARCHIVE_API void SetFileType(const char *inFileName, int inType);

extern "C" PXARCHIVE_API int GetTypeFile(const char *inFileName);

extern "C" PXARCHIVE_API void Open(const char *inFileName);
extern "C" PXARCHIVE_API void Export(const char *inOutputDir);

#endif //_EXPORTS_HPP_