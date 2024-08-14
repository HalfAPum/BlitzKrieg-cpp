//
// Created by o.narvatov on 8/13/2024.
//

#include "BlitzUnitBlueprint.h"

#include <godot_cpp/classes/packed_scene.hpp>

#include "../Constants.h"

BlitzUnit *BlitzUnitBlueprint::create() {
    return dynamic_cast<BlitzUnit*>(Constants::getInstance().blitz_unit_scene->instantiate());
}
