// Common/StringConvert.cpp

#include <Lwdp.h>
#include <LwApiLib/ComLib/ConvStr/StringConvert.h>


LWDP_NAMESPACE_BEGIN;

UString MultiByteToUnicodeString(const AString &srcString, UINT codePage)
{
	UString resultString;
	for (int32_ i = 0; i < srcString.Length(); i++)
		resultString += wchar_t(srcString[i]);
	/*
	if (!srcString.IsEmpty())
	{
	int32_ numChars = mbstowcs(resultString.GetBuffer(srcString.Length()), srcString, srcString.Length() + 1);
	if (numChars < 0) throw "Your environment does not support UNICODE";
	resultString.ReleaseBuffer(numChars);
	}
	*/
	return resultString;
}

AString UnicodeStringToMultiByte(const UString &srcString, UINT codePage)
{
	AString resultString;
	for (int32_ i = 0; i < srcString.Length(); i++)
		resultString += char(srcString[i]);
	/*
	if (!srcString.IsEmpty())
	{
	int32_ numRequiredBytes = srcString.Length() * 6 + 1;
	int32_ numChars = wcstombs(resultString.GetBuffer(numRequiredBytes), srcString, numRequiredBytes);
	if (numChars < 0) throw "Your environment does not support UNICODE";
	resultString.ReleaseBuffer(numChars);
	}
	*/
	return resultString;
}

LWDP_NAMESPACE_END;
