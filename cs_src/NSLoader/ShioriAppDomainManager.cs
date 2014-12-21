using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime;
using System.Reflection;
using System.Runtime.InteropServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;

namespace NShiori
{
    [Guid("233CD954-86BB-4984-A2A2-C4BFA7F9C14D")]
    [ComVisible(true)]
    public sealed class ShioriAppDomainManager : AppDomainManager, IShiori1, IShioriAddDomainInitialize
    {
        private static readonly NLog.Logger logger = NLog.LogManager.GetCurrentClassLogger();

        #region AppDomainManager override

        static ShioriAppDomainManager()
        {
            logger.Trace("[static constructor]");
        }


        public ShioriAppDomainManager()
        {
            logger.Trace("[constructor]");
        }

        public override void InitializeNewDomain(AppDomainSetup appDomainInfo)
        {
            logger.Trace("[InitializeNewDomain] start");
            this.InitializationFlags = AppDomainManagerInitializationOptions.RegisterWithHost;
        }

        public override AppDomain CreateDomain(string friendlyName, Evidence securityInfo, AppDomainSetup appDomainInfo)
        {
            var appDomain = base.CreateDomain(friendlyName, securityInfo, appDomainInfo);
            logger.Trace("[InitializeNewDomain] Created AppDomain {0}", friendlyName);
            return appDomain;
        }



        #endregion
        #region SHIORI::load

        /// <summary>
        /// load処理。
        /// 以下の処理を行います。
        ///   ・新しいAppDomainの作成
        ///   ・新しいAppDomainに"NSLoader.DLL"を読み込む
        ///   ・新しいAppDomainに"(.net shiori).dll"を読み込む
        ///   ・新しいAppDomainの shioriAddDomainLoad(dir)を呼び出す。
        ///       →Ghost.load(dir)が呼び出される。
        /// </summary>
        /// <param name="loaddir"></param>
        /// <returns></returns>
        public bool load(string loaddir)
        {
            try
            {
                return loadImpl(loaddir);
            }
            catch (Exception ex) {
                var ex2 = new Exception("load処理に失敗しました。", ex);
                logger.Error(ex2);
                return false;
            }
        }

        private bool loadImpl(string loaddir)
        {
            var dir = Path.GetFullPath(loaddir);
            logger.Trace("[load] loaddir=[{0}]", dir);

            // ローダ設定情報
            var config = new NSLoaderConfig
            {
                ShioriAssemblyName="Pasta",
                ShioriTypeName="Pasta.Ghost",
            };

            // ベースディレクトリは[ghost]フォルダとします。
            var appBase = GetRootDirectory(dir);
            logger.Trace("[load] appBasePath =[{0}]", appBase);

            // アセンブリ検索パス
            //   ・[rootDir]ghost/master/lib/net
            //   ・[rootDir]ghost/master
            var appRelative
                = @"ghost\master\lib\net;"
                + @"ghost\master"
                ;


            // アプリケーションドメイン設定
            // セキュリティのため、Ghostのルートディレクトリは
            // ghostディレクトリに固定されます。
            var setting = new AppDomainSetup
            {
                ApplicationName = config.ShioriAssemblyName,
                ApplicationBase = appBase,
                PrivateBinPath = appRelative,
                PrivateBinPathProbe = "",
            };

            // 新しいAppDomainの作成
            var myAssm=Assembly.GetExecutingAssembly();
            //            var domain = AppDomain.CreateDomain(myAssm.FullName);
            var assemblyFileName = Path.GetFullPath(Path.Combine(dir, config.ShioriAssemblyName + ".dll"));
            var domain = AppDomain.CreateDomain(assemblyFileName, null, setting);

            // Ghostの取得とloadの呼び出し
            Ghost =domain.CreateInstanceAndUnwrap(config.ShioriAssemblyName, config.ShioriTypeName);
            return Ghost.load(loaddir);
        }

        private static string GetRootDirectory(string dir)
        {
            try
            {
                var d1 = Path.GetDirectoryName(dir);
                var rootDir = Path.GetDirectoryName(d1);
                if (rootDir == null) throw new DirectoryNotFoundException("ディレクトリ階層が浅すぎます。");
                var check = Path.GetDirectoryName(rootDir);
                if (check == null) throw new DirectoryNotFoundException("ディレクトリ階層が浅すぎます。");

                return Path.GetFullPath(rootDir);
            }
            catch (Exception ex) {
                var mes = string.Format("不適切なloaddir[{0}]が指定されました。", dir);
                throw new DirectoryNotFoundException(mes, ex); 
            }
        }

        #endregion
        #region SHIORI::その他のメソッド

        /// <summary>Ghostインスタンス</summary>
        public dynamic Ghost { get; set; }

        /// <summary>
        /// 読み込まれたAppDomain側のload処理。
        /// </summary>
        /// <param name="loaddir"></param>
        /// <returns></returns>
        public bool shioriAddDomainLoad(string loaddir)
        {
            logger.Trace("[shioriAddDomainLoad] start");
            return Ghost.load(loaddir);
        }

        /// <summary>
        /// unload処理。
        /// </summary>
        /// <returns></returns>
        public bool unload()
        {
            try
            {
                logger.Trace("[unload] start");
                try
                {
                    return Ghost.unload();
                }
                finally
                {
                    ((IDisposable)Ghost).Dispose();
                }
            }
            catch (Exception ex) { logger.Error(ex); }
            finally {
                Ghost = null;
                logger.Trace("[unload] end");
            }
            return false;
        }

        /// <summary>
        /// request処理。
        /// </summary>
        /// <param name="req"></param>
        /// <param name="res"></param>
        /// <returns></returns>
        public bool request(string req, out string res)
        {
            try
            {
                return Ghost.request(req, out res);
            }
            catch (Exception ex)
            {
                logger.Error(ex);
                res = "SHIORI/3.0 400 Bad Request\r\n"
                    + "Charset: UTF-8\r\n"
                    + "Sender: NSLoader\r\n"
                    + "X-NSLoader-Reason: " + ex.ToString()
                    + "\r\n\r\n";
                return false;
            }
        }


        #endregion
    }
}
