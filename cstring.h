/**
 * @file cstring.h
 * @brief A custom dynamic string class implementation using manual memory management.
 * * This class provides a C++ wrapper around C-style memory allocation (malloc/realloc)
 * to handle dynamic strings with automatic resizing and various utility methods.
 */

#ifndef STRING_H
#define STRING_H

#include <cstdio>
#include <cstring>
#include <cstdlib>
#include <cctype>
#include <ostream>
#include <vector>

/** @brief The default buffer size for a new String object if not specified. */
#define STRING_DEFAULT_SIZE 64

class String
{
private:
    int length;   /**< Actual number of characters in the string (excluding null terminator) */
    int currSize; /**< Total capacity of the currently allocated memory buffer */
    char *str;    /**< Pointer to the heap-allocated character array */

public:
    // --- Constructors and Destructor ---

    /** @brief Initializes a String with a specific buffer size. */
    String(int size);

    /** @brief Initializes a String with the default size (64). */
    String();

    /** @brief Initializes a String using printf-style formatting. */
    String(const char *fmt, ...);

    /** @brief Copy Constructor: Performs a deep copy of another String's data. */
    String(const String &other);

    /** @brief Destructor: Frees the dynamically allocated memory. */
    ~String();

    // --- Output ---

    /** @brief Prints the string followed by a newline to stdout. */
    void Print() const;

    // --- Setters ---
    void Sync() { length = strlen(str); }

    void Optimize() { Reserve(length + 1); };
    /** * @brief Resizes the buffer to exactly newCapacity.
     * If newCapacity is smaller than current length, the string is truncated.
     */
    void Reserve(int newCapacity);

    /** @brief Sets a character at index i. Automatically expands buffer if needed. */
    void Set(int i, char c);

    /** @brief Overwrites/inserts a String object at index i. */
    void Set(int i, const String &s);

    /** @brief Overwrites/inserts a C-string at index i. */
    void Set(int i, const char *str);

    // --- Appenders ---

    /** @brief Appends a single character to the end of the string. */
    void Append(char c);

    /** @brief Appends a String object to the end of the string. */
    void Append(const String &s);

    /** @brief Appends a C-string to the end of the string. */
    void Append(const char *str);

    // --- Const Appenders (Return a new String) ---
    /** @brief Returns a new String with a character appended. */
    String Concat(char c) const;
    /** @brief Returns a new String with another String appended. */
    String Concat(const String &s) const;
    /** @brief Returns a new String with a C-string appended. */
    String Concat(const char *str) const;

    // --- Modifiers ---

    /** @brief Resets the string to empty without freeing the buffer. */
    void Flush();

    /** @brief Removes and returns the last character of the string. */
    char Pop();

    /** @brief Removes all leading whitespace characters. */
    void Ltrim();

    /** @brief Removes all trailing whitespace characters. */
    void Rtrim();

    /** @brief Truncates the string at index i. Returns the number of characters removed. */
    int RCut(int i);

    /** @brief Removes the first i characters. Returns the number of characters removed. */
    int LCut(int i);

    /** @brief Converts character at index i to uppercase. */
    void ToUpper(int i);

    /** @brief Converts character at index i to lowercase. */
    void ToLower(int i);

    /** @brief Converts the entire string to uppercase. */
    void ToUpper();

    /** @brief Converts the entire string to lowercase. */
    void ToLower();

    void Capitalize()
    {
        this->ToLower();
        this->ToUpper(0);
    }

    int Find(const String &s, int start = 0) const;
    bool Contains(const String &s) const { return Find(s) != -1; }
    bool StartsWith(const String &s) const;
    bool EndsWith(const String &s) const;

    int Count(char c) const;

    // --- Getters ---

    /** @brief Returns character at index i, or '\0' if out of bounds. */
    char GetChar(int i) const;

    /** @brief Returns the current length of the string (excluding null terminator). */
    int GetLength() const;

    /** * @brief Splits the string into an array of Strings based on a delimiter.
     * @param delimiter The character to split the string on.
     * @return A vector of String objects resulting from the split.
     */
    std::vector<String> Split(char delimiter) const;

    // --- Operator Overloading ---

    /** @brief Appends another String using the += operator. */
    String &operator+=(const String &s);

    /** @brief Appends a C-string using the += operator. */
    String &operator+=(const char *s);

    /** @brief Repeats the string content 'times' number of times. */
    String &operator*=(int times);

    /** @brief Accesses character at index i (Read/Write). */
    char &operator[](int i);

    /** @brief Accesses character at index i (Read Only). */
    const char &operator[](int i) const;

    /** @brief Copy Assignment: Safely copies data from another String. */
    String &operator=(const String &other);

    /** @brief Move Assignment: Efficiently transfers ownership of memory from another String. */
    String &operator=(String &&other) noexcept;

    // Add this inside the public: section of the class
    friend std::ostream &operator<<(std::ostream &os, const String &s);

    // Add these to the class as friends so they can access 'str' and 'length'
    friend std::istream &operator>>(std::istream &is, String &s);
    // --- Comparison Operators ---

    bool operator==(const String &other) const;
    bool operator!=(const String &other) const { return !(*this == other); }
    bool operator<(const String &other) const;
    bool operator>(const String &other) const { return other < *this; }
    bool operator<=(const String &other) const { return !(other < *this); }
    bool operator>=(const String &other) const { return !(*this < other); }

    // iterator support
    char *begin() { return str; }
    char *end() { return str + length; }
    const char *begin() const { return str; }
    const char *end() const { return str + length; }

    // Debug
    void PrintInfo() const;
};

// --- Non-member Overloads ---

/** @brief Concatenates two String objects. */
String operator+(const String &lhs, const String &rhs);

/** @brief Concatenates a String and a C-string. */
String operator+(const String &lhs, const char *rhs);

/** @brief Concatenates a C-string and a String. */
String operator+(const char *lhs, const String &rhs);

// Non-member global function
std::istream &getline(std::istream &is, String &s, char delim = '\n');
#endif // STRING_H