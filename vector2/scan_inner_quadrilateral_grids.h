//
// Created by o.narvatov on 8/12/2024.
//

#ifndef SCAN_INNER_QUADRILATERAL_GRIDS_H
#define SCAN_INNER_QUADRILATERAL_GRIDS_H
#include <stack>
#include <unordered_set>
#include <godot_cpp/variant/rect2.hpp>
#include <godot_cpp/variant/vector2.hpp>

#include "hashfunction.h"
#include "quadrilateral.h"
#include "../blitzunit.h"
#include "../UnitGridXXS.h"

using namespace godot;

constexpr int MINIMAL_SCAN_DISTANCE = 1;

const auto bottom_left_grid_increment = Vector2(1, -1);
const auto bottom_right_grid_increment = Vector2(-1, -1);
const auto top_right_grid_increment = Vector2(-1, 1);
const auto top_left_grid_increment = Vector2(1, 1);

const auto top_increment = Vector2(0, -1);
const auto bottom_increment = Vector2(0, 1);
const auto left_increment = Vector2(-1, 0);
const auto right_increment = Vector2(1, 0);

inline void push_top_left(
    std::stack<Vector2> &stack,
    const Quadrilateral &quadrilateral,
    const Vector2 &increment
) {
    if (abs(quadrilateral.top_left_vertex.y - quadrilateral.bottom_left_vertex.y) <= MINIMAL_SCAN_DISTANCE) return;

    if (abs(quadrilateral.top_left_vertex.x - quadrilateral.top_right_vertex.x) <= MINIMAL_SCAN_DISTANCE) return;

    stack.push(quadrilateral.top_left_vertex + increment);
}

inline void push_top_right(
    std::stack<Vector2> &stack,
    const Quadrilateral &quadrilateral,
    const Vector2 &increment
) {
    if (abs(quadrilateral.top_right_vertex.y - quadrilateral.bottom_right_vertex.y) <= MINIMAL_SCAN_DISTANCE) return;

    if (abs(quadrilateral.top_right_vertex.x - quadrilateral.top_left_vertex.x) <= MINIMAL_SCAN_DISTANCE) return;

    stack.push(quadrilateral.top_right_vertex + increment);
}

inline void push_bottom_right(
    std::stack<Vector2> &stack,
    const Quadrilateral &quadrilateral,
    const Vector2 &increment
) {
    if (abs(quadrilateral.bottom_right_vertex.y - quadrilateral.top_right_vertex.y) <= MINIMAL_SCAN_DISTANCE) return;

    if (abs(quadrilateral.bottom_right_vertex.x - quadrilateral.bottom_left_vertex.x) <= MINIMAL_SCAN_DISTANCE) return;

    stack.push(quadrilateral.bottom_right_vertex + increment);
}

inline void push_bottom_left(
    std::stack<Vector2> &stack,
    const Quadrilateral &quadrilateral,
    const Vector2 &increment
) {
    if (abs(quadrilateral.bottom_left_vertex.y - quadrilateral.top_left_vertex.y) <= MINIMAL_SCAN_DISTANCE) return;

    if (abs(quadrilateral.bottom_left_vertex.x - quadrilateral.bottom_right_vertex.x) <= MINIMAL_SCAN_DISTANCE) return;

    stack.push(quadrilateral.bottom_left_vertex + increment);
}

inline void scan_inner_quadrilateral_grids(
    const std::array<Vector2, 4> &quadrilateral_array,
    const Rect2 &drag_rect_area,
    std::unordered_set<Vector2, HashFunction> &scan_set,
    int (*axis_converter) (int axis_coordinate)
) {
    const auto quadrilateral = Quadrilateral(
        Vector2(axis_converter(quadrilateral_array[3].x), axis_converter(quadrilateral_array[3].y)),
        Vector2(axis_converter(quadrilateral_array[2].x), axis_converter(quadrilateral_array[2].y)),
        Vector2(axis_converter(quadrilateral_array[1].x), axis_converter(quadrilateral_array[1].y)),
        Vector2(axis_converter(quadrilateral_array[0].x), axis_converter(quadrilateral_array[0].y))
    );

    std::stack<Vector2> stack;

    const int rect_x = drag_rect_area.size.x;
    const int rect_y = drag_rect_area.size.y;

    if (rect_x >= 0 && rect_y >= 0) {
        //regular - not mirorred

        push_bottom_left(stack, quadrilateral, bottom_left_grid_increment);
        push_bottom_right(stack, quadrilateral, bottom_right_grid_increment);
        push_top_right(stack, quadrilateral, top_right_grid_increment);
        push_top_left(stack, quadrilateral, top_left_grid_increment);
    } else if (rect_x >= 0 && rect_y < 0) {
        //x - mirorred

        push_bottom_left(stack, quadrilateral, top_left_grid_increment);
        push_bottom_right(stack, quadrilateral, top_right_grid_increment);
        push_top_right(stack, quadrilateral, bottom_right_grid_increment);
        push_top_left(stack, quadrilateral, bottom_left_grid_increment);
    } else if (rect_x < 0 && rect_y >= 0) {
        //y - mirorred

        push_bottom_left(stack, quadrilateral, bottom_right_grid_increment);
        push_bottom_right(stack, quadrilateral, bottom_left_grid_increment);
        push_top_right(stack, quadrilateral, top_left_grid_increment);
        push_top_left(stack, quadrilateral, top_right_grid_increment);
    } else {
        //x && y - mirorred

        push_bottom_left(stack, quadrilateral, top_right_grid_increment);
        push_bottom_right(stack, quadrilateral, top_left_grid_increment);
        push_top_right(stack, quadrilateral, bottom_left_grid_increment);
        push_top_left(stack, quadrilateral, bottom_right_grid_increment);
    }

    while (!stack.empty()) {
        auto grid = stack.top();
        stack.pop();

        if (scan_set.find(grid) != scan_set.end()) continue;

        const int x = grid.x;
        const int z = grid.y;

        if (x < 0 || x >= MAP_SIZE / GRID_SIZE_XXS) continue;
        if (z < 0 || z >= MAP_SIZE / GRID_SIZE_XXS) continue;

        scan_set.emplace(grid);

        stack.push(grid + top_increment);
        stack.push(grid + bottom_increment);
        stack.push(grid + left_increment);
        stack.push(grid + right_increment);
    }
}

#endif //SCAN_INNER_QUADRILATERAL_GRIDS_H
