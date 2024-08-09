//
// Created by o.narvatov on 8/9/2024.
//

#ifndef SPLIT_GRID_CELL_H
#define SPLIT_GRID_CELL_H
#include "GridCell.h"

//Stable. Was tested.
inline array<GridCell, 4> split_grid_cell(const GridCell &cell, const BlitzUnit* unit) {
    EnemyGrid* next_precision_enemy_grid = cell.enemy_grid->next_precision_grid();

    //Assme precision level goes up by two
    const int sx = cell.x * 2;
    const int sz = cell.z * 2;

    const auto position = unit->get_position();
    const int unit_x = position.x;
    const int unit_z = position.z;

    const int half_grid = cell.enemy_grid->grid_size / 2;

    const int xmod = unit_x % cell.enemy_grid->grid_size;
    const int zmod = unit_z % cell.enemy_grid->grid_size;

    const auto top_left_cell = GridCell(sx, sz, next_precision_enemy_grid);
    const auto top_right_cell = GridCell(sx + 1, sz, next_precision_enemy_grid);
    const auto bottom_left_cell = GridCell(sx, sz + 1, next_precision_enemy_grid);
    const auto bottom_right_cell = GridCell(sx + 1, sz + 1, next_precision_enemy_grid);

    array<GridCell, 4> result;

    if (xmod < half_grid) {
        //Left

        if (zmod < half_grid) {
            //Top
            result = {
                bottom_right_cell,
                bottom_left_cell,
                top_right_cell,
                top_left_cell,
            };
        } else {
            //Bottom
            result = {
                top_right_cell,
                top_left_cell,
                bottom_right_cell,
                bottom_left_cell,
            };
        }
    } else {
        //Right

        if (zmod < half_grid) {
            //Top
            result = {
                bottom_left_cell,
                bottom_right_cell,
                top_left_cell,
                top_right_cell,
            };
        } else {
            //Bottom
            result = {
                top_left_cell,
                top_right_cell,
                bottom_left_cell,
                bottom_right_cell,
            };
        }
    }

    return result;
}

#endif //SPLIT_GRID_CELL_H
