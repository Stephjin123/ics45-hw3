#include <gtest/gtest.h>

#include "string.hpp"

TEST(StringFunction, strlen) {
    EXPECT_EQ(String::strlen(""), 0);
    EXPECT_EQ(String::strlen("foo"), 3);
    EXPECT_EQ(String::strlen("a"), 1);
    EXPECT_EQ(String::strlen("hello world"), 11);   // space is a char
    EXPECT_EQ(String::strlen("ab\0cd"), 2);         // stops at first '\0'
}

TEST(StringFunction, strcpy) {
    char result[10];
    EXPECT_EQ(String::strcpy(result, "foo"), result);
    EXPECT_STREQ(result, "foo");

    EXPECT_EQ(String::strcpy(result, "a"), result);
    EXPECT_STREQ(result, "a");

    EXPECT_EQ(String::strcpy(result, ""), result);
    EXPECT_STREQ(result, "");

    // longer then shorter: terminator must be copied, not left from before
    String::strcpy(result, "abcdefgh");
    String::strcpy(result, "xy");
    EXPECT_STREQ(result, "xy");
    EXPECT_EQ(result[2], '\0');
}

TEST(StringFunction, strncpy) {
    // n larger than src: whole string copied, terminated, nothing after touched
    char a[20] = "XXXXXXXXXXXXXXXXXXX";
    EXPECT_EQ(String::strncpy(a, "foo", 10), a);
    EXPECT_STREQ(a, "foo");
    EXPECT_EQ(a[4], 'X');               // no padding with '\0'

    // n smaller than src: exactly n chars copied, THEN terminated
    char b[20] = "XXXXXXXXXXXXXXXXXXX";
    EXPECT_EQ(String::strncpy(b, "hello", 3), b);
    EXPECT_STREQ(b, "hel");
    EXPECT_EQ(b[3], '\0');
    EXPECT_EQ(b[4], 'X');               // writes at most n + 1 chars

    // n equal to length of src
    char c[20] = "XXXXXXXXXXXXXXXXXXX";
    String::strncpy(c, "abc", 3);
    EXPECT_STREQ(c, "abc");
    EXPECT_EQ(c[4], 'X');

    // n == 0: empty string, only the terminator written
    char d[20] = "XXXXXXXXXXXXXXXXXXX";
    String::strncpy(d, "abc", 0);
    EXPECT_STREQ(d, "");
    EXPECT_EQ(d[1], 'X');

    // empty src
    char e[20] = "XXXXXXXXXXXXXXXXXXX";
    EXPECT_EQ(String::strncpy(e, "", 5), e);
    EXPECT_STREQ(e, "");
    EXPECT_EQ(e[1], 'X');               // no padding

    // shorter after longer
    char f[20];
    String::strncpy(f, "abcdefgh", 19);
    String::strncpy(f, "xy", 19);
    EXPECT_STREQ(f, "xy");
}

TEST(StringFunction, strcat) {
    char result[20] = "foo";
    EXPECT_EQ(String::strcat(result, "bar"), result);
    EXPECT_STREQ(result, "foobar");

    // append empty
    EXPECT_EQ(String::strcat(result, ""), result);
    EXPECT_STREQ(result, "foobar");

    // append to empty
    char empty[20] = "";
    String::strcat(empty, "abc");
    EXPECT_STREQ(empty, "abc");

    // repeated appends
    char r[20] = "a";
    String::strcat(r, "b");
    String::strcat(r, "c");
    EXPECT_STREQ(r, "abc");
    EXPECT_EQ(String::strlen(r), 3);
}

TEST(StringFunction, strncat) {
    // n smaller than src: only n chars appended, always terminated
    char a[20] = "foo";
    a[4] = 'X'; a[5] = 'X'; a[6] = 'X';
    EXPECT_EQ(String::strncat(a, "barbaz", 3), a);
    EXPECT_STREQ(a, "foobar");

    // n larger than src: whole src appended
    char b[20] = "ab";
    String::strncat(b, "cd", 10);
    EXPECT_STREQ(b, "abcd");

    // n equal to length of src
    char c[20] = "ab";
    String::strncat(c, "cd", 2);
    EXPECT_STREQ(c, "abcd");

    // n == 0: nothing appended, still a valid string
    char d[20] = "ab";
    String::strncat(d, "cd", 0);
    EXPECT_STREQ(d, "ab");

    // dest empty
    char e[20] = "";
    String::strncat(e, "hello", 2);
    EXPECT_STREQ(e, "he");
}

