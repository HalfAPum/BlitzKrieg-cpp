//
// Created by o.narvatov on 8/5/2024.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

using namespace godot;
using namespace std;

class Constants {
    Constants() {}
public:
    Constants(Constants const&) = delete;
    void operator=(Constants const&) = delete;

    static Constants& getInstance()
    {
        static Constants instance;
        return instance;
    }

    void get_grid(double x);

    float gravity = ProjectSettings::get_singleton()->get_setting("physics/3d/default_gravity");

    //Mouse
    const StringName LEFT_CLICK = StringName("LeftClick");
    const StringName RIGHT_CLICK = StringName("RightClick");

    //Keyboard
    const StringName SHIFT = StringName("Shift");

    //Entity
    const StringName PROGRESS = StringName("progress");

    //Scenes
    const Ref<PackedScene> blitz_unit_scene = ResourceLoader::get_singleton()->load("res://blitz_unit.tscn");

};



#endif //CONSTANTS_H
