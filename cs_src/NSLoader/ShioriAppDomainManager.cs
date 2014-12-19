using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime;
using System.Runtime.InteropServices;

namespace ShioriLoader
{
    [Guid("233CD954-86BB-4984-A2A2-C4BFA7F9C14D")]
    [ComVisible(true)]
    public sealed class ShioriAppDomainManager : AppDomainManager, IShiori1
    {
        public bool unload()
        {
            throw new NotImplementedException();
        }

        public bool load(string loaddir)
        {
            throw new NotImplementedException();
        }

        public bool request(string req, out string res)
        {
            throw new NotImplementedException();
        }
    }
}
