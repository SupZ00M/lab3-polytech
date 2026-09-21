#include "My_string.hpp"
#include <cstring>
#include <string>
#include <exception>

MyString::MyString() : data(nullptr), length(0) { allocate_from(""); }
MyString::MyString(const char* str) : data(nullptr), length(0) { allocate_from(str); }
MyString::MyString(const MyString& other) : data(nullptr), length(0) { copy_from(other); }
MyString::MyString(MyString&& other) noexcept : data(nullptr), length(0) { move_from(other); }

MyString::~MyString() { delete[] data; }

void MyString::copy_from(const MyString& other) {
    length = other.length;
    data = new char[length + 1];
    std::strcpy(data, other.data);
}

void MyString::move_from(MyString& other) noexcept {
    data = other.data;
    length = other.length;
    other.data = new char[1];
    other.data[0] = '\0';
    other.length = 0;
}

void MyString::allocate_from(const char* str) {
    if (!str) {
        length = 0;
        data = new char[1];
        data[0] = '\0';
    } else {
        length = static_cast<int>(std::strlen(str));
        data = new char[length + 1];
        std::strcpy(data, str);
    }
}

char MyString::get(int i) const {
    if (i < 0 || i >= length) throw std::runtime_error("Index out of range");
    return data[i];
}

void MyString::set(int i, char c) {
    if (i < 0 || i >= length) throw std::runtime_error("Index out of range");
    data[i] = c;
}




void MyString::set_new_string(const char* str) {
    delete[] data;
    if (!str) {
        length = 0;
        data = new char[1];
        data[0] = '\0';
    }
    else {
        length = strlen(str);
        data = new char[length + 1];
        strcpy(data, str);
    }
}

void MyString::print() const {
    std::cout << data << "\n";
}

void MyString::read_line() {
    std::string temp;
    std::getline(std::cin, temp);
    set_new_string(temp.c_str());
}
MyString& MyString::operator=(const MyString& other) {
    if (this != &other) {
        delete[] data;
        length = other.length;
        data = new char[length + 1];
        strcpy(data, other.data);
    }
    return *this;
}

MyString& MyString::operator=(MyString&& other) noexcept {
    if (this == &other) return *this;
    delete[] data;
    move_from(other);
    return *this;
}

MyString& MyString::operator=(const char* str) {
    set_new_string(str);
    return *this;
}





char& MyString::operator[](int i) {
    if (i < 0 || i >= length) throw std::runtime_error("[]: index out of range");
    return data[i];
}

const char& MyString::operator[](int i) const {
    if (i < 0 || i >= length) throw std::runtime_error("[]: index out of range");
    return data[i];
}
MyString& MyString::operator+=(const MyString& other) {
    int new_len = length + other.length;
    char* new_data = new char[new_len + 1];
    std::strcpy(new_data, data);
    std::strcat(new_data, other.data);
    delete[] data;
    data = new_data;
    length = new_len;
    return *this;
}
MyString& MyString::operator+=(const char* str) {
    if (!str) return *this;
    int add = static_cast<int>(std::strlen(str));
    int new_len = length + add;
    char* new_data = new char[new_len + 1];
    std::strcpy(new_data, data);
    std::strcat(new_data, str);
    delete[] data;
    data = new_data;
    length = new_len;
    return *this;
}

MyString operator+(const MyString& a, const MyString& b) { MyString r(a); r += b; return r; }
MyString operator+(const MyString& a, const char* b)     { MyString r(a); r += b; return r; }
MyString operator+(const char* a, const MyString& b)     { MyString r(a); r += b; return r; }

static int cmp(const char* a, const char* b) { return std::strcmp(a, b); }

bool MyString::operator==(const MyString& o) const { return cmp(data, o.data) == 0; }
bool MyString::operator!=(const MyString& o) const { return cmp(data, o.data) != 0; }
bool MyString::operator< (const MyString& o) const { return cmp(data, o.data) <  0; }
bool MyString::operator<=(const MyString& o) const { return cmp(data, o.data) <= 0; }
bool MyString::operator> (const MyString& o) const { return cmp(data, o.data) >  0; }
bool MyString::operator>=(const MyString& o) const { return cmp(data, o.data) >= 0; }

bool MyString::operator==(const char* s) const { return cmp(data, s) == 0; }
bool MyString::operator!=(const char* s) const { return cmp(data, s) != 0; }
bool MyString::operator< (const char* s) const { return cmp(data, s) <  0; }
bool MyString::operator<=(const char* s) const { return cmp(data, s) <= 0; }

bool operator==(const char* a, const MyString& b) { return std::strcmp(a, b.c_str()) == 0; }
bool operator!=(const char* a, const MyString& b) { return std::strcmp(a, b.c_str()) != 0; }
bool operator< (const char* a, const MyString& b) { return std::strcmp(a, b.c_str()) <  0; }
bool operator<=(const char* a, const MyString& b) { return std::strcmp(a, b.c_str()) <= 0; }
std::ostream& operator<<(std::ostream& os, const MyString& s) {
    os << s.c_str();
    return os;
}

std::istream& operator>>(std::istream& is, MyString& s) {
    std::string tmp;
    std::getline(is, tmp);   // до первого переноса строки
    s.set_new_string(tmp.c_str());
    return is;
}