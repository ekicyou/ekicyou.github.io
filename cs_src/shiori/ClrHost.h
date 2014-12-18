#pragma once
class ClrHost
{
public:
    ClrHost(const HINSTANCE hinst);
    ~ClrHost();

private:
    const HINSTANCE hinst;

public:
    BOOL unload(void);
    BOOL load(HGLOBAL hGlobal_loaddir, long loaddir_len);
    HGLOBAL request(HGLOBAL hGlobal_request, long& len);
};

// EOF