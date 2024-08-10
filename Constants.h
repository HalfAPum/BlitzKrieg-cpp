//
// Created by o.narvatov on 8/5/2024.
//

#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <godot_cpp/classes/project_settings.hpp>

#include "blitzunit.h"

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

    const StringName LEFT_CLICK = StringName("LeftClick");
    const StringName RIGHT_CLICK = StringName("RightClick");
};



#endif //CONSTANTS_H
