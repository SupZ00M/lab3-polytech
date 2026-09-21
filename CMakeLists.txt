cmake_minimum_required(VERSION 3.15)
project(lab3 VERSION 1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if (MSVC)
    add_compile_options(/W4 /permissive-)
else()
    add_compile_options(-Wall -Wextra -Wpedantic)
endif()


add_library(lab3_lib STATIC
    src/1-2/Matrix.cpp
    src/1-2/My_string.cpp
    src/3/WorkerData.cpp
    src/3/WorkerDb.cpp
    src/4/NotificationQueue.cpp
    src/4/NotificationPriorityQueue.cpp
    src/4/Notification.cpp
    src/5/BaseFile.cpp
    src/5/IFile.cpp
    src/6/BoolArray.cpp
)
target_include_directories(lab3_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/src
    ${CMAKE_CURRENT_SOURCE_DIR}/src/1-2
    ${CMAKE_CURRENT_SOURCE_DIR}/src/3
     ${CMAKE_CURRENT_SOURCE_DIR}/src/4
      ${CMAKE_CURRENT_SOURCE_DIR}/src/5
      ${CMAKE_CURRENT_SOURCE_DIR}/src/6
)


add_executable(lab3 src/lab3.cpp)
target_link_libraries(lab3 PRIVATE lab3_lib)

set_target_properties(lab3 PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

enable_testing()

function(add_unit_test name source)
    add_executable(${name} ${source})
    target_link_libraries(${name} PRIVATE lab3_lib)
    set_target_properties(${name} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
    add_test(NAME ${name} COMMAND ${name})
    set_tests_properties(${name} PROPERTIES
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/bin
    )
endfunction()

add_unit_test(test_my_string    tests/test_my_string.cpp)
add_unit_test(test_matrix       tests/test_matrix.cpp)
add_unit_test(test_worker_db    tests/test_worker_db.cpp)
add_unit_test(test_notification tests/test_notification.cpp)
add_unit_test(test_base_file    tests/test_base_file.cpp)
add_unit_test(test_bool_array   tests/test_bool_array.cpp)