#include <iostream>
#include <iomanip>
#include "string.hpp"
using namespace std;

// ===================== static helper functions =====================
// All loops through C strings live here; the interface methods below
// only call these helpers. None of these check MAXLEN.

int String::strlen(const char *s) {
    int len = 0;
    while (s[len] != '\0')
        ++len;
    return len;
}

char *String::strcpy(char *dest, const char *src) {
    int i = 0;
    for (; src[i] != '\0'; ++i)
        dest[i] = src[i];
    dest[i] = '\0';
    return dest;
}

// Homework version (NOT the C standard one): copies at most n chars,
// then ALWAYS adds a terminator, so dest needs room for n + 1 chars.
char *String::strncpy(char *dest, const char *src, int n) {
    int i = 0;
    for (; i < n && src[i] != '\0'; ++i)
        dest[i] = src[i];
    dest[i] = '\0';
    return dest;
}

char *String::strcat(char *dest, const char *src) {
    strcpy(dest + strlen(dest), src);
    return dest;
}

// Appends at most n chars of src, then always adds a terminator.
char *String::strncat(char *dest, const char *src, int n) {
    strncpy(dest + strlen(dest), src, n);
    return dest;
}

// < 0 if left < right, 0 if equal, > 0 if left > right
int String::strcmp(const char *left, const char *right) {
    int i = 0;
    while (left[i] != '\0' && left[i] == right[i])
        ++i;
    return (unsigned char)left[i] - (unsigned char)right[i];
}

int String::strncmp(const char *left, const char *right, int n) {
    for (int i = 0; i < n; ++i) {
        if (left[i] != right[i] || left[i] == '\0')
            return (unsigned char)left[i] - (unsigned char)right[i];
    }
    return 0;
}

void String::reverse_cpy(char *dest, const char *src) {
    int len = strlen(src);
    for (int i = 0; i < len; ++i)
        dest[i] = src[len - 1 - i];
    dest[len] = '\0';
}

// Searching for '\0' returns a pointer to the terminator.
const char *String::strchr(const char *str, char c) {
    for (int i = 0; ; ++i) {
        if (str[i] == c)
            return str + i;
        if (str[i] == '\0')
            return nullptr;
    }
}

// An empty needle matches at the start of haystack.
const char *String::strstr(const char *haystack, const char *needle) {
    if (needle[0] == '\0')
        return haystack;
    for (int start = 0; haystack[start] != '\0'; ++start) {
        int i = 0;
        // reaching haystack's '\0' is always a mismatch,
        // so we never read past the end of haystack
        while (needle[i] != '\0' && haystack[start + i] == needle[i])
            ++i;
        if (needle[i] == '\0')
            return haystack + start;
    }
    return nullptr;
}

// ===================== interface methods =====================

// the default argument "" appears only in the declaration
String::String(const char *s) {
    // truncate to MAXLEN - 1 chars (room for '\0'), no message required
    strncpy(buf, s, MAXLEN - 1);
}

String::String(const String &s) {
    strcpy(buf, s.buf);
}

String &String::operator=(const String &s) {
    if (this != &s)
        strcpy(buf, s.buf);
    return *this;
}

char &String::operator[](int index) {
    if (!in_bounds(index)) {
        cout << "ERROR: Index Out Of Bounds" << endl;
        return buf[0];
    }
    return buf[index];
}

int String::size() const {
    return strlen(buf);
}

String String::reverse() const {
    String result;
    reverse_cpy(result.buf, buf);
    return result;
}

int String::indexOf(char c) const {
    const char *p = strchr(buf, c);
    return p == nullptr ? -1 : p - buf;
}

int String::indexOf(const String &s) const {
    const char *p = strstr(buf, s.buf);
    return p == nullptr ? -1 : p - buf;
}

bool String::operator==(const String &s) const { return strcmp(buf, s.buf) == 0; }
bool String::operator!=(const String &s) const { return strcmp(buf, s.buf) != 0; }
bool String::operator>(const String &s) const  { return strcmp(buf, s.buf) > 0; }
bool String::operator<(const String &s) const  { return strcmp(buf, s.buf) < 0; }
bool String::operator<=(const String &s) const { return strcmp(buf, s.buf) <= 0; }
bool String::operator>=(const String &s) const { return strcmp(buf, s.buf) >= 0; }

String String::operator+(const String &s) const {
    String result(*this);
    result += s;            // += prints the capacity message if needed
    return result;
}

String &String::operator+=(const String &s) {
    int len = size();
    int slen = s.size();
    int room = MAXLEN - 1 - len;    // how many more chars fit
    int n = slen;
    if (slen > room) {              // exactly full is fine; one more is not
        cout << "ERROR: String Capacity Exceeded" << endl;
        n = room;                   // recover by truncating
    }
    // n <= slen, so s += s is safe: strncat never reads the part
    // of buf it is overwriting
    strncat(buf, s.buf, n);
    return *this;
}

void String::print(ostream &out) const {
    out << buf;
}

void String::read(istream &in) {
    // setw stops >> from writing more than MAXLEN - 1 chars + '\0'
    in >> setw(MAXLEN) >> buf;
}

String::~String() {
    // nothing to do: buf is a member array, not allocated with new
}

ostream &operator<<(ostream &out, const String &s) {
    s.print(out);
    return out;
}

istream &operator>>(istream &in, String &s) {
    s.read(in);
    return in;
}
