//
// Created by o.narvatov on 8/9/2024.
//

#include "UnitGridAbstractFactory.h"


UnitGrid *get_or_null(const map<int, UnitGrid*>& precision_map, const int grid_level) {
    const auto it = precision_map.find(grid_level);

    if (it == precision_map.end()) return nullptr;

    return it->second;
}

UnitGrid *UnitGridAbstractFactory::next_precision_grid(const int grid_level) const {
    return get_or_null(precision_map, grid_level - 1);
}

UnitGrid *UnitGridAbstractFactory::previous_precision_grid(const int grid_level) const {
    return get_or_null(precision_map, grid_level + 1);
}

