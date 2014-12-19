using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime;
using System.Runtime.InteropServices;
using System.Diagnostics;

namespace NShiori
{
    [Guid("233CD954-86BB-4984-A2A2-C4BFA7F9C14D")]
    [ComVisible(true)]
    public sealed class ShioriAppDomainManager : AppDomainManager, IShiori1
    {
        public bool unload()
        {
            Debug.WriteLine("[unload] 終了");
            return true;
        }

        public bool load(string loaddir)
        {
            Debug.WriteLine("[load] loaddir=[{0}]", loaddir);
            throw new NotImplementedException();
        }

        public bool request(string req, out string res)
        {
            Debug.WriteLine("[request] 未実装...");
            res = null;
            return true;
        }

    }
}
