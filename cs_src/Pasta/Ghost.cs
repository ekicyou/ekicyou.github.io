using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace Pasta
{
    public class Ghost : MarshalByRefObject, IDisposable
    {
        private static readonly NLog.Logger logger = NLog.LogManager.GetCurrentClassLogger();

        public void Dispose()
        {
            logger.Trace("[Dispose]");
        }

        public bool unload()
        {
            logger.Trace("[unload]");
            throw new NotImplementedException();
        }
        public bool load(string loaddir)
        {
            logger.Trace("[load]");
            throw new NotImplementedException();
        }
        public bool request(string req, out string res)
        {
            logger.Trace("[request]");
            throw new NotImplementedException();
        }

    }
}
