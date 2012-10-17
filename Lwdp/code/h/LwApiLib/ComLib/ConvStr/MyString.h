// Common/String.h

#ifndef _LWDP_COMMON_STRING_H
#define _LWDP_COMMON_STRING_H

LWDP_NAMESPACE_BEGIN;

char_* CharPrevA( const char_* start, const char_* ptr );
char_* CharNextA( const char_* ptr );

template <class T>
inline int32_ MyStringLen(const T *s)
{
	int32_ i;
	for (i = 0; s[i] != '\0'; i++);
	return i;
}

template <class T>
inline T * MyStringCopy(T *dest, const T *src)
{
	T *destStart = dest;
	while ((*dest++ = *src++) != 0);
	return destStart;
}

inline tchar_* MyStringGetNextCharPointer(tchar_ *p)
  { return (p + 1); }
inline const tchar_* MyStringGetNextCharPointer(const tchar_ *p)
  { return (p + 1); }
inline tchar_* MyStringGetPrevCharPointer(const tchar_ *, tchar_ *p)
  { return (p - 1); }
inline const tchar_* MyStringGetPrevCharPointer(const tchar_ *, const tchar_ *p)
  { return (p - 1); }

char_ 		MyCharUpper(char_ c);
wchar_ 		MyCharUpper(wchar_ c);
char_ * 	MyStringUpper(char_ *s);
wchar_ * 	MyStringUpper(wchar_ *s);

char_ 		MyCharLower(char_ c);
wchar_ 		MyCharLower(wchar_ c);
char_ *		MyStringLower(char_ *s);
wchar_ * 	MyStringLower(wchar_ *s);

//////////////////////////////////////
// Compare
int32_ MyStringCompare(const char_ *s1, const char_ *s2);
int32_ MyStringCompare(const wchar_ *s1, const wchar_ *s2);
int32_ MyStringCompareNoCase(const wchar_ *s1, const wchar_ *s2);
int32_ MyStringCompareNoCase(const char_ *s1, const char_ *s2);


template <class T>
class CStringBase
{
	void TrimLeftWithCharSet(const CStringBase &charSet)
	{
		const T *p = _chars;
		while (charSet.Find(*p) >= 0 && (*p != 0))
			p = GetNextCharPointer(p);
		Delete(0, (int32_)(p - _chars));
	}
	void TrimRightWithCharSet(const CStringBase &charSet)
	{
		const T *p = _chars;
		const T *pLast = NULL;
		while (*p != 0)
		{
			if (charSet.Find(*p) >= 0)
			{
				if (pLast == NULL)
					pLast = p;
			}
			else
				pLast = NULL;
			p = GetNextCharPointer(p);
		}
		if (pLast != NULL)
		{
			int32_ i = (int32_)(pLast - _chars);
			Delete(i, _length - i);
		}

	}
	void MoveItems(int32_ destIndex, int32_ srcIndex)
	{
		memmove(_chars + destIndex, _chars + srcIndex,
				sizeof(T) * (_length - srcIndex + 1));
	}
  
	void InsertSpace(int32_ &index, int32_ size)
	{
		CorrectIndex(index);
		GrowLength(size);
		MoveItems(index + size, index);
	}

  static T *GetNextCharPointer(T *p)
    { return MyStringGetNextCharPointer(p); }
  static const T *GetNextCharPointer(const T *p)
    { return MyStringGetNextCharPointer(p); }
  static T *GetPrevCharPointer(T *base, T *p)
    { return MyStringGetPrevCharPointer(base, p); }
  static const T *GetPrevCharPointer(const T *base, const T *p)
    { return MyStringGetPrevCharPointer(base, p); }
protected:
	T *_chars;
	int32_ _length;
	int32_ _capacity;
  
