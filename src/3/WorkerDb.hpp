#ifndef WORKER_DB_HPP
#define WORKER_DB_HPP

#include "My_string.hpp"
#include "WorkerData.hpp"

class WorkerDb {
public:

    struct Entry {
        MyString    key;      
        WorkerData  data;
    };

private:
    Entry* entries;   
    int    count;      
    int    capacity; 

    void ensure_capacity(int needed);
    int  find_index(const MyString& key) const;  
    void reallocate(int new_cap);

public:

    WorkerDb();
    WorkerDb(const WorkerDb& other);
    WorkerDb(WorkerDb&& other) noexcept;
    ~WorkerDb();

    WorkerDb& operator=(const WorkerDb& other);
    WorkerDb& operator=(WorkerDb&& other) noexcept;

  
    WorkerData& operator[](const MyString& key);
   

    
    int size() const { return count; }

    // ---------- Итератор (задание 3.2) ----------
    class Iterator {
    private:
        Entry* ptr;
    public:
        Iterator() : ptr(nullptr) {}
        explicit Iterator(Entry* p) : ptr(p) {}

        // Разыменование
        WorkerData& operator*()  const { return ptr->data; }
        WorkerData* operator->() const { return &ptr->data; }

        // Ключ
        const MyString& key() const { return ptr->key; }

        // Инкремент — префиксный и постфиксный
        Iterator& operator++()    { ++ptr; return *this; }
        Iterator  operator++(int) { Iterator tmp(*this); ++ptr; return tmp; }

        // Сравнение
        bool operator==(const Iterator& o) const { return ptr == o.ptr; }
        bool operator!=(const Iterator& o) const { return ptr != o.ptr; }
    };

    Iterator begin() { return Iterator(entries); }
    Iterator end()   { return Iterator(entries + count); }
};
void print_db(WorkerDb& db);

// Средний возраст
double get_avg_age(WorkerDb& db);

#endif