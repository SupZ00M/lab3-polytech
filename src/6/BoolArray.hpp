#ifndef BOOL_ARRAY_HPP
#define BOOL_ARRAY_HPP

#include <cstddef>
#include <cstdint>
#include <iostream>

class BoolArray {
private:
    std::uint8_t* data;   // байты, в которых упакованы биты
    std::size_t   nbits;  // количество логических бит

    // Обнулить неиспользуемые биты в последнем байте
    void clear_unused_bits();

public:
   
    explicit BoolArray(std::size_t n = 0, bool fill = false);
    BoolArray(const BoolArray& other);
    BoolArray(BoolArray&& other) noexcept;
    ~BoolArray();
    BoolArray& operator=(const BoolArray& other);
    BoolArray& operator=(BoolArray&& other) noexcept;


    std::size_t size() const { return nbits; }

    // Прокси-ссылка на бит
    // Реализует "оператор неявного приведения типа" (operator bool)
    // и "оператор присваивания" (operator=(bool)).
    class BitReference {
    private:
        std::uint8_t* byte;   // указатель на байт
        std::size_t   bit;    // номер бита в байте (0..7)
    public:
        BitReference(std::uint8_t* b, std::size_t i) : byte(b), bit(i) {}

        // Неявное приведение к bool — для чтения
        operator bool() const {
            return (*byte >> bit) & 1u;
        }

        // Присваивание bool — для записи
        BitReference& operator=(bool value) {
            if (value)
                *byte |=  (1u << bit);
            else
                *byte &= ~(1u << bit);
            return *this;
        }

        // Присваивание от другого BitReference
        BitReference& operator=(const BitReference& other) {
            return *this = static_cast<bool>(other);
        }
    };

    BitReference operator[](std::size_t i);
    bool         operator[](std::size_t i) const;

    // Изменение размера 
    // Если new_size > size(), новые элементы заполняются fill.
    // Если new_size < size(), хвост отбрасывается.
    void resize(std::size_t new_size, bool fill = false);

    //Вывод 
    void print(std::ostream& os = std::cout) const;
};

#endif