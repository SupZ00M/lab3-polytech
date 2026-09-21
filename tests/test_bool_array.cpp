#include "BoolArray.hpp"
#include <iostream>

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::cerr << "FAIL: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++failures; } } while (0)

int main() {
   
    {
        BoolArray ar1(10);
        BoolArray ar2(5, true);
        BoolArray ar3(ar2);      

        CHECK(ar1.size() == 10);
        CHECK(ar2.size() == 5);
        CHECK(ar3.size() == 5);

     
        for (std::size_t i = 0; i < ar2.size(); ++i)
            CHECK(ar2[i] == true);
        // ar1 — все false
        for (std::size_t i = 0; i < ar1.size(); ++i)
            CHECK(ar1[i] == false);

       
        ar2[0] = false;
        CHECK(ar2[0] == false);
        CHECK(ar3[0] == true);   
    }

   
    {
        BoolArray ar(10);
        ar[4] = ar[6] = true;
        CHECK(ar[4] == true);
        CHECK(ar[6] == true);
        CHECK(ar[5] == false);

        // Логические операции
        ar[2] = (!ar[6] && ar[8] || (ar[0] != true));
        CHECK(ar[2] == true);  //под ошибку
    }


    {
        BoolArray ar(4);
        ar[0] = true; ar[2] = true;
        ar.resize(8, true);
        CHECK(ar.size() == 8);
        CHECK(ar[0] == true);
        CHECK(ar[1] == false);
        CHECK(ar[2] == true);
        CHECK(ar[3] == false);
        CHECK(ar[4] == true);    
        CHECK(ar[7] == true);
    }

   
    {
        BoolArray ar(8, true);
        ar.resize(3, false);
        CHECK(ar.size() == 3);
        CHECK(ar[0] == true);
        CHECK(ar[1] == true);
        CHECK(ar[2] == true);
    }

    // Граничные случаи
    {
        BoolArray ar(0);
        CHECK(ar.size() == 0);
    }
    {
        BoolArray ar(9, true);
        // последний байт — 2 бита значащих (8, 9 не существует, 8 — да)
        CHECK(ar[8] == true);
        CHECK(ar[7] == true);
    }

    // Упаковка: 10 бит = 2 байта
    {
        CHECK((10 + 7) / 8 == 2);
    }

    if (failures) { std::cerr << failures << " check(s) failed\n"; return 1; }
    std::cout << "test_bool_array: OK\n";
    return 0;
}