	void SetCapacity(int32_ newCapacity)
	{
		int32_ realCapacity = newCapacity + 1;
		if (realCapacity == _capacity)
			return;
		/*
		const int32_ kMaxStringSize = 0x20000000;
		if (newCapacity > kMaxStringSize || newCapacity < _length)
		throw 1052337;
		*/
		T *newBuffer = new T[realCapacity];
		if (_capacity > 0)
		{
			for (int32_ i = 0; i < _length; i++)
				newBuffer[i] = _chars[i];
			delete []_chars;
		}
		_chars = newBuffer;
		_chars[_length] = 0;
		_capacity = realCapacity;
	}

	void GrowLength(int32_ n)
	{
		int32_ freeSize = _capacity - _length - 1;
		if (n <= freeSize)
			return;
		int32_ delta;
		if (_capacity > 64)
			delta = _capacity / 2;
		else if (_capacity > 8)
			delta = 16;
		else
			delta = 4;
		if (freeSize + delta < n)
			delta = n - freeSize;
		SetCapacity(_capacity + delta);
	}

	void CorrectIndex(int32_ &index) const
	{
		if (index > _length)
			index = _length;
	}

public:
	CStringBase(): _chars(0), _length(0), _capacity(0) { SetCapacity(3); }
	CStringBase(T c):  _chars(0), _length(0), _capacity(0)
	{
		SetCapacity(1);
		_chars[0] = c;
		_chars[1] = 0;
		_length = 1;
	}
	CStringBase(const T *chars): _chars(0), _length(0), _capacity(0)
	{
		int32_ length = MyStringLen(chars);
		SetCapacity(length);
		MyStringCopy(_chars, chars); // can be optimized by memove()
		_length = length;
	}
	CStringBase(const CStringBase &s):  _chars(0), _length(0), _capacity(0)
	{
		SetCapacity(s._length);
		MyStringCopy(_chars, s._chars);
		_length = s._length;
	}
	~CStringBase() {  delete []_chars; }

	operator const T*() const { return _chars;}

	T Back() const { return _chars[_length - 1]; }

  // The minimum size of the character buffer in characters.
  // This value does not include space for a null terminator.
	T* GetBuffer(int32_ minBufLength)
	{
		if (minBufLength >= _capacity)
			SetCapacity(minBufLength);
		return _chars;
	}
	void ReleaseBuffer() { ReleaseBuffer(MyStringLen(_chars)); }
	void ReleaseBuffer(int32_ newLength)
	{
		_chars[newLength] = 0;
		_length = newLength;
	}

	CStringBase& operator=(T c)
	{
		Empty();
		SetCapacity(1);
		_chars[0] = c;
		_chars[1] = 0;
		_length = 1;
		return *this;
	}
	CStringBase& operator=(const T *chars)
	{
		Empty();
		int32_ length = MyStringLen(chars);
		SetCapacity(length);
		MyStringCopy(_chars, chars);
		_length = length;
		return *this;
	}
	CStringBase& operator=(const CStringBase& s)
	{
		if (&s == this)
			return *this;
		Empty();
		SetCapacity(s._length);
		MyStringCopy(_chars, s._chars);
		_length = s._length;
		return *this;
	}
  
	CStringBase& operator+=(T c)
	{
		GrowLength(1);
		_chars[_length] = c;
		_chars[++_length] = 0;
		return *this;
	}
	CStringBase& operator+=(const T *s)
	{
		int32_ len = MyStringLen(s);
		GrowLength(len);
		MyStringCopy(_chars + _length, s);
		_length += len;
		return *this;
	}
	CStringBase& operator+=(const CStringBase &s)
	{
		GrowLength(s._length);
		MyStringCopy(_chars + _length, s._chars);
		_length += s._length;
		return *this;
	}
	void Empty()
	{
		_length = 0;
		_chars[0] = 0;
	}
	int32_ Length() const { return _length; }
	bool IsEmpty() const { return (_length == 0); }

