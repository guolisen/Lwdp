// Common/IntToString.cpp
#if 0
#include "StdAfx.h"

#include "IntToString.h"

void ConvertUInt64ToString(UInt64 value, char *s, UInt32 base)
{
  if (base < 2 || base > 36)
  {
    *s = '\0';
    return;
  }
  char temp[72];
  int32_ pos = 0;
  do
  {
    int32_ delta = (int32_)(value % base);
    temp[pos++] = (char)((delta < 10) ? ('0' + delta) : ('a' + (delta - 10)));
    value /= base;
  }
  while (value != 0);
  do
    *s++ = temp[--pos];
  while (pos > 0);
  *s = '\0';
}

void ConvertUInt64ToString(UInt64 value, wchar_t *s)
{
  wchar_t temp[32];
  int32_ pos = 0;
  do
  {
    temp[pos++] = (wchar_t)(L'0' + (int32_)(value % 10));
    value /= 10;
  }
  while (value != 0);
  do
    *s++ = temp[--pos];
  while (pos > 0);
  *s = L'\0';
}

void ConvertUInt32ToString(UInt32 value, char *s) { ConvertUInt64ToString(value, s); }
void ConvertUInt32ToString(UInt32 value, wchar_t *s) { ConvertUInt64ToString(value, s); }

void ConvertInt64ToString(Int64 value, char *s)
{
  if (value < 0)
  {
    *s++ = '-';
    value = -value;
  }
  ConvertUInt64ToString(value, s);
}

void ConvertInt64ToString(Int64 value, wchar_t *s)
{
  if (value < 0)
  {
    *s++ = L'-';
    value = -value;
  }
  ConvertUInt64ToString(value, s);
}

void ConvertUInt32ToHexWithZeros(UInt32 value, char *s)
{
  for (int32_ i = 0; i < 8; i++)
  {
    int32_ t = value & 0xF;
    value >>= 4;
    s[7 - i] = (char)((t < 10) ? ('0' + t) : ('A' + (t - 10)));
  }
  s[8] = '\0';
}
#endif
