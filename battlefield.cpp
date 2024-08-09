//
// Created by o.narvatov on 8/6/2024.
//


#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include "battlefield.h"

#include "blitzunit.h"
#include "EnemyGridXXS.h"

class EnemyGrid;

void Battlefield::_ready() {
    for (int i = 1; i < 20; i+=3) {
        Ref<PackedScene> scene = ResourceLoader::get_singleton()->load("res://blitz_unit.tscn");
        auto *instance = dynamic_cast<BlitzUnit*>(scene->instantiate());

        instance->set_position(Vector3(i,0.5,1));

        instance->isEnemy = true;

        add_child(instance);

        EnemyGridXXS::instance()->add_enemy(instance);
    }
}
