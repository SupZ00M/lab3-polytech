#include "WorkerDb.hpp"

#include <iostream>

static int failures = 0;
#define CHECK(cond) do { if (!(cond)) { \
    std::cerr << "FAIL: " #cond " at " << __FILE__ << ":" << __LINE__ << "\n"; \
    ++failures; } } while (0)

int main() {

    {
        WorkerDb db;
        db["Ivanov"] = WorkerData("Ivan", 34, "Engineer", 80000.0);
        db["Petrov"] = WorkerData("Petr", 43, "Manager",  95000.0);

        CHECK(db["Ivanov"].name == "Ivan");
        CHECK(db["Petrov"].age == 43);
        CHECK(db.size() == 2);

      
        db["Sidorov"].name = "Sidor";
        CHECK(db.size() == 3);
    }

    
    {
        WorkerDb db;
        db["A"] = WorkerData("Alice", 20, "", 0);
        db["B"] = WorkerData("Bob",   30, "", 0);
        db["C"] = WorkerData("Carol", 40, "", 0);

        int count = 0;
        int sum_age = 0;
        for (auto it = db.begin(); it != db.end(); ++it) {
            ++count;
            sum_age += it->age;
        }
        CHECK(count == 3);
        CHECK(sum_age == 90);
    }

    
    {
        WorkerDb db;
        db["A"] = WorkerData("Alice", 20, "", 0);
        db["B"] = WorkerData("Bob",   40, "", 0);

        CHECK(get_avg_age(db) == 30.0);
    }

   
    {
        WorkerDb db;
        CHECK(db.size() == 0);
        CHECK(get_avg_age(db) == 0.0);
    }

    if (failures) { std::cerr << failures << " check(s) failed\n"; return 1; }
    std::cout << "test_worker_db: OK\n";
    return 0;
}