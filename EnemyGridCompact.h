//
// Created by o.narvatov on 8/7/2024.
//

#ifndef ENEMYGRIDCOMPACT_H
#define ENEMYGRIDCOMPACT_H
#include "EnemyGrid.h"
#include "GridArray.h"

class BlitzUnit;

using namespace std;

class EnemyGridCompact : public EnemyGrid {
protected:
    GridArray *grid_array;

    explicit EnemyGridCompact(GridArray *g_array, const int grid_level) : EnemyGrid(g_array->grid_size, grid_level), grid_array(g_array) {}
public:
    void add_enemy(BlitzUnit *unit) override;
    void remove_enemy(BlitzUnit *unit) override;

    int get_enemy_count(int x, int z) const override;

    bool is_last_level_of_precision() const override { return false; }

    ~EnemyGridCompact() override {
        delete grid_array;
    }

    void print() const override { grid_array->print(); }
};



#endif //ENEMYGRIDCOMPACT_H
