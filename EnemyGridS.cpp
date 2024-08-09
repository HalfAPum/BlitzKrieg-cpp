//
// Created by o.narvatov on 8/8/2024.
//

#include "EnemyGridS.h"
#include "EnemyGridXS.h"

EnemyGridS *EnemyGridS::p_inst;

EnemyGrid *EnemyGridS::next_precision_grid() {
    return EnemyGridXS::instance();
}
