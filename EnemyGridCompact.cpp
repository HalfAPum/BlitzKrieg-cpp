//
// Created by o.narvatov on 8/7/2024.
//

#include "EnemyGridCompact.h"

void EnemyGridCompact::add_enemy(BlitzUnit *unit) {
    const auto position = unit->get_position();

    grid_array->increment(position.x, position.z);
}

void EnemyGridCompact::remove_enemy(BlitzUnit *unit) {
    grid_array->decrement(unit->old_x, unit->old_z);
}

int EnemyGridCompact::get_enemy_count(const int x, const int z) const {
    return grid_array->get(x, z);
}
