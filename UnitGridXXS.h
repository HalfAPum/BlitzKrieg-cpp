//
// Created by o.narvatov on 8/6/2024.
//

#ifndef ENEMYGRIDXXS_H
#define ENEMYGRIDXXS_H

#include <godot_cpp/variant/utility_functions.hpp>

#include "blitzunit.h"
#include "UnitGrid.h"

class BlitzUnit;

using namespace std;
using namespace godot;

constexpr int ARRAY_SIZE_XXS = 8;
constexpr int GRID_SIZE_XXS = 4;
constexpr int GRID_LEVEL_XXS = 0;

class UnitGridXXS final : public UnitGrid {
public:
    explicit UnitGridXXS(const UnitGridType unit_grid_type) : UnitGrid(unit_grid_type, GRID_SIZE_XXS, GRID_LEVEL_XXS) {}
private:
    array<array<vector<BlitzUnit*>, ARRAY_SIZE_XXS>, ARRAY_SIZE_XXS> units_array = {};
public:

    void add_enemy(BlitzUnit* unit) override;

    void remove_enemy(BlitzUnit *unit) override;

    int get_enemy_count(int x, int z) const override;

    BlitzUnit* find_if(int x, int z, const BlitzUnit* unit, bool (*predicate)(const BlitzUnit*, const BlitzUnit*)) const;

    void print() const override;

    vector<BlitzUnit*> get_units(int x, int z) override;
};



#endif //ENEMYGRIDXXS_H
