//
// Created by o.narvatov on 8/13/2024.
//

#ifndef SIMPLEUNITFACTORYT1_H
#define SIMPLEUNITFACTORYT1_H
#include <queue>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/timer.hpp>

#include "BlitzUnitBlueprint.h"

using namespace godot;

class SimpleUnitFactoryT1 : public Node3D {
    GDCLASS(SimpleUnitFactoryT1, Node3D)
public:
    void _ready() override;

    void add_task(const BlitzUnitBlueprint &new_task);

    void add_5_tasks(const BlitzUnitBlueprint &new_task);

private:
    Timer *unit_spawn_timer = nullptr;

    bool is_working = false;
    BlitzUnitBlueprint currentWork;
    std::queue<BlitzUnitBlueprint> queue;

    void _create_unit();

    Node3D* unit_spawn = nullptr;
protected:
    static void _bind_methods();
};



#endif //SIMPLEUNITFACTORYT1_H
