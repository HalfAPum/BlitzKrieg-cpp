//
// Created by o.narvatov on 8/7/2024.
//

#include "GridArray.h"

//When call this method we assume x and z are have absolute position
int &GridArray::get_lvalue_reference_internal(const int x, const int z) {
    return get_lvalue_reference(x / grid_size, z / grid_size);
}

void GridArray::increment(const int x, const int z) {
    ++get_lvalue_reference_internal(x, z);
}

void GridArray::decrement(const int x, const int z) {
    --get_lvalue_reference_internal(x, z);
}

int GridArray::get(const int x, const int z) {
    return get_lvalue_reference(x, z);
}

