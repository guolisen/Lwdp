// Copyright 2008-2011 Zhang Yun Gui, rhcad@hotmail.com
// http://sourceforge.net/projects/x3c/

#include <UtilFunc/PluginInc.h>
#include "Cx_ClipboardUtil.h"
#ifdef _WIN32
#include <UtilFunc/ConvStr.h>
#include <UtilFunc/SysErrStr.h>

bool Cx_ClipboardUtil::CopyText(HWND wndOwner, const tstring& text)
{
    bool bRet = false;
    uint32_ err = 0;

    if (!text.empty() && ::OpenClipboard(wndOwner))
    {
        HANDLE hMem = ::GlobalAlloc(GMEM_MOVEABLE, sizeof(tchar_) * (text.size() + 1));
        if (hMem != NULL)
        {
            tchar_* str = (tchar_*)::GlobalLock(hMem);
            if (NULL == str)
            {
                ::GlobalFree(hMem);
            }
            else
            {
                wcscpy_s(str, text.size() + 1, text.c_str());
                ::GlobalUnlock(hMem);

                ::EmptyClipboard();
                if (::SetClipboardData(CF_UNICODETEXT, hMem))
                {
                    bRet = true;
                }
                else
                {
                    err = Api_GetLastError();
                    X3LOG_WARNING2(__T"@TextUtility:IDS_SETCLIP_FAIL", x3::GetSystemErrorString(err));
                }
            }
        }
        else
        {
            err = Api_GetLastError();
            X3LOG_WARNING2(__T"@TextUtility:IDS_ALLOCMEM_FAIL", x3::GetSystemErrorString(err));
        }

        ::CloseClipboard();
    }
    else
    {
        err = Api_GetLastError();
        X3LOG_WARNING2(__T"@TextUtility:IDS_OPENCLIP_FAIL", x3::GetSystemErrorString(err));
    }

    return bRet;
}

bool Cx_ClipboardUtil::PasteText(HWND wndOwner, tstring& text, bool clear)
{
    text.resize(0);

    if (!::IsClipboardFormatAvailable(CF_UNICODETEXT)
        && !::IsClipboardFormatAvailable(CF_TEXT))
    {
        X3LOG_INFO(__T"@TextUtility:IDS_CLIP_NOTEXT");
        return false;
    }

    if (!::OpenClipboard(wndOwner))
    {
        uint32_ err = Api_GetLastError();
        X3LOG_WARNING2(__T"@TextUtility:IDS_OPENCLIP_FAIL", x3::GetSystemErrorString(err));
    }
    else
    {
        if (::IsClipboardFormatAvailable(CF_UNICODETEXT))
        {
            HANDLE hMem = ::GetClipboardData(CF_UNICODETEXT);
            if (hMem != NULL)
            {
                const tchar_* lpStr = (const tchar_*)::GlobalLock(hMem);
                if (lpStr != NULL)
                {
                    text = lpStr;
                    ::GlobalUnlock(hMem);
                }
            }
        }
        else if (::IsClipboardFormatAvailable(CF_TEXT))
        {
            HANDLE hMem = ::GetClipboardData(CF_UNICODETEXT);
            if (hMem != NULL)
            {
                const tchar_* lpStr = (const tchar_*)::GlobalLock(hMem);
                if (lpStr != NULL)
                {
                    text = x3::a2w(lpStr);
                    ::GlobalUnlock(hMem);
                }
            }
        }

        if (clear)
        {
            ::EmptyClipboard();
        }

        ::CloseClipboard();
    }

    return !text.empty();
}

bool Cx_ClipboardUtil::CanPasteText()
{
    return ::IsClipboardFormatAvailable(CF_UNICODETEXT)
        || ::IsClipboardFormatAvailable(CF_TEXT);
}

#endif // _WIN32
