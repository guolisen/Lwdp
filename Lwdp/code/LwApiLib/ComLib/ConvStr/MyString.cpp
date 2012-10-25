// Common/String.cpp

#include <LwDp.h>

#include <ctype.h>
#ifdef HAVE_WCTYPE_H
#include <wctype.h>
#endif
//#include "StringConvert.h" // FIXED
#include <LwApiLib/ComLib/ConvStr/MyString.h>

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

LWDP_NAMESPACE_BEGIN;


char_* CharPrevA( const char_* start, const char_* ptr ) { // OK for MBS
  while (*start && (start < ptr)) {
    const char_* next = CharNextA( start );
    if (next >= ptr)
      break;
    start = next;
  }
  return (char_*)start;
}

char_* CharNextA( const char_* ptr ) {
  if (!*ptr)
    return (char_*)ptr;
// #ifdef HAVE_MBRTOWC
//  if (global_use_utf16_conversion)
//  {
//    wchar_t wc;
//    size_t len  = mbrtowc(&wc,ptr,MB_LEN_MAX,0);  // mbrtowc stales on some configurations.
//    if (len >= 1) return (LPSTR)(ptr + len);
//    printf("INTERNAL ERROR - CharNextA\n");
//    exit(EXIT_FAILURE);
//  } else {
//    return (LPSTR)(ptr + 1);
//  }
//#else
  return (char_*)(ptr + 1); // p7zip search only for ASCII characters like '/' so no need to worry about current locale
//#endif
}

char_ MyCharLower(char_ c)
{
	int32_ r = c & 0xFF;
	return tolower(r);
}

wchar_ MyCharLower(wchar_ c)
{
#ifdef HAVE_TOWUPPER
   return towlower(c);
#else
   int32_ ret = c;
   if ((ret >= 1) && (ret <256)) ret = tolower(ret);
	   return (wchar_)ret;
#endif
}

char_ * MyStringLower(char_ *s)
{
	if (s == 0)
		return 0;
	char_ *ret = s;
	while (*s)
	{
		*s = MyCharLower(*s);
		s++;
	}
	return ret;
}

wchar_ * MyStringLower(wchar_ *s)
{
	if (s == 0)
		return 0;
	wchar_ *ret = s;
	while (*s)
	{
		*s = MyCharLower(*s);
		s++;
	}
	return ret;
}

char_ MyCharUpper(char_ c)
{
	int32_ r = c & 0xFF;
	return toupper(r);
}

wchar_ MyCharUpper(wchar_ c)
{
#ifdef HAVE_TOWUPPER
   return towupper(c);
#else
   int32_ ret = c;
   if ((ret >= 1) && (ret <256)) ret = toupper(ret);
   return (wchar_)ret;
#endif
}

char_ * MyStringUpper(char_ *s)
{
	if (s == 0)
		return 0;
	char_ *ret = s;
	while (*s)
	{
		*s = MyCharUpper(*s);
		s++;
	}
	return ret;
}
wchar_ * MyStringUpper(wchar_ *s)
{
	if (s == 0)
		return 0;
	wchar_ *ret = s;
	while (*s)
	{
		*s = MyCharUpper(*s);
		s++;
	}
	return ret;
}

int32_ MyStringCompare(const char_ *s1, const char_ *s2)
{
	while (true)
	{
		uint8_ c1 = (uint8_)*s1++;
		uint8_ c2 = (uint8_)*s2++;
		if (c1 < c2) return -1;
		if (c1 > c2) return 1;
		if (c1 == 0) return 0;
	}
}

int32_ MyStringCompare(const wchar_ *s1, const wchar_ *s2)
{
	while (true)
	{
		wchar_ c1 = *s1++;
		wchar_ c2 = *s2++;
		if (c1 < c2) return -1;
		if (c1 > c2) return 1;
		if (c1 == 0) return 0;
	}
}

int32_ MyStringCompareNoCase(const wchar_ *s1, const wchar_ *s2)
{
	while (true)
	{
		wchar_ c1 = *s1++;
		wchar_ c2 = *s2++;
		if (c1 != c2)
		{
			wchar_ u1 = MyCharUpper(c1);
			wchar_ u2 = MyCharUpper(c2);
			if (u1 < u2) return -1;
			if (u1 > u2) return 1;
		}
		if (c1 == 0) return 0;
	}
}

int32_ MyStringCompareNoCase(const char_ *s1, const char_ *s2)
{
	return 0;
	//return MyStringCompareNoCase(MultiByteToUnicodeString(s1), MultiByteToUnicodeString(s2));
}



LWDP_NAMESPACE_END;

