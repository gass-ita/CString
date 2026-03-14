#include "cstring.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <iostream>

// Constructors and Destructor
String::String(int size)
{
	currSize = size;
	length = 0;
	str = (char *)malloc(sizeof(char) * currSize);
	if (str)
	{
		str[0] = 0;
	}
}

String::String() : String(STRING_DEFAULT_SIZE) {}

String::~String()
{
	free(str);
}

String::String(const String &other)
{
	currSize = other.currSize;
	length = other.length;
	str = (char *)malloc(currSize);
	memcpy(str, other.str, length + 1);
}

// Output
void String::Print() const
{
	if (str)
		printf("%s\n", str);
}

// Setters
void String::Set(int i, char c)
{
	while (i + 1 >= currSize)
	{
		char *tstr = (char *)realloc((void *)str, currSize * 2);
		if (!tstr)
		{
			fprintf(stderr, "ERR: Unable to allocate memory!\n");
			return;
		}
		this->str = tstr;
		currSize = currSize * 2;
	}

	if (c == 0)
	{
		str[i] = 0;
		if (i < length)
			length = i;
		return;
	}

	if (i > length)
	{
		for (int j = length; j < i; j++)
			str[j] = ' ';
	}

	str[i] = c;
	if (i >= length)
	{
		length = i + 1;
		str[length] = 0;
	}
}

void String::Set(int i, const String &s)
{
	int len = s.GetLength();
	for (int j = 0; j < len; j++)
	{
		this->Set(i + j, s.GetChar(j));
	}
}

void String::Set(int i, const char *inputStr)
{
	size_t len = strlen(inputStr);
	for (size_t j = 0; j < len; j++)
	{
		this->Set(i + j, inputStr[j]);
	}
}

// Appenders
void String::Append(char c)
{
	Set(this->length, c);
}

void String::Append(const String &s)
{
	Set(this->length, s);
}

void String::Append(const char *inputStr)
{
	Set(this->length, inputStr);
}

// Modifiers
void String::Flush()
{
	if (str)
	{
		this->str[0] = 0;
		this->length = 0;
	}
}

char String::Pop()
{
	if (length == 0)
		return 0;
	char c = str[length - 1];
	str[--length] = 0;
	return c;
}

void String::Ltrim()
{
	int i = 0;
	while (i < length && isspace((unsigned char)str[i]))
		i++;
	if (i > 0)
	{
		for (int j = i; j < length; j++)
			str[j - i] = str[j];
		length -= i;
		str[length] = 0;
	}
}

void String::Rtrim()
{
	int i = length - 1;
	while (i >= 0 && isspace((unsigned char)str[i]))
		i--;
	str[i + 1] = 0;
	length = i + 1;
}

int String::RCut(int i)
{
	if (i >= length || i < 0)
	{
		return 0;
	}
	int cutLength = length - i;
	this->Set(i, (char)0);
	return cutLength;
}

int String::LCut(int i)
{
	if (i >= length || i < 0)
	{
		return 0;
	}
	int cutLength = i;
	for (int j = i; j < length; j++)
		str[j - i] = str[j];
	length -= cutLength;
	str[length] = 0;
	return cutLength;
}

void String::ToUpper(int i)
{
	if (i >= length || i < 0)
		return;
	str[i] = toupper((unsigned char)str[i]);
}

void String::ToLower(int i)
{
	if (i >= length || i < 0)
		return;
	str[i] = tolower((unsigned char)str[i]);
}

void String::ToUpper()
{
	for (int i = 0; i < length; i++)
	{
		ToUpper(i);
	}
}

void String::ToLower()
{
	for (int i = 0; i < length; i++)
	{
		ToLower(i);
	}
}

String *String::Split(char delimiter, int *count) const
{
	*count = 0;
	if (length == 0)
		return nullptr;

	// 1. Count how many segments we will have
	int segments = 1;
	for (int i = 0; i < length; i++)
	{
		if (str[i] == delimiter)
			segments++;
	}

	// 2. Allocate an array of String objects
	// Note: This calls the default constructor for each element
	String *result = new String[segments];

	int currentSegment = 0;
	String temp; // Temporary buffer for building each piece

	for (int i = 0; i <= length; i++)
	{
		// If we hit the delimiter or the end of the string
		if (str[i] == delimiter || str[i] == '\0')
		{
			result[currentSegment] = temp; // Requires the Assignment Operator!
			temp.Flush();
			currentSegment++;
		}
		else
		{
			temp.Append(str[i]);
		}
	}

	*count = segments;
	return result;
}

// Getters
char String::GetChar(int i) const
{
	if (i >= length || i < 0)
	{
		return 0;
	}
	return str[i];
}

int String::GetLength() const
{
	return this->length;
}

String &String::operator+=(const String &s)
{
	this->Append(s);
	return *this;
}

String &String::operator+=(const char *s)
{
	this->Append(s);
	return *this;
}

String &String::operator*=(int times)
{
	String original = *this;
	*this = original; // Reset to original before appending
	for (int i = 1; i < times; i++)
	{
		this->Append(original);
	}
	return *this;
}

char &String::operator[](int i)
{
	if (i < 0 || i >= length)
		return str[length]; // Return reference to null terminator for out-of-bounds access
	return str[i];
}

const char &String::operator[](int i) const
{
	if (i < 0 || i >= length)
		return str[length]; // Return reference to null terminator for out-of-bounds access
	return str[i];
}

/**
 * @brief Overloads the stream insertion operator to allow use with std::cout.
 * @param os The output stream reference.
 * @param s The String object to print.
 * @return A reference to the output stream for chaining.
 */
std::ostream &operator<<(std::ostream &os, const String &s)
{
	if (s.str)
		os << s.str;
	return os;
}

// Implementation of + (Creates a NEW object)
String operator+(const String &lhs, const String &rhs)
{
	String result(lhs.GetLength() + rhs.GetLength() + 1);
	result.Append(lhs);
	result.Append(rhs);
	return result;
}

String operator+(const String &lhs, const char *rhs)
{
	String result(lhs.GetLength() + strlen(rhs) + 1);
	result.Append(lhs);
	result.Append(rhs);
	return result;
}

String operator+(const char *lhs, const String &rhs)
{
	String result(strlen(lhs) + rhs.GetLength() + 1);
	result.Append(lhs);
	result.Append(rhs);
	return result;
}

// deep copy assignment operator
String &String::operator=(const String &other)
{
	if (this != &other)
	{
		free(str);
		currSize = other.currSize;
		length = other.length;
		str = (char *)malloc(currSize);
		memcpy(str, other.str, length + 1);
	}
	return *this;
}

// move operator
String &String::operator=(String &&other) noexcept
{
	if (this != &other)
	{
		free(str);
		str = other.str;
		length = other.length;
		currSize = other.currSize;

		other.str = nullptr;
		other.length = 0;
		other.currSize = 0;
	}
	return *this;
}

bool String::operator==(const String &other) const
{
	if (length != other.length)
		return false;
	return strcmp(str, other.str) == 0;
}

// Lexicographical comparison use ascii values of characters
bool String::operator<(const String &other) const
{
	return strcmp(str, other.str) < 0;
}
