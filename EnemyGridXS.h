//
// Created by o.narvatov on 8/6/2024.
//

#ifndef ENEMYGRIDXS_H
#define ENEMYGRIDXS_H

#include "EnemyGridCompact.h"
#include "GridArrayXS.h"

class BlitzUnit;
class EnemyGridS;

using namespace std;

class EnemyGridXS : public EnemyGridCompact {
    static EnemyGridXS *p_inst;
public:
    static EnemyGridXS* instance(){
        if (!p_inst) {
            p_inst = new EnemyGridXS();
        }

        return p_inst;
    }

private:
    EnemyGridXS() : EnemyGridCompact(new GridArrayXS(), 1) {}
public:
    EnemyGrid *next_precision_grid() override;

    EnemyGrid *previous_precision_grid() override;
};



#endif //ENEMYGRIDXS_H
