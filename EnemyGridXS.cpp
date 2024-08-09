//
// Created by o.narvatov on 8/8/2024.
//

#include "EnemyGridXS.h"
#include "EnemyGridS.h"
#include "EnemyGridXXS.h"

EnemyGridXS *EnemyGridXS::p_inst;

EnemyGrid *EnemyGridXS::next_precision_grid() {
    return EnemyGridXXS::instance();
}


EnemyGrid *EnemyGridXS::previous_precision_grid() {
    return EnemyGridS::instance();
}
