#include "My_string.hpp"
#include <iostream>
#include <cstdlib>
#include <utility>
#include <sstream>

static int failures = 0;

#define CHECK(cond)                                                    \
    do {                                                               \
        if (!(cond)) {                                                 \
            std::cerr << "FAIL: " #cond " at "                         \
                      << __FILE__ << ":" << __LINE__ << "\n";          \
            ++failures;                                                \
        }                                                              \
    } while (0)

int main() {
   
    {
        MyString s1("AAA"), s2;
        s2 = s1;
        s1 = "CCC";
        s2 = MyString("tmp");
        s1 = s1;
        CHECK(s1 == "CCC");
        CHECK(s2 == "tmp");
    }


    {
        MyString s1("abc"), s2("def"), s3;
        s1 += s2 += "111";
        CHECK(s2 == "def111");
        CHECK(s1 == "abcdef111");
        s3 += s3;                    
        CHECK(s3 == "");
    }

   
    {
        MyString s1("abc");
        MyString s2 = s1 + "ccc";
        CHECK(s2 == "abcccc");
        s1 = "abc" + s2;
        CHECK(s1 == "abcabcccc");
        s2 = std::move(s1);
        CHECK(s2 == "abcabcccc");
        CHECK(s1 == "");                
    }

    
    {
        MyString s1("abc"), s2("ab"), s3 = s1;
        CHECK(s2 < s1);
        CHECK(s1 == s3);
        CHECK(s1 != s2);
        CHECK(s1 <= s3);
        CHECK("ab" == s2);
        CHECK("aba" < s1);
        CHECK("aba" <= s3);
    }

  
    {
        MyString s1("abc");
        s1[0] = s1[1] = 'd';
        CHECK(s1 == "ddc");
    }

    {
        MyString s("123");
        std::ostringstream oss;
        oss << s;
        CHECK(oss.str() == "123");
    }

    if (failures) {
        std::cerr << failures << " check(s) failed\n";
        return 1;
    }
    std::cout << "test_my_string: OK\n";
    return 0;
}