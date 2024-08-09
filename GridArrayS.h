//
// Created by o.narvatov on 8/7/2024.
//

#ifndef GRIDARRAYS_H
#define GRIDARRAYS_H
#include <array>
#include <godot_cpp/variant/utility_functions.hpp>

#include "GridArray.h"

using namespace std;

constexpr int ARRAY_SIZE_S = 2;
constexpr int GRID_SIZE_S = 16;

class GridArrayS final : public GridArray {
    array<array<int, ARRAY_SIZE_S>, ARRAY_SIZE_S> units_array {{}};
public:
    GridArrayS() : GridArray(GRID_SIZE_S, GRID_SIZE_S) {}

    void print() const override {
        UtilityFunctions::print("EnemyGridS matrix");
        for (int i = 0; i < ARRAY_SIZE_S; ++i) {
            string row;
            for (int j = 0; j < ARRAY_SIZE_S; ++j) {
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


#endif //GRIDARRAYS_H
