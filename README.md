IEでDXGI描画を行うための仕組みを探すデモ
========================================
手がかりを探していくぞ！


## 手がかり
 1. IViewObjectPresentSite::CreateSurfacePresenterr
    D3DデバイスからISurfacePresenterなるものを返すらしい。
    恐らくはISurfacePresenterは既にD3Dデバイスと接続済みで、
    後は何かしら描画するだけでD3Dデバイスにいい感じに出力してくれるのだろう。
      * じゃあ、IViewObjectPresentSiteはどこから取得するのか？

 2. どうも、[IOleClientSite]が持っているインターフェース、らしい？
      * 関連：IOleClientSiteから派生可能なインターフェース
        * IViewObjectPresentSite
        * IServiceProvider
          * ->QueryServiceで、IWebBrowser2？

 3. よくわからないが、IServiceProvider,IHTMLOMWindowServicesを実装した
    Windowクラスを作ればよい？



IViewObjectPresentSite::CreateSurfacePresenterr メソッド
--------------------------------------------------------
DXGI バッファー チェーンISurfacePresenterインターフェイスを返します。
 
### 構文
HRESULT retVal = object.CreateSurfacePresenter(pDevice, width, height, backBufferCount, format, mode, ppQueue);

### パラメーター
  * pDevice[in]
    タイプ: IUnknown
    使用するデバイスです。Mshtml でハードウェア アクセラレーションが使用されていない場合は、NULL を渡します。

  * width[in]
    UINT型:
    フロント バッファーの幅。これはまた、バック バッファーのサイズによって決まります。

  * height[in]
    UINT型:
    フロント バッファーの高さ。これはまた、バック バッファーのサイズによって決まります。

  * backBufferCount[in]
    UINT型:
    作成するバック バッファーの数です。

  * format[in]
    タイプ: dxgi_format」を参照
    DXGI レンダリング フォーマット。[DXGI_FORMAT_B8G8R8A8_UNORM] のみサポート。

  * mode [in]
    種類： VIEW_OBJECT_ALPHA_MODE
    バッファーのアルファ チャネルを解釈する方法を示します。

  * ppQueue[out, retval]
    種類： ISurfacePresenter
    ISurfacePresenterインターフェイスです。

### 戻り値
タイプ: HRESULT
このメソッドが成功した場合S_OKを返します。それ以外の場合は、 HRESULTエラー コードを返します。

### 解説
pDeviceは有効な IDirect3DDevice9Ex、ID3D10Device、または ID3D11Device である必要があります。PDeviceが NULL の場合はハードウェアによって加速バッファーではなく Windows イメージング コンポーネント (WIC) ビットマップ バッファーのチェーンを作成します。


### MSHTMLがサポートするインターフェース
■[web2]に存在する型一覧
  →○ [struct IConnectionPointContainer]
  →○ [struct IDataObject]
  →○ [struct IDispatch]
  →○ [struct IHTMLOMWindowServices]
  →○ [struct IInternetSecurityMgrSite]
  →○ [struct IOleCommandTarget]
  →○ [struct IOleControl]
  →○ [struct IOleInPlaceActiveObject]
  →○ [struct IOleInPlaceObject]
  →○ [struct IOleObject]
  →○ [struct IOleWindow]
  →○ [struct IPersist]
  →○ [struct IPersistPropertyBag]
  →○ [struct IPersistStorage]
  →○ [struct IPersistStream]
  →○ [struct IPersistStreamInit]
  →○ [struct IProvideClassInfo]
  →○ [struct IProvideClassInfo2]
  →○ [struct IServiceProvider]
  →○ [struct IStdMarshalInfo]
  →○ [struct IViewObject]
  →○ [struct IViewObject2]
  →○ [struct IWebBrowser]
  →○ [struct IWebBrowser2]
  →○ [struct IWebBrowserApp]
  →○ [struct IObjectSafety]

■[doc2]に存在する型一覧
  →○ [struct IConnectionPointContainer]
  →○ [struct ICustomDoc]
  →○ [struct IDataObject]
  →○ [struct IDispatch]
  →○ [struct IDispatchEx]
  →○ [struct IDisplayServices]
  →○ [struct IDocumentEvent]
  →○ [struct IDocumentRange]
  →○ [struct IDocumentSelector]
  →○ [struct IDocumentTraversal]
  →○ [struct IEventTarget]
  →○ [struct IHighlightRenderingServices]
  →○ [struct IHTMLChangePlayback]
  →○ [struct IHTMLDocument]
  →○ [struct IHTMLDocument2]
  →○ [struct IHTMLDocument3]
  →○ [struct IHTMLDocument4]
  →○ [struct IHTMLDocument5]
  →○ [struct IHTMLDocument6]
  →○ [struct IHTMLDocument7]
  →○ [struct IIMEServices]
  →○ [struct IInternetHostSecurityManager]
  →○ [struct IMarkupContainer]
  →○ [struct IMarkupContainer2]
  →○ [struct IMarkupServices]
  →○ [struct IMarkupServices2]
  →○ [struct IMarkupTextFrags]
  →○ [struct IMonikerProp]
  →○ [struct IObjectIdentity]
  →○ [struct IObjectWithSite]
  →○ [struct IOleCache]
  →○ [struct IOleCache2]
  →○ [struct IOleCommandTarget]
  →○ [struct IOleContainer]
  →○ [struct IOleControl]
  →○ [struct IOleDocument]
  →○ [struct IOleDocumentView]
  →○ [struct IOleInPlaceActiveObject]
  →○ [struct IOleInPlaceObject]
  →○ [struct IOleInPlaceObjectWindowless]
  →○ [struct IOleItemContainer]
  →○ [struct IOleObject]
  →○ [struct IOleWindow]
  →○ [struct IParseDisplayName]
  →○ [struct IPerPropertyBrowsing]
  →○ [struct IPersist]
  →○ [struct IPersistFile]
  →○ [struct IPersistMoniker]
  →○ [struct IPersistStreamInit]
  →○ [struct IPointerInactive]
  →○ [struct IProvideClassInfo]
  →○ [struct IProvideClassInfo2]
  →○ [struct IProvideMultipleClassInfo]
  →○ [struct IServiceProvider]
  →○ [struct ISpecifyPropertyPages]
  →○ [struct ISupportErrorInfo]
  →○ [struct ISVGDocument]
  →○ [struct IViewObject]
  →○ [struct IViewObject2]
  →○ [struct IViewObjectEx]
  →○ [struct IXMLGenericParse]
  →○ [struct IObjectSafety]
