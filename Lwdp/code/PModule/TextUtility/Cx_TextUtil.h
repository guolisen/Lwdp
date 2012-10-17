// Copyright 2008-2011 Zhang Yun Gui, rhcad@hotmail.com
// http://sourceforge.net/projects/x3c/

#ifndef _X3_UTILITY_TEXTUTIL_H
#define _X3_UTILITY_TEXTUTIL_H

#include <Utility/Ix_StringConvert.h>
#include <Utility/Ix_TextFileUtil.h>

class Cx_TextUtil
    : public Ix_StringConvert
    , public Ix_TextFileUtil
{
    X3BEGIN_CLASS_DECLARE(Cx_TextUtil)
        X3DEFINE_INTERFACE_ENTRY(Ix_StringConvert)
        X3DEFINE_INTERFACE_ENTRY(Ix_TextFileUtil)
    X3END_CLASS_DECLARE()
protected:
    Cx_TextUtil() {}
    virtual ~Cx_TextUtil() {}

private:
    // From Ix_TextFileUtil
    //
    virtual bool ReadTextFile(tstring& content, 
        const tstring& filename, 
        ULONG limitMB = 16, int32_ codepage = 0);
    virtual bool SaveTextFile(const tstring& content, 
        const tstring& filename, 
        bool utf16 = true, int32_ codepage = 0);
    virtual bool SaveTextFile(const tstring& content, 
        const tstring& filename, 
        bool utf16 = true, int32_ codepage = 0);
    virtual bool IsUTF16File(const tstring& filename, bool& utf16);
    virtual bool IsUTF8File(const tstring& filename, bool& utf8);
    virtual bool UnicodeToAnsi(const tstring& filename, int32_ codepage = 0);
    virtual bool AnsiToUnicode(const tstring& filename, int32_ codepage = 0);
    virtual long_ GetLineCount(const tstring& text);
    virtual tstring GetLine(const tstring& text, 
        long_ line, const tchar_** nextLine = NULL);
    virtual bool IsSpaceLine(const tstring& text);

    // From Ix_StringConvert
    //
    virtual bool TrimSpace(tstring& text, const tchar_* targets);
    virtual bool TrimLeft(tstring& text, const tchar_* targets);
    virtual bool TrimRight(tstring& text, const tchar_* targets);
    virtual bool RemoveInvalidChars(tstring& text, const tchar_* targets);
    virtual bool ReplaceAll(tstring& text, 
        const tstring& match, 
        const tstring& newtext);
    virtual bool ReplaceChar(tstring& text, 
        const tstring& match, 
        const tstring& chars);
    virtual bool ToDBC(tstring& text, bool punct = false);
    virtual tstring ToAnsi(const tstring& text, int32_ codepage = 0);
    virtual tstring ToUnicode(const tstring& text, int32_ codepage = 0);

private:
    uint32_ GetHeadBytes(const tstring& wstrFileName, BYTE buf[5]);
    bool GetFileContent(tstring& content, BYTE* buf, long_ size, int32_ codepage);
    bool ReadTextFile(BYTE head[5], tstring& content, 
        const tstring& wstrFileName, 
        ULONG nLenLimitMB, int32_ codepage);
};

#endif // _X3_UTILITY_TEXTUTIL_H
