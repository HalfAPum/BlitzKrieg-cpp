//
// Created by o.narvatov on 8/6/2024.
//


#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include "battlefield.h"

#include "blitzunit.h"

class UnitGrid;

void Battlefield::_ready() {
    const auto camera3D = get_node<Node3D>("camera_base");
    camera3D->set_position({16,8,16});

    for (int i = 1; i < 20; i+=3) {
        Ref<PackedScene> scene = ResourceLoader::get_singleton()->load("res://blitz_unit.tscn");
        auto *instance = dynamic_cast<BlitzUnit*>(scene->instantiate());
        instance->isEnemy = true;

        instance->set_position(Vector3(i,0.5,1));

        add_child(instance);
    }
}
