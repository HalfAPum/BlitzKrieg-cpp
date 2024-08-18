//
// Created by o.narvatov on 8/18/2024.
//

#include "YAxisGrid.h"

YAxisGrid::YAxisGrid() {
    y_grid_array = Height_Array().get_height_array();
}


float YAxisGrid::get_y_value(const Vector3 &entity_position) const {
    const int x = (entity_position.x * Z_AXIS_PRECISION);
    const int z = (entity_position.z * Z_AXIS_PRECISION);

    return (*y_grid_array)[x][z];
}
