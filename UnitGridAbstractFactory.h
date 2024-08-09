//
// Created by o.narvatov on 8/9/2024.
//

#ifndef GRIDABSTACTFACTORY_H
#define GRIDABSTACTFACTORY_H
#include <map>

class UnitGrid;

#include "UnitGridXXS.h"
#include "UnitGridXS.h"
#include "UnitGridS.h"



class UnitGridAbstractFactory {
public:
    UnitGridXXS* grid_xxs = nullptr;
    UnitGrid* grid_xxs_cast = nullptr;
    UnitGrid* grid_xs = nullptr;
    UnitGrid* grid_s = nullptr;

    explicit UnitGridAbstractFactory(const UnitGridType unit_grid_type) :
    grid_xxs(new UnitGridXXS(unit_grid_type)),
    grid_xxs_cast(grid_xxs),
    grid_xs(new UnitGridXXS(unit_grid_type)),
    grid_s(new UnitGridXXS(unit_grid_type))
    {}

    map<int, UnitGrid*> precision_map {
        { GRID_LEVEL_XXS, grid_xxs },
        { GRID_LEVEL_XS, grid_xs },
        { GRID_LEVEL_S, grid_s }
    };

    UnitGrid *next_precision_grid(int grid_level) const;

    UnitGrid *previous_precision_grid(int grid_level) const;
};



#endif //GRIDABSTACTFACTORY_H
