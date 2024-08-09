//
// Created by o.narvatov on 8/9/2024.
//

#ifndef UNITGIRDFACTORY_H
#define UNITGIRDFACTORY_H
#include "UnitGridAbstractFactory.h"
#include "UnitGridType.h"

static UnitGridAbstractFactory *enemy_unit_grid_abstract_factory = new UnitGridAbstractFactory(ENEMY);
static UnitGridAbstractFactory *ally_unit_grid_abstract_factory = new UnitGridAbstractFactory(ALLY);
static UnitGridAbstractFactory *player_unit_grid_abstract_factory = new UnitGridAbstractFactory(PLAYER);

inline UnitGridAbstractFactory *get_unit_grid_factory(const BlitzUnit* unit) {
    if (unit->isEnemy) {
        return enemy_unit_grid_abstract_factory;
    }

    if (unit->isAlly) {
        return ally_unit_grid_abstract_factory;
    }

    return player_unit_grid_abstract_factory;
}

inline UnitGridAbstractFactory *get_unit_grid_factory_enum(const UnitGrid* unit_grid) {
    switch(unit_grid->grid_type) {
        case ENEMY:
            return enemy_unit_grid_abstract_factory;
        case ALLY:
            return ally_unit_grid_abstract_factory;
        case PLAYER:
            return player_unit_grid_abstract_factory;
    }

    return enemy_unit_grid_abstract_factory;
}

#endif //UNITGIRDFACTORY_H