// Common/StringConvert.h

#ifndef __COMMON_STRING_CONVERT_H
#define __COMMON_STRING_CONVERT_H
#if 0
#include "MyString.h"

LWDP_NAMESPACE_BEGIN;

UString MultiByteToUnicodeString(const AString &srcString, UINT codePage = 0);
AString UnicodeStringToMultiByte(const UString &srcString, UINT codePage = 0);


inline const wchar_* GetUnicodeString(const wchar_* unicodeString)
  { return unicodeString; }
inline const UString& GetUnicodeString(const UString &unicodeString)
  { return unicodeString; }
inline UString GetUnicodeString(const AString &ansiString)
  { return MultiByteToUnicodeString(ansiString); }
inline UString GetUnicodeString(const AString &multiByteString, UINT codePage)
  { return MultiByteToUnicodeString(multiByteString, codePage); }
inline const wchar_* GetUnicodeString(const wchar_* unicodeString, UINT)
  { return unicodeString; }
inline const UString& GetUnicodeString(const UString &unicodeString, UINT)
  { return unicodeString; }

inline const char_* GetAnsiString(const char_* ansiString)
  { return ansiString; }
inline const AString& GetAnsiString(const AString &ansiString)
  { return ansiString; }
inline AString GetAnsiString(const UString &unicodeString)
  { return UnicodeStringToMultiByte(unicodeString); }

inline const char_* GetOemString(const char_* oemString)
  { return oemString; }
inline const AString& GetOemString(const AString &oemString)
  { return oemString; }
inline AString GetOemString(const UString &unicodeString)
  { return UnicodeStringToMultiByte(unicodeString, CP_OEMCP); }


#ifdef _UNICODE
  inline const wchar_* GetSystemString(const wchar_* unicodeString)
    { return unicodeString;}
  inline const UString& GetSystemString(const UString &unicodeString)
    { return unicodeString;}
  inline const wchar_* GetSystemString(const wchar_* unicodeString, UINT /* codePage */)
    { return unicodeString;}
  inline const UString& GetSystemString(const UString &unicodeString, UINT /* codePage */)
    { return unicodeString;}
  inline UString GetSystemString(const AString &multiByteString, UINT codePage)
    { return MultiByteToUnicodeString(multiByteString, codePage);}
  inline UString GetSystemString(const AString &multiByteString)
    { return MultiByteToUnicodeString(multiByteString);}
#else
  inline const char_* GetSystemString(const char_ *ansiString)
    { return ansiString; }
  inline const AString& GetSystemString(const AString &multiByteString, UINT)
    { return multiByteString; }
  inline const char_ * GetSystemString(const char_ *multiByteString, UINT)
    { return multiByteString; }
  inline AString GetSystemString(const UString &unicodeString)
    { return UnicodeStringToMultiByte(unicodeString); }
  inline AString GetSystemString(const UString &unicodeString, UINT codePage)
    { return UnicodeStringToMultiByte(unicodeString, codePage); }
#endif

LWDP_NAMESPACE_END;
#endif
#endif
