//
// Created by o.narvatov on 8/7/2024.
//

#ifndef ENEMYGRIDS_H
#define ENEMYGRIDS_H

#include "UnitGrid.h"
#include "UnitGridCompact.h"
#include "grid/array/GridArrayS.h"

class BlitzUnit;

using namespace std;

constexpr int GRID_LEVEL_S = 2;

class UnitGridS final : public UnitGridCompact {
public:
    explicit UnitGridS(const UnitGridType unit_grid_type) : UnitGridCompact(unit_grid_type, new GridArrayS(), GRID_LEVEL_S) {}
};



#endif //ENEMYGRIDS_H
