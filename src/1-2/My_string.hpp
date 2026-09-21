#ifndef MY_STRING_HPP
#define MY_STRING_HPP

#include <iostream>

class MyString {
private:
    char* data;
    int length;
    void allocate_from(const char* str);
    void copy_from(const MyString& other);
    void move_from(MyString& other) noexcept;


public:
    MyString();
    MyString(const char* str);
    MyString(const MyString& other);
    MyString(MyString&& other) noexcept;  
    ~MyString();
    const char* c_str() const { return data; }
    char* c_str()             { return data; }
    
char&       operator[](int i);
const char& operator[](int i) const;


    char get(int i) const;
    void set(int i, char c);
    void set_new_string(const char* str);
    void print() const;
    void read_line();
    int get_length() const { return length; }
    MyString& operator=(const MyString& other);
    MyString& operator=(MyString&& other) noexcept;
    MyString& operator=(const char* str);
    MyString& operator+=(const MyString& other);
    MyString& operator+=(const char* str);
     bool operator==(const MyString& other) const;
    bool operator!=(const MyString& other) const;
    bool operator< (const MyString& other) const;
    bool operator<=(const MyString& other) const;
    bool operator> (const MyString& other) const;
    bool operator>=(const MyString& other) const;

    bool operator==(const char* str) const;
    bool operator!=(const char* str) const;
    bool operator< (const char* str) const;
    bool operator<=(const char* str) const;
};
MyString operator+(const MyString& a, const MyString& b);
MyString operator+(const MyString& a, const char* b);
MyString operator+(const char* a, const MyString& b);
bool operator==(const char* a, const MyString& b);
bool operator!=(const char* a, const MyString& b);
bool operator< (const char* a, const MyString& b);
bool operator<=(const char* a, const MyString& b);
std::ostream& operator<<(std::ostream& os, const MyString& s);
std::istream& operator>>(std::istream& is, MyString& s);




#endif