//
// Created by o.narvatov on 8/6/2024.
//

#include "UnitGridXXS.h"
#include <algorithm>

#include "UnitGirdFactory.h"

inline UnitGrid *get_previous_presicion_grid(const UnitGrid* unit_grid) {
    return get_unit_grid_factory_enum(unit_grid)->previous_precision_grid(unit_grid->grid_size);
}

void UnitGridXXS::add_enemy(BlitzUnit *unit) {
    const auto position = unit->get_position();

    units_array[position.x / grid_size][position.z / grid_size].push_back(unit);

    //increment counters
    for (auto current_level = get_previous_presicion_grid(this); current_level != nullptr;) {
        current_level->add_enemy(unit);

        current_level = get_previous_presicion_grid(current_level);
    }
}

void UnitGridXXS::remove_enemy(BlitzUnit *unit) {
    auto &vector = units_array[unit->old_x / grid_size][unit->old_z / grid_size];
    const auto position = std::find(vector.begin(), vector.end(), unit);

    if (position != vector.end()) {
        vector.erase(position);
    }

    //decrement counters
    for (auto current_level = get_previous_presicion_grid(this); current_level != nullptr;) {
        current_level->remove_enemy(unit);

        current_level = get_previous_presicion_grid(current_level);
    }
}



BlitzUnit* UnitGridXXS::find_if(const int x, const int z, const BlitzUnit* unit, bool (*predicate)(const BlitzUnit*, const BlitzUnit*)) const {
    for (const auto enemy: units_array[x][z]) {
        if (predicate(unit, enemy)) return enemy;
    }

    return nullptr;
}

int UnitGridXXS::get_enemy_count(const int x, const int z) const {
    return units_array[x][z].size();
}

void UnitGridXXS::print() const {
    UtilityFunctions::print("EnemyGridXXS matrix");
    for (int i = 0; i < ARRAY_SIZE_XXS; ++i) {
        string row;
        for (int j = 0; j < ARRAY_SIZE_XXS; ++j) {
            row += to_string(get_enemy_count(j, i)) + ' ';
        }
        char arr[1024];
        strcpy(arr, row.c_str());
        UtilityFunctions::print(arr);
    }
}


