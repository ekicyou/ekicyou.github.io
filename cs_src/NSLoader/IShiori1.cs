using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace NShiori
{
    /// <summary>
    /// ネイティブload/unload/requestを処理するための低レベルインターフェース
    /// </summary>
    [Guid("BC6068DA-EA2B-4A49-A2DE-7DE8F5355EF2")]
    [ComVisible(true)]
    public interface IShiori1
    {
        /// <summary>
        /// SHIORIアセンブリにloadコマンドを発行します。
        /// </summary>
        /// <param name="loaddir"></param>
        /// <returns></returns>
        bool load(string loaddir);

        /// <summary>
        /// SHIORIアセンブリにunloadコマンドを発行します。
        /// </summary>
        /// <returns></returns>       
        bool unload();

        /// <summary>
        /// SHIORIアセンブリにrequestコマンドを発行します。
        /// </summary>
        /// <param name="req"></param>
        /// <param name="res"></param>
        /// <returns></returns>
        bool request(string req, out string res);
    }
}
