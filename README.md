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
