//
// Created by o.narvatov on 8/7/2024.
//

#ifndef ENEMYGRIDS_H
#define ENEMYGRIDS_H

#include "EnemyGrid.h"
#include "EnemyGridCompact.h"
#include "GridArrayS.h"

class BlitzUnit;

using namespace std;

class EnemyGridS final : public EnemyGridCompact {
    static EnemyGridS *p_inst;
public:
    static EnemyGridS* instance(){
        if (!p_inst) {
            p_inst = new EnemyGridS();
        }

        return p_inst;
    }

private:
    EnemyGridS() : EnemyGridCompact(new GridArrayS(), 2) {}
public:
    EnemyGrid *next_precision_grid() override;

    EnemyGrid *previous_precision_grid() override { return nullptr; }
};



#endif //ENEMYGRIDS_H
