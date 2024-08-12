//
// Created by o.narvatov on 8/6/2024.
//

#ifndef ENEMYGRID_H
#define ENEMYGRID_H
#include "blitzunit.h"
#include "UnitGridType.h"

class BlitzUnit;

class UnitGrid {
public:
    UnitGridType grid_type = ENEMY;
    int grid_size = -1;
    int grid_level = -1;

    explicit UnitGrid(
        const UnitGridType unit_grid_type,
        const int g_size,
        const int level
    ) : grid_type(unit_grid_type), grid_size(g_size), grid_level(level) {}

    virtual void add_enemy(BlitzUnit* unit) {}
    //old_x and old_z are supposed to be in the old square before current position
    virtual void remove_enemy(BlitzUnit* unit) {}

    virtual int get_enemy_count(int x, int z) const { return 0; }

    virtual bool is_last_level_of_precision() const { return grid_level == 0; }

    virtual ~UnitGrid() = default;

    //debug
    virtual void print() const {}

    virtual std::vector<BlitzUnit*> get_units(int x, int z) { return {}; };

};



#endif //ENEMYGRID_H
