
using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.InteropServices;
using System.Text;

namespace PxArchiverApp
{
    class ArchiverProvider
    {
        [DllImport("PXARCHIVE.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void CreateArchiver(int inVersion);

        [DllImport("PXARCHIVE.dll", CallingConvention = CallingConvention.Cdecl)]
        public static extern void DestroyArchiver();

        [DllImport("PXARCHIVE.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Save(
            [MarshalAs(UnmanagedType.LPStr)] string inFileName);

        [DllImport("PXARCHIVE.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void AddFile(
            [MarshalAs(UnmanagedType.LPStr)] string inFileName,
            [MarshalAs(UnmanagedType.LPStr)] string inName,
            [MarshalAs(UnmanagedType.LPStr)] string inFileFormat);

        [DllImport("PXARCHIVE.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetCompressed(
            [MarshalAs(UnmanagedType.LPStr)] string inFileName,
            int inCompressed);

        [DllImport("PXARCHIVE.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetLanguage(
            [MarshalAs(UnmanagedType.LPStr)] string inFileName,
            int inLanguage);

        [DllImport("PXARCHIVE.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void SetFileType(
            [MarshalAs(UnmanagedType.LPStr)] string inFileName,
            int inType);
        
        [DllImport("PXARCHIVE.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern int GetTypeFile(
            [MarshalAs(UnmanagedType.LPStr)] string inFileName);

        [DllImport("PXARCHIVE.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Open(
            [MarshalAs(UnmanagedType.LPStr)] string inFileName);

        [DllImport("PXARCHIVE.dll", CharSet = CharSet.Ansi, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Export(
            [MarshalAs(UnmanagedType.LPStr)] string inOutputDir);

    }
}
