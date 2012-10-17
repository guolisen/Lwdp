// Copyright 2008-2011 Zhang Yun Gui, rhcad@hotmail.com
// http://sourceforge.net/projects/x3c/
//
// Changes:
// 2011-01-15, Zhang Yun Gui: IsSpaceChar() support '\r' and '\n'.
//

#include <UtilFunc/PluginInc.h>
#include "Cx_TextUtil.h"
#include <UtilFunc/ConvStr.h>
#include <UtilFunc/SysErrStr.h>
#include <UtilFunc/ctrim.h>
#include <UtilFunc/AutoNew.h>

// First bytes      Encoding assumed:
// EF BB BF         UTF-8
// FE FF            UTF-16 (big-endian)
// FF FE            UTF-16 (little-endian)
// 00 00 FE FF      UTF-32 (big-endian)
// FF FE 00 00      UTF-32 (little-endian)

uint32_ Cx_TextUtil::GetHeadBytes(const tstring& filename, BYTE head[5])
{
    uint32_ dwBytesRead = 0;
    HANDLE hFile = NULL;

    if (Api_OpenFileForRead(hFile, filename.c_str()))
    {
        ::Api_ReadFile(hFile, head, 5, &dwBytesRead, NULL);
        Api_CloseFile(hFile);
    }
    else
    {
        uint32_ err = Api_GetLastError();
        X3LOG_ERROR2(__T"@TextUtility:IDS_OPEN_FAIL",
            x3::GetSystemErrorString(err) <<__T", " << filename);
    }

    return dwBytesRead;
}

bool Cx_TextUtil::IsUTF16File(const tstring& filename, bool& utf16)
{
    BYTE head[5] = { 0, 0, 0, 0, 0 };
    uint32_ dwBytesRead = GetHeadBytes(filename, head);
    bool bRet = false;

    if (dwBytesRead > 2)
    {
        utf16 = (0xFF == head[0] && 0xFE == head[1]);
        bRet = (utf16 || !(0xFE == head[0] && 0xFF == head[1]));
    }

    return bRet;
}

bool Cx_TextUtil::IsUTF8File(const tstring& filename, bool& bUTF8)
{
    BYTE head[5] = { 0, 0, 0, 0, 0 };
    uint32_ dwBytesRead = GetHeadBytes(filename, head);
    bool bRet = (dwBytesRead > 0);

    bUTF8 = (dwBytesRead > 3) && (0xEF == head[0] && 0xBB == head[1] && 0xBF == head[2]);

    return bRet;
}

bool Cx_TextUtil::UnicodeToAnsi(const tstring& filename, int32_ codepage)
{
    BYTE head[5] = { 0, 0, 0, 0, 0 };
    uint32_ dwBytesRead = GetHeadBytes(filename, head);
    bool bRet = true;

    if (dwBytesRead > 2 && (0xFF == head[0] && 0xFE == head[1]))
    {
        tstring content;
        bRet = ReadTextFile(content, filename)
            && SaveTextFile(content, filename, false, codepage);
    }

    return bRet;
}

bool Cx_TextUtil::AnsiToUnicode(const tstring& filename, int32_ codepage)
{
    bool bRet = false;
    BYTE head[5];
    tstring content;

    if (ReadTextFile(head, content, filename, 16, codepage))
    {
        bRet = (0xFF == head[0] && 0xFE == head[1])
            || SaveTextFile(content, filename, true);
    }

    return bRet;
}

bool Cx_TextUtil::GetFileContent(tstring& content, BYTE* buf, long_ size, int32_ codepage)
{
    bool bRet = true;

    if (0xFF == buf[0] && 0xFE == buf[1])   // UTF-16 (little-endian)
    {
        content.resize((size - 2) / 2);
        memcpy((void*)content.c_str(), buf + 2, content.size() * sizeof(tchar_));
    }
    else            // ANSI/ASCII
    {
        bRet = (buf[0] != 0xFE || buf[0] != 0xFF);

        if (size > 3 && (0xEF == buf[0] && 0xBB == buf[1] && 0xBF == buf[2]))
        {
            codepage = 65001;//CP_UTF8
        }
        content = x3::a2w((const tchar_*)buf, codepage);
    }

    return bRet;
}