■[site]に存在する型一覧
  →○ [struct IAdviseSink]
  →○ [struct IAxWinAmbientDispatch]
  →○ [struct IAxWinAmbientDispatchEx]
  →○ [struct IAxWinHostWindow]
  →○ [struct IAxWinHostWindowLic]
  →○ [struct IDispatch]
  →○ [struct IDocHostUIHandler]
  →○ [struct IObjectWithSite]
  →○ [struct IOleClientSite]
  →○ [struct IOleContainer]
  →○ [struct IOleControlSite]
  →○ [struct IOleInPlaceSite]
  →○ [struct IOleInPlaceSiteEx]
  →○ [struct IOleInPlaceSiteWindowless]
  →○ [struct IOleWindow]
  →○ [struct IServiceProvider]
  →○ [struct IViewObjectPresentNotifySite]
  →○ [struct IViewObjectPresentSite]


mshtml.dllに関連するGUID情報
----------------------------
[htmlfile_FullWindowEmbed]が有力か？


{05f6fe1a-ecef-11d0-aae7-00c04fc9b304} IntDitherer Class
{18845040-0fa5-11d1-ba19-00c04fd912d0} Microsoft HTML Load Options
{25336920-03F9-11cf-8FD0-00AA00686F13} HTML Document
    ProgID htmlfile
    TypeLib {3050f1c5-98b5-11cf-bb82-00aa00bdce0b}

{25336921-03F9-11CF-8FD0-00AA00686F13} Microsoft HTML Document 6.0
    ProgID htmlfile_FullWindowEmbed

{3050F391-98B5-11CF-BB82-00AA00BDCE0B} Microsoft HTML Window Security Proxy
{3050F3B2-98B5-11CF-BB82-00AA00BDCE0B} Microsoft HTML Javascript Pluggable Protocol
{3050f3B3-98b5-11cf-bb82-00aa00bdce0b} %Microsoft Image Element Browse Property Page%
{3050f3B4-98b5-11cf-bb82-00aa00bdce0b} %Microsoft Document Browse Property Page%
{3050f3BB-98b5-11cf-bb82-00aa00bdce0b} %Microsoft Anchor Element Browse Property Page%
{3050F3C2-98B5-11CF-BB82-00AA00BDCE0B} Microsoft HTML DwnBindInfo
{3050F3D6-98B5-11CF-BB82-00AA00BDCE0B} IImgCtx
    ProgID IImgCtx

{3050F3D9-98B5-11CF-BB82-00AA00BDCE0B} MHTML Document
    ProgID mhtmlfile

{3050f3DA-98B5-11CF-BB82-00AA00BDCE0B} Microsoft HTML Mailto Pluggable Protocol
{3050F406-98B5-11CF-BB82-00AA00BDCE0B} Microsoft HTML About Pluggable Protocol
{3050f499-98b5-11cf-bb82-00aa00bdce0b} Microsoft HTML Recalc
{3050f4f8-98b5-11cf-bb82-00aa00bdce0b} Microsoft Html Component
{3050F5C8-98B5-11CF-BB82-00AA00BDCE0B} Microsoft HTA Document 6.0
{3050f667-98b5-11cf-bb82-00aa00bdce0b} Microsoft Html Popup Window
{3050f67D-98b5-11cf-bb82-00aa00bdce0b} Microsoft Html Document for Popup Window
{3050f6b3-98b5-11cf-bb82-00aa00bdce0b} Template Printer class
    ProgID TemplatePrinter.TemplatePrinter.1

{3050f819-98b5-11cf-bb82-00aa00bdce0b} {3050f819-98b5-11cf-bb82-00aa00bdce0b}
    ProgID HtmlDlgSafeHelper.HtmlDlgSafeHelper.1

{30510483-98B5-11CF-BB82-00AA00BDCE0B} Microsoft HTML Element Context Stream

{30590066-98b5-11cf-bb82-00aa00bdce0b} SVG Document
    ProgID svgfile

{30590067-98b5-11cf-bb82-00aa00bdce0b} XHTML Document
    ProgID xhtmlfile

{429AF92C-A51F-11d2-861E-00C04FA35C89} HostDialogHelper

{607fd4e8-0a03-11d1-ab1d-00c04fc9b304} CoICOFilter Class
    ProgID ICOFilter.CoICOFilter.1

{7E8BC44E-AEFF-11D1-89C2-00C04FB6BFC4}

{AE24FDAE-03C6-11D1-8B76-0080C744F389} Microsoft Scriptlet Component
    ProgID ScriptBridge.ScriptBridge.1
