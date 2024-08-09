//
// Created by o.narvatov on 8/6/2024.
//

#ifndef ENEMYGRID_H
#define ENEMYGRID_H
#include "blitzunit.h"

class BlitzUnit;

class EnemyGrid {
public:
    int grid_size = 0;
    int grid_level = 0;

    explicit EnemyGrid(const int g_size, const int level) : grid_size(g_size), grid_level(level) {}

    virtual void add_enemy(BlitzUnit* unit) {}
    //old_x and old_z are supposed to be in the old square before current position
    virtual void remove_enemy(BlitzUnit* unit) {}

    virtual int get_enemy_count(int x, int z) const { return 0; }

    virtual bool is_last_level_of_precision() const { return true; }

    virtual ~EnemyGrid() = default;

    virtual EnemyGrid* next_precision_grid() { return nullptr; }
    virtual EnemyGrid* previous_precision_grid() { return nullptr; }

    //debug
    virtual void print() const {}
};



#endif //ENEMYGRID_H