bool Cx_TextUtil::ReadTextFile(BYTE head[5], tstring& content,
                               const tstring& filename,
                               ULONG nLenLimitMB, int32_ codepage)
{
    memset(head, 0, sizeof(BYTE) * 5);
    content.resize(0);

    bool bRet = false;
    HANDLE hFile = NULL;

    if (!Api_OpenFileForRead(hFile, filename.c_str()))
    {
        uint32_ err = Api_GetLastError();
        X3LOG_ERROR2(__T"@TextUtility:IDS_OPEN_FAIL",
            x3::GetSystemErrorString(err) <<__T", " << filename);
    }
    else
    {
        uint32_ dwLength = ::Api_GetFileSize(hFile, NULL);
        KAutoNewArr<BYTE> buf;

        if ((long_)dwLength > 0)
        {
            if (dwLength > nLenLimitMB * 1024L * 1024L)
            {
                X3LOG_WARNING2(__T"@TextUtility:IDS_HUGE_FILE",
                    (dwLength / (1024.0*1024.0)) <<__T"MB, " << filename);
                dwLength = nLenLimitMB * 1024L * 1024L;
            }
            buf.Realloc(dwLength + 8);
            memset(buf.ptr, 0, buf.count);
        }

        if (buf)
        {
            uint32_ dwBytesRead = 0;
            ::Api_ReadFile(hFile, buf.ptr, dwLength, &dwBytesRead, NULL);
            if (dwBytesRead > 0)
            {
                memcpy(head, buf.ptr, 5 < dwBytesRead ? 5 : dwBytesRead);
                bRet = GetFileContent(content, buf.ptr, dwBytesRead, codepage);
                if (!bRet)
                {
                    X3LOG_WARNING2(__T"@TextUtility:IDS_NOT_ANSIFILE", filename);
                }
            }
        }

        Api_CloseFile(hFile);
    }

    return bRet;
}

bool Cx_TextUtil::ReadTextFile(tstring& content,
                               const tstring& filename,
                               ULONG nLenLimitMB, int32_ codepage)
{
    BYTE head[5];
    return ReadTextFile(head, content, filename, nLenLimitMB, codepage);
}

bool Cx_TextUtil::SaveTextFile(const tstring& content,
                               const tstring& filename,
                               bool utf16, int32_ codepage)
{
    bool bRet = false;
    HANDLE hFile = NULL;

    ::Api_SetFileAttributesNormal(filename.c_str());

    if (!Api_OpenFileForWrite(hFile, filename.c_str()))
    {
        uint32_ err = Api_GetLastError();
        X3LOG_ERROR2(__T"@TextUtility:IDS_WRITE_FAIL",
            x3::GetSystemErrorString(err) <<__T", " << filename);
    }
    else
    {
        uint32_ dwLen, dwBytes;

        if (utf16)
        {
            BYTE head[] = { 0xFF, 0xFE };
            ::Api_WriteFile(hFile, head, 2, &dwBytes, NULL);

            dwLen = (uint32_)(content.size() * sizeof(tchar_));
            ::Api_WriteFile(hFile, content.c_str(), dwLen, &dwBytes, NULL);
            bRet = (dwBytes == dwLen);
        }
        else
        {
            tstring strAnsi (x3::w2a(content, codepage));

            dwLen = x3::GetSize(strAnsi);
            ::Api_WriteFile(hFile, strAnsi.c_str(), dwLen, &dwBytes, NULL);
            bRet = (dwBytes == dwLen);
        }

        Api_CloseFile(hFile);
    }

    return bRet;
}

