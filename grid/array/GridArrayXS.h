//
// Created by o.narvatov on 8/7/2024.
//

#ifndef GRIDARRAYXS_H
#define GRIDARRAYXS_H
#include <array>
#include <godot_cpp/variant/utility_functions.hpp>

#include "GridArray.h"

using namespace std;

constexpr int ARRAY_SIZE_XS = 4;
constexpr int GRID_SIZE_XS = 8;

class GridArrayXS final : public GridArray {
    array<array<int, ARRAY_SIZE_XS>, ARRAY_SIZE_XS> units_array {{}};
public:
    GridArrayXS() : GridArray(GRID_SIZE_XS, ARRAY_SIZE_XS) {}

    void print() const override {
        UtilityFunctions::print("GridXS matrix");
        for (int i = 0; i < ARRAY_SIZE_XS; ++i) {
            string row;
            for (int j = 0; j < ARRAY_SIZE_XS; ++j) {
                row += to_string(units_array[j][i]) + ' ';
            }
            char arr[1024];
            strcpy(arr, row.c_str());
            UtilityFunctions::print(arr);
        }
    }
protected:
    int &get_lvalue_reference(const int x, const int z) override {
        return units_array[x][z];
    }
};



#endif //GRIDARRAYXS_H
