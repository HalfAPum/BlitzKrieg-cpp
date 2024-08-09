//
// Created by o.narvatov on 8/7/2024.
//

#include "UnitGridCompact.h"

void UnitGridCompact::add_enemy(BlitzUnit *unit) {
    const auto position = unit->get_position();

    grid_array->increment(position.x, position.z);
}

void UnitGridCompact::remove_enemy(BlitzUnit *unit) {
    grid_array->decrement(unit->old_x, unit->old_z);
}

int UnitGridCompact::get_enemy_count(const int x, const int z) const {
    return grid_array->get(x, z);
}
