//
// Created by o.narvatov on 8/6/2024.
//

#include "EnemyGridXXS.h"
#include "EnemyGridXS.h"
#include <algorithm>

EnemyGridXXS *EnemyGridXXS::p_inst;

void EnemyGridXXS::add_enemy(BlitzUnit *unit) {
    const auto position = unit->get_position();

    units_array[position.x / grid_size][position.z / grid_size].push_back(unit);

    //increment counters
    for (auto current_level = previous_precision_grid(); current_level != nullptr;) {
        current_level->add_enemy(unit);

        current_level = current_level->previous_precision_grid();
    }
}

void EnemyGridXXS::remove_enemy(BlitzUnit *unit) {
    auto &vector = units_array[unit->old_x / grid_size][unit->old_z / grid_size];
    const auto position = std::find(vector.begin(), vector.end(), unit);

    if (position != vector.end()) {
        vector.erase(position);
    }

    //decrement counters
    for (auto current_level = previous_precision_grid(); current_level != nullptr;) {
        current_level->remove_enemy(unit);

        current_level = current_level->previous_precision_grid();
    }
}



BlitzUnit* EnemyGridXXS::find_if(const int x, const int z, const BlitzUnit* unit, bool (*predicate)(const BlitzUnit*, const BlitzUnit*)) const {
    for (const auto enemy: units_array[x][z]) {
        if (predicate(unit, enemy)) return enemy;
    }

    return nullptr;
}

int EnemyGridXXS::get_enemy_count(const int x, const int z) const {
    return units_array[x][z].size();
}

EnemyGrid *EnemyGridXXS::previous_precision_grid() {
    return EnemyGridXS::instance();
}

void EnemyGridXXS::print() const {
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


