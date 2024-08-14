//
// Created by o.narvatov on 8/13/2024.
//

#include "SimpleUnitFactoryT1.h"

#include <godot_cpp/variant/utility_functions.hpp>

void SimpleUnitFactoryT1::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_create_unit"), &SimpleUnitFactoryT1::_create_unit);
}

void SimpleUnitFactoryT1::_ready() {
    unit_spawn = get_node<Node3D>("unit_spawn");

    queue.emplace(BlitzUnitBlueprint());
    // queue.emplace(BlitzUnitBlueprint());

    unit_spawn_timer = new Timer;

    add_child(unit_spawn_timer);

    unit_spawn_timer->set_one_shot(true);


    unit_spawn_timer->set_timer_process_callback(Timer::TimerProcessCallback::TIMER_PROCESS_PHYSICS);
    unit_spawn_timer->connect("timeout", Callable(this, "_create_unit"));

    unit_spawn_timer->start();
}

constexpr auto PI = 3.141592f;

void SimpleUnitFactoryT1::_create_unit() {
    UtilityFunctions::print("Create unit");

    if (is_working) {
        const auto unit = currentWork.create();

        unit->set_global_transform(unit_spawn->get_global_transform());

        unit->spawn_rotation_radians = PI / 2;
        unit->move_command(unit->get_position() + Vector3(-8, 0, 0));
        unit->move_disabled_state = true;

        add_sibling(unit);

        is_working = false;
    }

    if (!queue.empty()) {
        currentWork = queue.front();
        queue.pop();

        // unit_spawn_timer->start(currentWork.assemble_time);
        unit_spawn_timer->start(1);

        is_working = true;
    }
}

void SimpleUnitFactoryT1::add_task(const BlitzUnitBlueprint &new_task) {
    queue.push(new_task);
}

void SimpleUnitFactoryT1::add_5_tasks(const BlitzUnitBlueprint &new_task) {
    queue.push(new_task);
    queue.push(new_task);
    queue.push(new_task);
    queue.push(new_task);
    queue.push(new_task);
}
