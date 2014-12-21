using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace NShiori
{
    /// <summary>
    /// SHIORIアセンブリの初回初期化処理
    /// </summary>
    public interface IShioriAddDomainInitialize
    {
        bool shioriAddDomainLoad(string loaddir);
    }
}