bool Cx_TextUtil::SaveTextFile(const tstring& content,
                               const tstring& filename,
                               bool utf16, int32_ codepage)
{
    bool bRet = false;
    HANDLE hFile = NULL;

    ::Api_SetFileAttributesNormal(filename.c_str());

    if (!Api_OpenFileForWrite(hFile, filename.c_str()))
    {
        uint32_ err = Api_GetLastError();
        X3LOG_ERROR2(__T"@TextUtility:IDS_WRITE_FAIL",
            x3::GetSystemErrorString(err) <<__T", " << filename);
    }
    else
    {
        uint32_ dwLen, dwBytes;

        if (utf16)
        {
            tstring wstrUnicode (x3::a2w(content, codepage));

            BYTE head[] = { 0xFF, 0xFE };
            ::Api_WriteFile(hFile, head, 2, &dwBytes, NULL);

            dwLen = (uint32_)(wstrUnicode.size() * sizeof(tchar_));
            ::Api_WriteFile(hFile, wstrUnicode.c_str(), dwLen, &dwBytes, NULL);
            bRet = (dwBytes == dwLen);
        }
        else
        {
            dwLen = x3::GetSize(content);
            ::Api_WriteFile(hFile, content.c_str(), dwLen, &dwBytes, NULL);
            bRet = (dwBytes == dwLen);
        }

        Api_CloseFile(hFile);
    }

    return bRet;
}

long_ Cx_TextUtil::GetLineCount(const tstring& text)
{
    if (text.empty())
    {
        return 0;
    }

    long_ nCount = 1;
    const tchar_* pszStart = text.c_str();
    const tchar_* pszEnd;

    for (; (pszEnd = wcsstr(pszStart,__T"\n\r")) != NULL; nCount++)
    {
        pszStart = pszEnd + 1;
        if (*pszStart != *pszEnd
            && (L'\n' == *pszStart || L'\r' == *pszStart))
        {
            pszStart++;
        }
    }

    return nCount;
}

tstring Cx_TextUtil::GetLine(const tstring& text,
                                  long_ line, const tchar_** nextline)
{
    if (line < 0)
    {
        if (nextline != NULL)
        {
            *nextline = NULL;
        }
        return__T"";
    }

    long_ nCount = 0;
    const tchar_* pszStart = text.c_str();
    const tchar_* pszEnd = pszStart + wcscspn(pszStart,__T"\n\r");

    while (line > nCount && *pszEnd != 0)
    {
        pszStart = pszEnd + 1;
        if (*pszStart != *pszEnd
            && (L'\n' == *pszStart || L'\r' == *pszStart))
        {
            pszStart++;
        }
        pszEnd = pszStart + wcscspn(pszStart,__T"\n\r");
        nCount++;
    }

    if (nextline != NULL)
    {
        if (0 == *pszEnd)
        {
            *nextline = NULL;
        }
        else
        {
            const tchar_* p = pszEnd + 1;
            if (*p != *pszEnd && (L'\n' == *p || L'\r' == *p))
            {
                p++;
            }
            *nextline = p;
        }
    }

    return tstring(pszStart, pszEnd - pszStart);
}

static inline bool IsSpaceChar(tchar_ cChar, const tchar_* targets = NULL)
{
    return targets ? (wcschr(targets, cChar) != NULL)
        : (0x0020 == cChar || 0x3000 == cChar || L'\t' == cChar
        || L'\n' == cChar || L'\r' == cChar);
}

bool Cx_TextUtil::IsSpaceLine(const tstring& text)
{
    int32_ i = x3::GetSize(text);
    while (--i >= 0 && IsSpaceChar(text[i])) {}
    return i < 0;
}

bool Cx_TextUtil::TrimSpace(tstring& text, const tchar_* targets)
{
    const int32_ nOldLen = x3::GetSize(text);
    int32_ i = nOldLen;
    while (--i >= 0 && IsSpaceChar(text[i], targets)) {}

    if (i + 1 < nOldLen)
    {
        text.erase(text.begin() + (i + 1), text.end());
    }

    int32_ n = x3::GetSize(text);
    for (i = 0; i < n && IsSpaceChar(text[i], targets); i++) {}

    if (i > 0)
    {
        text.erase(text.begin(), text.begin() + i);
    }

    return nOldLen > x3::GetSize(text);
}

