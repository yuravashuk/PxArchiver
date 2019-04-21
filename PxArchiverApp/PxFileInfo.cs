
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace PxArchiverApp
{
    public enum MetaType
    {
        Double,
        Float,
        Int,
        Boolean
    }

    public class PxFileMeta
    {
        public string name;
        public MetaType type;
        public double doubleValue;
        public float floatValue;
        public int intValue;
        public bool boolValue;
    }

    public class PxFileInfo
    {
        public int language;
        public int type;
        public int compressed;
        public string filePath;
        public List<PxFileMeta> metaData;
    }
}
