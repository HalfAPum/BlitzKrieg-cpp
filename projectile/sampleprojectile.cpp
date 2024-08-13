//
// Created by o.narvatov on 8/12/2024.
//

#include "sampleprojectile.h"
#include "../collision/collision_detection.h"

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

void SampleProjectile::_physics_process(double p_delta) {
    const auto position = get_position();
    const auto position2D = Vector2(position.x, position.z);

    std::vector<collision_pair> collisions;

    detect_entity_collisions(collisions, position2D, 0);

    bool owner_in_vecotor = false;
    for (const auto collision: collisions) {
        if (collision.first == owner) {
            owner_in_vecotor = true;
            continue;
        }

        collision.first->take_damage(collision_damage);
    }

    if (owner_in_vecotor) {
        if (collisions.size() > 1) {
            _free();
        }
    } else if (!collisions.empty()) {
        _free();
    }
}



void SampleProjectile::_free() {
    queue_free();
}

SampleProjectile::~SampleProjectile() {
    delete destruction_timer;
}
