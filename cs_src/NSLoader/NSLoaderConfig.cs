using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.Serialization;

namespace NShiori
{
    /// <summary>
    /// NSLoader.jsonのパラメータ
    /// </summary>
    [DataContract]
    public class NSLoaderConfig
    {
        /// <summary>.net SHIORIのアセンブリ名(拡張子含まず)</summary>
        [DataMember]
        public string ShioriAssemblyName { get; set; }

        /// <summary>.net SHIORIのアセンブリ名(拡張子含まず)</summary>
        [DataMember]
        public string ShioriTypeName { get; set; }

        /// <summary>【省略可能】ターゲットフレームワーク。</summary>
        [DataMember]
        public string TargetFrameworkName { get; set; }

    }
}
