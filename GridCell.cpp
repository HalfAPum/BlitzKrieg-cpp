//
// Created by o.narvatov on 8/7/2024.
//

#include "GridCell.h"

#include "UnitGirdFactory.h"

int GridCell::get_enemy_count() const {
    return unit_grid->get_enemy_count(x, z);
}

BlitzUnit *GridCell::find_if(const BlitzUnit* unit, bool (*predicate)(const BlitzUnit*, const BlitzUnit*)) const {
    const UnitGridXXS *grid_xxs = get_unit_grid_factory_enum(unit_grid)->grid_xxs;
    if (unit_grid->is_last_level_of_precision()) {
        return grid_xxs->find_if(x, z, unit, predicate);
    }

    const int difference = unit_grid->grid_level * 2;

    const int gridxxs_x_start = x * difference;
    const int gridxxs_x_end = gridxxs_x_start + difference;

    const int gridxxs_z_start = z * difference;
    const int gridxxs_z_end = gridxxs_z_start + difference;

    for (int i = gridxxs_x_start; i != gridxxs_x_end; ++i) {
        for (int j = gridxxs_z_start; j != gridxxs_z_end; ++j) {
            BlitzUnit *found = grid_xxs->find_if(i, j, unit, predicate);

            if (found != nullptr) return found;
        }
    }

    return nullptr;
}

