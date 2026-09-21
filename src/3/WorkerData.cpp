#include "WorkerData.hpp"

WorkerData::WorkerData()
    : name(""), age(0), position(""), salary(0.0) {}

WorkerData::WorkerData(const MyString& name_, int age_,
                       const MyString& position_, double salary_)
    : name(name_), age(age_), position(position_), salary(salary_) {}