bool Cx_TextUtil::TrimLeft(tstring& text, const tchar_* targets)
{
    const int32_ len = x3::GetSize(text);
    int32_ i = 0;

    for (; i < len && IsSpaceChar(text[i], targets); i++) {}
    if (i > 0)
    {
        text.erase(text.begin(), text.begin() + i);
    }

    return i > 0;
}

bool Cx_TextUtil::TrimRight(tstring& text, const tchar_* targets)
{
    const int32_ len = x3::GetSize(text);
    int32_ i = len;
    while (--i >= 0 && IsSpaceChar(text[i], targets)) {}

    if (i + 1 < len)
    {
        text.erase(text.begin() + (i + 1), text.end());
    }

    return i + 1 < len;
}

bool Cx_TextUtil::RemoveInvalidChars(tstring& text, const tchar_* targets)
{
    std::vector<long_> arrIndex;
    long_ i = x3::GetSize(text);

    while (--i >= 0)
    {
        tchar_ ch = text[i];

        if (targets && IsSpaceChar(ch, targets))
        {
            arrIndex.push_back(i);
        }
        else if (!targets)
        {
            if ( (ch >= 0x0001 && ch <= 0x0008)
                || (ch >= 0x000b && ch <= 0x000c)
                || (ch >= 0x000e && ch <= 0x001f)
                || (ch == 0xDBC0) )
            {
                arrIndex.push_back(i);
            }
        }
    }

    long_ nCount = x3::GetSize(arrIndex);
    for (i = 0; i < nCount; i++)
    {
        text.erase(arrIndex[i]);
    }

    return nCount > 0;
}

bool Cx_TextUtil::ReplaceAll(tstring& text,
                             const tstring& match,
                             const tstring& newtext)
{
    long_ count = 0;
    size_t index = text.find(match);

    while (index != tstring::npos)
    {
        count++;
        text.replace(index, match.size(), newtext);
        index = text.find(match, index + newtext.size());
    }

    return count > 0;
}

bool Cx_TextUtil::ReplaceChar(tstring& text,
                              const tstring& match,
                              const tstring& chars)
{
    return trim::replace_each(text, match, chars) > 0;
}

bool Cx_TextUtil::ToDBC(tstring& text, bool punct)
{
    bool changed = false;
    tstring dest(L'\0', text.size() + 1);

#ifdef _WIN32
    if (!text.empty() && punct)
    {
        int32_ ret = LCMapStringW(
            MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED),
            LCMAP_HALFWIDTH,
            text.c_str(), text.size(),
            &dest[0], dest.size() + 1);

        if (ret > 0 && dest != text)
        {
            text = dest;
            changed = true;
        }
    } else
#endif
    if (!text.empty())
    {
        const tchar_* psrc = text.c_str();
        tchar_* pdest = &dest[0];
        int32_ i = 0, n = 0;

        for (; psrc[i] != 0; i++)
        {
            if (psrc[i] >= 0xA3B0 && psrc[i] <= 0xA3B9)     // Chinese 0..9
            {
                pdest[i] = tchar_(L'0' + psrc[i] - 0xA3B0);
                n++;
            }
            else if (psrc[i] >= 0xA3C1 && psrc[i] <= 0xA3DA)    // A..Z
            {
                pdest[i] = tchar_(L'A' + psrc[i] - 0xA3C1);
                n++;
            }
            else if (psrc[i] >= 0xA3E1 && psrc[i] <= 0xA3FA)    // a..z
            {
                pdest[i] = tchar_(L'a' + psrc[i] - 0xA3E1);
                n++;
            }
            else
            {
                pdest[i] = psrc[i];
            }
        }
        pdest[i] = 0;

        if (n > 0)
        {
            text = dest;
            changed = true;
        }
    }

    return changed;
}

tstring Cx_TextUtil::ToAnsi(const tstring& text, int32_ codepage)
{
    return x3::w2a(text, codepage);
}

tstring Cx_TextUtil::ToUnicode(const tstring& text, int32_ codepage)
{
    return x3::a2w(text, codepage);
}
