#include "string.hpp"
#include <iostream>
#include <iomanip>

// ===================== static helper functions =====================

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
    dest[i] = '\0';                 // copy the terminator too
    return dest;
}

// Standard C behaviour: copies at most n chars; if src is shorter than n,
// pads the rest with '\0'; if src has n or more chars, NO terminator is added.
char *String::strncpy(char *dest, const char *src, int n) {
    int i = 0;
    for (; i < n && src[i] != '\0'; ++i)
        dest[i] = src[i];
    for (; i < n; ++i)
        dest[i] = '\0';
    return dest;
}

char *String::strcat(char *dest, const char *src) {
    strcpy(dest + strlen(dest), src);   // start writing at dest's '\0'
    return dest;
}

// Appends at most n chars of src, then ALWAYS adds a terminator.
char *String::strncat(char *dest, const char *src, int n) {
    int start = strlen(dest);
    int i = 0;
    for (; i < n && src[i] != '\0'; ++i)
        dest[start + i] = src[i];
    dest[start + i] = '\0';
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
    dest[len] = '\0';               // terminator stays at the END
}

// Searching for '\0' returns a pointer to the terminator (standard behaviour).
const char *String::strchr(const char *str, char c) {
    for (int i = 0; ; ++i) {
        if (str[i] == c)
            return str + i;
        if (str[i] == '\0')
            return nullptr;
    }
}

// Empty needle matches at the start of haystack (standard behaviour).
const char *String::strstr(const char *haystack, const char *needle) {
    if (needle[0] == '\0')
        return haystack;
    for (int start = 0; haystack[start] != '\0'; ++start) {
        int i = 0;
        // stops at end of needle, or at a mismatch; reaching haystack's
        // '\0' is always a mismatch, so we never read past haystack
        while (needle[i] != '\0' && haystack[start + i] == needle[i])
            ++i;
        if (needle[i] == '\0')
            return haystack + start;
    }
    return nullptr;
}

// ===================== interface methods =====================

String::String(const char *s) {
    // truncate to MAXLEN - 1 chars, no message required;
    // strncat (not strncpy) so we never write past the terminator
    buf[0] = '\0';
    strncat(buf, s, MAXLEN - 1);
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
        std::cout << "ERROR: Index Out Of Bounds" << std::endl;
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
    result += s;                    // += prints the message if needed
    return result;
}

String &String::operator+=(const String &s) {
    int len = size();
    int slen = s.size();
    int room = MAXLEN - 1 - len;    // how many more chars fit
    int n = slen;
    if (slen > room) {
        std::cout << "ERROR: String Capacity Exceeded" << std::endl;
        n = room;                   // truncate
    }
    // n <= slen, so this also works for s += s (we never read
    // the part of buf that strncat is overwriting)
    strncat(buf, s.buf, n);
    return *this;
}

void String::print(std::ostream &out) const {
    out << buf;
}

void String::read(std::istream &in) {
    // setw stops >> from writing more than MAXLEN - 1 chars + '\0'
    in >> std::setw(MAXLEN) >> buf;
}

String::~String() {
    // nothing to do: buf is a member array, not allocated with new
}

std::ostream &operator<<(std::ostream &out, const String &s) {
    s.print(out);
    return out;
}

std::istream &operator>>(std::istream &in, String &s) {
    s.read(in);
    return in;
}
