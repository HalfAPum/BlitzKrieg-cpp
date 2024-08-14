//
// Created by o.narvatov on 8/13/2024.
//

#ifndef BLUEPRINT_H
#define BLUEPRINT_H
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/defs.hpp>

#include "../blitzunit.h"

using namespace godot;

class BlitzUnitBlueprint {
private:

public:
    real_t assemble_time = 13;

    BlitzUnit* create();
};



#endif //BLUEPRINT_H