	CStringBase Mid(int32_ startIndex) const
		{ return Mid(startIndex, _length - startIndex); }
	CStringBase Mid(int32_ startIndex, int32_ count) const
	{
		if (startIndex + count > _length)
			count = _length - startIndex;

		if (startIndex == 0 && startIndex + count == _length)
			return *this;

		CStringBase<T> result;
		result.SetCapacity(count);
		// MyStringNCopy(result._chars, _chars + startIndex, count);
		for (int32_ i = 0; i < count; i++)
			result._chars[i] = _chars[startIndex + i];
		result._chars[count] = 0;
		result._length = count;
		return result;
	}
	CStringBase Left(int32_ count) const
		{ return Mid(0, count); }
	CStringBase Right(int32_ count) const
	{
		if (count > _length)
			count = _length;
		return Mid(_length - count, count);
	}

	void MakeUpper()
		{ MyStringUpper(_chars); }
	void MakeLower()
		{ MyStringLower(_chars); }

	int32_ Compare(const CStringBase& s) const
		{ return MyStringCompare(_chars, s._chars); }

	int32_ Compare(const T *s) const
		{ return MyStringCompare(_chars, s); }

	int32_ CompareNoCase(const CStringBase& s) const
		{ return MyStringCompareNoCase(_chars, s._chars); }

	int32_ CompareNoCase(const T *s) const
		{ return MyStringCompareNoCase(_chars, s); }

  /*
  int32_ Collate(const CStringBase& s) const
    { return MyStringCollate(_chars, s._chars); }
  int32_ CollateNoCase(const CStringBase& s) const
    { return MyStringCollateNoCase(_chars, s._chars); }
  */

	int32_ Find(T c) const { return Find(c, 0); }
	int32_ Find(T c, int32_ startIndex) const
	{
		const T *p = _chars + startIndex;
		for (;;)
		{
			if (*p == c)
				return (int32_)(p - _chars);
			if (*p == 0)
				return -1;
			p = GetNextCharPointer(p);
		}
	}
	int32_ Find(const CStringBase &s) const { return Find(s, 0); }
	int32_ Find(const CStringBase &s, int32_ startIndex) const
	{
		if (s.IsEmpty())
			return startIndex;
		for (; startIndex < _length; startIndex++)
		{
			int32_ j;
			for (j = 0; j < s._length && startIndex + j < _length; j++)
				if (_chars[startIndex+j] != s._chars[j])
					break;
			if (j == s._length)
				return startIndex;
		}
		return -1;
	}
	int32_ ReverseFind(T c) const
	{
		if (_length == 0)
			return -1;
		const T *p = _chars + _length - 1;
		for (;;)
		{
			if (*p == c)
				return (int32_)(p - _chars);
			if (p == _chars)
				return -1;
			p = GetPrevCharPointer(_chars, p);
		}
	}
	int32_ FindOneOf(const CStringBase &s) const
	{
		for (int32_ i = 0; i < _length; i++)
			if (s.Find(_chars[i]) >= 0)
				return i;
		return -1;
	}

	void TrimLeft(T c)
	{
		const T *p = _chars;
		while (c == *p)
			p = GetNextCharPointer(p);
		Delete(0, p - _chars);
	}
	
private:
	CStringBase GetTrimDefaultCharSet()
	{
		CStringBase<T> charSet;
		charSet += (T)' ';
		charSet += (T)'\n';
		charSet += (T)'\t';
		return charSet;
	}
	
public:

	void TrimLeft()
	{
		TrimLeftWithCharSet(GetTrimDefaultCharSet());
	}
	void TrimRight()
	{
		TrimRightWithCharSet(GetTrimDefaultCharSet());
	}
	void TrimRight(T c)
	{
		const T *p = _chars;
		const T *pLast = NULL;
		while (*p != 0)
		{
			if (*p == c)
			{
				if (pLast == NULL)
					pLast = p;
			}
			else
				pLast = NULL;
			p = GetNextCharPointer(p);
		}
		if (pLast != NULL)
		{
			int32_ i = pLast - _chars;
			Delete(i, _length - i);
		}
	}
	void Trim()
	{
		TrimRight();
		TrimLeft();
	}

