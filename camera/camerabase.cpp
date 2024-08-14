//
// Created by o.narvatov on 8/14/2024.
//

#include "camerabase.h"

#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "../Constants.h"

constexpr real_t camera_move_speed = 20;

void CameraBase::_ready() {
    camera_socket = get_node<CameraSocket>("camera_socket");
    camera_3d = camera_socket->get_node<Camera3D>("Camera3D");
}

void CameraBase::_unhandled_input(const Ref<InputEvent> &p_event) {

}

void CameraBase::_process(double p_delta) {
    if (!can_process) return;

    camera_base_move(p_delta);
}

void CameraBase::camera_base_move(double delta) {
    if (!can_move_base) return;

    auto velocity = Vector3();

    if (Input::get_singleton()->is_action_pressed(Constants::getInstance().CAMERA_FORWARD)) {
        velocity -= get_transform().get_basis().get_column(2);
    }
    if (Input::get_singleton()->is_action_pressed(Constants::getInstance().CAMERA_BACKWARD)) {
        velocity += get_transform().get_basis().get_column(2);
    }
    if (Input::get_singleton()->is_action_pressed(Constants::getInstance().CAMERA_RIGHT)) {
        velocity += get_transform().get_basis().get_column(0);
    }
    if (Input::get_singleton()->is_action_pressed(Constants::getInstance().CAMERA_LEFT)) {
        velocity -= get_transform().get_basis().get_column(0);
    }

    set_position(get_position() + (velocity.normalized() * delta * camera_move_speed));
}


