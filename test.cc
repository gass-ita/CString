#include "cstring.h"
#include <iostream>
#include <cassert>
#include <sstream>
#include <vector>
#include <algorithm>
#include <utility>

// --- Lightweight Test Framework ---
int tests_passed = 0;
int tests_failed = 0;

#define ASSERT_TRUE(condition)                                                                                            \
    do                                                                                                                    \
    {                                                                                                                     \
        if (!(condition))                                                                                                 \
        {                                                                                                                 \
            std::cerr << "[FAIL] " << __FUNCTION__ << " line " << __LINE__ << ": Expected true, got false." << std::endl; \
            tests_failed++;                                                                                               \
        }                                                                                                                 \
        else                                                                                                              \
        {                                                                                                                 \
            tests_passed++;                                                                                               \
        }                                                                                                                 \
    } while (0)

#define ASSERT_EQ(actual, expected)                                                                  \
    do                                                                                               \
    {                                                                                                \
        if ((actual) != (expected))                                                                  \
        {                                                                                            \
            std::cerr << "[FAIL] " << __FUNCTION__ << " line " << __LINE__                           \
                      << ": Expected '" << (expected) << "', got '" << (actual) << "'" << std::endl; \
            tests_failed++;                                                                          \
        }                                                                                            \
        else                                                                                         \
        {                                                                                            \
            tests_passed++;                                                                          \
        }                                                                                            \
    } while (0)

// --- Test Suites ---

void test_constructors_and_memory()
{
    std::cout << "Running test_constructors_and_memory..." << std::endl;

    // Default & Size
    String s1;
    ASSERT_EQ(s1.GetLength(), 0);

    String s2(100);
    ASSERT_EQ(s2.GetLength(), 0);

    // Variadic printf-style
    String s3("Hello %s, number %d", "World", 42);
    ASSERT_TRUE(s3 == String("Hello World, number 42"));

    // Copy Constructor (Deep Copy Check)
    String original("Deep Copy");
    String copy(original);
    ASSERT_TRUE(original == copy);
    copy.Set(0, 'K');
    ASSERT_TRUE(original != copy); // Ensure mutating copy doesn't mutate original

    // Reserve & Optimize
    String s4("Tiny");
    s4.Reserve(1000);
    ASSERT_EQ(s4.GetLength(), 4); // Length shouldn't change
    s4.Optimize();                // Should shrink buffer back down
    ASSERT_EQ(s4.GetLength(), 4);
}

void test_appends_and_concatenations()
{
    std::cout << "Running test_appends_and_concatenations..." << std::endl;

    String s("Start");
    s.Append('-');
    s.Append("Middle");
    s.Append(String("-End"));
    ASSERT_TRUE(s == String("Start-Middle-End"));

    String c1 = s.Concat('!');
    ASSERT_TRUE(c1 == String("Start-Middle-End!"));
    ASSERT_TRUE(s == String("Start-Middle-End")); // Ensure original is unmodified

    String c2 = c1.Concat(String("?"));
    String c3 = c2.Concat("...");
    ASSERT_TRUE(c3 == String("Start-Middle-End!?..."));
}

void test_modifiers_and_formatting()
{
    std::cout << "Running test_modifiers_and_formatting..." << std::endl;

    String s("  \tHello World  \n");
    s.Ltrim();
    ASSERT_TRUE(s == String("Hello World  \n"));

    s.Rtrim();
    ASSERT_TRUE(s == String("Hello World"));

    s.ToUpper();
    ASSERT_TRUE(s == String("HELLO WORLD"));

    s.ToLower();
    ASSERT_TRUE(s == String("hello world"));

    s.Capitalize();
    ASSERT_TRUE(s == String("Hello world"));

    s.ToUpper(6);
    ASSERT_TRUE(s == String("Hello World"));

    // Cut tests
    String cut_test("0123456789");
    int removed = cut_test.RCut(8); // truncate at index 8
    ASSERT_EQ(removed, 2);
    ASSERT_TRUE(cut_test == String("01234567"));

    removed = cut_test.LCut(2); // remove first 2
    ASSERT_EQ(removed, 2);
    ASSERT_TRUE(cut_test == String("234567"));

    char popped = cut_test.Pop();
    ASSERT_EQ(popped, '7');
    ASSERT_TRUE(cut_test == String("23456"));

    cut_test.Flush();
    ASSERT_EQ(cut_test.GetLength(), 0);
}

void test_setters()
{
    std::cout << "Running test_setters..." << std::endl;

    String s("abc");
    s.Set(1, 'x');
    ASSERT_TRUE(s == String("axc"));

    // Out of bounds set (Should trigger auto-expansion. Assuming it pads with spaces or nulls.
    // We will just check if the length expanded and the char is there).
    s.Set(10, 'Z');
    ASSERT_TRUE(s.GetLength() >= 11);
    ASSERT_EQ(s.GetChar(10), 'Z');

    String s2("Hello");
    s2.Set(1, "xxx"); // Hxxxo or Hxxx? Depends on your impl, assuming overwrite.
    ASSERT_EQ(s2.GetChar(1), 'x');
}

