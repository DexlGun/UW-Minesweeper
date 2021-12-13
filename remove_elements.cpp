#include <iostream>

int* remove_elements(int * & original_array, std::size_t original_capacity, int * & remove_array, std::size_t remove_capacity);

int* remove_elements(int * & original_array, std::size_t original_capacity, int * & remove_array, std::size_t remove_capacity) {
    for (int* i = remove_array; i < (int*)original_capacity; i++)
original_array[*remove_array] = original_array[*remove_array+1];
original_array[original_capacity] = 0;
}