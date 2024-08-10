//
// Created by o.narvatov on 8/9/2024.
//

#ifndef UNITGIRDFACTORY_H
#define UNITGIRDFACTORY_H
#include "UnitGridAbstractFactory.h"
#include "UnitGridType.h"

class UnitGridFactory {
    UnitGridFactory() {}
public:
    UnitGridFactory(UnitGridFactory const&) = delete;
    void operator=(UnitGridFactory const&) = delete;

    static UnitGridFactory& instance()
    {
        static UnitGridFactory instance;
        return instance;
    }

    UnitGridAbstractFactory *enemy_unit_grid_abstract_factory = new UnitGridAbstractFactory(ENEMY);
    UnitGridAbstractFactory *ally_unit_grid_abstract_factory = new UnitGridAbstractFactory(ALLY);
    UnitGridAbstractFactory *player_unit_grid_abstract_factory = new UnitGridAbstractFactory(PLAYER);

};

inline UnitGridAbstractFactory *get_unit_grid_factory(const BlitzUnit* unit) {
    if (unit->isEnemy) {
        return UnitGridFactory::instance().enemy_unit_grid_abstract_factory;
    }

    if (unit->isAlly) {
        return UnitGridFactory::instance().ally_unit_grid_abstract_factory;
    }

    return UnitGridFactory::instance().player_unit_grid_abstract_factory;
}

inline UnitGridAbstractFactory *get_unit_grid_factory_enum(const UnitGrid* unit_grid) {
    switch(unit_grid->grid_type) {
        case ENEMY:
            return UnitGridFactory::instance().enemy_unit_grid_abstract_factory;
        case ALLY:
            return UnitGridFactory::instance().ally_unit_grid_abstract_factory;
        case PLAYER:
            return UnitGridFactory::instance().player_unit_grid_abstract_factory;
    }

    return UnitGridFactory::instance().enemy_unit_grid_abstract_factory;
}

#endif //UNITGIRDFACTORY_H