	int32_ Insert(int32_ index, T c)
	{
		InsertSpace(index, 1);
		_chars[index] = c;
		_length++;
		return _length;
	}
	int32_ Insert(int32_ index, const CStringBase &s)
	{
		CorrectIndex(index);
		if (s.IsEmpty())
			return _length;
		int32_ numInsertChars = s.Length();
		InsertSpace(index, numInsertChars);
		for (int32_ i = 0; i < numInsertChars; i++)
			_chars[index + i] = s[i];
		_length += numInsertChars;
		return _length;
	}

  // !!!!!!!!!!!!!!! test it if newChar = '\0'
	int32_ Replace(T oldChar, T newChar)
	{
		if (oldChar == newChar)
			return 0;
		int32_ number  = 0;
		int32_ pos  = 0;
		while (pos < Length())
		{
			pos = Find(oldChar, pos);
			if (pos < 0)
				break;
			_chars[pos] = newChar;
			pos++;
			number++;
		}
		return number;
	}
	int32_ Replace(const CStringBase &oldString, const CStringBase &newString)
	{
		if (oldString.IsEmpty())
			return 0;
		if (oldString == newString)
			return 0;
		int32_ oldStringLength = oldString.Length();
		int32_ newStringLength = newString.Length();
		int32_ number  = 0;
		int32_ pos  = 0;
		while (pos < _length)
		{
			pos = Find(oldString, pos);
			if (pos < 0)
				break;
			Delete(pos, oldStringLength);
			Insert(pos, newString);
			pos += newStringLength;
			number++;
		}
		return number;
	}
	
	int32_ Delete(int32_ index, int32_ count = 1)
	{
		if (index + count > _length)
			count = _length - index;
		if (count > 0)
		{
			MoveItems(index, index + count);
			_length -= count;
		}
		return _length;
	}
	void DeleteBack() { Delete(_length - 1); }
};

template <class T>
CStringBase<T> operator+(const CStringBase<T>& s1, const CStringBase<T>& s2)
{
	CStringBase<T> result(s1);
	result += s2;
	return result;
}

template <class T>
CStringBase<T> operator+(const CStringBase<T>& s, T c)
{
	CStringBase<T> result(s);
	result += c;
	return result;
}

template <class T>
CStringBase<T> operator+(T c, const CStringBase<T>& s)
{
	CStringBase<T> result(c);
	result += s;
	return result;
}

template <class T>
CStringBase<T> operator+(const CStringBase<T>& s, const T * chars)
{
	CStringBase<T> result(s);
	result += chars;
	return result;
}

template <class T>
CStringBase<T> operator+(const T * chars, const CStringBase<T>& s)
{
	CStringBase<T> result(chars);
	result += s;
	return result;
}

template <class T>
bool operator==(const CStringBase<T>& s1, const CStringBase<T>& s2)
	{ return (s1.Compare(s2) == 0); }

template <class T>
bool operator<(const CStringBase<T>& s1, const CStringBase<T>& s2)
	{ return (s1.Compare(s2) < 0); }

template <class T>
bool operator==(const T *s1, const CStringBase<T>& s2)
	{ return (s2.Compare(s1) == 0); }

template <class T>
bool operator==(const CStringBase<T>& s1, const T *s2)
	{ return (s1.Compare(s2) == 0); }

template <class T>
bool operator!=(const CStringBase<T>& s1, const CStringBase<T>& s2)
	{ return (s1.Compare(s2) != 0); }

template <class T>
bool operator!=(const T *s1, const CStringBase<T>& s2)
	{ return (s2.Compare(s1) != 0); }

template <class T>
bool operator!=(const CStringBase<T>& s1, const T *s2)
	{ return (s1.Compare(s2) != 0); }

typedef CStringBase<char_> 	AString;
typedef CStringBase<wchar_> UString;

#ifdef _UNICODE
	typedef UString TString;
#else
	typedef AString TString;
#endif

LWDP_NAMESPACE_END;
#endif
