//
// Created by o.narvatov on 8/7/2024.
//

#ifndef ENEMYGRIDCOMPACT_H
#define ENEMYGRIDCOMPACT_H
#include "UnitGrid.h"
#include "grid/array/GridArray.h"

class BlitzUnit;

using namespace std;

class UnitGridCompact : public UnitGrid {
protected:
    GridArray *grid_array;

    explicit UnitGridCompact(
        const UnitGridType unit_grid_type,
        GridArray *g_array,
        const int grid_level
    ) : UnitGrid(unit_grid_type, g_array->grid_size, grid_level), grid_array(g_array) {}
public:
    void add_enemy(BlitzUnit *unit) override;
    void remove_enemy(BlitzUnit *unit) override;

    int get_enemy_count(int x, int z) const override;

    ~UnitGridCompact() override {
        delete grid_array;
    }

    void print() const override { grid_array->print(); }
};



#endif //ENEMYGRIDCOMPACT_H
