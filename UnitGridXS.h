//
// Created by o.narvatov on 8/6/2024.
//

#ifndef ENEMYGRIDXS_H
#define ENEMYGRIDXS_H

#include "UnitGridCompact.h"
#include "grid/array/GridArrayXS.h"

class BlitzUnit;
class UnitGridS;

using namespace std;

constexpr int GRID_LEVEL_XS = 1;

class UnitGridXS final : public UnitGridCompact {
public:
    explicit UnitGridXS(const UnitGridType unit_grid_type) : UnitGridCompact(unit_grid_type, new GridArrayXS(), GRID_LEVEL_XS) {}
};



#endif //ENEMYGRIDXS_H
