//
// Created by o.narvatov on 8/7/2024.
//

#ifndef GRIDCELL_H
#define GRIDCELL_H
#include "UnitGrid.h"
#include "UnitGridXXS.h"

class UnitGrid;

using namespace std;

class GridCell {
public:
    int x = 0;
    int z = 0;
    UnitGrid* unit_grid = nullptr;

    GridCell() = default;
    GridCell(const int _x, const int _z, UnitGrid* u_grid) : x(_x), z(_z), unit_grid(u_grid) {}

    int get_enemy_count() const;

    BlitzUnit* find_if(const BlitzUnit* unit, bool (*predicate)(const BlitzUnit*, const BlitzUnit*)) const;

    //Test
    void as_text() {
        string temp = "Grid x: " + to_string(x) + " z: " + to_string(z) + " scale: " + to_string(unit_grid->grid_size);
        char tab2[1024];
        strcpy(tab2, temp.c_str());
        UtilityFunctions::print(tab2);
    }
};



#endif //GRIDCELL_H