void test_search_and_split()
{
    std::cout << "Running test_search_and_split..." << std::endl;

    String s("apple,banana,orange,apple");

    ASSERT_EQ(s.Find(String("banana")), 6);
    ASSERT_EQ(s.Find(String("grape")), -1);

    ASSERT_TRUE(s.Contains(String("orange")));
    ASSERT_TRUE(s.StartsWith(String("apple")));
    ASSERT_TRUE(s.EndsWith(String("apple")));
    ASSERT_EQ(s.Count('a'), 6);

    std::vector<String> tokens = s.Split(',');
    ASSERT_EQ(tokens.size(), 4);
    if (tokens.size() == 4)
    {
        ASSERT_TRUE(tokens[0] == String("apple"));
        ASSERT_TRUE(tokens[1] == String("banana"));
        ASSERT_TRUE(tokens[2] == String("orange"));
        ASSERT_TRUE(tokens[3] == String("apple"));
    }
}

void test_operators_and_iterators()
{
    std::cout << "Running test_operators_and_iterators..." << std::endl;

    // Operator +=
    String s1("A");
    s1 += "B";
    s1 += String("C");
    ASSERT_TRUE(s1 == String("ABC"));

    // Operator *=
    s1 *= 3;
    ASSERT_TRUE(s1 == String("ABCABCABC"));

    // Operator []
    ASSERT_EQ(s1[0], 'A');
    s1[0] = 'Z';
    ASSERT_EQ(s1[0], 'Z');

    // Move semantics
    String temp("Temporary");
    String moved_to = std::move(temp);
    ASSERT_TRUE(moved_to == String("Temporary"));
    // Note: temp is now in a valid but unspecified state.

    // Iterators
    String it_test("123");
    int sum = 0;
    for (char c : it_test)
    {
        sum += (c - '0');
    }
    ASSERT_EQ(sum, 6);

    // Std Algorithms compatibility
    std::reverse(it_test.begin(), it_test.end());
    ASSERT_TRUE(it_test == String("321"));
}

void test_streams_and_globals()
{
    std::cout << "Running test_streams_and_globals..." << std::endl;

    // Global +
    String s1 = String("Hello ") + String("World");
    String s2 = String("Hello ") + "World";
    String s3 = "Hello " + String("World");

    ASSERT_TRUE(s1 == String("Hello World"));
    ASSERT_TRUE(s2 == String("Hello World"));
    ASSERT_TRUE(s3 == String("Hello World"));

    // Streams
    std::stringstream ss;
    String stream_str("StreamTest");
    ss << stream_str;

    String read_str;
    ss >> read_str;
    ASSERT_TRUE(read_str == String("StreamTest"));

    // Getline
    std::stringstream multi_line("Line1\nLine2\nLine3");
    String line;
    getline(multi_line, line);
    ASSERT_TRUE(line == String("Line1"));
}

void test_stress_and_edge_cases()
{
    std::cout << "Running test_stress_and_edge_cases..." << std::endl;

    // Heavy reallocation stress test
    String heavy;
    for (int i = 0; i < 10000; ++i)
    {
        heavy.Append('X');
    }
    ASSERT_EQ(heavy.GetLength(), 10000);
    ASSERT_EQ(heavy.Count('X'), 10000);

    // Empty string edge cases
    String empty;
    ASSERT_EQ(empty.Pop(), '\0'); // Assuming safe pop
    ASSERT_EQ(empty.RCut(5), 0);
    ASSERT_EQ(empty.Find(String("A")), -1);

    std::vector<String> split_empty = empty.Split(',');
    // Depending on implementation, this could be 0 or 1. Assuming 0 for a truly empty string.
    ASSERT_TRUE(split_empty.size() <= 1);
}

int main()
{
    std::cout << "--- Starting String Class Test Suite ---" << std::endl;

    test_constructors_and_memory();
    test_appends_and_concatenations();
    test_modifiers_and_formatting();
    test_setters();
    test_search_and_split();
    test_operators_and_iterators();
    test_streams_and_globals();
    test_stress_and_edge_cases();

    std::cout << "----------------------------------------" << std::endl;
    std::cout << "Tests Passed: " << tests_passed << std::endl;
    std::cout << "Tests Failed: " << tests_failed << std::endl;

    if (tests_failed == 0)
    {
        std::cout << "\n✅ ALL TESTS PASSED! Your memory management is holding up." << std::endl;
        return 0;
    }
    else
    {
        std::cout << "\n❌ SOME TESTS FAILED. Check the logs above for memory leaks or logic bugs." << std::endl;
        return 1;
    }
}