//
// Created by o.narvatov on 8/6/2024.
//

#ifndef ENEMYGRIDXXS_H
#define ENEMYGRIDXXS_H

#include <godot_cpp/variant/utility_functions.hpp>

#include "blitzunit.h"
#include "EnemyGrid.h"

class BlitzUnit;

using namespace std;
using namespace godot;

constexpr int ARRAY_SIZE_XXS = 8;
constexpr int GRID_SIZE_XXS = 4;

class EnemyGridXXS : public EnemyGrid {
    static EnemyGridXXS *p_inst;
    EnemyGridXXS() : EnemyGrid(GRID_SIZE_XXS, 0) {}
public:
    static EnemyGridXXS* instance(){
        if (!p_inst) {
            p_inst = new EnemyGridXXS();
        }

        return p_inst;
    }

private:
    array<array<vector<BlitzUnit*>, ARRAY_SIZE_XXS>, ARRAY_SIZE_XXS> units_array = {};
public:

    void add_enemy(BlitzUnit* unit) override;

    void remove_enemy(BlitzUnit *unit) override;

    void printContent() const {
        for (auto i : units_array) {
            UtilityFunctions::print("Column");
            for (auto j : i) {
                UtilityFunctions::print("Row");
                for (auto e : j) {
                    UtilityFunctions::print(e);
                }
            }
        }
    }

    int get_enemy_count(int x, int z) const override;

    EnemyGrid *next_precision_grid() override { return nullptr; }

    EnemyGrid *previous_precision_grid() override;

    bool is_last_level_of_precision() const override { return true; }

    BlitzUnit* find_if(int x, int z, const BlitzUnit* unit, bool (*predicate)(const BlitzUnit*, const BlitzUnit*)) const;

    void print() const override;

};



#endif //ENEMYGRIDXXS_H
