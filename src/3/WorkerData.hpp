#ifndef WORKER_DATA_HPP
#define WORKER_DATA_HPP

#include "My_string.hpp"

struct WorkerData {
    MyString name;  
    int      age;     
    MyString position;   
    double   salary;    

    WorkerData();
    WorkerData(const MyString& name, int age,
               const MyString& position = MyString(""),
               double salary = 0.0);
};

#endif