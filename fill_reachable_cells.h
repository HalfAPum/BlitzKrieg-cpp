//
// Created by o.narvatov on 8/8/2024.
//

#ifndef FILL_INITIAL_ADJACENT_CELLS_ALGORITHM_H
#define FILL_INITIAL_ADJACENT_CELLS_ALGORITHM_H
#include <stack>

#include "GridCell.h"

//Stable. Corvered with custom tests in separate project.
inline void fill_reachable_cells(
    stack<GridCell> &stack,
    const int unit_x,
    const int unit_z,
    UnitGrid *unit_grid
) {
    const int grid_size = unit_grid->grid_size;

    const int grid_x = unit_x / grid_size;
    const int grid_z = unit_z / grid_size;
    const int xmod = unit_x % grid_size;
    const int zmod = unit_z % grid_size;

    const int half_grid = grid_size / 2;

    if (xmod < half_grid) {
        //Left

        if (zmod < half_grid) {
            //Top

            //Left-top
            stack.emplace(grid_x - 1, grid_z, unit_grid);
            stack.emplace(grid_x - 1, grid_z - 1, unit_grid);
            stack.emplace(grid_x, grid_z - 1, unit_grid);
        } else {
            //Bottom

            //Left-Bottom
            stack.emplace(grid_x - 1, grid_z, unit_grid);
            stack.emplace(grid_x - 1, grid_z + 1, unit_grid);
            stack.emplace(grid_x, grid_z + 1, unit_grid);
        }
    } else {
        //Right

        if (zmod < half_grid) {
            //Top
            //Right-top
            stack.emplace(grid_x, grid_z - 1, unit_grid);
            stack.emplace(grid_x + 1, grid_z - 1, unit_grid);
            stack.emplace(grid_x + 1, grid_z, unit_grid);
        } else {
            //Bottom
            //Right-bottom
            stack.emplace(grid_x, grid_z + 1, unit_grid);
            stack.emplace(grid_x + 1, grid_z + 1, unit_grid);
            stack.emplace(grid_x + 1, grid_z, unit_grid);
        }
    }

    stack.emplace(grid_x, grid_z, unit_grid);
}

#endif //FILL_INITIAL_ADJACENT_CELLS_ALGORITHM_H