TEST(StringFunction, strcmp) {
    EXPECT_EQ(String::strcmp("", ""), 0);
    EXPECT_EQ(String::strcmp("abc", "abc"), 0);

    EXPECT_LT(String::strcmp("abc", "abd"), 0);     // differ at last char
    EXPECT_GT(String::strcmp("abd", "abc"), 0);
    EXPECT_LT(String::strcmp("abc", "bbc"), 0);     // differ at first char
    EXPECT_GT(String::strcmp("bbc", "abc"), 0);

    EXPECT_LT(String::strcmp("ab", "abc"), 0);      // prefix is smaller
    EXPECT_GT(String::strcmp("abc", "ab"), 0);
    EXPECT_LT(String::strcmp("", "a"), 0);
    EXPECT_GT(String::strcmp("a", ""), 0);

    EXPECT_LT(String::strcmp("Z", "a"), 0);         // 'Z' (90) < 'a' (97)
}

TEST(StringFunction, strncmp) {
    EXPECT_EQ(String::strncmp("abc", "abc", 3), 0);
    EXPECT_EQ(String::strncmp("abcX", "abcY", 3), 0);   // differ after n
    EXPECT_LT(String::strncmp("abcX", "abcY", 4), 0);   // differ at n-1
    EXPECT_GT(String::strncmp("abd", "abc", 3), 0);
    EXPECT_EQ(String::strncmp("abc", "xyz", 0), 0);     // n == 0

    EXPECT_LT(String::strncmp("ab", "abc", 5), 0);      // shorter, n beyond
    EXPECT_GT(String::strncmp("abc", "ab", 5), 0);
    EXPECT_EQ(String::strncmp("ab", "ab", 10), 0);      // equal, n beyond
    EXPECT_EQ(String::strncmp("", "", 3), 0);
}

TEST(StringFunction, reverse_cpy) {
    char result[20];

    String::reverse_cpy(result, "");
    EXPECT_STREQ(result, "");

    String::reverse_cpy(result, "a");
    EXPECT_STREQ(result, "a");

    String::reverse_cpy(result, "ab");
    EXPECT_STREQ(result, "ba");

    String::reverse_cpy(result, "abc");
    EXPECT_STREQ(result, "cba");

    String::reverse_cpy(result, "racecar");
    EXPECT_STREQ(result, "racecar");

    // shorter after longer: terminator in the right place
    String::reverse_cpy(result, "hello world");
    String::reverse_cpy(result, "xyz");
    EXPECT_STREQ(result, "zyx");
    EXPECT_EQ(result[3], '\0');
}

TEST(StringFunction, strchr) {
    const char *s = "hello";

    EXPECT_EQ(String::strchr(s, 'h'), s);           // first char
    EXPECT_EQ(String::strchr(s, 'o'), s + 4);       // last char
    EXPECT_EQ(String::strchr(s, 'l'), s + 2);       // FIRST of two
    EXPECT_EQ(String::strchr(s, 'z'), nullptr);     // not found
    EXPECT_EQ(String::strchr(s, 'H'), nullptr);     // case sensitive
    EXPECT_EQ(String::strchr(s, '\0'), s + 5);      // terminator found

    const char *e = "";
    EXPECT_EQ(String::strchr(e, 'a'), nullptr);
    EXPECT_EQ(String::strchr(e, '\0'), e);
}

TEST(StringFunction, strstr) {
    const char *s = "hello world";

    EXPECT_EQ(String::strstr(s, "hello"), s);           // at start
    EXPECT_EQ(String::strstr(s, "o w"), s + 4);         // middle
    EXPECT_EQ(String::strstr(s, "world"), s + 6);       // at end
    EXPECT_EQ(String::strstr(s, "d"), s + 10);          // last char
    EXPECT_EQ(String::strstr(s, "hello world"), s);     // whole string
    EXPECT_EQ(String::strstr(s, "xyz"), nullptr);       // not found
    EXPECT_EQ(String::strstr(s, "worldX"), nullptr);    // runs off the end
    EXPECT_EQ(String::strstr(s, "hello world!"), nullptr); // needle longer
    EXPECT_EQ(String::strstr(s, ""), s);                // empty needle

    // first occurrence, and restart after partial match
    const char *t = "aaab";
    EXPECT_EQ(String::strstr(t, "aab"), t + 1);
    const char *u = "abcabd";
    EXPECT_EQ(String::strstr(u, "abd"), u + 3);
    EXPECT_EQ(String::strstr(u, "ab"), u);

    const char *e = "";
    EXPECT_EQ(String::strstr(e, "a"), nullptr);
    EXPECT_EQ(String::strstr(e, ""), e);
}
