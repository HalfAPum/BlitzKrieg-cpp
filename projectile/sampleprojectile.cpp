//
// Created by o.narvatov on 8/12/2024.
//

#include "sampleprojectile.h"

#include <godot_cpp/variant/utility_functions.hpp>

void SampleProjectile::_bind_methods() {
    ClassDB::bind_method(D_METHOD("_free"), &SampleProjectile::_free);
}

void SampleProjectile::_ready() {
    set_gravity_scale(0);

    destruction_timer = new Timer;

    if (!use_remote_timer) return;
    add_child(destruction_timer);

    destruction_timer->set_one_shot(true);


    destruction_timer->set_timer_process_callback(Timer::TimerProcessCallback::TIMER_PROCESS_PHYSICS);
    destruction_timer->connect("timeout", Callable(this, "_free"));

    destruction_timer->set_wait_time(5);
    destruction_timer->start();
}


void SampleProjectile::_free() {
    queue_free();
}

SampleProjectile::~SampleProjectile() {
    delete destruction_timer;
}
