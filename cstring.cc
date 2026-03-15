#include "cstring.h"
#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <iostream>
#include <vector>
#include <cstdarg>

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

String::String(const char *fmt, ...) : String()
{
	if (!fmt)
		return;

	va_list args;

	// 1. First pass: Determine how much space we need
	va_start(args, fmt);
	// Passing nullptr and 0 tells vsnprintf to just return the needed length
	int needed = vsnprintf(nullptr, 0, fmt, args);
	va_end(args);

	if (needed < 0)
		return; // Formatting error

	// 2. Prepare the buffer
	// Ensure we have enough room (needed + 1 for null terminator)
	if (needed >= currSize)
	{
		char *temp = (char *)realloc(str, needed + 1);
		if (temp)
		{
			str = temp;
			currSize = needed + 1;
		}
		else
		{
			return; // Memory allocation failed
		}
	}

	// 3. Second pass: Actually write the string
	va_start(args, fmt);
	vsnprintf(str, currSize, fmt, args);
	va_end(args);

	length = needed;
}

String::String(const String &other)
{
	currSize = other.currSize;
	length = other.length;
	str = (char *)malloc(currSize);
	memcpy(str, other.str, length + 1);
}

// String::String() {}

// Output
void String::Print() const
{
	if (str)
		printf("%s\n", str);
}

// Setters

void String::Reserve(int newCapacity)
{
	// printf("Reserving new capacity: %d\n", newCapacity);
	// 1. Safety check: we need at least 1 byte for a null terminator
	if (newCapacity < 1)
		newCapacity = 1;

	// 2. If the new capacity is smaller than our string, we must truncate
	// (newCapacity - 1) is the maximum index we can keep
	if (newCapacity <= length)
	{
		length = newCapacity - 1;
		// The null terminator will be placed after realloc
	}

	// 3. Perform the resize
	char *temp = (char *)realloc(str, newCapacity);

	if (temp)
	{
		str = temp;
		currSize = newCapacity;

		// 4. Ensure the string is correctly terminated at the new length
		str[length] = '\0';
	}
	else
	{
		// realloc failed - we keep the old pointer and size
		fprintf(stderr, "Error: Reserve failed to allocate %d bytes.\n", newCapacity);
	}
}

void String::Set(int i, char c)
{
	if (i + 1 >= currSize)
	{
		int newCap = currSize;
		if (newCap == 0)
			newCap = STRING_DEFAULT_SIZE;
		while (i + 1 >= newCap)
			newCap *= 2;

		this->Reserve(newCap);
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

// Const appenders
String String::Concat(char c) const
{
	String result = *this;
	result.Append(c);
	return result;
}

String String::Concat(const String &s) const
{
	String result = *this;
	result.Append(s);
	return result;
}

String String::Concat(const char *inputStr) const
{
	String result = *this;
	result.Append(inputStr);
	return result;
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

int String::Find(const String &s, int start) const
{
	if (start < 0 || start >= length)
		return -1;
	int sLen = s.GetLength();
	if (sLen == 0)
		return start; // Empty string is found at the starting index

	for (int i = start; i <= length - sLen; i++)
	{
		int j;
		for (j = 0; j < sLen; j++)
		{
			if (str[i + j] != s.GetChar(j))
				break;
		}
		if (j == sLen)
			return i; // Found a match
	}
	return -1; // No match found
}

bool String::StartsWith(const String &s) const
{
	int sLen = s.GetLength();
	if (sLen > length)
		return false;
	for (int i = 0; i < sLen; i++)
	{
		if (str[i] != s.GetChar(i))
			return false;
	}
	return true;
}

bool String::EndsWith(const String &s) const
{
	int sLen = s.GetLength();
	if (sLen > length)
		return false;
	for (int i = 0; i < sLen; i++)
	{
		if (str[length - sLen + i] != s.GetChar(i))
			return false;
	}
	return true;
}

int String::Count(char c) const
{
	int count = 0;
	for (int i = 0; i < length; i++)
	{
		if (str[i] == c)
			count++;
	}
	return count;
}

std::vector<String> String::Split(char delimiter) const
{
	std::vector<String> result;

	if (length == 0)
	{
		return result; // Returns an empty vector
	}

	String temp; // Temporary buffer for building each piece

	for (int i = 0; i <= length; i++)
	{
		// If we hit the delimiter or the null terminator (end of string)
		if (str[i] == delimiter || str[i] == '\0')
		{
			// Use std::move to trigger your move assignment operator
			// This is much faster than a deep copy!
			result.push_back(std::move(temp));

			// Reset temp for the next segment
			temp.Flush();
		}
		else
		{
			temp.Append(str[i]);
		}
	}

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

/** * @brief Reads a single word from the stream.
 * Skips leading whitespace, then reads until next whitespace.
 */
std::istream &operator>>(std::istream &is, String &s)
{
	s.Flush(); // Clear existing content
	char c;

	// Skip leading whitespace
	while (is.get(c) && isspace(c))
		;

	if (is.eof())
		return is;

	// Read until we hit whitespace
	do
	{
		if (isspace(c))
		{
			is.putback(c); // Put the whitespace back for the next read
			break;
		}
		s.Append(c);
	} while (is.get(c));

	return is;
}

/** * @brief Reads a full line of text.
 */
std::istream &getline(std::istream &is, String &s, char delim)
{
	s.Flush();
	char c;

	while (is.get(c))
	{
		if (c == delim)
		{
			break; // Stop at delimiter, but don't put it back
		}
		s.Append(c);
	}

	return is;
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

void String::PrintInfo() const
{
	printf("--- String Debug Info ---\n");
	printf("Address:  %p\n", (void *)str);
	printf("Length:   %d\n", length);
	printf("Capacity: %d\n", currSize);

	// Print first 10 characters
	printf("Content:  \"");
	for (int j = 0; j < 10 && j < length; j++)
	{
		// Handle non-printable characters or spaces visually
		if (str[j] == ' ')
			printf("[ ]");
		else if (str[j] == '\0')
			printf("\\0");
		else
			printf("%c", str[j]);
	}

	if (length > 10)
		printf("...");
	printf("\"\n");
	printf("-------------------------\n");
}
