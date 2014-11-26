#pragma once
#include <atlstr.h>

namespace shiori{

    // SHIORI REQUEST種別
    enum RequestType {
        REQUEST_NOTIFY,		// 通知のみのリクエスト、応答なし。
        REQUEST_GET,		// 値取得するリクエスト、応答あり。
        REQUEST_LOAD,		// Loadリクエスト      、応答なし。
        REQUEST_UNLOAD,		// Unloadリクエスト    、応答なし、エージェント終了を待つ。
    };

    // SHIORI RESPONSE種別
    enum ResponseType {
        RESPONSE_NORMAL,    // 通常レスポンス。そのままSHIORIに返す。
        RESPONSE_ERROR,		// 例外。内部サーバエラーに変換する。
    };

    // SHIORI REQUEST
    struct Request {
        explicit Request(const RequestType tp, CComBSTR& req)
            :reqType(tp), value(req.Detach()){}
        explicit Request(const RequestType tp)
            :reqType(tp){}
        const RequestType reqType;
        CComBSTR value;
    };

    // SHIORI RESPONSE
    struct Response{
        explicit Response(const ResponseType tp, CComBSTR& res) 
            :resType(tp), value(res.Detach()) {}
        const ResponseType resType;
        CComBSTR value;
    };


}


