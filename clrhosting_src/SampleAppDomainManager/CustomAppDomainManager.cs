// ----------------------------------------------------------------------------------------------
// Copyright (c) Mattias Högström.
// ----------------------------------------------------------------------------------------------
// This source code is subject to terms and conditions of the Microsoft Public License. A 
// copy of the license can be found in the License.html file at the root of this distribution. 
// If you cannot locate the Microsoft Public License, please send an email to 
// dlr@microsoft.com. By using this source code in any fashion, you are agreeing to be bound 
// by the terms of the Microsoft Public License.
// ----------------------------------------------------------------------------------------------
// You must not remove this notice, or any other, from this software.
// ----------------------------------------------------------------------------------------------

using System;
using System.Security.Policy;
using System.Runtime.InteropServices;

namespace SampleAppDomainManager
{
   [GuidAttribute("0C19678A-CE6C-487B-AD36-0A8B7D7CC035"), ComVisible(true)]
   public sealed class CustomAppDomainManager : AppDomainManager, ICustomAppDomainManager
   {
      public CustomAppDomainManager()
      {
         System.Console.WriteLine("*** Instantiated CustomAppDomainManager");
      }

      public override void InitializeNewDomain(AppDomainSetup appDomainInfo)
      {
         System.Console.WriteLine("*** InitializeNewDomain");
         this.InitializationFlags = AppDomainManagerInitializationOptions.RegisterWithHost;
      }

      public override AppDomain CreateDomain(string friendlyName, Evidence securityInfo, AppDomainSetup appDomainInfo)
      {
         var appDomain = base.CreateDomain(friendlyName, securityInfo, appDomainInfo);
         System.Console.WriteLine("*** Created AppDomain {0}", friendlyName);
         return appDomain;
      }

      public void Run(string assemblyFilename, string friendlyName)
      {
         if (!System.IO.File.Exists(assemblyFilename))
         {
            const string message = "Application cannot be found";
            System.Diagnostics.Trace.WriteLine(message);
            System.Console.Error.WriteLine(message);
            return;
         }

         AppDomain ad = null;
         try
         {
            ad = AppDomain.CreateDomain(friendlyName);
            int exitCode = ad.ExecuteAssembly(assemblyFilename);
            System.Diagnostics.Trace.WriteLine(string.Format("ExitCode={0}", exitCode));
         }
         catch (System.Exception)
         {
            string message = string.Format("Unhandled Exception in {0}",
                                           System.IO.Path.GetFileNameWithoutExtension(assemblyFilename));
            System.Console.Error.WriteLine(message);
         }
         finally
         {
            if (ad != null)
            {
               AppDomain.Unload(ad);
               System.Console.WriteLine("*** Unloaded AppDomain {0}", friendlyName);
            }
         }
      }
   }
}

