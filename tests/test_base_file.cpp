#include "BaseFile.hpp"
#include <iostream>
#include <cstdio>
#include <utility>

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::cerr << "FAIL: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++failures; } } while (0)

int main() {
    const char* fname = "test_base_file.tmp";

   
    {
        BaseFile f1(const_cast<char*>(fname), const_cast<char*>("w"));
        CHECK(f1.is_open() == 1);

        BaseFile f2(std::move(f1));
        CHECK(f1.is_open() == 0);  
        CHECK(f2.is_open() == 1);
    }


    {
        BaseFile a(const_cast<char*>(fname), const_cast<char*>("w"));
        BaseFile b(const_cast<char*>(fname), const_cast<char*>("w"));
        b = std::move(a);
        CHECK(a.is_open() == 0);
        CHECK(b.is_open() == 1);
    }

    
    {
        BaseFile f(const_cast<char*>(fname), const_cast<char*>("w"));
        CHECK(f.write("hello", 5) == 5);
        f.seek(0);
    }
    {
        BaseFile f(const_cast<char*>(fname), const_cast<char*>("r"));
        char buf[16] = {0};
        size_t n = f.read(buf, sizeof(buf));
        CHECK(n == 5);
        CHECK(buf[0] == 'h' && buf[4] == 'o');
    }

    std::remove(fname);
    if (failures) { std::cerr << failures << " check(s) failed\n"; return 1; }
    std::cout << "test_base_file: OK\n";
    return 0;
}