//
// Created by o.narvatov on 8/8/2024.
//

#ifndef CAN_ATTACK_ENEMY_ALGORITHM_H
#define CAN_ATTACK_ENEMY_ALGORITHM_H
#include "blitzunit.h"

bool can_attack_enemy(const BlitzUnit* unit, const BlitzUnit* enemy) {
    const auto unit_position = unit->get_position();

    const auto enemy_position = enemy->get_position();
    const auto distance = Vector2(unit_position.x, unit_position.z).distance_to(
        Vector2(enemy_position.x, enemy_position.z)
    );
    //calculate distance
    if (distance < unit->attack_radius) {
        return true;
    }

    return false;
}

#endif //CAN_ATTACK_ENEMY_ALGORITHM_H
