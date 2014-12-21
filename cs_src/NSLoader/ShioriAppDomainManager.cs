using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime;
using System.Runtime.InteropServices;
using System.Diagnostics;
using System.Security.Policy;

namespace NShiori
{
    [Guid("233CD954-86BB-4984-A2A2-C4BFA7F9C14D")]
    [ComVisible(true)]
    public sealed class ShioriAppDomainManager : AppDomainManager, IShiori1
    {
        #region AppDomainManager override

        static ShioriAppDomainManager()
        {
            Debug.WriteLine("[static constructor]");
        }


        public ShioriAppDomainManager()
        {
            Debug.WriteLine("[constructor]");
        }

        public override void InitializeNewDomain(AppDomainSetup appDomainInfo)
        {
            Debug.WriteLine("[InitializeNewDomain] start");
            this.InitializationFlags = AppDomainManagerInitializationOptions.RegisterWithHost;
        }

        public override AppDomain CreateDomain(string friendlyName, Evidence securityInfo, AppDomainSetup appDomainInfo)
        {
            var appDomain = base.CreateDomain(friendlyName, securityInfo, appDomainInfo);
            System.Console.WriteLine("*** Created AppDomain {0}", friendlyName);
            return appDomain;
        }


        #endregion
        #region IShiori1

        public bool unload()
        {
            Debug.WriteLine("[unload] 終了");
            return true;
        }

        public bool load(string loaddir)
        {
            Debug.WriteLine("[load] loaddir=[{0}]", loaddir);
            return true;
        }

        public bool request(string req, out string res)
        {
            Debug.WriteLine("[request] 未実装...");
            res = null;
            return true;
        }


        #endregion
    }
}
