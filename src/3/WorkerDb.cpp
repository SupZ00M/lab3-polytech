#include "WorkerDb.hpp"
#include <stdexcept>
#include <utility>


void WorkerDb::ensure_capacity(int needed) {
    if (needed <= capacity) return;
    int new_cap = (capacity == 0) ? 4 : capacity * 2;
    while (new_cap < needed) new_cap *= 2;
    reallocate(new_cap);
}

void WorkerDb::reallocate(int new_cap) {
    Entry* new_entries = new Entry[new_cap];
    for (int i = 0; i < count; ++i)
        new_entries[i] = std::move(entries[i]);
    delete[] entries;
    entries  = new_entries;
    capacity = new_cap;
}

int WorkerDb::find_index(const MyString& key) const {
    for (int i = 0; i < count; ++i)
        if (entries[i].key == key)
            return i;
    return -1;
}


WorkerDb::WorkerDb()
    : entries(nullptr), count(0), capacity(0) {}

WorkerDb::WorkerDb(const WorkerDb& other)
    : entries(nullptr), count(0), capacity(0) {
    if (other.count == 0) return;
    entries = new Entry[other.capacity];
    capacity = other.capacity;
    for (int i = 0; i < other.count; ++i)
        entries[i] = other.entries[i];
    count = other.count;
}

WorkerDb::WorkerDb(WorkerDb&& other) noexcept
    : entries(other.entries), count(other.count), capacity(other.capacity) {
    other.entries  = nullptr;
    other.count    = 0;
    other.capacity = 0;
}

WorkerDb::~WorkerDb() {
    delete[] entries;
}

WorkerDb& WorkerDb::operator=(const WorkerDb& other) {
    if (this == &other) return *this;
    delete[] entries;
    entries  = nullptr;
    count    = 0;
    capacity = 0;
    if (other.count == 0) return *this;
    entries  = new Entry[other.capacity];
    capacity = other.capacity;
    for (int i = 0; i < other.count; ++i)
        entries[i] = other.entries[i];
    count = other.count;
    return *this;
}

WorkerDb& WorkerDb::operator=(WorkerDb&& other) noexcept {
    if (this == &other) return *this;
    delete[] entries;
    entries       = other.entries;
    count         = other.count;
    capacity      = other.capacity;
    other.entries  = nullptr;
    other.count    = 0;
    other.capacity = 0;
    return *this;
}


WorkerData& WorkerDb::operator[](const MyString& key) {
    int idx = find_index(key);
    if (idx >= 0)
        return entries[idx].data;

   
    ensure_capacity(count + 1);
    entries[count].key  = key;       
    entries[count].data = WorkerData(); 
    ++count;
    return entries[count - 1].data;
}

void print_db(WorkerDb& db) {
    for (auto it = db.begin(); it != db.end(); ++it) {
        std::cout << it.key() << ": "
                  << "name=" << it->name << ", "
                  << "age="  << it->age   << ", "
                  << "position=" << it->position << ", "
                  << "salary=" << it->salary << "\n";
    }
}

double get_avg_age(WorkerDb& db) {
    if (db.size() == 0) return 0.0;
    int sum = 0;
    for (auto it = db.begin(); it != db.end(); ++it)
        sum += it->age;
    return static_cast<double>(sum) / db.size();
}

