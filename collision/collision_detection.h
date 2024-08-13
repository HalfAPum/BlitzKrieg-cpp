//
// Created by o.narvatov on 8/13/2024.
//

#ifndef COLLISION_DETECTION_H
#define COLLISION_DETECTION_H
#include <stack>
#include <godot_cpp/variant/vector2.hpp>
#include "../UnitGirdFactory.h"
#include "../UnitGridXXS.h"
#include "../GridCell.h"
#include "../fill_reachable_cells.h"

using namespace godot;

//Collision unit with distance
using collision_pair = pair<BlitzUnit*, real_t>;

// UnitGridXXS *enemy_grid_xxs = UnitGridFactory::instance().enemy_unit_grid_abstract_factory->grid_xxs;
// UnitGridXXS *ally_grid_xxs = UnitGridFactory::instance().ally_unit_grid_abstract_factory->grid_xxs;
// UnitGridXXS *player_grid_xxs = UnitGridFactory::instance().player_unit_grid_abstract_factory->grid_xxs;

inline void detect_entity_collisions(vector<collision_pair> &collided_units, const Vector2 &position, real_t entity_collision_radius, UnitGridXXS *grid);

inline void detect_entity_collisions(vector<collision_pair> &collided_units, const Vector2 &position, const real_t entity_collision_radius) {
    detect_entity_collisions(collided_units, position, entity_collision_radius, UnitGridFactory::instance().enemy_unit_grid_abstract_factory->grid_xxs);
    detect_entity_collisions(collided_units, position, entity_collision_radius, UnitGridFactory::instance().ally_unit_grid_abstract_factory->grid_xxs);
    detect_entity_collisions(collided_units, position, entity_collision_radius, UnitGridFactory::instance().player_unit_grid_abstract_factory->grid_xxs);
}

inline void detect_entity_collisions(
    vector<collision_pair> &collided_units,
    const Vector2 &position,
    const real_t entity_collision_radius,
    UnitGridXXS *grid
) {
    std::stack<GridCell> stack;

    fill_reachable_cells(stack, position.x, position.y, grid);

    while (!stack.empty()) {
        const auto cell = stack.top();
        stack.pop();

        const int x = cell.x;
        const int z = cell.z;

        if (x < 0 || x >= MAP_SIZE / GRID_SIZE_XXS) continue;
        if (z < 0 || z >= MAP_SIZE / GRID_SIZE_XXS) continue;

        for (auto *unit : grid->get_units(x, z)) {
            const auto unit_position = unit->get_position2D();

            //Is the same unit
            if (position == unit_position) continue;

            auto collision_radius = max(unit->projectile_collision_radius, entity_collision_radius);

            if (abs(unit_position.x - position.x) > collision_radius) continue;
            //position.y is actually position.z but it's Vector2.
            if (abs(unit_position.y - position.y) > collision_radius) continue;

            const auto distance = position.distance_to(unit_position);

            if (distance <= collision_radius) {
                collided_units.emplace_back(unit, distance);
            }
        }
    }
}

#endif //COLLISION_DETECTION_H
